#ifndef __MY_C_TYPES_H__
#define __MY_C_TYPES_H__

#include <stddef.h>
#include <stdint.h>

#include "inline.h"


// all types. Commit this if types defines upper--------------------------

typedef void            uni; // do not check this type because void type is not exist, error type
typedef uint8_t         u8;
typedef uint16_t        u16;
typedef uint32_t        u24;
typedef uint32_t        u32;
typedef uint64_t        u64;

typedef char            c8;
typedef int8_t          i8;
typedef int16_t         i16;
typedef int32_t         i24;
typedef int32_t         i32;
typedef int64_t         i64;

typedef float           f32;
typedef double          f64;
typedef long double     f128; // platform depend type

typedef unsigned char   b;

// bus types defining (platform depend) ----------------------------------------------
typedef size_t      reg;    // platform depend type
typedef ptrdiff_t   sreg;   // platform depend type
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
#define LONG_DOUBLE_TYPE 	((u8) 12) // platform depend type
// other types
#define BOOL_TYPE 			((u8) 13)
#define UINT24_TYPE 		((u8) 14)
#define INT24_TYPE 			((u8) 15)
#define REG_TYPE 			((u8) 16) // platform depend type
#define SREG_TYPE 			((u8) 17) // platform depend type
#define TYPE_ARRAY_LENGTH   ((u8)(SREG_TYPE + 1))
//--------------------------------------------------------


/*
 * *************************************************
 * Define user copy
 * *************************************************
 */
forceinline void MY_CTYPE_USER_DATA_MEMCPY(u8 n, u8* from, u8* to)
{
    while((n)--) {
        *(to)++ = *(from)++;
    }
}

forceinline void MY_CTYPE_USER_DATA_REVCPY(u8 n, u8* from, u8* to)
{
    while((n)--) {
        *(to)++ = *((from) + (n));
    }
}


/*
 * *************************************************
 * Define user copy
 * WARNING!!! this function without direction
 * and platform dependent
 * *************************************************
 */
forceinline void MY_CTYPE_USER_DATA_COPY_REGISTER(u8* from, u8* to)
{
    *(to) = *(from);
}

/*
 * *************************************************
 * Define user bit operations
 * *************************************************
 */

#define MY_CTYPE_SET_BIT(to, pos, type) (to) |= ((type)(1UL << (pos)))
#define MY_CTYPE_RESET_BIT(to, pos, type) (to) &= ~((type)(1UL << (pos)))
#define MY_CTYPE_TOGGLE_BIT(to, pos, type) (to) ^= (type)(1UL << (pos))
#define MY_CTYPE_CHECK_BIT(from, pos) ((from) >> (pos)) & 1UL
#define MY_CTYPE_WRITE_BIT(to, pos, value, type) (to) = (type)(((to) & ~(1UL << (pos))) | ((value) << (pos)))
//*********************************************

extern const u8 typeLengthMappingArray[TYPE_ARRAY_LENGTH];


forceinline const u8* myCTypeGetTablePointer()
{
    return &typeLengthMappingArray[VOID_TYPE];
}


forceinline u8 getMYCTypeLen(u8 type) {
    if(type < TYPE_ARRAY_LENGTH) {
        return typeLengthMappingArray[type];
    }
    return ((u8)0);
}


// copy types -------------------------------------------------
void myCTypeMemcpy(u8 type, u8* from, u8* to);
void myCTypeRevcpy(u8 type, u8* from, u8* to);
// copy sizeof -------------------------------------------------
void myMemcpy(u8 n, u8* from, u8* to);
void myRevcpy(u8 n, u8* from, u8* to);
// init data type ---------------------------------------
void myCTypePointerInit(u8 type, u8* ptr);
// init data sizeof ---------------------------------------
void myPointerInit(u8 n, u8* ptr);
// string compleate------------------------------------------
u8 myStrnCmp(u8 n, const c8* str1, const c8* str2);


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
static_assert((sizeof(c8) == 1),    "MY_CTYPES: size of char     type must be equal 1, change --> my_types.h: typedef c8 ");
static_assert((sizeof(i8) == 1),    "MY_CTYPES: size of int8     type must be equal 1, change --> my_types.h: typedef i8 ");
static_assert((sizeof(i16) == 2),   "MY_CTYPES: size of int16    type must be equal 2, change --> my_types.h: typedef i16 ");
static_assert((sizeof(i24) == 4),   "MY_CTYPES: size of int24    type must be equal 4, change --> my_types.h: typedef i24 ");
static_assert((sizeof(i32) == 4),   "MY_CTYPES: size of int32    type must be equal 4, change --> my_types.h: typedef i32 ");
static_assert((sizeof(i64) == 8),   "MY_CTYPES: size of int64    type must be equal 8, change --> my_types.h: typedef i64 ");

// floating point
static_assert((sizeof(f32) == 4),   "MY_CTYPES: size of float         type must be equal 4, change --> my_types.h: typedef f32 ");
static_assert((sizeof(f64) == 8),   "MY_CTYPES: size of double        type must be equal 8, change --> my_types.h: typedef f64 ");
//static_assert((sizeof(f128) == 16), "MY_CTYPES: size of long double   type must be equal 16, change --> my_types.h: typedef f128 "); // platform depend

#else // if old version C
#define C99MY_CTYPES_STATIC_ASSERTION_CREATE(COND,MSG) typedef int my_ctype_static_assertion_##MSG[(COND)? 1 : -1] // define custom static assertion if version C less than C11
//--------------------------------------------------------------------------------------------------------------

// unsigned
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(u8)  == 1), size_of_uint8_type_must_be_equal_1_change_typedef_u8);
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(u16) == 2), size_of_uint16_type_must_be_equal_2_change_typedef_u16);
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(u24) == 4), size_of_uint24_type_must_be_equal_4_change_typedef_u24);
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(u32) == 4), size_of_uint32_type_must_be_equal_4_change_typedef_u32);
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(u64) == 8), size_of_uint64_type_must_be_equal_8_change_typedef_u64);

// signed
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(c8) == 1), size_of_char_type_must_be_equal_1_change_typedef_c8);
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(i8) == 1), size_of_int8_type_must_be_equal_1_change_typedef_i8);
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(i16) == 2), size_of_int16_type_must_be_equal_2_change_typedef_i16);
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(i24) == 4), size_of_int24_type_must_be_equal_4_change_typedef_i24);
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(i32) == 4), size_of_int32_type_must_be_equal_4_change_typedef_i32);
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(i64) == 8), size_of_int64_type_must_be_equal_8_change_typedef_i64);

// floating point
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(f32) == 4), size_of_float_type_must_be_equal_4_change_typedef_f32);
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(f64) == 8), size_of_double_type_must_be_equal_8_change_typedef_f64);
//C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(f128) == 16), size_of_long_double_type_must_be_equal_16_change_typedef_f128); // platform depend

// other type
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(b) == 1), size_of_long_bool_type_must_be_equal_1_change_typedef_b);


//--------------------------------------------------------------------------------------------------------------
#undef C99MY_CTYPES_STATIC_ASSERTION_CREATE
#endif /* (__STDC_VERSION__ >= 201112L) */


#endif // MY_TYPES_H
