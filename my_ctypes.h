#ifndef __MY_C_TYPES_H__
#define __MY_C_TYPES_H__

#include "stdint.h"
#include "stddef.h"


// all types. Commit this if types defines upper--------------------------

typedef void            u0; // do not check this type because void type is not exist, error type
typedef uint8_t         u8;
typedef uint16_t        u16;
typedef uint32_t        u24;
typedef uint32_t        u32;
typedef uint64_t        u64;

typedef char            c;
typedef int8_t          i8;
typedef int16_t         i16;
typedef int32_t         i24;
typedef int32_t         i32;
typedef int64_t         i64;

typedef float           f32;
typedef double          f64;
typedef long double     f128;

typedef unsigned char   b;

// bus types defining ----------------------------------------------
typedef size_t reg;
//------------------------------------------------------------------


// types mapping ------------------------------------------
#define VOID_TYPE 			((u8) 0)
// unsigned type
#define UINT8_TYPE  		((u8) 1)
#define UINT16_TYPE 		((u8) 2)
#define UINT32_TYPE 		((u8) 3)
#define UINT64_TYPE 		((u8) 4)
//signed type
#define CHAR_TYPE 			((u8) 5)
#define INT8_TYPE 			((u8) 6)
#define INT16_TYPE 			((u8) 7)
#define INT32_TYPE 			((u8) 8)
#define INT64_TYPE 			((u8) 9)
// floating point type
#define FLOAT_TYPE 			((u8) 10)
#define DOUBLE_TYPE 		((u8) 11)
#define LONG_DOUBLE_TYPE 	((u8) 12)
// other types
#define BOOL_TYPE 			((u8) 13)
#define UINT24_TYPE 		((u8) 14)
#define INT24_TYPE 			((u8) 15)
#define REG_TYPE 			((u8) 16)
#define TYPE_ARRAY_LENGTH   ((uint8_t)(REG_TYPE + 1))
//--------------------------------------------------------


/*
 * *************************************************
 * Define user bit operations
 * *************************************************
 */

#define MY_TYPE_SET_BIT(to, pos, type) (to) |= ((type)(1UL << (pos)))
#define MY_TYPE_RESET_BIT(to, pos, type) (to) &= ~((type)(1UL << (pos)))
#define MY_TYPE_TOGGLE_BIT(to, pos, type) (to) ^= (type)(1UL << (pos))
#define MY_TYPE_CHECK_BIT(from, pos) ((from) >> (pos)) & 1UL
#define MY_TYPE_WRITE_BIT(to, pos, value, type) (to) = (type)(((to) & ~(1UL << (pos))) | ((value) << (pos)))
//*********************************************


// type len getting ------------------------------------------------------------------------------------------------
extern const u8 typeLengthMappingArray[TYPE_ARRAY_LENGTH];

// getting length of type
inline u8 getMyTypeLen_ptr(u8* type) {
    if((*type) < TYPE_ARRAY_LENGTH) {
        return typeLengthMappingArray[*type];
    }
    return 0;
}

inline u8 getMYTypeLen(u8 type) {
    if(type < TYPE_ARRAY_LENGTH) {
        return typeLengthMappingArray[type];
    }
    return 0;
}


/*
 * ******************************************************************************
 * USER types check do not change !!!!!
 * ******************************************************************************
 */

// strict type control (PRECOMPILE)------------------------------------------------
#if (__STDC_VERSION__ >= 201112L) // if C version equal or more than C11
#include "assert.h"

// unsigned
static_assert((sizeof(u8) == 1),    "MY_CTYPES: size of uint8   type must be equal 1, change --> my_types.h: typedef u8 ");
static_assert((sizeof(u16) == 2),   "MY_CTYPES: size of uint16  type must be equal 2, change --> my_types.h: typedef u16 ");
static_assert((sizeof(u24) == 4),   "MY_CTYPES: size of uint24  type must be equal 4, change --> my_types.h: typedef u24 ");
static_assert((sizeof(u32) == 4),   "MY_CTYPES: size of uint32  type must be equal 4, change --> my_types.h: typedef u32 ");
static_assert((sizeof(u64) == 8),   "MY_CTYPES: size of uint64  type must be equal 8, change --> my_types.h: typedef u64 ");

// signed
static_assert((sizeof(c) == 1),     "MY_CTYPES: size of char     type must be equal 1, change --> my_types.h: typedef c ");
static_assert((sizeof(i8) == 1),    "MY_CTYPES: size of int8     type must be equal 1, change --> my_types.h: typedef i8 ");
static_assert((sizeof(i16) == 2),   "MY_CTYPES: size of int16    type must be equal 2, change --> my_types.h: typedef i16 ");
static_assert((sizeof(i24) == 4),   "MY_CTYPES: size of int24    type must be equal 4, change --> my_types.h: typedef i24 ");
static_assert((sizeof(i32) == 4),   "MY_CTYPES: size of int32    type must be equal 4, change --> my_types.h: typedef i32 ");
static_assert((sizeof(i64) == 8),   "MY_CTYPES: size of int64    type must be equal 8, change --> my_types.h: typedef i64 ");

// floating point
static_assert((sizeof(f32) == 4),   "MY_CTYPES: size of float         type must be equal 4, change --> my_types.h: typedef f32 ");
static_assert((sizeof(f64) == 8),   "MY_CTYPES: size of double        type must be equal 8, change --> my_types.h: typedef f64 ");
static_assert((sizeof(f128) == 16), "MY_CTYPES: size of long double   type must be equal 16, change --> my_types.h: typedef f128 ");

#else // if old version C
#define C99MY_CTYPES_STATIC_ASSERTION_CREATE(COND,MSG) typedef int my_crc_static_assertion_##MSG[(COND)? 1 : -1] // define custom static assertion if version C less than C11
//--------------------------------------------------------------------------------------------------------------

// unsigned
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(u8)  == 1), size_of_uint8_type_must_be_equal_1_change_typedef_u8);
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(u16) == 2), size_of_uint16_type_must_be_equal_2_change_typedef_u16);
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(u24) == 4), size_of_uint24_type_must_be_equal_4_change_typedef_u24);
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(u32) == 4), size_of_uint32_type_must_be_equal_4_change_typedef_u32);
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(u64) == 8), size_of_uint64_type_must_be_equal_8_change_typedef_u64);

// signed
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(c) == 1), size_of_char_type_must_be_equal_1_change_typedef_c);
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(i8) == 1), size_of_int8_type_must_be_equal_1_change_typedef_i8);
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(i16) == 2), size_of_int16_type_must_be_equal_2_change_typedef_i16);
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(i24) == 4), size_of_int24_type_must_be_equal_4_change_typedef_i24);
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(i32) == 4), size_of_int32_type_must_be_equal_4_change_typedef_i32);
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(i64) == 8), size_of_int64_type_must_be_equal_8_change_typedef_i64);

// floating point
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(f32) == 4), size_of_float_type_must_be_equal_4_change_typedef_f32);
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(f64) == 8), size_of_double_type_must_be_equal_8_change_typedef_f64);
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(f128) == 16), size_of_long_double_type_must_be_equal_16_change_typedef_f128);

// other type
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(b) == 1), size_of_long_bool_type_must_be_equal_1_change_typedef_b);

//--------------------------------------------------------------------------------------------------------------
#undef C99MY_CTYPES_STATIC_ASSERTION_CREATE
#endif /* (__STDC_VERSION__ >= 201112L) */


#endif // MY_TYPES_H
