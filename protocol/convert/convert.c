#include "convert.h"
#include <stdio.h>

#ifdef T
#undef T
#endif

#define T u8
#include "convert_template.c"
#define T u16
#include "convert_template.c"
#define T u24
#include "convert_template.c"
#define T u32
#include "convert_template.c"
#define T u64
#include "convert_template.c"

#define T c8
#include "convert_template.c"
#define T i8
#include "convert_template.c"
#define T i16
#include "convert_template.c"
#define T i24
#include "convert_template.c"
#define T i32
#include "convert_template.c"
#define T i64
#include "convert_template.c"

#define T f32
#include "convert_template.c"
#define T f64
#include "convert_template.c"
#define T f128
#include "convert_template.c"


#define T b
#include "convert_template.c"

#define T reg
#include "convert_template.c"
#define T sreg
#include "convert_template.c"

/*
 * ******************************************
 * LSB - first univarsal
 * ******************************************
 */

void TEMPLATE(convertRead_LSB, uni)(u8 n, u8* data, u16* pos, u8* value)
{
    MY_CTYPE_USER_DATA_MEMCPY(n, (u8*)(data + *pos), value);
    *pos += n;
}

void TEMPLATE(convertWrite_LSB, uni)(u8 n, u8* data, u16* pos, u8* value)
{
    MY_CTYPE_USER_DATA_MEMCPY(n, value, (u8*)(data + *pos));
    *pos += n;
}

// position not a pointer
void TEMPLATE(convertWrite_cpos_LSB, uni)(u8 n, u8* data, u16 pos, u8* value)
{
    MY_CTYPE_USER_DATA_MEMCPY(n, value, (u8*)(data + pos));
}

void TEMPLATE(convertRead_cpos_LSB, uni)(u8 n, u8* data, u16 pos, u8* value)
{
    MY_CTYPE_USER_DATA_MEMCPY(n, (u8*)(data + pos), value);
}

/*
 * ******************************************
 * MSB - first univarsal
 * ******************************************
 */

void TEMPLATE(convertRead_MSB, uni)(u8 n, u8* data, u16* pos, u8* value)
{
    MY_CTYPE_USER_DATA_REVCPY(n, (u8*)(data + *pos), value);
    *pos += n;
}

void TEMPLATE(convertWrite_MSB, uni)(u8 n, u8* data, u16* pos, u8* value)
{
    MY_CTYPE_USER_DATA_REVCPY(n, value, (u8*)(data + *pos));
    *pos += n;
}

// position not a pointer
void TEMPLATE(convertRead_cpos_MSB, uni)(u8 n, u8* data, u16 pos, u8* value)
{
    MY_CTYPE_USER_DATA_REVCPY(n, (u8*)(data + pos), value);
}

void TEMPLATE(convertWrite_cpos_MSB, uni)(u8 n, u8* data, u16 pos, u8* value)
{
    MY_CTYPE_USER_DATA_REVCPY(n, value, (u8*)(data + pos));
}




//---------TEST----------------------------------------------------------
#ifndef CONVERT_TEST_DISABLE

#define CONVERT_RAND_TEST_COUNT 50000


/*
 * ******************************************
 * universal type test
 * ******************************************
 */
__attribute__((unused)) static int convertTest_uni (int testN)
{
    srand(sizeof(reg)); // use type size as seed for random generator

    u8 buff[sizeof(u64) + 1];
    u16 pos = 0;

    u64 value = (u64)(sizeof(u64));
    u64 valueLast = 0;
    u64 value_check = 0;


    int testCounter = 0;
    while(testN--) {
        // lsb--------------------------------------------------------
        TEMPLATE(convertWrite_LSB, uni)(sizeof(u64), buff, &pos, (u8*)&value);
        if(pos != sizeof(u64)) {
            ++testCounter;
        }
        pos = 0;
        TEMPLATE(convertRead_LSB, uni)(sizeof(u64), buff, &pos, (u8*)&value_check);
        if(value_check != value) {
            ++testCounter;
        }
        pos = 0;

        // cpos
        TEMPLATE(convertWrite_cpos_LSB, uni)(sizeof(u64), buff, pos, (u8*)&value);
        pos = 0;
        TEMPLATE(convertRead_cpos_LSB, uni)(sizeof(u64), buff, pos, (u8*)&value_check);
        if(value_check != value) {
            ++testCounter;
        }
        pos = 0;



        // msb---------------------------------------------------------
        TEMPLATE(convertWrite_MSB, uni)(sizeof(u64), buff, &pos, (u8*)&value);
        if(pos != sizeof(u64)) {
            ++testCounter;
        }
        pos = 0;
        TEMPLATE(convertRead_MSB, uni)(sizeof(u64), buff, &pos, (u8*)&value_check);
        if(value_check != value) {
            ++testCounter;
        }
        pos = 0;

        // cpos
        TEMPLATE(convertWrite_cpos_MSB, uni)(sizeof(u64), buff, pos, (u8*)&value);
        pos = 0;
        TEMPLATE(convertRead_cpos_MSB, uni)(sizeof(u64), buff, pos, (u8*)&value_check);
        if(value_check != value) {
            ++testCounter;
        }
        pos = 0;

        valueLast = value;
        value = (u64)(rand());
        while((value == 0) || (valueLast == value)) {
            value = (u64)(rand());
        }
    }

    return testCounter;
}

int convertTest()
{
    int convertTestcnt = 0;

    printf("\n\nCONVERT TEST START--> randon cnt: %d------------------------------------------\n", CONVERT_RAND_TEST_COUNT);
    int test = TEMPLATE(convertTest, u8)(CONVERT_RAND_TEST_COUNT);
    printf("u8 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, u16)(CONVERT_RAND_TEST_COUNT);
    printf("u16 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, u24)(CONVERT_RAND_TEST_COUNT);
    printf("u24 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, u32)(CONVERT_RAND_TEST_COUNT);
    printf("u32 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, u64)(CONVERT_RAND_TEST_COUNT);
    printf("u64 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, c8)(CONVERT_RAND_TEST_COUNT);
    printf("c8 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, i8)(CONVERT_RAND_TEST_COUNT);
    printf("i8 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, i16)(CONVERT_RAND_TEST_COUNT);
    printf("i16 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, i24)(CONVERT_RAND_TEST_COUNT);
    printf("i24 test exit with error: %d\n", test);
    convertTestcnt += test;


    test = TEMPLATE(convertTest, i32)(CONVERT_RAND_TEST_COUNT);
    printf("i32 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, i64)(CONVERT_RAND_TEST_COUNT);
    printf("i64 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, f32)(CONVERT_RAND_TEST_COUNT);
    printf("f32 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, f64)(CONVERT_RAND_TEST_COUNT);
    printf("f64 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, f128)(CONVERT_RAND_TEST_COUNT);
    printf("f128 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, b)(CONVERT_RAND_TEST_COUNT);
    printf("b test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, reg)(CONVERT_RAND_TEST_COUNT);
    printf("reg test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, sreg)(CONVERT_RAND_TEST_COUNT);
    printf("sreg test exit with error: %d\n", test);
    convertTestcnt += test;

    test = convertTest_uni(CONVERT_RAND_TEST_COUNT);
    printf("universal test exit with error: %d\n", test);
    convertTestcnt += test;
    printf("CONVERT TEST FINISHED!!!------------------------------------------\n");


    printf("CONVERT TEST EXIT WITH ERROR: %d\n", convertTestcnt);

    fflush(stdout);

    return convertTestcnt;
}
#endif /* CONVERT_TEST_DISABLE */

#ifdef CONVERT_TEST_DISABLE
    #undef CONVERT_TEST_DISABLE
#endif /* CONVERT_TEST_DISABLE */

