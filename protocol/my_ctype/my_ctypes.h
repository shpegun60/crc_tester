#ifndef __MY_C_TYPES_H__
#define __MY_C_TYPES_H__ 1

#include <stddef.h>
#include <stdint.h>

#include "inline.h"
#include "my_ctype_id.h"

/************************************************************************************
 *  null pointer macro if not declarated upper
 */
#ifndef NULL
#define NULL ((void *)0)
#endif /* NULL */

#ifndef MIN
    #define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif /* MIN */

#ifndef MAX
    #define MAX(a,b) (((a) < (b)) ? (b) : (a))
#endif /* MAX */

#ifndef ABS
    #define ABS(N) (((N) < 0.0) ? (-1.0 * (N)) : (N))
#endif /* ABS */

/************************************************************************************
 *  macro for cast null pointer to user type
 */
#ifndef NULLPTR
    #define NULLPTR(type) ((type)NULL)
#endif /* NULLPTR */

/************************************************************************************
 *  macro for find sizeof struct-field without declare object
 *  (evaluation at compile time)
 *
 */
#ifndef SIZEOF_STRUCT
    #define SIZEOF_STRUCT(structName, field) ((size_t) sizeof(((structName *)0)->field))
#endif /* SIZEOF_STRUCT */

/************************************************************************************
 *  macro for find typeof struct-field without declare object
 *  WARNING!!! Do not change this macro if you use GNU-C compiler, change operator typeof to __typeof__ if you use ISO-C compiler!!!
 *  OR use only __typeof__ operator because it supported on GNU and ISO compilers
 *
 *  Type evaluation at compile time for type abstraction in C
 */
#ifndef TYPEOF_STRUCT
    #define TYPEOF_STRUCT(structName, field) __typeof__(((structName *)0)->field)
#endif /* TYPEOF_STRUCT */

/************************************************************************************
 *  macro for find typeof from user value
 *  WARNING!!! Do not change this macro if you use GNU-C compiler, change operator typeof to __typeof__ if you use ISO-C compiler!!!
 *  OR use only __typeof__ operator because it supported on GNU and ISO
 *
 *  Type evaluation at compile time
 */
#ifndef TYPEOF_DATA
    #define TYPEOF_DATA(data) __typeof__(data)
#endif /* TYPEOF_DATA */


/************************************************************************************
 *  macro for declare unused variable in function
 */
#ifndef UNUSED
    #define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */
#endif /* UNUSED */


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

// bool type -------------------------
typedef unsigned char   b;

#define CTYPE_FALSE (b)(0 != 0)
#define CTYPE_TRUE  (b)(0 == 0)

// bus types defining (platform depend) ----------------------------------------------
typedef size_t      reg;    // platform depend type
typedef ptrdiff_t   sreg;   // platform depend type
//------------------------------------------------------------------------------------


/************************************************************************************
 *  macro for user bit operations
 */
#define MY_CTYPE_SET_BIT(to, pos, type) (to) |= ((type)(1UL << (pos)))
#define MY_CTYPE_RESET_BIT(to, pos, type) (to) &= ~((type)(1UL << (pos)))
#define MY_CTYPE_TOGGLE_BIT(to, pos, type) (to) ^= (type)(1UL << (pos))
#define MY_CTYPE_CHECK_BIT(from, pos) (((from) >> (pos)) & 1UL)
#define MY_CTYPE_WRITE_BIT(to, pos, value, type) (to) = (type)(((to) & ~(1UL << (pos))) | ((value) << (pos)))
// register operations
#define MY_CTYPE_REG_SET_BIT(_r, _b)    (*((volatile reg*)(_r)) |=  (_b))
#define MY_CTYPE_REG_CLR_BIT(_r, _b)    (*((volatile reg*)(_r)) &= ~(_b))
#define MY_CTYPE_REG_SET(_r, _d)        (*((volatile reg*)(_r))  =  (_d))
#define MY_CTYPE_REG_CPY(_r, _d)        (*((volatile reg*)(_r))  =  *((volatile reg*)(_d)))


/************************************************************************************
 *  Extern array which contain`s size for all types
 */
extern const reg typeLengthMappingArray[TYPE_ARRAY_LENGTH];


/************************************************************************************
 *  Macro for user copy
 */
STATIC_FORCEINLINE void MY_CTYPE_USER_DATA_MEMCPY(reg n, u8* from, u8* to)
{
    while((n)--) {
        *(to)++ = *(from)++;
    }
}

STATIC_FORCEINLINE void MY_CTYPE_USER_DATA_REVCPY(reg n, u8* from, u8* to)
{
    while((n)--) {
        *(to)++ = *((from) + (n));
    }
}

/************************************************************************************
 *  Macro from linux
 */
STATIC_FORCEINLINE void MY_CTYPE_WRITE_ONCE_SIZE(volatile void *p, void *res, reg size)
{
    switch (size) {
    case 1: *(volatile u8  *)p = *(u8  *)res; break;
    case 2: *(volatile u16 *)p = *(u16 *)res; break;
    case 4: *(volatile u32 *)p = *(u32 *)res; break;
    case 8: *(volatile u64 *)p = *(u64 *)res; break;
    default:
            //barrier(); /// need block all---------------------------
            MY_CTYPE_USER_DATA_MEMCPY(size, (u8*)res, (u8*)p);
            //barrier(); /// need unblock all-------------------------
    }
}

STATIC_FORCEINLINE void MY_CTYPE_READ_ONCE_SIZE(const volatile void *p, void *res, reg size)
{
    switch (size) {
    case 1: *(u8  *)res = *(volatile u8  *)p; break;
    case 2: *(u16 *)res = *(volatile u16 *)p; break;
    case 4: *(u32 *)res = *(volatile u32 *)p; break;
    case 8: *(u64 *)res = *(volatile u64 *)p; break;
    default:
            //barrier(); /// need block all---------------------------
            MY_CTYPE_USER_DATA_MEMCPY(size, (u8*)p, (u8*)res);
            //barrier(); /// need unblock all-------------------------
    }
}



/*
 * *************************************************
 * Define user copy
 * WARNING!!! this function without direction
 * and platform dependent
 * *************************************************
 */
STATIC_FORCEINLINE void MY_CTYPE_COPY_REGISTERS(volatile reg* from, volatile reg* to)
{
    MY_CTYPE_REG_CPY(to, from);
}

STATIC_FORCEINLINE void MY_CTYPE_REVCOPY_REGISTERS(volatile reg* from, volatile reg* to)
{
    volatile reg rev;
    volatile reg cpy = *(from);

    MY_CTYPE_USER_DATA_REVCPY(sizeof(reg), (u8*)&cpy, (u8*)&rev);
    MY_CTYPE_REG_CPY(to, rev);
}

STATIC_FORCEINLINE const reg* myCTypeGetTablePointer()
{
    return &typeLengthMappingArray[VOID_TYPE];
}


STATIC_FORCEINLINE reg getMYCTypeLen(reg type)
{
    if(type < TYPE_ARRAY_LENGTH) {
        return typeLengthMappingArray[type];
    }
    return 0;
}



// copy types -------------------------------------------------
void cTypeMemcpy(reg type, u8* from, u8* to);
void cTypeRevcpy(reg type, u8* from, u8* to);

// init data type ---------------------------------------
void cTypePointerInit(reg type, u8* ptr);

// init data sizeof ---------------------------------------
void pointerInit(reg n, u8* ptr);
// fill data sizeof ---------------------------------------
void pointerFill(reg n, u8* ptr, u8 data);

// string compleate------------------------------------------
int cTypeStrnCmp(reg n, const c8* str1, const c8* str2);
// string reverse compleate------------------------------------------
int cTypeStrnRevCmp(reg n, const c8* str1, const c8* str2);



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

// bool type
static_assert((sizeof(b) == 1),    "MY_CTYPES: size of bool     type must be equal 1, change --> my_types.h: typedef b ");

// reg types
static_assert((sizeof(reg) == sizeof(uni*)),   "MY_CTYPES: size of reg            type must be equal pointer size, change --> my_types.h: typedef reg ");
static_assert((sizeof(sreg) == sizeof(uni*)),   "MY_CTYPES: size of sreg            type must be equal pointer size, change --> my_types.h: typedef sreg ");

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

// bool type
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(b) == 1), size_of_bool_type_must_be_equal_1_change_typedef_b);

// reg types
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(reg) == sizeof(uni*)), size_of_reg_type_must_be_equal_pointer_size_change_typedef_reg);
C99MY_CTYPES_STATIC_ASSERTION_CREATE((sizeof(sreg) == sizeof(uni*)), size_of_sreg_type_must_be_equal_pointer_size_change_typedef_sreg);


//--------------------------------------------------------------------------------------------------------------
#undef C99MY_CTYPES_STATIC_ASSERTION_CREATE
#endif /* (__STDC_VERSION__ >= 201112L) */


#endif // MY_TYPES_H
