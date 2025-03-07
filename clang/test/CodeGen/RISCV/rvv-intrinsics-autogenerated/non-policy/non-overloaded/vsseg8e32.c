// NOTE: Assertions have been autogenerated by utils/update_cc_test_checks.py UTC_ARGS: --version 2
// REQUIRES: riscv-registered-target
// RUN: %clang_cc1 -triple riscv64 -target-feature +v -target-feature +zfh \
// RUN:   -target-feature +experimental-zvfh -disable-O0-optnone  \
// RUN:   -emit-llvm %s -o - | opt -S -passes=mem2reg | \
// RUN:   FileCheck --check-prefix=CHECK-RV64 %s

#include <riscv_vector.h>

// CHECK-RV64-LABEL: define dso_local void @test_vsseg8e32_v_f32mf2
// CHECK-RV64-SAME: (ptr noundef [[BASE:%.*]], <vscale x 1 x float> [[V0:%.*]], <vscale x 1 x float> [[V1:%.*]], <vscale x 1 x float> [[V2:%.*]], <vscale x 1 x float> [[V3:%.*]], <vscale x 1 x float> [[V4:%.*]], <vscale x 1 x float> [[V5:%.*]], <vscale x 1 x float> [[V6:%.*]], <vscale x 1 x float> [[V7:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0:[0-9]+]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    call void @llvm.riscv.vsseg8.nxv1f32.i64(<vscale x 1 x float> [[V0]], <vscale x 1 x float> [[V1]], <vscale x 1 x float> [[V2]], <vscale x 1 x float> [[V3]], <vscale x 1 x float> [[V4]], <vscale x 1 x float> [[V5]], <vscale x 1 x float> [[V6]], <vscale x 1 x float> [[V7]], ptr [[BASE]], i64 [[VL]])
// CHECK-RV64-NEXT:    ret void
//
void test_vsseg8e32_v_f32mf2(float *base, vfloat32mf2_t v0, vfloat32mf2_t v1, vfloat32mf2_t v2, vfloat32mf2_t v3, vfloat32mf2_t v4, vfloat32mf2_t v5, vfloat32mf2_t v6, vfloat32mf2_t v7, size_t vl) {
  return __riscv_vsseg8e32_v_f32mf2(base, v0, v1, v2, v3, v4, v5, v6, v7, vl);
}

// CHECK-RV64-LABEL: define dso_local void @test_vsseg8e32_v_f32m1
// CHECK-RV64-SAME: (ptr noundef [[BASE:%.*]], <vscale x 2 x float> [[V0:%.*]], <vscale x 2 x float> [[V1:%.*]], <vscale x 2 x float> [[V2:%.*]], <vscale x 2 x float> [[V3:%.*]], <vscale x 2 x float> [[V4:%.*]], <vscale x 2 x float> [[V5:%.*]], <vscale x 2 x float> [[V6:%.*]], <vscale x 2 x float> [[V7:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    call void @llvm.riscv.vsseg8.nxv2f32.i64(<vscale x 2 x float> [[V0]], <vscale x 2 x float> [[V1]], <vscale x 2 x float> [[V2]], <vscale x 2 x float> [[V3]], <vscale x 2 x float> [[V4]], <vscale x 2 x float> [[V5]], <vscale x 2 x float> [[V6]], <vscale x 2 x float> [[V7]], ptr [[BASE]], i64 [[VL]])
// CHECK-RV64-NEXT:    ret void
//
void test_vsseg8e32_v_f32m1(float *base, vfloat32m1_t v0, vfloat32m1_t v1, vfloat32m1_t v2, vfloat32m1_t v3, vfloat32m1_t v4, vfloat32m1_t v5, vfloat32m1_t v6, vfloat32m1_t v7, size_t vl) {
  return __riscv_vsseg8e32_v_f32m1(base, v0, v1, v2, v3, v4, v5, v6, v7, vl);
}

// CHECK-RV64-LABEL: define dso_local void @test_vsseg8e32_v_i32mf2
// CHECK-RV64-SAME: (ptr noundef [[BASE:%.*]], <vscale x 1 x i32> [[V0:%.*]], <vscale x 1 x i32> [[V1:%.*]], <vscale x 1 x i32> [[V2:%.*]], <vscale x 1 x i32> [[V3:%.*]], <vscale x 1 x i32> [[V4:%.*]], <vscale x 1 x i32> [[V5:%.*]], <vscale x 1 x i32> [[V6:%.*]], <vscale x 1 x i32> [[V7:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    call void @llvm.riscv.vsseg8.nxv1i32.i64(<vscale x 1 x i32> [[V0]], <vscale x 1 x i32> [[V1]], <vscale x 1 x i32> [[V2]], <vscale x 1 x i32> [[V3]], <vscale x 1 x i32> [[V4]], <vscale x 1 x i32> [[V5]], <vscale x 1 x i32> [[V6]], <vscale x 1 x i32> [[V7]], ptr [[BASE]], i64 [[VL]])
// CHECK-RV64-NEXT:    ret void
//
void test_vsseg8e32_v_i32mf2(int32_t *base, vint32mf2_t v0, vint32mf2_t v1, vint32mf2_t v2, vint32mf2_t v3, vint32mf2_t v4, vint32mf2_t v5, vint32mf2_t v6, vint32mf2_t v7, size_t vl) {
  return __riscv_vsseg8e32_v_i32mf2(base, v0, v1, v2, v3, v4, v5, v6, v7, vl);
}

// CHECK-RV64-LABEL: define dso_local void @test_vsseg8e32_v_i32m1
// CHECK-RV64-SAME: (ptr noundef [[BASE:%.*]], <vscale x 2 x i32> [[V0:%.*]], <vscale x 2 x i32> [[V1:%.*]], <vscale x 2 x i32> [[V2:%.*]], <vscale x 2 x i32> [[V3:%.*]], <vscale x 2 x i32> [[V4:%.*]], <vscale x 2 x i32> [[V5:%.*]], <vscale x 2 x i32> [[V6:%.*]], <vscale x 2 x i32> [[V7:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    call void @llvm.riscv.vsseg8.nxv2i32.i64(<vscale x 2 x i32> [[V0]], <vscale x 2 x i32> [[V1]], <vscale x 2 x i32> [[V2]], <vscale x 2 x i32> [[V3]], <vscale x 2 x i32> [[V4]], <vscale x 2 x i32> [[V5]], <vscale x 2 x i32> [[V6]], <vscale x 2 x i32> [[V7]], ptr [[BASE]], i64 [[VL]])
// CHECK-RV64-NEXT:    ret void
//
void test_vsseg8e32_v_i32m1(int32_t *base, vint32m1_t v0, vint32m1_t v1, vint32m1_t v2, vint32m1_t v3, vint32m1_t v4, vint32m1_t v5, vint32m1_t v6, vint32m1_t v7, size_t vl) {
  return __riscv_vsseg8e32_v_i32m1(base, v0, v1, v2, v3, v4, v5, v6, v7, vl);
}

// CHECK-RV64-LABEL: define dso_local void @test_vsseg8e32_v_u32mf2
// CHECK-RV64-SAME: (ptr noundef [[BASE:%.*]], <vscale x 1 x i32> [[V0:%.*]], <vscale x 1 x i32> [[V1:%.*]], <vscale x 1 x i32> [[V2:%.*]], <vscale x 1 x i32> [[V3:%.*]], <vscale x 1 x i32> [[V4:%.*]], <vscale x 1 x i32> [[V5:%.*]], <vscale x 1 x i32> [[V6:%.*]], <vscale x 1 x i32> [[V7:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    call void @llvm.riscv.vsseg8.nxv1i32.i64(<vscale x 1 x i32> [[V0]], <vscale x 1 x i32> [[V1]], <vscale x 1 x i32> [[V2]], <vscale x 1 x i32> [[V3]], <vscale x 1 x i32> [[V4]], <vscale x 1 x i32> [[V5]], <vscale x 1 x i32> [[V6]], <vscale x 1 x i32> [[V7]], ptr [[BASE]], i64 [[VL]])
// CHECK-RV64-NEXT:    ret void
//
void test_vsseg8e32_v_u32mf2(uint32_t *base, vuint32mf2_t v0, vuint32mf2_t v1, vuint32mf2_t v2, vuint32mf2_t v3, vuint32mf2_t v4, vuint32mf2_t v5, vuint32mf2_t v6, vuint32mf2_t v7, size_t vl) {
  return __riscv_vsseg8e32_v_u32mf2(base, v0, v1, v2, v3, v4, v5, v6, v7, vl);
}

// CHECK-RV64-LABEL: define dso_local void @test_vsseg8e32_v_u32m1
// CHECK-RV64-SAME: (ptr noundef [[BASE:%.*]], <vscale x 2 x i32> [[V0:%.*]], <vscale x 2 x i32> [[V1:%.*]], <vscale x 2 x i32> [[V2:%.*]], <vscale x 2 x i32> [[V3:%.*]], <vscale x 2 x i32> [[V4:%.*]], <vscale x 2 x i32> [[V5:%.*]], <vscale x 2 x i32> [[V6:%.*]], <vscale x 2 x i32> [[V7:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    call void @llvm.riscv.vsseg8.nxv2i32.i64(<vscale x 2 x i32> [[V0]], <vscale x 2 x i32> [[V1]], <vscale x 2 x i32> [[V2]], <vscale x 2 x i32> [[V3]], <vscale x 2 x i32> [[V4]], <vscale x 2 x i32> [[V5]], <vscale x 2 x i32> [[V6]], <vscale x 2 x i32> [[V7]], ptr [[BASE]], i64 [[VL]])
// CHECK-RV64-NEXT:    ret void
//
void test_vsseg8e32_v_u32m1(uint32_t *base, vuint32m1_t v0, vuint32m1_t v1, vuint32m1_t v2, vuint32m1_t v3, vuint32m1_t v4, vuint32m1_t v5, vuint32m1_t v6, vuint32m1_t v7, size_t vl) {
  return __riscv_vsseg8e32_v_u32m1(base, v0, v1, v2, v3, v4, v5, v6, v7, vl);
}

// CHECK-RV64-LABEL: define dso_local void @test_vsseg8e32_v_f32mf2_m
// CHECK-RV64-SAME: (<vscale x 1 x i1> [[MASK:%.*]], ptr noundef [[BASE:%.*]], <vscale x 1 x float> [[V0:%.*]], <vscale x 1 x float> [[V1:%.*]], <vscale x 1 x float> [[V2:%.*]], <vscale x 1 x float> [[V3:%.*]], <vscale x 1 x float> [[V4:%.*]], <vscale x 1 x float> [[V5:%.*]], <vscale x 1 x float> [[V6:%.*]], <vscale x 1 x float> [[V7:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    call void @llvm.riscv.vsseg8.mask.nxv1f32.i64(<vscale x 1 x float> [[V0]], <vscale x 1 x float> [[V1]], <vscale x 1 x float> [[V2]], <vscale x 1 x float> [[V3]], <vscale x 1 x float> [[V4]], <vscale x 1 x float> [[V5]], <vscale x 1 x float> [[V6]], <vscale x 1 x float> [[V7]], ptr [[BASE]], <vscale x 1 x i1> [[MASK]], i64 [[VL]])
// CHECK-RV64-NEXT:    ret void
//
void test_vsseg8e32_v_f32mf2_m(vbool64_t mask, float *base, vfloat32mf2_t v0, vfloat32mf2_t v1, vfloat32mf2_t v2, vfloat32mf2_t v3, vfloat32mf2_t v4, vfloat32mf2_t v5, vfloat32mf2_t v6, vfloat32mf2_t v7, size_t vl) {
  return __riscv_vsseg8e32_v_f32mf2_m(mask, base, v0, v1, v2, v3, v4, v5, v6, v7, vl);
}

// CHECK-RV64-LABEL: define dso_local void @test_vsseg8e32_v_f32m1_m
// CHECK-RV64-SAME: (<vscale x 2 x i1> [[MASK:%.*]], ptr noundef [[BASE:%.*]], <vscale x 2 x float> [[V0:%.*]], <vscale x 2 x float> [[V1:%.*]], <vscale x 2 x float> [[V2:%.*]], <vscale x 2 x float> [[V3:%.*]], <vscale x 2 x float> [[V4:%.*]], <vscale x 2 x float> [[V5:%.*]], <vscale x 2 x float> [[V6:%.*]], <vscale x 2 x float> [[V7:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    call void @llvm.riscv.vsseg8.mask.nxv2f32.i64(<vscale x 2 x float> [[V0]], <vscale x 2 x float> [[V1]], <vscale x 2 x float> [[V2]], <vscale x 2 x float> [[V3]], <vscale x 2 x float> [[V4]], <vscale x 2 x float> [[V5]], <vscale x 2 x float> [[V6]], <vscale x 2 x float> [[V7]], ptr [[BASE]], <vscale x 2 x i1> [[MASK]], i64 [[VL]])
// CHECK-RV64-NEXT:    ret void
//
void test_vsseg8e32_v_f32m1_m(vbool32_t mask, float *base, vfloat32m1_t v0, vfloat32m1_t v1, vfloat32m1_t v2, vfloat32m1_t v3, vfloat32m1_t v4, vfloat32m1_t v5, vfloat32m1_t v6, vfloat32m1_t v7, size_t vl) {
  return __riscv_vsseg8e32_v_f32m1_m(mask, base, v0, v1, v2, v3, v4, v5, v6, v7, vl);
}

// CHECK-RV64-LABEL: define dso_local void @test_vsseg8e32_v_i32mf2_m
// CHECK-RV64-SAME: (<vscale x 1 x i1> [[MASK:%.*]], ptr noundef [[BASE:%.*]], <vscale x 1 x i32> [[V0:%.*]], <vscale x 1 x i32> [[V1:%.*]], <vscale x 1 x i32> [[V2:%.*]], <vscale x 1 x i32> [[V3:%.*]], <vscale x 1 x i32> [[V4:%.*]], <vscale x 1 x i32> [[V5:%.*]], <vscale x 1 x i32> [[V6:%.*]], <vscale x 1 x i32> [[V7:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    call void @llvm.riscv.vsseg8.mask.nxv1i32.i64(<vscale x 1 x i32> [[V0]], <vscale x 1 x i32> [[V1]], <vscale x 1 x i32> [[V2]], <vscale x 1 x i32> [[V3]], <vscale x 1 x i32> [[V4]], <vscale x 1 x i32> [[V5]], <vscale x 1 x i32> [[V6]], <vscale x 1 x i32> [[V7]], ptr [[BASE]], <vscale x 1 x i1> [[MASK]], i64 [[VL]])
// CHECK-RV64-NEXT:    ret void
//
void test_vsseg8e32_v_i32mf2_m(vbool64_t mask, int32_t *base, vint32mf2_t v0, vint32mf2_t v1, vint32mf2_t v2, vint32mf2_t v3, vint32mf2_t v4, vint32mf2_t v5, vint32mf2_t v6, vint32mf2_t v7, size_t vl) {
  return __riscv_vsseg8e32_v_i32mf2_m(mask, base, v0, v1, v2, v3, v4, v5, v6, v7, vl);
}

// CHECK-RV64-LABEL: define dso_local void @test_vsseg8e32_v_i32m1_m
// CHECK-RV64-SAME: (<vscale x 2 x i1> [[MASK:%.*]], ptr noundef [[BASE:%.*]], <vscale x 2 x i32> [[V0:%.*]], <vscale x 2 x i32> [[V1:%.*]], <vscale x 2 x i32> [[V2:%.*]], <vscale x 2 x i32> [[V3:%.*]], <vscale x 2 x i32> [[V4:%.*]], <vscale x 2 x i32> [[V5:%.*]], <vscale x 2 x i32> [[V6:%.*]], <vscale x 2 x i32> [[V7:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    call void @llvm.riscv.vsseg8.mask.nxv2i32.i64(<vscale x 2 x i32> [[V0]], <vscale x 2 x i32> [[V1]], <vscale x 2 x i32> [[V2]], <vscale x 2 x i32> [[V3]], <vscale x 2 x i32> [[V4]], <vscale x 2 x i32> [[V5]], <vscale x 2 x i32> [[V6]], <vscale x 2 x i32> [[V7]], ptr [[BASE]], <vscale x 2 x i1> [[MASK]], i64 [[VL]])
// CHECK-RV64-NEXT:    ret void
//
void test_vsseg8e32_v_i32m1_m(vbool32_t mask, int32_t *base, vint32m1_t v0, vint32m1_t v1, vint32m1_t v2, vint32m1_t v3, vint32m1_t v4, vint32m1_t v5, vint32m1_t v6, vint32m1_t v7, size_t vl) {
  return __riscv_vsseg8e32_v_i32m1_m(mask, base, v0, v1, v2, v3, v4, v5, v6, v7, vl);
}

// CHECK-RV64-LABEL: define dso_local void @test_vsseg8e32_v_u32mf2_m
// CHECK-RV64-SAME: (<vscale x 1 x i1> [[MASK:%.*]], ptr noundef [[BASE:%.*]], <vscale x 1 x i32> [[V0:%.*]], <vscale x 1 x i32> [[V1:%.*]], <vscale x 1 x i32> [[V2:%.*]], <vscale x 1 x i32> [[V3:%.*]], <vscale x 1 x i32> [[V4:%.*]], <vscale x 1 x i32> [[V5:%.*]], <vscale x 1 x i32> [[V6:%.*]], <vscale x 1 x i32> [[V7:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    call void @llvm.riscv.vsseg8.mask.nxv1i32.i64(<vscale x 1 x i32> [[V0]], <vscale x 1 x i32> [[V1]], <vscale x 1 x i32> [[V2]], <vscale x 1 x i32> [[V3]], <vscale x 1 x i32> [[V4]], <vscale x 1 x i32> [[V5]], <vscale x 1 x i32> [[V6]], <vscale x 1 x i32> [[V7]], ptr [[BASE]], <vscale x 1 x i1> [[MASK]], i64 [[VL]])
// CHECK-RV64-NEXT:    ret void
//
void test_vsseg8e32_v_u32mf2_m(vbool64_t mask, uint32_t *base, vuint32mf2_t v0, vuint32mf2_t v1, vuint32mf2_t v2, vuint32mf2_t v3, vuint32mf2_t v4, vuint32mf2_t v5, vuint32mf2_t v6, vuint32mf2_t v7, size_t vl) {
  return __riscv_vsseg8e32_v_u32mf2_m(mask, base, v0, v1, v2, v3, v4, v5, v6, v7, vl);
}

// CHECK-RV64-LABEL: define dso_local void @test_vsseg8e32_v_u32m1_m
// CHECK-RV64-SAME: (<vscale x 2 x i1> [[MASK:%.*]], ptr noundef [[BASE:%.*]], <vscale x 2 x i32> [[V0:%.*]], <vscale x 2 x i32> [[V1:%.*]], <vscale x 2 x i32> [[V2:%.*]], <vscale x 2 x i32> [[V3:%.*]], <vscale x 2 x i32> [[V4:%.*]], <vscale x 2 x i32> [[V5:%.*]], <vscale x 2 x i32> [[V6:%.*]], <vscale x 2 x i32> [[V7:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    call void @llvm.riscv.vsseg8.mask.nxv2i32.i64(<vscale x 2 x i32> [[V0]], <vscale x 2 x i32> [[V1]], <vscale x 2 x i32> [[V2]], <vscale x 2 x i32> [[V3]], <vscale x 2 x i32> [[V4]], <vscale x 2 x i32> [[V5]], <vscale x 2 x i32> [[V6]], <vscale x 2 x i32> [[V7]], ptr [[BASE]], <vscale x 2 x i1> [[MASK]], i64 [[VL]])
// CHECK-RV64-NEXT:    ret void
//
void test_vsseg8e32_v_u32m1_m(vbool32_t mask, uint32_t *base, vuint32m1_t v0, vuint32m1_t v1, vuint32m1_t v2, vuint32m1_t v3, vuint32m1_t v4, vuint32m1_t v5, vuint32m1_t v6, vuint32m1_t v7, size_t vl) {
  return __riscv_vsseg8e32_v_u32m1_m(mask, base, v0, v1, v2, v3, v4, v5, v6, v7, vl);
}

