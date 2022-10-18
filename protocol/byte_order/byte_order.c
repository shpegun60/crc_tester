#include "byte_order.h"

#ifndef BYTE_ORDER_TEST_DISABLE

/*
 * ******************************************
 * test
 * ******************************************
 */

#include <stdio.h>
#include <inttypes.h>

__attribute__((unused)) static int byteOrderRevCheck(void * source, void * destination, int len)
{
    u8 * src = source;
    u8 * dst = destination;

    while(len--) {
        if(*src++ != *(dst + len)) {
            return 1;
        }
    }
    return 0;
}


int endiansTest()
{
    // dataset for test your system -------------------------------------------------------
    const u16 dataU16 = 12345U;
    const i16 dataI16 = -1000L;
    const u32 dataU32 = 1565465464;
    const i32 dataI32 = -1005500099L;
    const u64 dataU64 = 2147483647786453ULL;
    const i64 dataI64 = -412123456442352LL;
    const f32 dataF32 = 123.321;
    const f64 dataF64 = 987654321.123456789;
    // ------------------------------------------------------------------------------------


    // check values ----------
    u16 U16Check;
    i16 I16Check;
    u32 U32Check;
    i32 I32Check;
    u64 U64Check;
    i64 I64Check;
    f32 f32Check;
    f64 f64Check;
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
    // f32 -----------------------------------------------------------------------------
    /* LittleEndianf32 */
    printf("\nLittleEndianf32\ndata: %f\n", dataF32);
    f32Check = LittleEndianF32(dataF32);
    printf("result: %f\nmatched: %d\n", f32Check, f32Check == dataF32);

    if(f32Check != dataF32) {
        ++counter_notlittleEndian;
    }

    // f64 -----------------------------------------------------------------------------
    /* LittleEndianf64 */
    printf("\nLittleEndianf64\ndata: %f\n", dataF64);
    f64Check = LittleEndianF64(dataF64);
    printf("result: %f\nmatched: %d\n", f64Check, f64Check == dataF64);

    if(f64Check != dataF64) {
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

    if(byteOrderRevCheck((u8*)&dataU16, &U16Check, sizeof(dataU16))) {
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

    if(byteOrderRevCheck((u8*)&dataI16, &I16Check, sizeof(dataI16))) {
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

    if(byteOrderRevCheck((u8*)&dataU32, &U32Check, sizeof(dataU32))) {
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

    if(byteOrderRevCheck((u8*)&dataI32, &I32Check, sizeof(dataI32))) {
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

    if(byteOrderRevCheck((u8*)&dataU64, &U64Check, sizeof(dataU64))) {
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

    if(byteOrderRevCheck((u8*)&dataI64, &I64Check, sizeof(dataI64))) {
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


    // f32 -----------------------------------------------------------------------------
    /* BigEndianf32 */
    printf("\nBigEndianf32\ndata: %f\n", dataF32);
    f32Check = BigEndianF32(dataF32);

    if(byteOrderRevCheck((u8*)&dataF32, &f32Check, sizeof(dataF32))) {
        ++bytes_order_error;
        printf("bytes order check error: 1\n");
    } else {
        printf("bytes order check error: 0\n");
    }

    f32Check = BigEndianF32(f32Check);
    printf("result: %f\nmatched: %d\n", f32Check, f32Check == dataF32);

    if(f32Check != dataF32) {
        ++counter_notBigEndian;
    }

    // f64 -----------------------------------------------------------------------------
    /* BigEndianf64 */
    printf("\nBigEndianf64\ndata: %f\n", dataF64);
    f64Check = BigEndianF64(dataF64);

    if(byteOrderRevCheck((u8*)&dataF64, &f64Check, sizeof(dataF64))) {
        ++bytes_order_error;
        printf("bytes order check error: 1\n");
    } else {
        printf("bytes order check error: 0\n");
    }

    f64Check = BigEndianF64(f64Check);
    printf("result: %f\nmatched: %d\n", f64Check, f64Check == dataF64);

    if(f64Check != dataF64) {
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

    if(byteOrderRevCheck((u8*)&dataU16, &U16Check, sizeof(dataU16))) {
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

    if(byteOrderRevCheck((u8*)&dataI16, &I16Check, sizeof(dataI16))) {
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

    if(byteOrderRevCheck((u8*)&dataU32, &U32Check, sizeof(dataU32))) {
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

    if(byteOrderRevCheck((u8*)&dataI32, &I32Check, sizeof(dataI32))) {
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

    if(byteOrderRevCheck((u8*)&dataU64, &U64Check, sizeof(dataU64))) {
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

    if(byteOrderRevCheck((u8*)&dataI64, &I64Check, sizeof(dataI64))) {
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
    // f32 -----------------------------------------------------------------------------
    /* LittleEndianf32 */
    printf("\nLittleEndianf32\ndata: %f\n", dataF32);
    f32Check = LittleEndianF32(dataF32);

    if(byteOrderRevCheck((u8*)&dataF32, &f32Check, sizeof(dataF32))) {
        ++bytes_order_error;
        printf("bytes order check error: 1\n");
    } else {
        printf("bytes order check error: 0\n");
    }

    f32Check = LittleEndianF32(f32Check);
    printf("result: %f\nmatched: %d\n", f32Check, f32Check == dataF32);

    if(f32Check != dataF32) {
        ++counter_notlittleEndian;
    }

    // f64 -----------------------------------------------------------------------------
    /* LittleEndianf64 */
    printf("\nLittleEndianf64\ndata: %f\n", dataF64);
    f64Check = LittleEndianF64(dataF64);

    if(byteOrderRevCheck((u8*)&dataF64, &f64Check, sizeof(dataF64))) {
        ++bytes_order_error;
        printf("bytes order check error: 1\n");
    } else {
        printf("bytes order check error: 0\n");
    }

    f64Check = LittleEndianF64(f64Check);
    printf("result: %f\nmatched: %d\n", f64Check, f64Check == dataF64);

    if(f64Check != dataF64) {
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


    // f32 -----------------------------------------------------------------------------
    /* BigEndianf32 */
    printf("\nBigEndianf32\ndata: %f\n", dataF32);
    f32Check = BigEndianF32(dataF32);
    printf("result: %f\nmatched: %d\n", f32Check, f32Check == dataF32);

    if(f32Check != dataF32) {
        ++counter_notBigEndian;
    }

    // f64 -----------------------------------------------------------------------------
    /* BigEndianf64 */
    printf("\nBigEndianf64\ndata: %f\n", dataF64);
    f64Check = BigEndianF64(dataF64);
    printf("result: %f\nmatched: %d\n", f64Check, f64Check == dataF64);

    if(f64Check != dataF64) {
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
    (void)f32Check;
    (void)f64Check;
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
    return !test_passed;
}


#endif /* BYTE_ORDER_TEST_DISABLE */
