#ifndef __BYTE_ORDER_H__
#define __BYTE_ORDER_H__ 1

#include "my_ctypes.h"
#include "preprocessor_concatenation.h"

#ifndef BYTE_ORDER_TEST_DISABLE
    //#define BYTE_ORDER_TEST_DISABLE
#endif /* BYTE_ORDER_TEST_DISABLE */


#if defined (__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && defined(__ORDER_BIG_ENDIAN__) && defined(__ORDER_PDP_ENDIAN__) && !defined(MY_BYTE_ORDER_DEFINED)
    #define MY_BYTE_ORDER_DEFINED

    #define MY_LITTLE_ENDIAN   __ORDER_LITTLE_ENDIAN__
    #define MY_BIG_ENDIAN      __ORDER_BIG_ENDIAN__
    #define MY_PDP_ENDIAN      __ORDER_PDP_ENDIAN__
    #define MY_ENDIAN_ORDER    __BYTE_ORDER__

#elif !defined(MY_BYTE_ORDER_DEFINED) /* if not exists system byte-order macro, define other hack */

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wmultichar" // disable warning: multi-character character constant [-Wmultichar]


    #define MY_BYTE_ORDER_DEFINED

    #define MY_LITTLE_ENDIAN   0x41424344UL
    #define MY_BIG_ENDIAN      0x44434241UL
    #define MY_PDP_ENDIAN      0x42414443UL
    #define MY_ENDIAN_ORDER    ('ABCD')

#endif /* defined (__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && defined(__ORDER_BIG_ENDIAN__) && defined(__ORDER_PDP_ENDIAN__) && !defined(MY_BYTE_ORDER_DEFINED) */

#define MY_SYSTEM_IS_LITTLE_ENDIAN (*(u16*)"\0\1">>8) // runtime: is little endian system type
#define MY_SYSTEM_IS_BIG_ENDIAN (*(u16*)"\1\0">>8)    // runtime: is big endian system type


#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    #define CAT_ENDIAN(name) PREPROCESSOR_CONCAT(name, _LSB)
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    #define CAT_ENDIAN(name) PREPROCESSOR_CONCAT(name, _MSB)
#else
    #define CAT_ENDIAN(name) PREPROCESSOR_CONCAT(name, _)
    #error unsupported endianness
#endif /* MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN */


/*
 * ******************************************
 * test
 * ******************************************
 */

#ifndef BYTE_ORDER_TEST_DISABLE

/*
 * ****************************************
 * dataset read/write test for bytes order
 *  * return 0 if test compleated, else - not compleated
 * ****************************************
 */
int endiansTest();

#endif /* BYTE_ORDER_TEST_DISABLE */

/*
 * ********************************************************
 * main reverse function
 * ********************************************************
 */

STATIC_FORCEINLINE u16 Reverse16(const u16 value)
{
    return (u16)( 0
                       | ((value & 0x00ffU) << 8U)
                       | ((value & 0xff00U) >> 8U) );
}

STATIC_FORCEINLINE u32 Reverse32(const u32 value)
{
    return (u32)( 0
                       | ((value & 0x000000ffUL) << 24U)
                       | ((value & 0x0000ff00UL) << 8U)
                       | ((value & 0x00ff0000UL) >> 8U)
                       | ((value & 0xff000000UL) >> 24U) );
}

STATIC_FORCEINLINE u64 Reverse64(const u64 value)
{
    return (u64)( 0
                       | ((value & 0x00000000000000ffULL) << 56U)
                       | ((value & 0x000000000000ff00ULL) << 40U)
                       | ((value & 0x0000000000ff0000ULL) << 24U)
                       | ((value & 0x00000000ff000000ULL) << 8U)
                       | ((value & 0x000000ff00000000ULL) >> 8U)
                       | ((value & 0x0000ff0000000000ULL) >> 24U)
                       | ((value & 0x00ff000000000000ULL) >> 40U)
                       | ((value & 0xff00000000000000ULL) >> 56U) );
}

// pointrs input ----------------------------------------------
STATIC_FORCEINLINE void Reverse16_ptr(const u16 * const value, u16 * const to)
{
    *to = (u16)( 0
                       | (((*value) & 0x00ff) << 8U)
                       | (((*value) & 0xff00) >> 8U) );
}

STATIC_FORCEINLINE void Reverse32_ptr(const u32 * const value, u32 * const to)
{
    *to = (u32)( 0
                       | (((*value) & 0x000000ffUL) << 24U)
                       | (((*value) & 0x0000ff00UL) << 8U)
                       | (((*value) & 0x00ff0000UL) >> 8U)
                       | (((*value) & 0xff000000UL) >> 24U) );
}

STATIC_FORCEINLINE void Reverse64_ptr(const u64 * const value, u64 * const to)
{
    *to = (u64)( 0
                       | (((*value) & 0x00000000000000ffULL) << 56U)
                       | (((*value) & 0x000000000000ff00ULL) << 40U)
                       | (((*value) & 0x0000000000ff0000ULL) << 24U)
                       | (((*value) & 0x00000000ff000000ULL) << 8U)
                       | (((*value) & 0x000000ff00000000ULL) >> 8U)
                       | (((*value) & 0x0000ff0000000000ULL) >> 24U)
                       | (((*value) & 0x00ff000000000000ULL) >> 40U)
                       | (((*value) & 0xff00000000000000ULL) >> 56U) );
}

/*
 * ********************************************************
 * 16 bit variable reverse function
 * ********************************************************
 */
STATIC_FORCEINLINE u16 LittleEndianU16(u16 value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    return value;
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    return Reverse16(value);
#else
#    error unsupported endianness
#endif
}

STATIC_FORCEINLINE i16 LittleEndianI16(i16 value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    return value;
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    i16 data;
    Reverse16_ptr((u16 *) &value, (u16 *) &data);
    return data;
#else
#    error unsupported endianness
#endif
}

STATIC_FORCEINLINE u16 BigEndianU16(u16 value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    return Reverse16(value);
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    return value;
#else
#    error unsupported endianness
#endif
}

STATIC_FORCEINLINE i16 BigEndianI16(i16 value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    i16 data;
    Reverse16_ptr((u16 *) &value, (u16 *) &data);
    return data;
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    return value;
#else
#    error unsupported endianness
#endif
}

/*
 * ********************************************************
 * 32 bit variable reverse function
 * ********************************************************
 */

STATIC_FORCEINLINE u32 LittleEndianU32(u32 value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    return value;
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    return Reverse32(value);
#else
#    error unsupported endianness
#endif
}

STATIC_FORCEINLINE i32 LittleEndianI32(i32 value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    return value;
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    i32 data;
    Reverse32_ptr((u32 *) &value, (u32 *) &data);
    return data;
#else
#    error unsupported endianness
#endif
}

STATIC_FORCEINLINE u32 BigEndianU32(u32 value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    return Reverse32(value);
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    return value;
#else
#    error unsupported endianness
#endif
}

STATIC_FORCEINLINE i32 BigEndianI32(i32 value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    i32 data;
    Reverse32_ptr((u32 *) &value, (u32 *) &data);
    return data;
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    return value;
#else
#    error unsupported endianness
#endif
}


/*
 * ********************************************************
 * 64 bit variable reverse function
 * ********************************************************
 */

STATIC_FORCEINLINE u64 LittleEndianU64(u64 value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    return value;
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    return Reverse64(value);
#else
#    error unsupported endianness
#endif
}

STATIC_FORCEINLINE i64 LittleEndianI64(i64 value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    return value;
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    i64 data;
    Reverse64_ptr((u64 *) &value, (u64 *) &data);
    return data;
#else
#    error unsupported endianness
#endif
}

STATIC_FORCEINLINE u64 BigEndianU64(u64 value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    return Reverse64(value);
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    return value;
#else
#    error unsupported endianness
#endif
}

STATIC_FORCEINLINE i64 BigEndianI64(i64 value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    i64 data;
    Reverse64_ptr((u64 *) &value, (u64 *) &data);
    return data;

#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    return value;
#else
#    error unsupported endianness
#endif
}

/*
 * ********************************************************
 * f32 32 bit variable reverse function
 * ********************************************************
 */

STATIC_FORCEINLINE f32 LittleEndianF32(f32 value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    return value;
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    f32 data;
    Reverse32_ptr((u32 *) &value, (u32 *) &data);
    return data;
#else
#    error unsupported endianness
#endif
}

STATIC_FORCEINLINE f32 BigEndianF32(f32 value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    f32 data;
    Reverse32_ptr((u32 *) &value, (u32 *) &data);
    return data;
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    return value;
#else
#    error unsupported endianness
#endif
}

/*
 * ********************************************************
 * f64 64 bit variable reverse function
 * ********************************************************
 */

STATIC_FORCEINLINE f64 LittleEndianF64(f64 value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    return value;
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    f64 data;
    Reverse64_ptr((u64 *) &value, (u64 *) &data);
    return data;
#else
#    error unsupported endianness
#endif
}

STATIC_FORCEINLINE f64 BigEndianF64(f64 value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    f64 data;
    Reverse64_ptr((u64 *) &value, (u64 *) &data);
    return data;
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    return value;
#else
#    error unsupported endianness
#endif
}


#if !defined (__BYTE_ORDER__) && !defined(__ORDER_LITTLE_ENDIAN__) && !defined(__ORDER_BIG_ENDIAN__) && !defined(__ORDER_PDP_ENDIAN__)
#pragma GCC diagnostic pop
#endif /* !defined (__BYTE_ORDER__) && !defined(__ORDER_LITTLE_ENDIAN__) && !defined(__ORDER_BIG_ENDIAN__) && !defined(__ORDER_PDP_ENDIAN__) */


#endif /* __BYTE_ORDER_H__ */
