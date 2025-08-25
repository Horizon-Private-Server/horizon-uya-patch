/***************************************************
 * FILENAME :		math3d.h
 * DESCRIPTION :
 * NOTES :
 *          Many functions are pulled from the PS2SDK.
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# (c) 2005 Naomi Peori <naomi@peori.ca>
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

#ifndef _LIBUYA_MATH3D_H_
#define _LIBUYA_MATH3D_H_

//--------------------------------------------------------
typedef float VECTOR[4] __attribute__((__aligned__(16)));
typedef float vec2[2] __attribute__((__aligned__(16)));
typedef float vec3[3] __attribute__((__aligned__(16)));
typedef float vec4[4] __attribute__((__aligned__(16)));
typedef float MATRIX[16] __attribute__((__aligned__(16)));

typedef struct vec4f { // 0x10
/* 0x0 */ float x;
/* 0x4 */ float z;
/* 0x8 */ float y;
/* 0xc */ float w;
} vec4f;

typedef struct mtx3 { // 0x30
/* 0x00 */ VECTOR v0;
/* 0x10 */ VECTOR v1;
/* 0x20 */ VECTOR v2;
} mtx3;

typedef struct mtx4 { // 0x40
/* 0x00 */ VECTOR v0;
/* 0x10 */ VECTOR v1;
/* 0x20 */ VECTOR v2;
/* 0x30 */ VECTOR v3;
} mtx4;

//--------------------------------------------------------
void vector_write(VECTOR output, u128 input0);
u128 vector_read(VECTOR input0);
float vector_sqrmag(VECTOR input0);
void vector_reflect(VECTOR output, VECTOR input0, VECTOR normal);
void vector_fromyaw(VECTOR out, float radians);
void vector_apply(VECTOR output, VECTOR input0, MATRIX input1);
void vector_copy(VECTOR output, VECTOR input0);
void vector_normalize(VECTOR output, VECTOR input0);
float vector_innerproduct_unscaled(VECTOR input0, VECTOR input1);
float vector_innerproduct(VECTOR input0, VECTOR input1);
void vector_outerproduct(VECTOR output, VECTOR input0, VECTOR input1);
float vector_length(VECTOR input0);
void vector_lerp(VECTOR output, VECTOR input0, VECTOR input1, float t);
void vector_subtract(VECTOR output, VECTOR input0, VECTOR input1);
void vector_add(VECTOR output, VECTOR input0, VECTOR input1);
void vector_negate(VECTOR output, VECTOR input0);
void vector_scale(VECTOR output, VECTOR input0, float scalar);
void vector_multiply(VECTOR output, VECTOR input0, VECTOR input1);
void vector_projectonvertical(VECTOR output, VECTOR input0);
void vector_projectonhorizontal(VECTOR output, VECTOR input0);
void vector_print(VECTOR input0);
void vector_transform(VECTOR output, VECTOR input, MATRIX matrix);

void matrix_toeuler(VECTOR output, MATRIX input0);
void matrix_fromrows(MATRIX output, VECTOR input0, VECTOR input1, VECTOR input2, VECTOR input3);
void matrix_copy(MATRIX output, MATRIX input0);
void matrix_inverse(MATRIX output, MATRIX input0);
void matrix_multiply(MATRIX output, MATRIX input0, MATRIX input1);
void matrix_rotate_x(MATRIX output, MATRIX input0, float radians);
void matrix_rotate_y(MATRIX output, MATRIX input0, float radians);
void matrix_rotate_z(MATRIX output, MATRIX input0, float radians);
void matrix_scale(MATRIX output, MATRIX input0, VECTOR input1);
void matrix_translate(MATRIX output, MATRIX input0, VECTOR input1);
void matrix_transpose(MATRIX output, MATRIX input0);
void matrix_unit(MATRIX output);
float matrix_determinant(MATRIX input0);

#endif // _LIBUYA_MATH3D_H_
