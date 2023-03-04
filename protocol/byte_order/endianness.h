/**
* @file         endianness.h
* @brief        Convert Endianness of shorts, longs, long longs, regardless of architecture/OS
* @author       jtbr (https://gist.github.com/jtbr)
* @copyright    Shpegun60 (https://github.com/shpegun60)
*
* Defines (without pulling in platform-specific network include headers):
* bswap16, bswap32, bswap64, ntoh16, hton16, ntoh32 hton32, ntoh64, hton64
*
* Should support linux / macos / solaris / windows.
* Supports GCC (on any platform, including embedded), MSVC2015, and clang,
* and should support intel, solaris, and ibm compilers as well.
*
* Copyright 2020 github user jtbr, Released under MIT license
*
* SPDX-License-Identifier: MIT OR Apache-2.0
*/
//https://gist.github.com/jtbr/7a43e6281e6cca353b33ee501421860c
#ifndef __ENDIANNESS_H__
#define __ENDIANNESS_H__ 1

#include "my_ctypes_def.h"
#include "inline.h"

#ifdef __cplusplus
#   include <cstring> // for memcpy
#endif /* __cplusplus */

/* Detect platform endianness at compile time */

// If boost were available on all platforms, could use this instead to detect endianness
// #include <boost/predef/endian.h>

// When available, these headers can improve platform endianness detection
#ifdef __has_include // C++17, supported as extension to C++11 in clang, GCC 5+, vs2015
#   if __has_include(<endian.h>)
#       include <endian.h>                      // gnu libc normally provides, linux
#   elif __has_include(<machine/endian.h>)
#       include <machine/endian.h>              //open bsd, macos
#   elif __has_include(<sys/param.h>)
#       include <sys/param.h>                   // mingw, some bsd (not open/macos)
#   elif __has_include(<sys/isadefs.h>)
#       include <sys/isadefs.h> // solaris
#   endif
#endif /* __has_include */

#if !defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)
#   if (defined(__BYTE_ORDER__)  && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)             ||                                                          \
    (defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN)                             ||                                                          \
    (defined(_BYTE_ORDER) && _BYTE_ORDER == _BIG_ENDIAN)                                ||                                                          \
    (defined(BYTE_ORDER) && BYTE_ORDER == BIG_ENDIAN)                                   ||                                                          \
    (defined(__sun) && defined(__SVR4) && defined(_BIG_ENDIAN))                         ||                                                          \
    defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__)                ||                                                          \
    defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)                        ||                                                          \
    defined(_M_PPC)
#       define __BIG_ENDIAN__ 1
#   elif (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)         ||  /* gcc */                                               \
    (defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN)                          ||  /* linux header */                                      \
    (defined(_BYTE_ORDER) && _BYTE_ORDER == _LITTLE_ENDIAN)                             ||                                                          \
    (defined(BYTE_ORDER) && BYTE_ORDER == LITTLE_ENDIAN)                                ||  /* mingw header */                                      \
    (defined(__sun) && defined(__SVR4) && defined(_LITTLE_ENDIAN))                      ||  /* solaris */                                           \
    defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__)                ||                                                          \
    defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__)                        ||                                                          \
    defined(_M_IX86) || defined(_M_X64) || defined(_M_IA64)                             ||  /* msvc for intel processors */                         \
    defined(_M_ARM)                                                                         /* msvc code on arm executes in little endian mode */
#       define __LITTLE_ENDIAN__ 1
#   else
#       define _LITTLE_ENDIAN_TEST   0x41424344UL
#       define _BIG_ENDIAN_TEST      0x44434241UL
#       define _PDP_ENDIAN_TEST      0x42414443UL
#       define _ENDIAN_ORDER_TEST    ('ABCD')
#
#       if _ENDIAN_ORDER_TEST == _LITTLE_ENDIAN_TEST
#           define __LITTLE_ENDIAN__ 1
#       elif _ENDIAN_ORDER_TEST == _BIG_ENDIAN_TEST
#           define __BIG_ENDIAN__ 1
#       else
#           error unsupported endianness
#       endif /* manual endianess switcing */
#
#       undef _LITTLE_ENDIAN_TEST
#       undef _BIG_ENDIAN_TEST
#       undef _PDP_ENDIAN_TEST
#       undef _ENDIAN_ORDER_TEST
#
#   endif
#
#endif /* !defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__) */



#if !defined(__LITTLE_ENDIAN__) & !defined(__BIG_ENDIAN__)
#  error "UNKNOWN Platform / endianness. Configure endianness checks for this platform or set explicitly."
#endif /* !defined(__LITTLE_ENDIAN__) & !defined(__BIG_ENDIAN__) */

#if defined(bswap16) || defined(bswap32) || defined(bswap64) || defined(bswapf) || defined(bswapd)
#  error "unexpected define!" // freebsd may define these; probably just need to undefine them
#endif /* defined(bswap16) || defined(bswap32) || defined(bswap64) || defined(bswapf) || defined(bswapd) */

/* Define byte-swap functions, using fast processor-native built-ins where possible */
#if defined(_MSC_VER) // needs to be first because msvc doesn't short-circuit after failing defined(__has_builtin)
#   define bswap16(x)     _byteswap_ushort((x))
#   define bswap32(x)     _byteswap_ulong((x))
#   define bswap64(x)     _byteswap_uint64((x))
#elif (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
#   define bswap16(x)     __builtin_bswap16((x))
#   define bswap32(x)     __builtin_bswap32((x))
#   define bswap64(x)     __builtin_bswap64((x))
#elif defined(__has_builtin) && __has_builtin(__builtin_bswap64)  /* for clang; gcc 5 fails on this and && shortcircuit fails; must be after GCC check */
#   define bswap16(x)     __builtin_bswap16((x))
#   define bswap32(x)     __builtin_bswap32((x))
#   define bswap64(x)     __builtin_bswap64((x))
#else
    /* even in this case, compilers often optimize by using native instructions */
    STATIC_FORCEINLINE u16 bswap16(const u16 x)
    {
        return ((( x  >> 8 ) & 0xffu ) | (( x  & 0xffu ) << 8 ));
    }
    STATIC_FORCEINLINE u32 bswap32(const u32 x)
    {
        return ((( x & 0xff000000u ) >> 24 ) |
                (( x & 0x00ff0000u ) >> 8  ) |
                (( x & 0x0000ff00u ) << 8  ) |
                (( x & 0x000000ffu ) << 24 ));
    }
    STATIC_FORCEINLINE u64 bswap64(const u64 x)
    {
        return ((( x & 0xff00000000000000ull ) >> 56 ) |
                (( x & 0x00ff000000000000ull ) >> 40 ) |
                (( x & 0x0000ff0000000000ull ) >> 24 ) |
                (( x & 0x000000ff00000000ull ) >> 8  ) |
                (( x & 0x00000000ff000000ull ) << 8  ) |
                (( x & 0x0000000000ff0000ull ) << 24 ) |
                (( x & 0x000000000000ff00ull ) << 40 ) |
                (( x & 0x00000000000000ffull ) << 56 ));
    }
#endif /* swap function switching */

//! Byte-swap 32-bit float
STATIC_FORCEINLINE float bswapf(float f)
{
#ifdef __cplusplus
    /* Problem: de-referencing float pointer as u32 breaks strict-aliasing rules for C++ and C, even if it normally works
     *   u32 val = bswap32(*(reinterpret_cast<const u32 *>(&f)));
     *   return *(reinterpret_cast<float *>(&val));
     */
    // memcpy approach is guaranteed to work in C & C++ and fn calls should be optimized out:
    u32 asInt;
    std::memcpy(&asInt, reinterpret_cast<const void *>(&f), sizeof(u32));
    asInt = bswap32(asInt);
    std::memcpy(&f, reinterpret_cast<void *>(&asInt), sizeof(float));
    return f;
#else
    // union approach is guaranteed to work in C99 and later (but not in C++, though in practice it normally will):
    union { u32 asInt; float asFloat; } conversion_union;
    conversion_union.asFloat = f;
    conversion_union.asInt = bswap32(conversion_union.asInt);
    return conversion_union.asFloat;
#endif /* __cplusplus */
}

//! Byte-swap 64-bit double
STATIC_FORCEINLINE double bswapd(double d)
{
#ifdef __cplusplus
    u64 asInt;
    std::memcpy(&asInt, reinterpret_cast<const void *>(&d), sizeof(u64));
    asInt = bswap64(asInt);
    std::memcpy(&d, reinterpret_cast<void *>(&asInt), sizeof(double));
    return d;
#else
    union { u64 asInt; double asDouble; } conversion_union;
    conversion_union.asDouble = d;
    conversion_union.asInt = bswap64(conversion_union.asInt);
    return conversion_union.asDouble;
#endif /* __cplusplus */
}


/* Define network - host byte swaps as needed depending upon platform endianness */
// (note that network order is big endian)

#if defined(__LITTLE_ENDIAN__)
#   define ntoh16(x)     bswap16((x))
#   define hton16(x)     bswap16((x))
#   define ntoh32(x)     bswap32((x))
#   define hton32(x)     bswap32((x))
#   define ntoh64(x)     bswap64((x))
#   define hton64(x)     bswap64((x))
#   define ntohf(x)      bswapf((x))
#   define htonf(x)      bswapf((x))
#   define ntohd(x)      bswapd((x))
#   define htond(x)      bswapd((x))
#elif defined(__BIG_ENDIAN__)
#   define ntoh16(x)     (x)
#   define hton16(x)     (x)
#   define ntoh32(x)     (x)
#   define hton32(x)     (x)
#   define ntoh64(x)     (x)
#   define hton64(x)     (x)
#   define ntohf(x)      (x)
#   define htonf(x)      (x)
#   define ntohd(x)      (x)
#   define htond(x)      (x)
#  else
#    warning "UNKNOWN Platform / endianness; network / host byte swaps not defined."
#endif /* byte order selection */

#endif /*__ENDIANNESS_H__ */
