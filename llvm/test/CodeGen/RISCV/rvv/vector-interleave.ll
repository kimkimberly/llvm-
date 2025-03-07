; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -mtriple=riscv32 -mattr=+v,+zfh,+experimental-zvfh | FileCheck %s
; RUN: llc < %s -mtriple=riscv64 -mattr=+v,+zfh,+experimental-zvfh | FileCheck %s

; Integers

define <vscale x 32 x i1> @vector_interleave_nxv32i1_nxv16i1(<vscale x 16 x i1> %a, <vscale x 16 x i1> %b) {
; CHECK-LABEL: vector_interleave_nxv32i1_nxv16i1:
; CHECK:       # %bb.0:
; CHECK-NEXT:    vmv1r.v v9, v0
; CHECK-NEXT:    vsetvli a0, zero, e8, m2, ta, ma
; CHECK-NEXT:    vmv.v.i v10, 0
; CHECK-NEXT:    vmv1r.v v0, v8
; CHECK-NEXT:    vmerge.vim v12, v10, 1, v0
; CHECK-NEXT:    vmv1r.v v0, v9
; CHECK-NEXT:    vmerge.vim v8, v10, 1, v0
; CHECK-NEXT:    vwaddu.vv v16, v8, v12
; CHECK-NEXT:    li a0, -1
; CHECK-NEXT:    vwmaccu.vx v16, a0, v12
; CHECK-NEXT:    vmsne.vi v8, v18, 0
; CHECK-NEXT:    vmsne.vi v0, v16, 0
; CHECK-NEXT:    csrr a0, vlenb
; CHECK-NEXT:    srli a0, a0, 2
; CHECK-NEXT:    add a1, a0, a0
; CHECK-NEXT:    vsetvli zero, a1, e8, mf2, tu, ma
; CHECK-NEXT:    vslideup.vx v0, v8, a0
; CHECK-NEXT:    ret
  %res = call <vscale x 32 x i1> @llvm.experimental.vector.interleave2.nxv32i1(<vscale x 16 x i1> %a, <vscale x 16 x i1> %b)
  ret <vscale x 32 x i1> %res
}

define <vscale x 32 x i8> @vector_interleave_nxv32i8_nxv16i8(<vscale x 16 x i8> %a, <vscale x 16 x i8> %b) {
; CHECK-LABEL: vector_interleave_nxv32i8_nxv16i8:
; CHECK:       # %bb.0:
; CHECK-NEXT:    vsetvli a0, zero, e8, m2, ta, ma
; CHECK-NEXT:    vwaddu.vv v12, v8, v10
; CHECK-NEXT:    li a0, -1
; CHECK-NEXT:    vwmaccu.vx v12, a0, v10
; CHECK-NEXT:    vmv4r.v v8, v12
; CHECK-NEXT:    ret
  %res = call <vscale x 32 x i8> @llvm.experimental.vector.interleave2.nxv32i8(<vscale x 16 x i8> %a, <vscale x 16 x i8> %b)
  ret <vscale x 32 x i8> %res
}

define <vscale x 16 x i16> @vector_interleave_nxv16i16_nxv8i16(<vscale x 8 x i16> %a, <vscale x 8 x i16> %b) {
; CHECK-LABEL: vector_interleave_nxv16i16_nxv8i16:
; CHECK:       # %bb.0:
; CHECK-NEXT:    vsetvli a0, zero, e16, m2, ta, ma
; CHECK-NEXT:    vwaddu.vv v12, v8, v10
; CHECK-NEXT:    li a0, -1
; CHECK-NEXT:    vwmaccu.vx v12, a0, v10
; CHECK-NEXT:    vmv4r.v v8, v12
; CHECK-NEXT:    ret
  %res = call <vscale x 16 x i16> @llvm.experimental.vector.interleave2.nxv16i16(<vscale x 8 x i16> %a, <vscale x 8 x i16> %b)
  ret <vscale x 16 x i16> %res
}

define <vscale x 8 x i32> @vector_interleave_nxv8i32_nxv4i32(<vscale x 4 x i32> %a, <vscale x 4 x i32> %b) {
; CHECK-LABEL: vector_interleave_nxv8i32_nxv4i32:
; CHECK:       # %bb.0:
; CHECK-NEXT:    vsetvli a0, zero, e32, m2, ta, ma
; CHECK-NEXT:    vwaddu.vv v12, v8, v10
; CHECK-NEXT:    li a0, -1
; CHECK-NEXT:    vwmaccu.vx v12, a0, v10
; CHECK-NEXT:    vmv4r.v v8, v12
; CHECK-NEXT:    ret
  %res = call <vscale x 8 x i32> @llvm.experimental.vector.interleave2.nxv8i32(<vscale x 4 x i32> %a, <vscale x 4 x i32> %b)
  ret <vscale x 8 x i32> %res
}

define <vscale x 4 x i64> @vector_interleave_nxv4i64_nxv2i64(<vscale x 2 x i64> %a, <vscale x 2 x i64> %b) {
; CHECK-LABEL: vector_interleave_nxv4i64_nxv2i64:
; CHECK:       # %bb.0:
; CHECK-NEXT:    csrr a0, vlenb
; CHECK-NEXT:    srli a0, a0, 2
; CHECK-NEXT:    vsetvli a1, zero, e16, m1, ta, mu
; CHECK-NEXT:    vid.v v12
; CHECK-NEXT:    vand.vi v13, v12, 1
; CHECK-NEXT:    vmsne.vi v0, v13, 0
; CHECK-NEXT:    vsrl.vi v16, v12, 1
; CHECK-NEXT:    vadd.vx v16, v16, a0, v0.t
; CHECK-NEXT:    vsetvli zero, zero, e64, m4, ta, ma
; CHECK-NEXT:    vrgatherei16.vv v12, v8, v16, v0.t
; CHECK-NEXT:    vmv.v.v v8, v12
; CHECK-NEXT:    ret
  %res = call <vscale x 4 x i64> @llvm.experimental.vector.interleave2.nxv4i64(<vscale x 2 x i64> %a, <vscale x 2 x i64> %b)
  ret <vscale x 4 x i64> %res
}

declare <vscale x 32 x i1> @llvm.experimental.vector.interleave2.nxv32i1(<vscale x 16 x i1>, <vscale x 16 x i1>)
declare <vscale x 32 x i8> @llvm.experimental.vector.interleave2.nxv32i8(<vscale x 16 x i8>, <vscale x 16 x i8>)
declare <vscale x 16 x i16> @llvm.experimental.vector.interleave2.nxv16i16(<vscale x 8 x i16>, <vscale x 8 x i16>)
declare <vscale x 8 x i32> @llvm.experimental.vector.interleave2.nxv8i32(<vscale x 4 x i32>, <vscale x 4 x i32>)
declare <vscale x 4 x i64> @llvm.experimental.vector.interleave2.nxv4i64(<vscale x 2 x i64>, <vscale x 2 x i64>)

define <vscale x 128 x i1> @vector_interleave_nxv128i1_nxv64i1(<vscale x 64 x i1> %a, <vscale x 64 x i1> %b) {
; CHECK-LABEL: vector_interleave_nxv128i1_nxv64i1:
; CHECK:       # %bb.0:
; CHECK-NEXT:    vmv1r.v v9, v0
; CHECK-NEXT:    vsetvli a0, zero, e8, m8, ta, ma
; CHECK-NEXT:    vmv.v.i v24, 0
; CHECK-NEXT:    vmv1r.v v0, v8
; CHECK-NEXT:    vmerge.vim v16, v24, 1, v0
; CHECK-NEXT:    vmv1r.v v0, v9
; CHECK-NEXT:    vmerge.vim v8, v24, 1, v0
; CHECK-NEXT:    vsetvli a0, zero, e8, m4, ta, ma
; CHECK-NEXT:    vwaddu.vv v24, v8, v16
; CHECK-NEXT:    li a0, -1
; CHECK-NEXT:    vwmaccu.vx v24, a0, v16
; CHECK-NEXT:    vsetvli a1, zero, e8, m8, ta, ma
; CHECK-NEXT:    vmsne.vi v0, v24, 0
; CHECK-NEXT:    vsetvli a1, zero, e8, m4, ta, ma
; CHECK-NEXT:    vwaddu.vv v24, v12, v20
; CHECK-NEXT:    vwmaccu.vx v24, a0, v20
; CHECK-NEXT:    vsetvli a0, zero, e8, m8, ta, ma
; CHECK-NEXT:    vmsne.vi v8, v24, 0
; CHECK-NEXT:    ret
  %res = call <vscale x 128 x i1> @llvm.experimental.vector.interleave2.nxv128i1(<vscale x 64 x i1> %a, <vscale x 64 x i1> %b)
  ret <vscale x 128 x i1> %res
}

define <vscale x 128 x i8> @vector_interleave_nxv128i8_nxv64i8(<vscale x 64 x i8> %a, <vscale x 64 x i8> %b) {
; CHECK-LABEL: vector_interleave_nxv128i8_nxv64i8:
; CHECK:       # %bb.0:
; CHECK-NEXT:    vmv8r.v v24, v8
; CHECK-NEXT:    vsetvli a0, zero, e8, m4, ta, ma
; CHECK-NEXT:    vwaddu.vv v8, v24, v16
; CHECK-NEXT:    li a0, -1
; CHECK-NEXT:    vwmaccu.vx v8, a0, v16
; CHECK-NEXT:    vwaddu.vv v0, v28, v20
; CHECK-NEXT:    vwmaccu.vx v0, a0, v20
; CHECK-NEXT:    vmv8r.v v16, v0
; CHECK-NEXT:    ret
  %res = call <vscale x 128 x i8> @llvm.experimental.vector.interleave2.nxv128i8(<vscale x 64 x i8> %a, <vscale x 64 x i8> %b)
  ret <vscale x 128 x i8> %res
}

define <vscale x 64 x i16> @vector_interleave_nxv64i16_nxv32i16(<vscale x 32 x i16> %a, <vscale x 32 x i16> %b) {
; CHECK-LABEL: vector_interleave_nxv64i16_nxv32i16:
; CHECK:       # %bb.0:
; CHECK-NEXT:    vmv8r.v v24, v8
; CHECK-NEXT:    vsetvli a0, zero, e16, m4, ta, ma
; CHECK-NEXT:    vwaddu.vv v8, v24, v16
; CHECK-NEXT:    li a0, -1
; CHECK-NEXT:    vwmaccu.vx v8, a0, v16
; CHECK-NEXT:    vwaddu.vv v0, v28, v20
; CHECK-NEXT:    vwmaccu.vx v0, a0, v20
; CHECK-NEXT:    vmv8r.v v16, v0
; CHECK-NEXT:    ret
  %res = call <vscale x 64 x i16> @llvm.experimental.vector.interleave2.nxv64i16(<vscale x 32 x i16> %a, <vscale x 32 x i16> %b)
  ret <vscale x 64 x i16> %res
}

define <vscale x 32 x i32> @vector_interleave_nxv32i32_nxv16i32(<vscale x 16 x i32> %a, <vscale x 16 x i32> %b) {
; CHECK-LABEL: vector_interleave_nxv32i32_nxv16i32:
; CHECK:       # %bb.0:
; CHECK-NEXT:    vmv8r.v v24, v8
; CHECK-NEXT:    vsetvli a0, zero, e32, m4, ta, ma
; CHECK-NEXT:    vwaddu.vv v8, v24, v16
; CHECK-NEXT:    li a0, -1
; CHECK-NEXT:    vwmaccu.vx v8, a0, v16
; CHECK-NEXT:    vwaddu.vv v0, v28, v20
; CHECK-NEXT:    vwmaccu.vx v0, a0, v20
; CHECK-NEXT:    vmv8r.v v16, v0
; CHECK-NEXT:    ret
  %res = call <vscale x 32 x i32> @llvm.experimental.vector.interleave2.nxv32i32(<vscale x 16 x i32> %a, <vscale x 16 x i32> %b)
  ret <vscale x 32 x i32> %res
}

define <vscale x 16 x i64> @vector_interleave_nxv16i64_nxv8i64(<vscale x 8 x i64> %a, <vscale x 8 x i64> %b) {
; CHECK-LABEL: vector_interleave_nxv16i64_nxv8i64:
; CHECK:       # %bb.0:
; CHECK-NEXT:    addi sp, sp, -16
; CHECK-NEXT:    .cfi_def_cfa_offset 16
; CHECK-NEXT:    csrr a0, vlenb
; CHECK-NEXT:    slli a0, a0, 4
; CHECK-NEXT:    sub sp, sp, a0
; CHECK-NEXT:    .cfi_escape 0x0f, 0x0d, 0x72, 0x00, 0x11, 0x10, 0x22, 0x11, 0x10, 0x92, 0xa2, 0x38, 0x00, 0x1e, 0x22 # sp + 16 + 16 * vlenb
; CHECK-NEXT:    csrr a0, vlenb
; CHECK-NEXT:    slli a0, a0, 3
; CHECK-NEXT:    add a0, sp, a0
; CHECK-NEXT:    addi a0, a0, 16
; CHECK-NEXT:    vs8r.v v8, (a0) # Unknown-size Folded Spill
; CHECK-NEXT:    csrr a0, vlenb
; CHECK-NEXT:    srli a0, a0, 1
; CHECK-NEXT:    vsetvli a1, zero, e16, m2, ta, mu
; CHECK-NEXT:    vid.v v24
; CHECK-NEXT:    vand.vi v26, v24, 1
; CHECK-NEXT:    vmsne.vi v0, v26, 0
; CHECK-NEXT:    vsrl.vi v2, v24, 1
; CHECK-NEXT:    csrr a1, vlenb
; CHECK-NEXT:    slli a1, a1, 3
; CHECK-NEXT:    add a1, sp, a1
; CHECK-NEXT:    addi a1, a1, 16
; CHECK-NEXT:    vl8r.v v8, (a1) # Unknown-size Folded Reload
; CHECK-NEXT:    vadd.vx v2, v2, a0, v0.t
; CHECK-NEXT:    vmv4r.v v12, v16
; CHECK-NEXT:    vsetvli zero, zero, e64, m8, ta, ma
; CHECK-NEXT:    vrgatherei16.vv v24, v8, v2, v0.t
; CHECK-NEXT:    addi a0, sp, 16
; CHECK-NEXT:    vs8r.v v24, (a0) # Unknown-size Folded Spill
; CHECK-NEXT:    csrr a0, vlenb
; CHECK-NEXT:    slli a0, a0, 3
; CHECK-NEXT:    add a0, sp, a0
; CHECK-NEXT:    addi a0, a0, 16
; CHECK-NEXT:    vl8r.v v8, (a0) # Unknown-size Folded Reload
; CHECK-NEXT:    vmv4r.v v16, v12
; CHECK-NEXT:    vrgatherei16.vv v24, v16, v2, v0.t
; CHECK-NEXT:    addi a0, sp, 16
; CHECK-NEXT:    vl8r.v v8, (a0) # Unknown-size Folded Reload
; CHECK-NEXT:    vmv.v.v v16, v24
; CHECK-NEXT:    csrr a0, vlenb
; CHECK-NEXT:    slli a0, a0, 4
; CHECK-NEXT:    add sp, sp, a0
; CHECK-NEXT:    addi sp, sp, 16
; CHECK-NEXT:    ret
  %res = call <vscale x 16 x i64> @llvm.experimental.vector.interleave2.nxv16i64(<vscale x 8 x i64> %a, <vscale x 8 x i64> %b)
  ret <vscale x 16 x i64> %res
}

declare <vscale x 128 x i1> @llvm.experimental.vector.interleave2.nxv128i1(<vscale x 64 x i1>, <vscale x 64 x i1>)
declare <vscale x 128 x i8> @llvm.experimental.vector.interleave2.nxv128i8(<vscale x 64 x i8>, <vscale x 64 x i8>)
declare <vscale x 64 x i16> @llvm.experimental.vector.interleave2.nxv64i16(<vscale x 32 x i16>, <vscale x 32 x i16>)
declare <vscale x 32 x i32> @llvm.experimental.vector.interleave2.nxv32i32(<vscale x 16 x i32>, <vscale x 16 x i32>)
declare <vscale x 16 x i64> @llvm.experimental.vector.interleave2.nxv16i64(<vscale x 8 x i64>, <vscale x 8 x i64>)

; Floats

define <vscale x 4 x half> @vector_interleave_nxv4f16_nxv2f16(<vscale x 2 x half> %a, <vscale x 2 x half> %b) {
; CHECK-LABEL: vector_interleave_nxv4f16_nxv2f16:
; CHECK:       # %bb.0:
; CHECK-NEXT:    vsetvli a0, zero, e16, mf2, ta, ma
; CHECK-NEXT:    vwaddu.vv v10, v8, v9
; CHECK-NEXT:    li a0, -1
; CHECK-NEXT:    vwmaccu.vx v10, a0, v9
; CHECK-NEXT:    csrr a0, vlenb
; CHECK-NEXT:    srli a0, a0, 2
; CHECK-NEXT:    vsetvli a1, zero, e16, m1, ta, ma
; CHECK-NEXT:    vslidedown.vx v8, v10, a0
; CHECK-NEXT:    add a1, a0, a0
; CHECK-NEXT:    vsetvli zero, a1, e16, m1, tu, ma
; CHECK-NEXT:    vslideup.vx v10, v8, a0
; CHECK-NEXT:    vmv1r.v v8, v10
; CHECK-NEXT:    ret
  %res = call <vscale x 4 x half> @llvm.experimental.vector.interleave2.nxv4f16(<vscale x 2 x half> %a, <vscale x 2 x half> %b)
  ret <vscale x 4 x half> %res
}

define <vscale x 8 x half> @vector_interleave_nxv8f16_nxv4f16(<vscale x 4 x half> %a, <vscale x 4 x half> %b) {
; CHECK-LABEL: vector_interleave_nxv8f16_nxv4f16:
; CHECK:       # %bb.0:
; CHECK-NEXT:    vsetvli a0, zero, e16, m1, ta, ma
; CHECK-NEXT:    vwaddu.vv v10, v8, v9
; CHECK-NEXT:    li a0, -1
; CHECK-NEXT:    vwmaccu.vx v10, a0, v9
; CHECK-NEXT:    vmv2r.v v8, v10
; CHECK-NEXT:    ret
  %res = call <vscale x 8 x half> @llvm.experimental.vector.interleave2.nxv8f16(<vscale x 4 x half> %a, <vscale x 4 x half> %b)
  ret <vscale x 8 x half> %res
}

define <vscale x 4 x float> @vector_interleave_nxv4f32_nxv2f32(<vscale x 2 x float> %a, <vscale x 2 x float> %b) {
; CHECK-LABEL: vector_interleave_nxv4f32_nxv2f32:
; CHECK:       # %bb.0:
; CHECK-NEXT:    vsetvli a0, zero, e32, m1, ta, ma
; CHECK-NEXT:    vwaddu.vv v10, v8, v9
; CHECK-NEXT:    li a0, -1
; CHECK-NEXT:    vwmaccu.vx v10, a0, v9
; CHECK-NEXT:    vmv2r.v v8, v10
; CHECK-NEXT:    ret
  %res = call <vscale x 4 x float> @llvm.experimental.vector.interleave2.nxv4f32(<vscale x 2 x float> %a, <vscale x 2 x float> %b)
  ret <vscale x 4 x float> %res
}

define <vscale x 16 x half> @vector_interleave_nxv16f16_nxv8f16(<vscale x 8 x half> %a, <vscale x 8 x half> %b) {
; CHECK-LABEL: vector_interleave_nxv16f16_nxv8f16:
; CHECK:       # %bb.0:
; CHECK-NEXT:    vsetvli a0, zero, e16, m2, ta, ma
; CHECK-NEXT:    vwaddu.vv v12, v8, v10
; CHECK-NEXT:    li a0, -1
; CHECK-NEXT:    vwmaccu.vx v12, a0, v10
; CHECK-NEXT:    vmv4r.v v8, v12
; CHECK-NEXT:    ret
  %res = call <vscale x 16 x half> @llvm.experimental.vector.interleave2.nxv16f16(<vscale x 8 x half> %a, <vscale x 8 x half> %b)
  ret <vscale x 16 x half> %res
}

define <vscale x 8 x float> @vector_interleave_nxv8f32_nxv4f32(<vscale x 4 x float> %a, <vscale x 4 x float> %b) {
; CHECK-LABEL: vector_interleave_nxv8f32_nxv4f32:
; CHECK:       # %bb.0:
; CHECK-NEXT:    vsetvli a0, zero, e32, m2, ta, ma
; CHECK-NEXT:    vwaddu.vv v12, v8, v10
; CHECK-NEXT:    li a0, -1
; CHECK-NEXT:    vwmaccu.vx v12, a0, v10
; CHECK-NEXT:    vmv4r.v v8, v12
; CHECK-NEXT:    ret
  %res = call <vscale x 8 x float> @llvm.experimental.vector.interleave2.nxv8f32(<vscale x 4 x float> %a, <vscale x 4 x float> %b)
  ret <vscale x 8 x float> %res
}

define <vscale x 4 x double> @vector_interleave_nxv4f64_nxv2f64(<vscale x 2 x double> %a, <vscale x 2 x double> %b) {
; CHECK-LABEL: vector_interleave_nxv4f64_nxv2f64:
; CHECK:       # %bb.0:
; CHECK-NEXT:    csrr a0, vlenb
; CHECK-NEXT:    srli a0, a0, 2
; CHECK-NEXT:    vsetvli a1, zero, e16, m1, ta, mu
; CHECK-NEXT:    vid.v v12
; CHECK-NEXT:    vand.vi v13, v12, 1
; CHECK-NEXT:    vmsne.vi v0, v13, 0
; CHECK-NEXT:    vsrl.vi v16, v12, 1
; CHECK-NEXT:    vadd.vx v16, v16, a0, v0.t
; CHECK-NEXT:    vsetvli zero, zero, e64, m4, ta, ma
; CHECK-NEXT:    vrgatherei16.vv v12, v8, v16, v0.t
; CHECK-NEXT:    vmv.v.v v8, v12
; CHECK-NEXT:    ret
  %res = call <vscale x 4 x double> @llvm.experimental.vector.interleave2.nxv4f64(<vscale x 2 x double> %a, <vscale x 2 x double> %b)
  ret <vscale x 4 x double> %res
}


declare <vscale x 4 x half> @llvm.experimental.vector.interleave2.nxv4f16(<vscale x 2 x half>, <vscale x 2 x half>)
declare <vscale x 8 x half> @llvm.experimental.vector.interleave2.nxv8f16(<vscale x 4 x half>, <vscale x 4 x half>)
declare <vscale x 4 x float> @llvm.experimental.vector.interleave2.nxv4f32(<vscale x 2 x float>, <vscale x 2 x float>)
declare <vscale x 16 x half> @llvm.experimental.vector.interleave2.nxv16f16(<vscale x 8 x half>, <vscale x 8 x half>)
declare <vscale x 8 x float> @llvm.experimental.vector.interleave2.nxv8f32(<vscale x 4 x float>, <vscale x 4 x float>)
declare <vscale x 4 x double> @llvm.experimental.vector.interleave2.nxv4f64(<vscale x 2 x double>, <vscale x 2 x double>)

define <vscale x 64 x half> @vector_interleave_nxv64f16_nxv32f16(<vscale x 32 x half> %a, <vscale x 32 x half> %b) {
; CHECK-LABEL: vector_interleave_nxv64f16_nxv32f16:
; CHECK:       # %bb.0:
; CHECK-NEXT:    vmv8r.v v24, v8
; CHECK-NEXT:    vsetvli a0, zero, e16, m4, ta, ma
; CHECK-NEXT:    vwaddu.vv v8, v24, v16
; CHECK-NEXT:    li a0, -1
; CHECK-NEXT:    vwmaccu.vx v8, a0, v16
; CHECK-NEXT:    vwaddu.vv v0, v28, v20
; CHECK-NEXT:    vwmaccu.vx v0, a0, v20
; CHECK-NEXT:    vmv8r.v v16, v0
; CHECK-NEXT:    ret
  %res = call <vscale x 64 x half> @llvm.experimental.vector.interleave2.nxv64f16(<vscale x 32 x half> %a, <vscale x 32 x half> %b)
  ret <vscale x 64 x half> %res
}

define <vscale x 32 x float> @vector_interleave_nxv32f32_nxv16f32(<vscale x 16 x float> %a, <vscale x 16 x float> %b) {
; CHECK-LABEL: vector_interleave_nxv32f32_nxv16f32:
; CHECK:       # %bb.0:
; CHECK-NEXT:    vmv8r.v v24, v8
; CHECK-NEXT:    vsetvli a0, zero, e32, m4, ta, ma
; CHECK-NEXT:    vwaddu.vv v8, v24, v16
; CHECK-NEXT:    li a0, -1
; CHECK-NEXT:    vwmaccu.vx v8, a0, v16
; CHECK-NEXT:    vwaddu.vv v0, v28, v20
; CHECK-NEXT:    vwmaccu.vx v0, a0, v20
; CHECK-NEXT:    vmv8r.v v16, v0
; CHECK-NEXT:    ret
  %res = call <vscale x 32 x float> @llvm.experimental.vector.interleave2.nxv32f32(<vscale x 16 x float> %a, <vscale x 16 x float> %b)
  ret <vscale x 32 x float> %res
}

define <vscale x 16 x double> @vector_interleave_nxv16f64_nxv8f64(<vscale x 8 x double> %a, <vscale x 8 x double> %b) {
; CHECK-LABEL: vector_interleave_nxv16f64_nxv8f64:
; CHECK:       # %bb.0:
; CHECK-NEXT:    addi sp, sp, -16
; CHECK-NEXT:    .cfi_def_cfa_offset 16
; CHECK-NEXT:    csrr a0, vlenb
; CHECK-NEXT:    slli a0, a0, 4
; CHECK-NEXT:    sub sp, sp, a0
; CHECK-NEXT:    .cfi_escape 0x0f, 0x0d, 0x72, 0x00, 0x11, 0x10, 0x22, 0x11, 0x10, 0x92, 0xa2, 0x38, 0x00, 0x1e, 0x22 # sp + 16 + 16 * vlenb
; CHECK-NEXT:    csrr a0, vlenb
; CHECK-NEXT:    slli a0, a0, 3
; CHECK-NEXT:    add a0, sp, a0
; CHECK-NEXT:    addi a0, a0, 16
; CHECK-NEXT:    vs8r.v v8, (a0) # Unknown-size Folded Spill
; CHECK-NEXT:    csrr a0, vlenb
; CHECK-NEXT:    srli a0, a0, 1
; CHECK-NEXT:    vsetvli a1, zero, e16, m2, ta, mu
; CHECK-NEXT:    vid.v v24
; CHECK-NEXT:    vand.vi v26, v24, 1
; CHECK-NEXT:    vmsne.vi v0, v26, 0
; CHECK-NEXT:    vsrl.vi v2, v24, 1
; CHECK-NEXT:    csrr a1, vlenb
; CHECK-NEXT:    slli a1, a1, 3
; CHECK-NEXT:    add a1, sp, a1
; CHECK-NEXT:    addi a1, a1, 16
; CHECK-NEXT:    vl8r.v v8, (a1) # Unknown-size Folded Reload
; CHECK-NEXT:    vadd.vx v2, v2, a0, v0.t
; CHECK-NEXT:    vmv4r.v v12, v16
; CHECK-NEXT:    vsetvli zero, zero, e64, m8, ta, ma
; CHECK-NEXT:    vrgatherei16.vv v24, v8, v2, v0.t
; CHECK-NEXT:    addi a0, sp, 16
; CHECK-NEXT:    vs8r.v v24, (a0) # Unknown-size Folded Spill
; CHECK-NEXT:    csrr a0, vlenb
; CHECK-NEXT:    slli a0, a0, 3
; CHECK-NEXT:    add a0, sp, a0
; CHECK-NEXT:    addi a0, a0, 16
; CHECK-NEXT:    vl8r.v v8, (a0) # Unknown-size Folded Reload
; CHECK-NEXT:    vmv4r.v v16, v12
; CHECK-NEXT:    vrgatherei16.vv v24, v16, v2, v0.t
; CHECK-NEXT:    addi a0, sp, 16
; CHECK-NEXT:    vl8r.v v8, (a0) # Unknown-size Folded Reload
; CHECK-NEXT:    vmv.v.v v16, v24
; CHECK-NEXT:    csrr a0, vlenb
; CHECK-NEXT:    slli a0, a0, 4
; CHECK-NEXT:    add sp, sp, a0
; CHECK-NEXT:    addi sp, sp, 16
; CHECK-NEXT:    ret
  %res = call <vscale x 16 x double> @llvm.experimental.vector.interleave2.nxv16f64(<vscale x 8 x double> %a, <vscale x 8 x double> %b)
  ret <vscale x 16 x double> %res
}

declare <vscale x 64 x half> @llvm.experimental.vector.interleave2.nxv64f16(<vscale x 32 x half>, <vscale x 32 x half>)
declare <vscale x 32 x float> @llvm.experimental.vector.interleave2.nxv32f32(<vscale x 16 x float>, <vscale x 16 x float>)
declare <vscale x 16 x double> @llvm.experimental.vector.interleave2.nxv16f64(<vscale x 8 x double>, <vscale x 8 x double>)
