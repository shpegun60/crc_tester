#ifndef __BYTE_ORDER_H__
#define __BYTE_ORDER_H__ 1

#include "endianness.h"
#include "preprocessor_concatenation.h"

#ifndef BYTE_ORDER_TEST_DISABLE
    //#define BYTE_ORDER_TEST_DISABLE
#endif /* BYTE_ORDER_TEST_DISABLE */


#define MY_SYSTEM_IS_LITTLE_ENDIAN (*(u16*)"\0\1">>8) // runtime: is little endian system type
#define MY_SYSTEM_IS_BIG_ENDIAN (*(u16*)"\1\0">>8)    // runtime: is big endian system type


#if defined(__LITTLE_ENDIAN__)
    #define CAT_ENDIAN(name) PREPROCESSOR_CONCAT(name, _LSB)
#elif defined(__BIG_ENDIAN__)
    #define CAT_ENDIAN(name) PREPROCESSOR_CONCAT(name, _MSB)
#else
    #define CAT_ENDIAN(name) PREPROCESSOR_CONCAT(name, _LSB)
    #error unsupported endianness
#endif /* ORDER SELECTION */


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
 * 16 bit variable reverse functions
 * ********************************************************
 */
STATIC_FORCEINLINE u16 LittleEndianU16(const u16 value)
{
#if defined(__LITTLE_ENDIAN__)
    return value;
#elif defined(__BIG_ENDIAN__)
    return bswap16(value);
#else
    #error unsupported endianness
#endif /* ORDER SELECTION */
}

STATIC_FORCEINLINE i16 LittleEndianI16(i16 value)
{
#if defined(__LITTLE_ENDIAN__)
    return value;
#elif defined(__BIG_ENDIAN__)
#
#   ifdef __cplusplus
    u16 asUint;
    std::memcpy(&asUint, reinterpret_cast<const void *>(&value), sizeof(u16));
    asUint = bswap16(asUint);
    std::memcpy(&value, reinterpret_cast<void *>(&asUint), sizeof(i16));
    return value;
#   else
    union { u16 asUint; i16 asInt; } conversion_union;
    conversion_union.asInt = value;
    conversion_union.asUint = bswap16(conversion_union.asUint);
    return conversion_union.asInt;
#   endif /* __cplusplus */
#
#else
    #error unsupported endianness
#endif /* ORDER SELECTION */
}

STATIC_FORCEINLINE u16 BigEndianU16(const u16 value)
{
#if defined(__LITTLE_ENDIAN__)
    return bswap16(value);
#elif defined(__BIG_ENDIAN__)
    return value;
#else
#    error unsupported endianness
#endif /* ORDER SELECTION */
}

STATIC_FORCEINLINE i16 BigEndianI16(i16 value)
{
#if defined(__LITTLE_ENDIAN__)
#   ifdef __cplusplus
    u16 asUint;
    std::memcpy(&asUint, reinterpret_cast<const void *>(&value), sizeof(u16));
    asUint = bswap16(asUint);
    std::memcpy(&value, reinterpret_cast<void *>(&asUint), sizeof(i16));
    return value;
#   else
    union { u16 asUint; i16 asInt; } conversion_union;
    conversion_union.asInt = value;
    conversion_union.asUint = bswap16(conversion_union.asUint);
    return conversion_union.asInt;
#   endif /* __cplusplus */
#elif defined(__BIG_ENDIAN__)
    return value;
#else
#    error unsupported endianness
#endif /* ORDER SELECTION */
}

/*
 * ********************************************************
 * 32 bit variable reverse functions
 * ********************************************************
 */

STATIC_FORCEINLINE u32 LittleEndianU32(const u32 value)
{
#if defined(__LITTLE_ENDIAN__)
    return value;
#elif defined(__BIG_ENDIAN__)
    return bswap32(value);
#else
#    error unsupported endianness
#endif /* ORDER SELECTION */
}

STATIC_FORCEINLINE i32 LittleEndianI32(i32 value)
{
#if defined(__LITTLE_ENDIAN__)
    return value;
#elif defined(__BIG_ENDIAN__)
#
#   ifdef __cplusplus
    u32 asUint;
    std::memcpy(&asUint, reinterpret_cast<const void *>(&value), sizeof(u32));
    asUint = bswap32(asUint);
    std::memcpy(&value, reinterpret_cast<void *>(&asUint), sizeof(i32));
    return value;
#   else
    union { u32 asUint; i32 asInt; } conversion_union;
    conversion_union.asInt = value;
    conversion_union.asUint = bswap32(conversion_union.asUint);
    return conversion_union.asInt;
#   endif /* __cplusplus */
#
#else
    #error unsupported endianness
#endif /* ORDER SELECTION */
}

STATIC_FORCEINLINE u32 BigEndianU32(const u32 value)
{
#if defined(__LITTLE_ENDIAN__)
    return bswap32(value);
#elif defined(__BIG_ENDIAN__)
    return value;
#else
#    error unsupported endianness
#endif /* ORDER SELECTION */
}

STATIC_FORCEINLINE i32 BigEndianI32(i32 value)
{
#if defined(__LITTLE_ENDIAN__)
#   ifdef __cplusplus
    u32 asUint;
    std::memcpy(&asUint, reinterpret_cast<const void *>(&value), sizeof(u32));
    asUint = bswap32(asUint);
    std::memcpy(&value, reinterpret_cast<void *>(&asUint), sizeof(i32));
    return value;
#   else
    union { u32 asUint; i32 asInt; } conversion_union;
    conversion_union.asInt = value;
    conversion_union.asUint = bswap32(conversion_union.asUint);
    return conversion_union.asInt;
#   endif /* __cplusplus */
#elif defined(__BIG_ENDIAN__)
    return value;
#else
#    error unsupported endianness
#endif /* ORDER SELECTION */
}


/*
 * ********************************************************
 * 64 bit variable reverse functions
 * ********************************************************
 */

STATIC_FORCEINLINE u64 LittleEndianU64(const u64 value)
{
#if defined(__LITTLE_ENDIAN__)
    return value;
#elif defined(__BIG_ENDIAN__)
    return bswap64(value);
#else
#    error unsupported endianness
#endif /* ORDER SELECTION */
}

STATIC_FORCEINLINE i64 LittleEndianI64(i64 value)
{
#if defined(__LITTLE_ENDIAN__)
    return value;
#elif defined(__BIG_ENDIAN__)
#
#   ifdef __cplusplus
    u64 asUint;
    std::memcpy(&asUint, reinterpret_cast<const void *>(&value), sizeof(u64));
    asUint = bswap64(asUint);
    std::memcpy(&value, reinterpret_cast<void *>(&asUint), sizeof(i64));
    return value;
#   else
    union { u64 asUint; i64 asInt; } conversion_union;
    conversion_union.asInt = value;
    conversion_union.asUint = bswap64(conversion_union.asUint);
    return conversion_union.asInt;
#   endif /* __cplusplus */
#
#else
#    error unsupported endianness
#endif /* ORDER SELECTION */
}

STATIC_FORCEINLINE u64 BigEndianU64(const u64 value)
{
#if defined(__LITTLE_ENDIAN__)
    return bswap64(value);
#elif defined(__BIG_ENDIAN__)
    return value;
#else
#    error unsupported endianness
#endif /* ORDER SELECTION */
}

STATIC_FORCEINLINE i64 BigEndianI64(i64 value)
{
#if defined(__LITTLE_ENDIAN__)
#   ifdef __cplusplus
    u64 asUint;
    std::memcpy(&asUint, reinterpret_cast<const void *>(&value), sizeof(u64));
    asUint = bswap64(asUint);
    std::memcpy(&value, reinterpret_cast<void *>(&asUint), sizeof(i64));
    return value;
#   else
    union { u64 asUint; i64 asInt; } conversion_union;
    conversion_union.asInt = value;
    conversion_union.asUint = bswap64(conversion_union.asUint);
    return conversion_union.asInt;
#   endif /* __cplusplus */
#elif defined(__BIG_ENDIAN__)
    return value;
#else
#    error unsupported endianness
#endif /* ORDER SELECTION */
}

/*
 * ********************************************************
 * f32 32 bit variable reverse functions
 * ********************************************************
 */

STATIC_FORCEINLINE f32 LittleEndianF32(const f32 value)
{
#if defined(__LITTLE_ENDIAN__)
    return value;
#elif defined(__BIG_ENDIAN__)
    return bswapf(value);
#else
#    error unsupported endianness
#endif /* ORDER SELECTION */
}

STATIC_FORCEINLINE f32 BigEndianF32(const f32 value)
{
#if defined(__LITTLE_ENDIAN__)
    return bswapf(value);
#elif defined(__BIG_ENDIAN__)
    return value;
#else
#    error unsupported endianness
#endif /* ORDER SELECTION */
}

/*
 * ********************************************************
 * f64 64 bit variable reverse functions
 * ********************************************************
 */

STATIC_FORCEINLINE f64 LittleEndianF64(const f64 value)
{
#if defined(__LITTLE_ENDIAN__)
    return value;
#elif defined(__BIG_ENDIAN__)
    return bswapd(value);
#else
#    error unsupported endianness
#endif /* ORDER SELECTION */
}

STATIC_FORCEINLINE f64 BigEndianF64(const f64 value)
{
#if defined(__LITTLE_ENDIAN__)
    return bswapd(value);
#elif defined(__BIG_ENDIAN__)
    return value;
#else
#    error unsupported endianness
#endif /* ORDER SELECTION */
}


#endif /* __BYTE_ORDER_H__ */
