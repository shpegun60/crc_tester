#ifndef __BYTE_ORDER_H__
#define __BYTE_ORDER_H__

#include <stdint.h>

#if defined (__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && defined(__ORDER_BIG_ENDIAN__) && defined(__ORDER_PDP_ENDIAN__) && !defined(MY_BYTE_ORDER_DEFINED)
    #define MY_BYTE_ORDER_DEFINED

    #define MY_LITTLE_ENDIAN   __ORDER_LITTLE_ENDIAN__
    #define MY_BIG_ENDIAN      __ORDER_BIG_ENDIAN__
    #define MY_PDP_ENDIAN      __ORDER_PDP_ENDIAN__
    #define MY_ENDIAN_ORDER    __BYTE_ORDER__

#elif !defined(MY_BYTE_ORDER_DEFINED) /* if not exists system byte-order macro, define other hack */
    #define MY_BYTE_ORDER_DEFINED

    #pragma GCC diagnostic ignored "-Wmultichar"

    #define MY_LITTLE_ENDIAN   0x41424344UL
    #define MY_BIG_ENDIAN      0x44434241UL
    #define MY_PDP_ENDIAN      0x42414443UL
    #define MY_ENDIAN_ORDER    ('ABCD')

#endif /* defined (__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && defined(__ORDER_BIG_ENDIAN__) && defined(__ORDER_PDP_ENDIAN__) && !defined(MY_BYTE_ORDER_DEFINED) */

#define MY_SYSTEM_IS_LITTLE_ENDIAN (*(uint16_t*)"\0\1">>8) // runtime: is little endian system type
#define MY_SYSTEM_IS_BIG_ENDIAN (*(uint16_t*)"\1\0">>8)    // runtime: is big endian endian system type


//----------------------------------------------------
int endiansTest();
/*
 * ********************************************************
 * main reverse function
 * ********************************************************
 */

inline uint16_t Reverse16(const uint16_t value)
{
    return (uint16_t)( 0
                       | ((value & 0x00ffU) << 8U)
                       | ((value & 0xff00U) >> 8U) );
}

inline uint32_t Reverse32(const uint32_t value)
{
    return (uint32_t)( 0
                       | ((value & 0x000000ffUL) << 24U)
                       | ((value & 0x0000ff00UL) << 8U)
                       | ((value & 0x00ff0000UL) >> 8U)
                       | ((value & 0xff000000UL) >> 24U) );
}

inline uint64_t Reverse64(const uint64_t value)
{
    return (uint64_t)( 0
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
inline uint16_t Reverse16_ptr(const uint16_t * const value)
{
    return (uint16_t)( 0
                       | (((*value) & 0x00ff) << 8U)
                       | (((*value) & 0xff00) >> 8U) );
}

inline uint32_t Reverse32_ptr(const uint32_t * const value)
{
    return (uint32_t)( 0
                       | (((*value) & 0x000000ffUL) << 24U)
                       | (((*value) & 0x0000ff00UL) << 8U)
                       | (((*value) & 0x00ff0000UL) >> 8U)
                       | (((*value) & 0xff000000UL) >> 24U) );
}

inline uint64_t Reverse64_ptr(const uint64_t * const value)
{
    return (uint64_t)( 0
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

inline uint16_t LittleEndianU16(uint16_t value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    return value;
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    return Reverse16(value);
#else
#    error unsupported endianness
#endif
}

inline int16_t LittleEndianI16(int16_t value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    return value;
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    uint16_t data = Reverse16_ptr((uint16_t *) &value);
    return *((int16_t *)(&data));
#else
#    error unsupported endianness
#endif
}

inline uint16_t BigEndianU16(uint16_t value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    return Reverse16(value);
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    return value;
#else
#    error unsupported endianness
#endif
}

inline int16_t BigEndianI16(int16_t value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    uint16_t data = Reverse16_ptr((uint16_t *) &value);
    return *((int16_t *)(&data));
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

inline uint32_t LittleEndianU32(uint32_t value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    return value;
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    return Reverse32(value);
#else
#    error unsupported endianness
#endif
}

inline int32_t LittleEndianI32(int32_t value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    return value;
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    uint32_t data = Reverse32_ptr((uint32_t *) &value);
    return *((int32_t *)(&data));
#else
#    error unsupported endianness
#endif
}

inline uint32_t BigEndianU32(uint32_t value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    return Reverse32(value);
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    return value;
#else
#    error unsupported endianness
#endif
}

inline int32_t BigEndianI32(int32_t value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    uint32_t data = Reverse32_ptr((uint32_t *) &value);
    return *((int32_t *)(&data));
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

inline uint64_t LittleEndianU64(uint64_t value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    return value;
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    return Reverse64(value);
#else
#    error unsupported endianness
#endif
}

inline int64_t LittleEndianI64(int64_t value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    return value;
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    uint64_t data = Reverse64_ptr((uint64_t *) &value);
    return *((int64_t *)(&data));
#else
#    error unsupported endianness
#endif
}

inline uint64_t BigEndianU64(uint64_t value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    return Reverse64(value);
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    return value;
#else
#    error unsupported endianness
#endif
}

inline int64_t BigEndianI64(int64_t value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    uint64_t data = Reverse64_ptr((uint64_t *) &value);
    return *((int64_t *)(&data));
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    return value;
#else
#    error unsupported endianness
#endif
}

/*
 * ********************************************************
 * float 32 bit variable reverse function
 * ********************************************************
 */

inline float LittleEndianF32(float value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    return value;
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    uint32_t data = Reverse32_ptr((uint32_t *) &value);
    return *((float *)(&data));
#else
#    error unsupported endianness
#endif
}

inline float BigEndianF32(float value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    uint32_t data = Reverse32_ptr((uint32_t *) &value);
    return *((float *)(&data));
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    return value;
#else
#    error unsupported endianness
#endif
}

/*
 * ********************************************************
 * double 64 bit variable reverse function
 * ********************************************************
 */

inline double LittleEndianF64(double value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    return value;
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    uint64_t data = Reverse64_ptr((uint64_t *) &value);
    return *((double *)(&data));
#else
#    error unsupported endianness
#endif
}

inline double BigEndianF64(double value)
{
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    uint64_t data = Reverse64_ptr((uint64_t *) &value);
    return *((double *)(&data));
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    return value;
#else
#    error unsupported endianness
#endif
}


#endif /* __BYTE_ORDER_H__ */
