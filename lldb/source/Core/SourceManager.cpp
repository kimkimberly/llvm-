//===-- SourceManager.cpp -------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "lldb/Core/SourceManager.h"

#include "lldb/Core/Address.h"
#include "lldb/Core/AddressRange.h"
#include "lldb/Core/Debugger.h"
#include "lldb/Core/FormatEntity.h"
#include "lldb/Core/Highlighter.h"
#include "lldb/Core/Module.h"
#include "lldb/Core/ModuleList.h"
#include "lldb/Host/FileSystem.h"
#include "lldb/Symbol/CompileUnit.h"
#include "lldb/Symbol/Function.h"
#include "lldb/Symbol/LineEntry.h"
#include "lldb/Symbol/SymbolContext.h"
#include "lldb/Target/PathMappingList.h"
#include "lldb/Target/Target.h"
#include "lldb/Utility/AnsiTerminal.h"
#include "lldb/Utility/ConstString.h"
#include "lldb/Utility/DataBuffer.h"
#include "lldb/Utility/RegularExpression.h"
#include "lldb/Utility/Stream.h"
#include "lldb/lldb-enumerations.h"

#include "llvm/ADT/Twine.h"

#include <memory>
#include <optional>
#include <utility>

#include <cassert>
#include <cstdio>

namespace lldb_private {
class ExecutionContext;
}
namespace lldb_private {
class ValueObject;
}

using namespace lldb;
using namespace lldb_private;

static inline bool is_newline_char(char ch) { return ch == '\n' || ch == '\r'; }

static void resolve_tilde(FileSpec &file_spec) {
  if (!FileSystem::Instance().Exists(file_spec) &&
      file_spec.GetDirectory() &&
      file_spec.GetDirectory().GetCString()[0] == '~') {
    FileSystem::Instance().Resolve(file_spec);
  }
}

// SourceManager constructor
SourceManager::SourceManager(const TargetSP &target_sp)
    : m_last_line(0), m_last_count(0), m_default_set(false),
      m_target_wp(target_sp),
      m_debugger_wp(target_sp->GetDebugger().shared_from_this()) {}

SourceManager::SourceManager(const DebuggerSP &debugger_sp)
    : m_last_line(0), m_last_count(0), m_default_set(false), m_target_wp(),
      m_debugger_wp(debugger_sp) {}

// Destructor
SourceManager::~SourceManager() = default;

SourceManager::FileSP SourceManager::GetFile(const FileSpec &file_spec) {
  if (!file_spec)
    return nullptr;

  FileSpec resolved_fspec = file_spec;
  resolve_tilde(resolved_fspec);

  DebuggerSP debugger_sp(m_debugger_wp.lock());
  FileSP file_sp;
  if (debugger_sp && debugger_sp->GetUseSourceCache())
    file_sp = debugger_sp->GetSourceFileCache().FindSourceFile(resolved_fspec);

  TargetSP target_sp(m_target_wp.lock());

  // It the target source path map has been updated, get this file again so we
  // can successfully remap the source file
  if (target_sp && file_sp &&
      file_sp->GetSourceMapModificationID() !=
          target_sp->GetSourcePathMap().GetModificationID())
    file_sp.reset();

  // Update the file contents if needed if we found a file
  if (file_sp)
    file_sp->UpdateIfNeeded();

  // If file_sp is no good or it points to a non-existent file, reset it.
  if (!file_sp || !FileSystem::Instance().Exists(file_sp->GetFileSpec())) {
    if (target_sp)
      file_sp = std::make_shared<File>(resolved_fspec, target_sp.get());
    else
      file_sp = std::make_shared<File>(resolved_fspec, debugger_sp);

    if (debugger_sp && debugger_sp->GetUseSourceCache())
      debugger_sp->GetSourceFileCache().AddSourceFile(file_sp);
  }
  return file_sp;
}

static bool should_highlight_source(DebuggerSP debugger_sp) {
  if (!debugger_sp)
    return false;

  // We don't use ANSI stop column formatting if the debugger doesn't think it
  // should be using color.
  if (!debugger_sp->GetUseColor())
    return false;

  return debugger_sp->GetHighlightSource();
}

static bool should_show_stop_column_with_ansi(DebuggerSP debugger_sp) {
  // We don't use ANSI stop column formatting if we can't lookup values from
  // the debugger.
  if (!debugger_sp)
    return false;

  // We don't use ANSI stop column formatting if the debugger doesn't think it
  // should be using color.
  if (!debugger_sp->GetUseColor())
    return false;

  // We only use ANSI stop column formatting if we're either supposed to show
  // ANSI where available (which we know we have when we get to this point), or
  // if we're only supposed to use ANSI.
  const auto value = debugger_sp->GetStopShowColumn();
  return ((value == eStopShowColumnAnsiOrCaret) ||
          (value == eStopShowColumnAnsi));
}

static bool should_show_stop_column_with_caret(DebuggerSP debugger_sp) {
  // We don't use text-based stop column formatting if we can't lookup values
  // from the debugger.
  if (!debugger_sp)
    return false;

  // If we're asked to show the first available of ANSI or caret, then we do
  // show the caret when ANSI is not available.
  const auto value = debugger_sp->GetStopShowColumn();
  if ((value == eStopShowColumnAnsiOrCaret) && !debugger_sp->GetUseColor())
    return true;

  // The only other time we use caret is if we're explicitly asked to show
  // caret.
  return value == eStopShowColumnCaret;
}

static bool should_show_stop_line_with_ansi(DebuggerSP debugger_sp) {
  return debugger_sp && debugger_sp->GetUseColor();
}

size_t SourceManager::DisplaySourceLinesWithLineNumbersUsingLastFile(
    uint32_t start_line, uint32_t count, uint32_t curr_line, uint32_t column,
    const char *current_line_cstr, Stream *s,
    const SymbolContextList *bp_locs) {
  if (count == 0)
    return 0;

  Stream::ByteDelta delta(*s);

  if (start_line == 0) {
    if (m_last_line != 0 && m_last_line != UINT32_MAX)
      start_line = m_last_line + m_last_count;
    else
      start_line = 1;
  }

  if (!m_default_set) {
    FileSpec tmp_spec;
    uint32_t tmp_line;
    GetDefaultFileAndLine(tmp_spec, tmp_line);
  }

  m_last_line = start_line;
  m_last_count = count;

  if (FileSP last_file_sp = GetLastFile()) {
    const uint32_t end_line = start_line + count - 1;
    for (uint32_t line = start_line; line <= end_line; ++line) {
      if (!last_file_sp->LineIsValid(line)) {
        m_last_line = UINT32_MAX;
        break;
      }

      std::string prefix;
      if (bp_locs) {
        uint32_t bp_count = bp_locs->NumLineEntriesWithLine(line);

        if (bp_count > 0)
          prefix = llvm::formatv("[{0}]", bp_count);
        else
          prefix = "    ";
      }

      char buffer[3];
      snprintf(buffer, sizeof(buffer), "%2.2s",
               (line == curr_line) ? current_line_cstr : "");
      std::string current_line_highlight(buffer);

      auto debugger_sp = m_debugger_wp.lock();
      if (should_show_stop_line_with_ansi(debugger_sp)) {
        current_line_highlight = ansi::FormatAnsiTerminalCodes(
            (debugger_sp->GetStopShowLineMarkerAnsiPrefix() +
             current_line_highlight +
             debugger_sp->GetStopShowLineMarkerAnsiSuffix())
                .str());
      }

      s->Printf("%s%s %-4u\t", prefix.c_str(), current_line_highlight.c_str(),
                line);

      // So far we treated column 0 as a special 'no column value', but
      // DisplaySourceLines starts counting columns from 0 (and no column is
      // expressed by passing an empty optional).
      std::optional<size_t> columnToHighlight;
      if (line == curr_line && column)
        columnToHighlight = column - 1;

      size_t this_line_size =
          last_file_sp->DisplaySourceLines(line, columnToHighlight, 0, 0, s);
      if (column != 0 && line == curr_line &&
          should_show_stop_column_with_caret(debugger_sp)) {
        // Display caret cursor.
        std::string src_line;
        last_file_sp->GetLine(line, src_line);
        s->Printf("    \t");
        // Insert a space for every non-tab character in the source line.
        for (size_t i = 0; i + 1 < column && i < src_line.length(); ++i)
          s->PutChar(src_line[i] == '\t' ? '\t' : ' ');
        // Now add the caret.
        s->Printf("^\n");
      }
      if (this_line_size == 0) {
        m_last_line = UINT32_MAX;
        break;
      }
    }
  }
  return *delta;
}

size_t SourceManager::DisplaySourceLinesWithLineNumbers(
    const FileSpec &file_spec, uint32_t line, uint32_t column,
    uint32_t context_before, uint32_t context_after,
    const char *current_line_cstr, Stream *s,
    const SymbolContextList *bp_locs) {
  FileSP file_sp(GetFile(file_spec));

  uint32_t start_line;
  uint32_t count = context_before + context_after + 1;
  if (line > context_before)
    start_line = line - context_before;
  else
    start_line = 1;

  FileSP last_file_sp(GetLastFile());
  if (last_file_sp.get() != file_sp.get()) {
    if (line == 0)
      m_last_line = 0;
    m_last_file_spec = file_spec;
  }
  return DisplaySourceLinesWithLineNumbersUsingLastFile(
      start_line, count, line, column, current_line_cstr, s, bp_locs);
}

size_t SourceManager::DisplayMoreWithLineNumbers(
    Stream *s, uint32_t count, bool reverse, const SymbolContextList *bp_locs) {
  // If we get called before anybody has set a default file and line, then try
  // to figure it out here.
  FileSP last_file_sp(GetLastFile());
  const bool have_default_file_line = last_file_sp && m_last_line > 0;
  if (!m_default_set) {
    FileSpec tmp_spec;
    uint32_t tmp_line;
    GetDefaultFileAndLine(tmp_spec, tmp_line);
  }

  if (last_file_sp) {
    if (m_last_line == UINT32_MAX)
      return 0;

    if (reverse && m_last_line == 1)
      return 0;

    if (count > 0)
      m_last_count = count;
    else if (m_last_count == 0)
      m_last_count = 10;

    if (m_last_line > 0) {
      if (reverse) {
        // If this is the first time we've done a reverse, then back up one
        // more time so we end up showing the chunk before the last one we've
        // shown:
        if (m_last_line > m_last_count)
          m_last_line -= m_last_count;
        else
          m_last_line = 1;
      } else if (have_default_file_line)
        m_last_line += m_last_count;
    } else
      m_last_line = 1;

    const uint32_t column = 0;
    return DisplaySourceLinesWithLineNumbersUsingLastFile(
        m_last_line, m_last_count, UINT32_MAX, column, "", s, bp_locs);
  }
  return 0;
}

bool SourceManager::SetDefaultFileAndLine(const FileSpec &file_spec,
                                          uint32_t line) {
  m_default_set = true;
  FileSP file_sp(GetFile(file_spec));

  if (file_sp) {
    m_last_line = line;
    m_last_file_spec = file_spec;
    return true;
  } else {
    return false;
  }
}

// BEGIN SWIFT

// This is a vector of pairs that each represent a legitimate
// interesting entry point to the user program. the bool argument
// means whether we want to skip the prologue code or not when trying
// to resolve this name to a line entry FindEntryPoint() will attempt
// to resolve these to a valid line entry in the order in which they
// are provided. This could probably be extended with a notion of
// "main language", i.e. the language in which the main executable
// module is coded
static const std::vector<std::pair<ConstString, bool>> &GetEntryPointNames() {
  static std::vector<std::pair<ConstString, bool>> g_entry_point_names;
  if (g_entry_point_names.size() == 0) {
    g_entry_point_names.push_back({ConstString("main"), false});
    g_entry_point_names.push_back({ConstString("top_level_code"), true});
  }
  return g_entry_point_names;
}

static lldb_private::LineEntry FindEntryPoint(Module *exe_module) {
  if (!exe_module)
    return LineEntry();
  const std::vector<std::pair<ConstString, bool>> &entry_points(
      GetEntryPointNames());
  for (std::pair<ConstString, bool> entry_point : entry_points) {
    const ConstString entry_point_name = entry_point.first;
    const bool skip_prologue = entry_point.second;
    SymbolContextList sc_list;
    ModuleFunctionSearchOptions function_options;
    function_options.include_symbols = false; // Force it to be a debug symbol
    function_options.include_inlines = true;
    exe_module->FindFunctions(entry_point_name, CompilerDeclContext(),
                              lldb::eFunctionNameTypeBase, function_options,
                              sc_list);
    size_t num_matches = sc_list.GetSize();
    for (size_t idx = 0; idx < num_matches; idx++) {
      SymbolContext sc;
      sc_list.GetContextAtIndex(idx, sc);
      if (sc.function) {
        lldb_private::LineEntry line_entry;
        Address base_address = sc.function->GetAddressRange().GetBaseAddress();
        if (skip_prologue)
          base_address.SetOffset(sc.function->GetPrologueByteSize() +
                                 base_address.GetOffset());
        if (base_address.CalculateSymbolContextLineEntry(line_entry)) {
          return line_entry;
        }
      }
    }
  }
  return LineEntry();
}
// END SWIFT

bool SourceManager::GetDefaultFileAndLine(FileSpec &file_spec, uint32_t &line) {
  if (FileSP last_file_sp = GetLastFile()) {
    file_spec = m_last_file_spec;
    line = m_last_line;
    return true;
  } else if (!m_default_set) {
    TargetSP target_sp(m_target_wp.lock());

    if (target_sp) {
      // If nobody has set the default file and line then try here.  If there's
      // no executable, then we will try again later when there is one.
      // Otherwise, if we can't find it we won't look again, somebody will have
      // to set it (for instance when we stop somewhere...)
      Module *executable_ptr = target_sp->GetExecutableModulePointer();
      if (executable_ptr) {
        // BEGIN SWIFT
#if 0 // llvm.org
        SymbolContextList sc_list;
        ConstString main_name("main");

        ModuleFunctionSearchOptions function_options;
        function_options.include_symbols =
            false; // Force it to be a debug symbol.
        function_options.include_inlines = true;
        executable_ptr->FindFunctions(main_name, CompilerDeclContext(),
                                      lldb::eFunctionNameTypeBase,
                                      function_options, sc_list);
        for (const SymbolContext &sc : sc_list) {
          if (sc.function) {
            lldb_private::LineEntry line_entry;
            if (sc.function->GetAddressRange()
                    .GetBaseAddress()
                    .CalculateSymbolContextLineEntry(line_entry)) {
              SetDefaultFileAndLine(line_entry.file, line_entry.line);
              file_spec = m_last_file_spec;
              line = m_last_line;
              return true;
            }
          }
        }
#else // FIXME: upstream this!
        lldb_private::LineEntry line_entry(FindEntryPoint(executable_ptr));
        if (line_entry.IsValid()) {
          SetDefaultFileAndLine(line_entry.file, line_entry.line);
          file_spec = m_last_file_spec;
          line = m_last_line;
          return true;
        }
#endif
        // END SWIFT
      }
    }
  }
  return false;
}

void SourceManager::FindLinesMatchingRegex(FileSpec &file_spec,
                                           RegularExpression &regex,
                                           uint32_t start_line,
                                           uint32_t end_line,
                                           std::vector<uint32_t> &match_lines) {
  match_lines.clear();
  FileSP file_sp = GetFile(file_spec);
  if (!file_sp)
    return;
  return file_sp->FindLinesMatchingRegex(regex, start_line, end_line,
                                         match_lines);
}

SourceManager::File::File(const FileSpec &file_spec,
                          lldb::DebuggerSP debugger_sp)
    : m_file_spec_orig(file_spec), m_file_spec(file_spec),
      m_mod_time(FileSystem::Instance().GetModificationTime(file_spec)),
      m_debugger_wp(debugger_sp) {
  CommonInitializer(file_spec, nullptr);
}

SourceManager::File::File(const FileSpec &file_spec, Target *target)
    : m_file_spec_orig(file_spec), m_file_spec(file_spec),
      m_mod_time(FileSystem::Instance().GetModificationTime(file_spec)),
      m_debugger_wp(target ? target->GetDebugger().shared_from_this()
                           : DebuggerSP()) {
  CommonInitializer(file_spec, target);
}

void SourceManager::File::CommonInitializer(const FileSpec &file_spec,
                                            Target *target) {
  if (m_mod_time == llvm::sys::TimePoint<>()) {
    if (target) {
      m_source_map_mod_id = target->GetSourcePathMap().GetModificationID();

      if (!file_spec.GetDirectory() && file_spec.GetFilename()) {
        // If this is just a file name, lets see if we can find it in the
        // target:
        bool check_inlines = false;
        SymbolContextList sc_list;
        size_t num_matches =
            target->GetImages().ResolveSymbolContextForFilePath(
                file_spec.GetFilename().AsCString(), 0, check_inlines,
                SymbolContextItem(eSymbolContextModule |
                                  eSymbolContextCompUnit),
                sc_list);
        bool got_multiple = false;
        if (num_matches != 0) {
          if (num_matches > 1) {
            CompileUnit *test_cu = nullptr;
            for (const SymbolContext &sc : sc_list) {
              if (sc.comp_unit) {
                if (test_cu) {
                  if (test_cu != sc.comp_unit)
                    got_multiple = true;
                  break;
                } else
                  test_cu = sc.comp_unit;
              }
            }
          }
          if (!got_multiple) {
            SymbolContext sc;
            sc_list.GetContextAtIndex(0, sc);
            if (sc.comp_unit)
              m_file_spec = sc.comp_unit->GetPrimaryFile();
            m_mod_time = FileSystem::Instance().GetModificationTime(m_file_spec);
          }
        }
      }
      resolve_tilde(m_file_spec);
      // Try remapping if m_file_spec does not correspond to an existing file.
      if (!FileSystem::Instance().Exists(m_file_spec)) {
        // Check target specific source remappings (i.e., the
        // target.source-map setting), then fall back to the module
        // specific remapping (i.e., the .dSYM remapping dictionary).
        auto remapped = target->GetSourcePathMap().FindFile(m_file_spec);
        if (!remapped) {
          FileSpec new_spec;
          if (target->GetImages().FindSourceFile(m_file_spec, new_spec))
            remapped = new_spec;
        }
        if (remapped) {
          m_file_spec = *remapped;
          m_mod_time = FileSystem::Instance().GetModificationTime(m_file_spec);
        }
      }
    }
  }

  if (m_mod_time != llvm::sys::TimePoint<>())
    m_data_sp = FileSystem::Instance().CreateDataBuffer(m_file_spec);
}

uint32_t SourceManager::File::GetLineOffset(uint32_t line) {
  if (line == 0)
    return UINT32_MAX;

  if (line == 1)
    return 0;

  if (CalculateLineOffsets(line)) {
    if (line < m_offsets.size())
      return m_offsets[line - 1]; // yes we want "line - 1" in the index
  }
  return UINT32_MAX;
}

uint32_t SourceManager::File::GetNumLines() {
  CalculateLineOffsets();
  return m_offsets.size();
}

const char *SourceManager::File::PeekLineData(uint32_t line) {
  if (!LineIsValid(line))
    return nullptr;

  size_t line_offset = GetLineOffset(line);
  if (line_offset < m_data_sp->GetByteSize())
    return (const char *)m_data_sp->GetBytes() + line_offset;
  return nullptr;
}

uint32_t SourceManager::File::GetLineLength(uint32_t line,
                                            bool include_newline_chars) {
  if (!LineIsValid(line))
    return false;

  size_t start_offset = GetLineOffset(line);
  size_t end_offset = GetLineOffset(line + 1);
  if (end_offset == UINT32_MAX)
    end_offset = m_data_sp->GetByteSize();

  if (end_offset > start_offset) {
    uint32_t length = end_offset - start_offset;
    if (!include_newline_chars) {
      const char *line_start =
          (const char *)m_data_sp->GetBytes() + start_offset;
      while (length > 0) {
        const char last_char = line_start[length - 1];
        if ((last_char == '\r') || (last_char == '\n'))
          --length;
        else
          break;
      }
    }
    return length;
  }
  return 0;
}

bool SourceManager::File::LineIsValid(uint32_t line) {
  if (line == 0)
    return false;

  if (CalculateLineOffsets(line))
    return line < m_offsets.size();
  return false;
}

void SourceManager::File::UpdateIfNeeded() {
  // TODO: use host API to sign up for file modifications to anything in our
  // source cache and only update when we determine a file has been updated.
  // For now we check each time we want to display info for the file.
  auto curr_mod_time = FileSystem::Instance().GetModificationTime(m_file_spec);

  if (curr_mod_time != llvm::sys::TimePoint<>() &&
      m_mod_time != curr_mod_time) {
    m_mod_time = curr_mod_time;
    m_data_sp = FileSystem::Instance().CreateDataBuffer(m_file_spec);
    m_offsets.clear();
  }
}

size_t SourceManager::File::DisplaySourceLines(uint32_t line,
                                               std::optional<size_t> column,
                                               uint32_t context_before,
                                               uint32_t context_after,
                                               Stream *s) {
  // Nothing to write if there's no stream.
  if (!s)
    return 0;

  // Sanity check m_data_sp before proceeding.
  if (!m_data_sp)
    return 0;

  size_t bytes_written = s->GetWrittenBytes();

  auto debugger_sp = m_debugger_wp.lock();

  HighlightStyle style;
  // Use the default Vim style if source highlighting is enabled.
  if (should_highlight_source(debugger_sp))
    style = HighlightStyle::MakeVimStyle();

  // If we should mark the stop column with color codes, then copy the prefix
  // and suffix to our color style.
  if (should_show_stop_column_with_ansi(debugger_sp))
    style.selected.Set(debugger_sp->GetStopShowColumnAnsiPrefix(),
                       debugger_sp->GetStopShowColumnAnsiSuffix());

  HighlighterManager mgr;
  std::string path = GetFileSpec().GetPath(/*denormalize*/ false);
  // FIXME: Find a way to get the definitive language this file was written in
  // and pass it to the highlighter.
  const auto &h = mgr.getHighlighterFor(lldb::eLanguageTypeUnknown, path);

  const uint32_t start_line =
      line <= context_before ? 1 : line - context_before;
  const uint32_t start_line_offset = GetLineOffset(start_line);
  if (start_line_offset != UINT32_MAX) {
    const uint32_t end_line = line + context_after;
    uint32_t end_line_offset = GetLineOffset(end_line + 1);
    if (end_line_offset == UINT32_MAX)
      end_line_offset = m_data_sp->GetByteSize();

    assert(start_line_offset <= end_line_offset);
    if (start_line_offset < end_line_offset) {
      size_t count = end_line_offset - start_line_offset;
      const uint8_t *cstr = m_data_sp->GetBytes() + start_line_offset;

      auto ref = llvm::StringRef(reinterpret_cast<const char *>(cstr), count);

      h.Highlight(style, ref, column, "", *s);

      // Ensure we get an end of line character one way or another.
      if (!is_newline_char(ref.back()))
        s->EOL();
    }
  }
  return s->GetWrittenBytes() - bytes_written;
}

void SourceManager::File::FindLinesMatchingRegex(
    RegularExpression &regex, uint32_t start_line, uint32_t end_line,
    std::vector<uint32_t> &match_lines) {
  match_lines.clear();

  if (!LineIsValid(start_line) ||
      (end_line != UINT32_MAX && !LineIsValid(end_line)))
    return;
  if (start_line > end_line)
    return;

  for (uint32_t line_no = start_line; line_no < end_line; line_no++) {
    std::string buffer;
    if (!GetLine(line_no, buffer))
      break;
    if (regex.Execute(buffer)) {
      match_lines.push_back(line_no);
    }
  }
}

bool lldb_private::operator==(const SourceManager::File &lhs,
                              const SourceManager::File &rhs) {
  if (lhs.m_file_spec != rhs.m_file_spec)
    return false;
  return lhs.m_mod_time == rhs.m_mod_time;
}

bool SourceManager::File::CalculateLineOffsets(uint32_t line) {
  line =
      UINT32_MAX; // TODO: take this line out when we support partial indexing
  if (line == UINT32_MAX) {
    // Already done?
    if (!m_offsets.empty() && m_offsets[0] == UINT32_MAX)
      return true;

    if (m_offsets.empty()) {
      if (m_data_sp.get() == nullptr)
        return false;

      const char *start = (const char *)m_data_sp->GetBytes();
      if (start) {
        const char *end = start + m_data_sp->GetByteSize();

        // Calculate all line offsets from scratch

        // Push a 1 at index zero to indicate the file has been completely
        // indexed.
        m_offsets.push_back(UINT32_MAX);
        const char *s;
        for (s = start; s < end; ++s) {
          char curr_ch = *s;
          if (is_newline_char(curr_ch)) {
            if (s + 1 < end) {
              char next_ch = s[1];
              if (is_newline_char(next_ch)) {
                if (curr_ch != next_ch)
                  ++s;
              }
            }
            m_offsets.push_back(s + 1 - start);
          }
        }
        if (!m_offsets.empty()) {
          if (m_offsets.back() < size_t(end - start))
            m_offsets.push_back(end - start);
        }
        return true;
      }
    } else {
      // Some lines have been populated, start where we last left off
      assert("Not implemented yet" && false);
    }

  } else {
    // Calculate all line offsets up to "line"
    assert("Not implemented yet" && false);
  }
  return false;
}

bool SourceManager::File::GetLine(uint32_t line_no, std::string &buffer) {
  if (!LineIsValid(line_no))
    return false;

  size_t start_offset = GetLineOffset(line_no);
  size_t end_offset = GetLineOffset(line_no + 1);
  if (end_offset == UINT32_MAX) {
    end_offset = m_data_sp->GetByteSize();
  }
  buffer.assign((const char *)m_data_sp->GetBytes() + start_offset,
                end_offset - start_offset);

  return true;
}

void SourceManager::SourceFileCache::AddSourceFile(const FileSP &file_sp) {
  FileSpec file_spec = file_sp->GetFileSpec();
  FileCache::iterator pos = m_file_cache.find(file_spec);
  if (pos == m_file_cache.end())
    m_file_cache[file_spec] = file_sp;
  else {
    if (file_sp != pos->second)
      m_file_cache[file_spec] = file_sp;
  }
}

SourceManager::FileSP SourceManager::SourceFileCache::FindSourceFile(
    const FileSpec &file_spec) const {
  FileSP file_sp;
  FileCache::const_iterator pos = m_file_cache.find(file_spec);
  if (pos != m_file_cache.end())
    file_sp = pos->second;
  return file_sp;
}
