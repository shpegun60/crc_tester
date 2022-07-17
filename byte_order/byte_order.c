#include "byte_order.h"
#include <stdio.h>
#include <inttypes.h>

// strict type control (PRECOMPILE)------------------------------------------------
#if (__STDC_VERSION__ >= 201112L) // if C version equal or more than C11
#include "assert.h"

static_assert((sizeof(uint64_t) == 8), "MY_ENDIAN: size of data type uint64_t do not supported");
static_assert((sizeof(int64_t) == 8), "MY_ENDIAN: size of data type int64_t do not supported");
static_assert((sizeof(int64_t) == sizeof(uint64_t)), "MY_ENDIAN: some type`s do not supported");
static_assert((sizeof(double) == sizeof(uint64_t)), "MY_ENDIAN: some type`s do not supported");
static_assert((sizeof(double) == 8), "MY_ENDIAN: size of data type double do not supported");

static_assert((sizeof(uint32_t) == 4), "MY_ENDIAN: size of data type uint32_t do not supported");
static_assert((sizeof(int32_t) == 4), "MY_ENDIAN: size of data type int32_t do not supported");
static_assert((sizeof(int32_t) == sizeof(uint32_t)), "MY_ENDIAN: some type`s do not supported");
static_assert((sizeof(float) == sizeof(uint32_t)), "MY_ENDIAN: some type`s do not supported");
static_assert((sizeof(float) == 4), "MY_ENDIAN: size of data type float do not supported");

static_assert((sizeof(uint16_t) == 2), "MY_ENDIAN: size of data type uint16_t do not supported");
static_assert((sizeof(int16_t) == 2), "MY_ENDIAN: size of data type int16_t do not supported");
static_assert((sizeof(int16_t) == sizeof(uint16_t)), "MY_ENDIAN: some type`s do not supported");

static_assert((sizeof(uint8_t) == 1), "MY_ENDIAN: size of data type int8_t do not supported");

#else // if old version C
#define C99MY_ENDIAN_STATIC_ASSERTION_CREATE(COND,MSG) typedef int my_crc_static_assertion_##MSG[(COND)? 1 : -1] // define custom static assertion if version C less than C11
//--------------------------------------------------------------------------------------------------------------

C99MY_ENDIAN_STATIC_ASSERTION_CREATE((sizeof(uint64_t) == 8), not_support_endian_data_type_uint64_t);
C99MY_ENDIAN_STATIC_ASSERTION_CREATE((sizeof(int64_t) == 8), not_support_endian_data_type_int64_t);
C99MY_ENDIAN_STATIC_ASSERTION_CREATE((sizeof(int64_t) == sizeof(uint64_t)), some_types_not_supported);
C99MY_ENDIAN_STATIC_ASSERTION_CREATE((sizeof(double) == sizeof(uint64_t)), some_types_not_supported);
C99MY_ENDIAN_STATIC_ASSERTION_CREATE((sizeof(double) == 8), not_support_endian_data_type_double);

C99MY_ENDIAN_STATIC_ASSERTION_CREATE((sizeof(uint32_t) == 4), not_support_endian_data_type_uint32_t);
C99MY_ENDIAN_STATIC_ASSERTION_CREATE((sizeof(int32_t) == 4), not_support_endian_data_type_int32_t);
C99MY_ENDIAN_STATIC_ASSERTION_CREATE((sizeof(int32_t) == sizeof(uint32_t)), some_types_not_supported);
C99MY_ENDIAN_STATIC_ASSERTION_CREATE((sizeof(float) == sizeof(uint32_t)), some_types_not_supported);
C99MY_ENDIAN_STATIC_ASSERTION_CREATE((sizeof(float) == 4), not_support_endian_data_type_float);

C99MY_ENDIAN_STATIC_ASSERTION_CREATE((sizeof(uint16_t) == 2), not_support_endian_data_type_uint16_t);
C99MY_ENDIAN_STATIC_ASSERTION_CREATE((sizeof(int16_t) == 2), not_support_endian_data_type_int16_t);
C99MY_ENDIAN_STATIC_ASSERTION_CREATE((sizeof(int16_t) == sizeof(uint16_t)), some_types_not_supported);

C99MY_ENDIAN_STATIC_ASSERTION_CREATE((sizeof(uint8_t) == 1), not_support_endian_data_type_uint8_t);

//--------------------------------------------------------------------------------------------------------------
#undef C99MY_ENDIAN_STATIC_ASSERTION_CREATE
#endif /* (__STDC_VERSION__ >= 201112L) */


//    #if MY_ENDIAN_ORDER==MY_LITTLE_ENDIAN
//        #error "machine is little endian"
//    #elif MY_ENDIAN_ORDER==MY_BIG_ENDIAN
//        #error "machine is big endian"
//    #elif MY_ENDIAN_ORDER==MY_PDP_ENDIAN
//        #error "jeez, machine is PDP!"
//    #else
//        #error "What kind of hardware is this?!"
//    #endif

///*
// * ********************************************************
// * main reverse function
// * ********************************************************
// */

extern inline uint16_t Reverse16(const uint16_t value);
extern inline uint32_t Reverse32(const uint32_t value);
extern inline uint64_t Reverse64(const uint64_t value);

// pointrs input ----------------------------------------------
extern inline uint16_t Reverse16_ptr(const uint16_t * const value);
extern inline uint32_t Reverse32_ptr(const uint32_t * const value);
extern inline uint64_t Reverse64_ptr(const uint64_t * const value);

extern inline uint16_t LittleEndianU16(uint16_t value);
extern inline int16_t LittleEndianI16(int16_t value);
extern inline uint16_t BigEndianU16(uint16_t value);
extern inline int16_t BigEndianI16(int16_t value);

/*
* ********************************************************
* 32 bit variable reverse function
* ********************************************************
*/

extern inline uint32_t LittleEndianU32(uint32_t value);
extern inline int32_t LittleEndianI32(int32_t value);
extern inline uint32_t BigEndianU32(uint32_t value);
extern inline int32_t BigEndianI32(int32_t value);

/*
* ********************************************************
* 64 bit variable reverse function
* ********************************************************
*/

extern inline uint64_t LittleEndianU64(uint64_t value);
extern inline int64_t LittleEndianI64(int64_t value);
extern inline uint64_t BigEndianU64(uint64_t value);
extern inline int64_t BigEndianI64(int64_t value);

/*
 * ********************************************************
 * float 32 bit variable reverse function
 * ********************************************************
 */

extern inline float LittleEndianF32(float value);
extern inline float BigEndianF32(float value);

/*
 * ********************************************************
 * double 64 bit variable reverse function
 * ********************************************************
 */

extern inline double LittleEndianF64(double value);
extern inline double BigEndianF64(double value);

__attribute__((unused)) static int byteOrderCheck(void * source, void * destination, int len)
{
    uint8_t * src = source;
    uint8_t * dst = destination;

    while(len--) {
        if(*src++ != *(dst + len)) {
            return 1;
        }
    }
    return 0;
}

int endiansTest()
{
    // dataset for test toyr system -------------------------------------------------------
    const uint16_t dataU16 = 12345U;
    const int16_t dataI16 = -1000L;
    const uint32_t dataU32 = 1565465464;
    const int32_t dataI32 = -1005500099L;
    const uint64_t dataU64 = 2147483647786453ULL;
    const int64_t dataI64 = -412123456442352LL;
    const float  dataF32 = 123.321;
    const double dataF64 = 987654321.123456789;
    // ------------------------------------------------------------------------------------


    // check values ----------
    uint16_t U16Check;
    int16_t I16Check;
    uint32_t U32Check;
    int32_t I32Check;
    uint64_t U64Check;
    int64_t I64Check;
    float floatCheck;
    double doubleCheck;
    //------------------------


#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    /*
     * ***************************************************
     *  LITTLE ENDIAN FUNCTIONS
     * ***************************************************
     */

    int counter_notlittleEndian = 0;

    printf("\n---------------------------------------------\n");
    printf("LITTLE ENDIAN FUNCTIONS MATCH TEST");
    printf("\n---------------------------------------------\n");

    // u16 -----------------------------------------------------------------------------
    /* nLittleEndianU16 */
    printf("\nLittleEndianU16\ndata: %d\n", dataU16);
    U16Check = LittleEndianU16(dataU16);
    printf("result: %d\nmatched: %d\n", U16Check, U16Check == dataU16);

    if(U16Check != dataU16) {
        ++counter_notlittleEndian;
    }

    // i16 -----------------------------------------------------------------------------
    /* LittleEndianI16 */
    printf("\nLittleEndianI16\ndata: %d\n", dataI16);
    I16Check = LittleEndianI16(dataI16);
    printf("result: %d\nmatched: %d\n", I16Check, I16Check == dataI16);

    if(I16Check != dataI16) {
        ++counter_notlittleEndian;
    }

    // u32 -----------------------------------------------------------------------------
    /* LittleEndianU32 */
    printf("\nLittleEndianU32\ndata: %d\n", dataU32);
    U32Check = LittleEndianU32(dataU32);
    printf("result: %d\nmatched: %d\n", U32Check, U32Check == dataU32);

    if(U32Check != dataU32) {
        ++counter_notlittleEndian;
    }

    // i32 -----------------------------------------------------------------------------
    /* LittleEndianI32 */
    printf("\nLittleEndianI32\ndata: %d\n", dataI32);
    I32Check = LittleEndianI32(dataI32);
    printf("result: %d\nmatched: %d\n", I32Check, I32Check == dataI32);

    if(I32Check != dataI32) {
        ++counter_notlittleEndian;
    }

    // u64 -----------------------------------------------------------------------------
    /* LittleEndianU64 */
    printf("\nLittleEndianU64\ndata: %" PRId64 "\n", dataU64);
    U64Check = LittleEndianU64(dataU64);
    printf("result: %" PRId64 "\nmatched: %d\n", U64Check, U64Check == dataU64);

    if(U64Check != dataU64) {
        ++counter_notlittleEndian;
    }

    // i64 -----------------------------------------------------------------------------
    /* LittleEndianI64 */
    printf("\nLittleEndianI64\ndata: %" SCNd64 "\n", dataI64);
    I64Check = LittleEndianI64(dataI64);
    printf("result: %" SCNd64 "\nmatched: %d\n", I64Check, I64Check == dataI64);

    if(I64Check != dataI64) {
        ++counter_notlittleEndian;
    }
    // float -----------------------------------------------------------------------------
    /* LittleEndianFloat */
    printf("\nLittleEndianFloat\ndata: %f\n", dataF32);
    floatCheck = LittleEndianF32(dataF32);
    printf("result: %f\nmatched: %d\n", floatCheck, floatCheck == dataF32);

    if(floatCheck != dataF32) {
        ++counter_notlittleEndian;
    }

    // double -----------------------------------------------------------------------------
    /* LittleEndianDouble */
    printf("\nLittleEndianDouble\ndata: %f\n", dataF64);
    doubleCheck = LittleEndianF64(dataF64);
    printf("result: %f\nmatched: %d\n", doubleCheck, doubleCheck == dataF64);

    if(doubleCheck != dataF64) {
        ++counter_notlittleEndian;
    }


    /*
     * ***************************************************
     *  BIG ENDIAN FUNCTIONS
     * ***************************************************
     */

    printf("\n---------------------------------------------\n");
    printf("BIG ENDIAN FUNCTIONS MATCH TEST\ntwo conversation");
    printf("\n---------------------------------------------\n");

    int counter_notBigEndian = 0;
    int bytes_order_error = 0;

    // u16 -----------------------------------------------------------------------------
    /* BigEndianU16 */
    printf("\nBigEndianU16\ndata: %d\n", dataU16);
    U16Check = BigEndianU16(dataU16);

    if(byteOrderCheck((uint8_t*)&dataU16, &U16Check, sizeof(dataU16))) {
        ++bytes_order_error;
        printf("bytes order check error: 1\n");
    } else {
        printf("bytes order check error: 0\n");
    }

    U16Check = BigEndianU16(U16Check);
    printf("result: %d\nmatched: %d\n", U16Check, U16Check == dataU16);

    if(U16Check != dataU16) {
        ++counter_notBigEndian;
    }

    // i16 -----------------------------------------------------------------------------
    /* BigEndianI16 */
    printf("\nBigEndianI16\ndata: %d\n", dataI16);
    I16Check = BigEndianI16(dataI16);

    if(byteOrderCheck((uint8_t*)&dataI16, &I16Check, sizeof(dataI16))) {
        ++bytes_order_error;
        printf("bytes order check error: 1\n");
    } else {
        printf("bytes order check error: 0\n");
    }

    I16Check = BigEndianI16(I16Check);
    printf("result: %d\nmatched: %d\n", I16Check, I16Check == dataI16);

    if(I16Check != dataI16) {
        ++counter_notBigEndian;
    }

    // u32 -----------------------------------------------------------------------------
    /* BigEndianU32 */
    printf("\nBigEndianU32\ndata: %d\n", dataU32);
    U32Check = BigEndianU32(dataU32);

    if(byteOrderCheck((uint8_t*)&dataU32, &U32Check, sizeof(dataU32))) {
        ++bytes_order_error;
        printf("bytes order check error: 1\n");
    } else {
        printf("bytes order check error: 0\n");
    }

    U32Check = BigEndianU32(U32Check);
    printf("result: %d\nmatched: %d\n", U32Check, U32Check == dataU32);

    if(U32Check != dataU32) {
        ++counter_notBigEndian;
    }

    // i32 -----------------------------------------------------------------------------
    /* BigEndianI32 */
    printf("\nBigEndianI32\ndata: %d\n", dataI32);
    I32Check = BigEndianI32(dataI32);

    if(byteOrderCheck((uint8_t*)&dataI32, &I32Check, sizeof(dataI32))) {
        ++bytes_order_error;
        printf("bytes order check error: 1\n");
    } else {
        printf("bytes order check error: 0\n");
    }

    I32Check = BigEndianI32(I32Check);
    printf("result: %d\nmatched: %d\n", I32Check, I32Check == dataI32);

    if(I32Check != dataI32) {
        ++counter_notBigEndian;
    }

    // u64 -----------------------------------------------------------------------------
    /* BigEndianU64 */
    printf("\nBigEndianU64\ndata: %" PRId64 "\n", dataU64);
    U64Check = BigEndianU64(dataU64);

    if(byteOrderCheck((uint8_t*)&dataU64, &U64Check, sizeof(dataU64))) {
        ++bytes_order_error;
        printf("bytes order check error: 1\n");
    } else {
        printf("bytes order check error: 0\n");
    }

    U64Check = BigEndianU64(U64Check);
    printf("result: %" PRId64 "\nmatched: %d\n", U64Check, U64Check == dataU64);

    if(I32Check != dataI32) {
        ++counter_notBigEndian;
    }

    // i64 -----------------------------------------------------------------------------
    /* BigEndianI64 */
    printf("\nBigEndianI64\ndata: %" SCNd64 "\n", dataI64);
    I64Check = BigEndianI64(dataI64);

    if(byteOrderCheck((uint8_t*)&dataI64, &I64Check, sizeof(dataI64))) {
        ++bytes_order_error;
        printf("bytes order check error: 1\n");
    } else {
        printf("bytes order check error: 0\n");
    }

    I64Check = BigEndianI64(I64Check);
    printf("result: %" SCNd64 "\nmatched: %d\n", I64Check, I64Check == dataI64);

    if(I64Check != dataI64) {
        ++counter_notBigEndian;
    }


    // float -----------------------------------------------------------------------------
    /* BigEndianFloat */
    printf("\nBigEndianFloat\ndata: %f\n", dataF32);
    floatCheck = BigEndianF32(dataF32);

    if(byteOrderCheck((uint8_t*)&dataF32, &floatCheck, sizeof(dataF32))) {
        ++bytes_order_error;
        printf("bytes order check error: 1\n");
    } else {
        printf("bytes order check error: 0\n");
    }

    floatCheck = BigEndianF32(floatCheck);
    printf("result: %f\nmatched: %d\n", floatCheck, floatCheck == dataF32);

    if(floatCheck != dataF32) {
        ++counter_notBigEndian;
    }

    // double -----------------------------------------------------------------------------
    /* BigEndianDouble */
    printf("\nBigEndianDouble\ndata: %f\n", dataF64);
    doubleCheck = BigEndianF64(dataF64);

    if(byteOrderCheck((uint8_t*)&dataF64, &doubleCheck, sizeof(dataF64))) {
        ++bytes_order_error;
        printf("bytes order check error: 1\n");
    } else {
        printf("bytes order check error: 0\n");
    }

    doubleCheck = BigEndianF64(doubleCheck);
    printf("result: %f\nmatched: %d\n", doubleCheck, doubleCheck == dataF64);

    if(doubleCheck != dataF64) {
        ++counter_notBigEndian;
    }

#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
    /*
     * ***************************************************
     *  LITTLE ENDIAN FUNCTIONS
     * ***************************************************
     */

    int counter_notlittleEndian = 0;
    int bytes_order_error = 0;

    printf("\n---------------------------------------------\n");
    printf("LITTLE ENDIAN FUNCTIONS MATCH TEST\ntwo conversation");
    printf("\n---------------------------------------------\n");

    // u16 -----------------------------------------------------------------------------
    /* nLittleEndianU16 */
    printf("\nLittleEndianU16\ndata: %d\n", dataU16);
    U16Check = LittleEndianU16(dataU16);

    if(byteOrderCheck((uint8_t*)&dataU16, &U16Check, sizeof(dataU16))) {
        ++bytes_order_error;
        printf("bytes order check error: 1\n");
    } else {
        printf("bytes order check error: 0\n");
    }

    U16Check = LittleEndianU16(U16Check);
    printf("result: %d\nmatched: %d\n", U16Check, U16Check == dataU16);

    if(U16Check != dataU16) {
        ++counter_notlittleEndian;
    }

    // i16 -----------------------------------------------------------------------------
    /* LittleEndianI16 */
    printf("\nLittleEndianI16\ndata: %d\n", dataI16);
    I16Check = LittleEndianI16(dataI16);

    if(byteOrderCheck((uint8_t*)&dataI16, &I16Check, sizeof(dataI16))) {
        ++bytes_order_error;
        printf("bytes order check error: 1\n");
    } else {
        printf("bytes order check error: 0\n");
    }

    I16Check = LittleEndianI16(I16Check);
    printf("result: %d\nmatched: %d\n", I16Check, I16Check == dataI16);

    if(I16Check != dataI16) {
        ++counter_notlittleEndian;
    }

    // u32 -----------------------------------------------------------------------------
    /* LittleEndianU32 */
    printf("\nLittleEndianU32\ndata: %d\n", dataU32);
    U32Check = LittleEndianU32(dataU32);

    if(byteOrderCheck((uint8_t*)&dataU32, &U32Check, sizeof(dataU32))) {
        ++bytes_order_error;
        printf("bytes order check error: 1\n");
    } else {
        printf("bytes order check error: 0\n");
    }

    U32Check = LittleEndianU32(U32Check);
    printf("result: %d\nmatched: %d\n", U32Check, U32Check == dataU32);

    if(U32Check != dataU32) {
        ++counter_notlittleEndian;
    }

    // i32 -----------------------------------------------------------------------------
    /* LittleEndianI32 */
    printf("\nLittleEndianI32\ndata: %d\n", dataI32);
    I32Check = LittleEndianI32(dataI32);

    if(byteOrderCheck((uint8_t*)&dataI32, &I32Check, sizeof(dataI32))) {
        ++bytes_order_error;
        printf("bytes order check error: 1\n");
    } else {
        printf("bytes order check error: 0\n");
    }

    I32Check = LittleEndianI32(I32Check);
    printf("result: %d\nmatched: %d\n", I32Check, I32Check == dataI32);

    if(I32Check != dataI32) {
        ++counter_notlittleEndian;
    }

    // u64 -----------------------------------------------------------------------------
    /* LittleEndianU64 */
    printf("\nLittleEndianU64\ndata: %" PRId64 "\n", dataU64);
    U64Check = LittleEndianU64(dataU64);

    if(byteOrderCheck((uint8_t*)&dataU64, &U64Check, sizeof(dataU64))) {
        ++bytes_order_error;
        printf("bytes order check error: 1\n");
    } else {
        printf("bytes order check error: 0\n");
    }

    U64Check = LittleEndianU64(U64Check);
    printf("result: %" PRId64 "\nmatched: %d\n", U64Check, U64Check == dataU64);

    if(U64Check != dataU64) {
        ++counter_notlittleEndian;
    }

    // i64 -----------------------------------------------------------------------------
    /* LittleEndianI64 */
    printf("\nLittleEndianI64\ndata: %" SCNd64 "\n", dataI64);
    I64Check = LittleEndianI64(dataI64);

    if(byteOrderCheck((uint8_t*)&dataI64, &I64Check, sizeof(dataI64))) {
        ++bytes_order_error;
        printf("bytes order check error: 1\n");
    } else {
        printf("bytes order check error: 0\n");
    }

    I64Check = LittleEndianI64(I64Check);
    printf("result: %" SCNd64 "\nmatched: %d\n", I64Check, I64Check == dataI64);

    if(I64Check != dataI64) {
        ++counter_notlittleEndian;
    }
    // float -----------------------------------------------------------------------------
    /* LittleEndianFloat */
    printf("\nLittleEndianFloat\ndata: %f\n", dataF32);
    floatCheck = LittleEndianF32(dataF32);

    if(byteOrderCheck((uint8_t*)&dataF32, &floatCheck, sizeof(dataF32))) {
        ++bytes_order_error;
        printf("bytes order check error: 1\n");
    } else {
        printf("bytes order check error: 0\n");
    }

    floatCheck = LittleEndianF32(floatCheck);
    printf("result: %f\nmatched: %d\n", floatCheck, floatCheck == dataF32);

    if(floatCheck != dataF32) {
        ++counter_notlittleEndian;
    }

    // double -----------------------------------------------------------------------------
    /* LittleEndianDouble */
    printf("\nLittleEndianDouble\ndata: %f\n", dataF64);
    doubleCheck = LittleEndianF64(dataF64);

    if(byteOrderCheck((uint8_t*)&dataF64, &doubleCheck, sizeof(dataF64))) {
        ++bytes_order_error;
        printf("bytes order check error: 1\n");
    } else {
        printf("bytes order check error: 0\n");
    }

    doubleCheck = LittleEndianF64(doubleCheck);
    printf("result: %f\nmatched: %d\n", doubleCheck, doubleCheck == dataF64);

    if(doubleCheck != dataF64) {
        ++counter_notlittleEndian;
    }


    /*
     * ***************************************************
     *  BIG ENDIAN FUNCTIONS
     * ***************************************************
     */

    printf("\n---------------------------------------------\n");
    printf("BIG ENDIAN FUNCTIONS MATCH TEST");
    printf("\n---------------------------------------------\n");

    int counter_notBigEndian = 0;

    // u16 -----------------------------------------------------------------------------
    /* BigEndianU16 */
    printf("\nBigEndianU16\ndata: %d\n", dataU16);
    U16Check = BigEndianU16(dataU16);
    printf("result: %d\nmatched: %d\n", U16Check, U16Check == dataU16);

    if(U16Check != dataU16) {
        ++counter_notBigEndian;
    }

    // i16 -----------------------------------------------------------------------------
    /* BigEndianI16 */
    printf("\nBigEndianI16\ndata: %d\n", dataI16);
    I16Check = BigEndianI16(dataI16);
    printf("result: %d\nmatched: %d\n", I16Check, I16Check == dataI16);

    if(I16Check != dataI16) {
        ++counter_notBigEndian;
    }

    // u32 -----------------------------------------------------------------------------
    /* BigEndianU32 */
    printf("\nBigEndianU32\ndata: %d\n", dataU32);
    U32Check = BigEndianU32(dataU32);
    printf("result: %d\nmatched: %d\n", U32Check, U32Check == dataU32);

    if(U32Check != dataU32) {
        ++counter_notBigEndian;
    }

    // i32 -----------------------------------------------------------------------------
    /* BigEndianI32 */
    printf("\nBigEndianI32\ndata: %d\n", dataI32);
    I32Check = BigEndianI32(dataI32);
    printf("result: %d\nmatched: %d\n", I32Check, I32Check == dataI32);

    if(I32Check != dataI32) {
        ++counter_notBigEndian;
    }

    // u64 -----------------------------------------------------------------------------
    /* BigEndianU64 */
    printf("\nBigEndianU64\ndata: %" PRId64 "\n", dataU64);
    U64Check = BigEndianU64(dataU64);
    printf("result: %" PRId64 "\nmatched: %d\n", U64Check, U64Check == dataU64);

    if(I32Check != dataI32) {
        ++counter_notBigEndian;
    }

    // i64 -----------------------------------------------------------------------------
    /* BigEndianI64 */
    printf("\nBigEndianI64\ndata: %" SCNd64 "\n", dataI64);
    I64Check = BigEndianI64(dataI64);
    printf("result: %" SCNd64 "\nmatched: %d\n", I64Check, I64Check == dataI64);

    if(I64Check != dataI64) {
        ++counter_notBigEndian;
    }


    // float -----------------------------------------------------------------------------
    /* BigEndianFloat */
    printf("\nBigEndianFloat\ndata: %f\n", dataF32);
    floatCheck = BigEndianF32(dataF32);
    printf("result: %f\nmatched: %d\n", floatCheck, floatCheck == dataF32);

    if(floatCheck != dataF32) {
        ++counter_notBigEndian;
    }

    // double -----------------------------------------------------------------------------
    /* BigEndianDouble */
    printf("\nBigEndianDouble\ndata: %f\n", dataF64);
    doubleCheck = BigEndianF64(dataF64);
    printf("result: %f\nmatched: %d\n", doubleCheck, doubleCheck == dataF64);

    if(doubleCheck != dataF64) {
        ++counter_notBigEndian;
    }

#else
#    error unsupported endianness
#endif

    // dataset for test toyr system -------------------------------------------------------
    (void)dataU16;
    (void)dataI16;
    (void)dataU32;
    (void)dataI32;
    (void)dataU64;
    (void)dataI64;
    (void)dataF32;
    (void)dataF64;
    // ------------------------------------------------------------------------------------


    // check values ----------
    (void)U16Check;
    (void)I16Check;
    (void)U32Check;
    (void)I32Check;
    (void)U64Check;
    (void)I64Check;
    (void)floatCheck;
    (void)doubleCheck;
    //------------------------

    printf("\n----------> ENDIAN RESULT <---------------------------\n");
    int test_passed = (counter_notBigEndian == 0) && (counter_notlittleEndian == 0) && (bytes_order_error == 0);

    printf("Byte order counter exit with error: %d\n", bytes_order_error);
    printf("Big_endian test exit with error: %d\n", counter_notBigEndian);
    printf("Little_endian test exit with error: %d\n", counter_notlittleEndian);
    printf("Your system endian: %s\n", MY_SYSTEM_IS_LITTLE_ENDIAN ? "LITTLE_ENDIAN": "BIG_ENDIAN");
    printf("ENDIAN LIB Enable for: %s\n", (MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN) ? "LITTLE_ENDIAN": "BIG_ENDIAN");
    if((MY_SYSTEM_IS_LITTLE_ENDIAN && (MY_ENDIAN_ORDER != MY_LITTLE_ENDIAN)) || (MY_SYSTEM_IS_BIG_ENDIAN && (MY_ENDIAN_ORDER != MY_BIG_ENDIAN))) {
        printf("ERROR!!! : not matching byte orders with current system\n");
        test_passed = 0;
    }
    printf("ENDIAN LIB TEST PASSED: %d\n", test_passed);


    fflush(stdout);
    return test_passed;
}


#undef C99MY_ENDIAN_STATIC_ASSERTION_CREATE
