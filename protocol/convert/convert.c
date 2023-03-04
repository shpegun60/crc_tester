#include "convert.h"
#include "byte_order.h"

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
 * LSB - first universal
 * ******************************************
 */

void TEMPLATE(convertRead_LSB, uni)(reg n, u8* data, reg* pos, void* value)
{
    reg posInternal = (*pos);

    MY_CTYPE_USER_DATA_MEMCPY(n, (u8*)(data + posInternal), value);
    posInternal += n;
    (*pos) = posInternal;
}

void TEMPLATE(convertWrite_LSB, uni)(reg n, u8* data, reg* pos, void* value)
{
    reg posInternal = (*pos);

    MY_CTYPE_USER_DATA_MEMCPY(n, value, (u8*)(data + posInternal));
    posInternal += n;

    (*pos) = posInternal;
}


//with check buffer-----------------------------------------------------------------------------
void TEMPLATE(convertReadCheck_LSB, uni) (reg n, u8* data, reg* pos, void* value, reg buffSize, b* ok)
{
    reg posInternal = (*pos);

    if((posInternal + n) > buffSize) {
        (*ok) = CTYPE_FALSE;
        return;
    }

    MY_CTYPE_USER_DATA_MEMCPY(n, (u8*)(data + posInternal), value);
    posInternal += n;
    (*ok) = CTYPE_TRUE;
    (*pos) = posInternal;
}
void TEMPLATE(convertWriteCheck_LSB, uni) (reg n, u8* data, reg* pos, void* value, reg buffSize, b* ok)
{
    reg posInternal = (*pos);

    if((posInternal + n) > buffSize) {
        (*ok) = CTYPE_FALSE;
        return;
    }

    MY_CTYPE_USER_DATA_MEMCPY(n, value, (u8*)(data + posInternal));
    posInternal += n;
    (*ok) = CTYPE_TRUE;
    (*pos) = posInternal;
}

// position not a pointer
void TEMPLATE(convertWrite_cpos_LSB, uni)(reg n, u8* data, reg pos, void* value)
{
    MY_CTYPE_USER_DATA_MEMCPY(n, value, (u8*)(data + pos));
}

void TEMPLATE(convertRead_cpos_LSB, uni)(reg n, u8* data, reg pos, void* value)
{
    MY_CTYPE_USER_DATA_MEMCPY(n, (u8*)(data + pos), value);
}

//with check buffer-----------------------------------------------------------------------------
void TEMPLATE(convertReadCheck_cpos_LSB, uni) (reg n, u8* data, reg pos, void* value, reg buffSize, b* ok)
{
    if((pos + n) > buffSize) {
        (*ok) = CTYPE_FALSE;
        return;
    }

    MY_CTYPE_USER_DATA_MEMCPY(n, (u8*)(data + pos), value);
    (*ok) = CTYPE_TRUE;
}
void TEMPLATE(convertWriteCheck_cpos_LSB, uni) (reg n, u8* data, reg pos, void* value, reg buffSize, b* ok)
{
    if((pos + n) > buffSize) {
        (*ok) = CTYPE_FALSE;
        return;
    }

    MY_CTYPE_USER_DATA_MEMCPY(n, value, (u8*)(data + pos));
    (*ok) = CTYPE_TRUE;
}

/*
 * ******************************************
 * MSB - first universal
 * ******************************************
 */

void TEMPLATE(convertRead_MSB, uni)(reg n, u8* data, reg* pos, void* value)
{
    reg posInternal = (*pos);

    MY_CTYPE_USER_DATA_REVCPY(n, (u8*)(data + posInternal), value);
    posInternal += n;
    (*pos) = posInternal;
}

void TEMPLATE(convertWrite_MSB, uni)(reg n, u8* data, reg* pos, void* value)
{
    reg posInternal = (*pos);

    MY_CTYPE_USER_DATA_REVCPY(n, value, (u8*)(data + posInternal));
    posInternal += n;
    (*pos) = posInternal;
}

//with check buffer-----------------------------------------------------------------------------
void TEMPLATE(convertReadCheck_MSB, uni) (reg n, u8* data, reg* pos, void* value, reg buffSize, b* ok)
{
    reg posInternal = (*pos);

    if((posInternal + n) > buffSize) {
        (*ok) = CTYPE_FALSE;
        return;
    }

    MY_CTYPE_USER_DATA_REVCPY(n, (u8*)(data + posInternal), value);
    posInternal += n;
    (*ok) = CTYPE_TRUE;
    (*pos) = posInternal;
}
void TEMPLATE(convertWriteCheck_MSB, uni) (reg n, u8* data, reg* pos, void* value, reg buffSize, b* ok)
{
    reg posInternal = (*pos);

    if((posInternal + n) > buffSize) {
        (*ok) = CTYPE_FALSE;
        return;
    }

    MY_CTYPE_USER_DATA_REVCPY(n, value, (u8*)(data + posInternal));
    posInternal += n;
    (*ok) = CTYPE_TRUE;
    (*pos) = posInternal;
}

// position not a pointer
void TEMPLATE(convertRead_cpos_MSB, uni)(reg n, u8* data, reg pos, void* value)
{
    MY_CTYPE_USER_DATA_REVCPY(n, (u8*)(data + pos), value);
}

void TEMPLATE(convertWrite_cpos_MSB, uni)(reg n, u8* data, reg pos, void* value)
{
    MY_CTYPE_USER_DATA_REVCPY(n, value, (u8*)(data + pos));
}

//with check buffer-----------------------------------------------------------------------------
void TEMPLATE(convertReadCheck_cpos_MSB, uni) (reg n, u8* data, reg pos, void* value, reg buffSize, b* ok)
{
    if((pos + n) > buffSize) {
        (*ok) = CTYPE_FALSE;
        return;
    }

    MY_CTYPE_USER_DATA_REVCPY(n, (u8*)(data + pos), value);
    (*ok) = CTYPE_TRUE;
}
void TEMPLATE(convertWriteCheck_cpos_MSB, uni) (reg n, u8* data, reg pos, void* value, reg buffSize, b* ok)
{
    if((pos + n) > buffSize) {
        (*ok) = CTYPE_FALSE;
        return;
    }

    MY_CTYPE_USER_DATA_REVCPY(n, value, (u8*)(data + pos));
    (*ok) = CTYPE_TRUE;
}




//---------TEST----------------------------------------------------------
#ifndef CONVERT_TEST_DISABLE

#include <stdio.h>


/*
 * ******************************************
 * universal type test
 * ******************************************
 */

int TEMPLATE(convertTest_WRITE_READ, uni) (void (*read_ptr)(reg n, u8* data, reg* pos, void* value), void (*write_ptr)(reg n, u8* data, reg* pos, void* value), u64 value)
{
    u8 buff[sizeof(u64) * 2 + 2];
    reg pos = 0;
    u64 value_check;

    int notValidcnt = 0;

    // two write function check
    write_ptr(sizeof(u64), buff, &pos, (u8*)&value);
    if(pos != sizeof(u64)) {
        ++notValidcnt;
    }

    write_ptr(sizeof(u64), buff, &pos, (u8*)&value);
    if(pos != (sizeof(u64) * 2)) {
        ++notValidcnt;
    }
    pos = 0;

    // two read function check
    read_ptr(sizeof(u64), buff, &pos, (u8*)&value_check);
    if(value_check != value || pos != sizeof(u64)) {
        ++notValidcnt;
    }
    read_ptr(sizeof(u64), buff, &pos, (u8*)&value_check);
    if(value_check != value || pos != (sizeof(u64) * 2)) {
        ++notValidcnt;
    }
    return notValidcnt;
}

int TEMPLATE(convertTest_WRITE_READ_CPOS, uni) (void (*read_ptr)(reg n, u8* data, reg pos, void* value), void (*write_ptr)(reg n, u8* data, reg pos, void* value), u64 value)
{
    u8 buff[sizeof(u64) * 2 + 2];
    reg pos = 0;
    u64 value_check;

    int notValidcnt = 0;

    // two write function check
    write_ptr(sizeof(u64), buff, pos, (u8*)&value);
    pos += sizeof(u64);
    write_ptr(sizeof(u64), buff, pos, (u8*)&value);
    pos = 0;

    // two read function check
    read_ptr(sizeof(u64), buff, pos, (u8*)&value_check);
    if(value_check != value) {
        ++notValidcnt;
    }
    pos += sizeof(u64);

    read_ptr(sizeof(u64), buff, pos, (u8*)&value_check);
    if(value_check != value) {
        ++notValidcnt;
    }
    return notValidcnt;
}

int TEMPLATE(convertTest_WRITE_READ_BUFFER_CHK, uni) (void (*read_ptr)(reg n, u8* data, reg* pos, void* value, reg buffSize, b* ok), void (*write_ptr)(reg n, u8* data, reg* pos, void* value, reg buffSize, b* ok),  u64 value)
{
    reg buffsize = sizeof(u64) * 2 + 2;
    u8 buff[sizeof(u64) * 2 + 2];
    reg pos = 0;
    b ok = CTYPE_FALSE;
    u64 value_check;

    int notValidcnt = 0;

    // write function check
    ok = CTYPE_TRUE;
    for(reg i = 0; i < sizeof(u64); ++i) {
        write_ptr(sizeof(u64), buff, &pos, (u8*)&value, i, &ok);
        if(ok == CTYPE_TRUE || pos != 0) {
            ++notValidcnt;
        }
        ok = CTYPE_TRUE;
    }
    ok = CTYPE_FALSE;
    pos = 0;

    write_ptr(sizeof(u64), buff, &pos, (u8*)&value, buffsize, &ok);
    if(pos != sizeof(u64) || ok != CTYPE_TRUE) {
        ++notValidcnt;
    }
    ok = CTYPE_FALSE;

    write_ptr(sizeof(u64), buff, &pos, (u8*)&value, buffsize, &ok);
    if(pos != sizeof(u64) * 2 || ok != CTYPE_TRUE) {
        ++notValidcnt;
    }
    ok = CTYPE_FALSE;
    pos = 0;

    // read functions check
    ok = CTYPE_TRUE;
    for(reg i = 0; i < sizeof(u64); ++i) {
        read_ptr(sizeof(u64), buff, &pos, (u8*)&value_check, i, &ok);
        if(ok == CTYPE_TRUE || pos != 0) {
            ++notValidcnt;
        }
        ok = CTYPE_TRUE;
    }
    ok = CTYPE_FALSE;

    read_ptr(sizeof(u64), buff, &pos, (u8*)&value_check, buffsize, &ok);
    if(value_check != value || (ok == CTYPE_FALSE) || pos != sizeof(u64)) {
        ++notValidcnt;
    }
    ok = CTYPE_FALSE;

    read_ptr(sizeof(u64), buff, &pos, (u8*)&value_check, buffsize, &ok);
    if(value_check != value || (ok == CTYPE_FALSE) || pos != sizeof(u64)*2) {
        ++notValidcnt;
    }

    return notValidcnt;
}

int TEMPLATE(convertTest_WRITE_READ_CPOS_BUFFER_CHK, uni) (void (*read_ptr)(reg n, u8* data, reg pos, void* value, reg buffSize, b* ok), void (*write_ptr)(reg n, u8* data, reg pos, void* value, reg buffSize, b* ok),  u64 value)
{
    reg buffsize = sizeof(u64) * 2 + 2;
    u8 buff[sizeof(u64) * 2 + 2];
    reg pos = 0;
    b ok = CTYPE_FALSE;
    u64 value_check;

    int notValidcnt = 0;

    // write function check
    ok = CTYPE_TRUE;
    for(reg i = 0; i < sizeof(u64); ++i) {
        write_ptr(sizeof(u64), buff, pos, (u8*)&value, i, &ok);
        if(ok == CTYPE_TRUE) {
            ++notValidcnt;
        }
        ok = CTYPE_TRUE;
    }
    ok = CTYPE_FALSE;

    write_ptr(sizeof(u64), buff, pos, (u8*)&value, buffsize, &ok);
    if(ok != CTYPE_TRUE) {
        ++notValidcnt;
    }
    ok = CTYPE_FALSE;
    pos += sizeof(u64);

    write_ptr(sizeof(u64), buff, pos, (u8*)&value, buffsize, &ok);
    if(ok != CTYPE_TRUE) {
        ++notValidcnt;
    }
    ok = CTYPE_FALSE;
    pos = 0;

    // read functions check
    ok = CTYPE_TRUE;
    for(reg i = 0; i < sizeof(u64); ++i) {
        read_ptr(sizeof(u64), buff, pos, (u8*)&value_check, i, &ok);
        if(ok == CTYPE_TRUE) {
            ++notValidcnt;
        }
        ok = CTYPE_TRUE;
    }
    ok = CTYPE_FALSE;
    pos = 0;

    read_ptr(sizeof(u64), buff, pos, (u8*)&value_check, buffsize, &ok);
    if(value_check != value || (ok == CTYPE_FALSE)) {
        ++notValidcnt;
    }
    ok = CTYPE_FALSE;
    pos += sizeof(u64);

    read_ptr(sizeof(u64), buff, pos, (u8*)&value_check, buffsize, &ok);
    if(value_check != value || (ok == CTYPE_FALSE)) {
        ++notValidcnt;
    }

    return notValidcnt;
}




static int TEMPLATE(convertTest, uni) (int testN, int randomSeed)
{
    srand(sizeof(reg) + randomSeed); // use type size as seed for random generator

    u64 value = (u64)(sizeof(u64));
    u64 valueLast = 0;

    int testCounter = 0;
    while(testN--) {
        /*
         *****************************************************************
            lsb--------------------------------------------------------
         *****************************************************************
        */

        testCounter += TEMPLATE(convertTest_WRITE_READ, uni)(TEMPLATE(convertRead_LSB, uni), TEMPLATE(convertWrite_LSB, uni), value);

        // buffer functions check test-----------------------------------------------------
        testCounter += TEMPLATE(convertTest_WRITE_READ_BUFFER_CHK, uni) (TEMPLATE(convertReadCheck_LSB, uni), TEMPLATE(convertWriteCheck_LSB, uni),  value);

        // cpos
        testCounter += TEMPLATE(convertTest_WRITE_READ_CPOS, uni) (TEMPLATE(convertRead_cpos_LSB, uni), TEMPLATE(convertWrite_cpos_LSB, uni), value);

        // buffer functions check test cpos-----------------------------------------------------
        testCounter += TEMPLATE(convertTest_WRITE_READ_CPOS_BUFFER_CHK, uni) (TEMPLATE(convertReadCheck_cpos_LSB, uni), TEMPLATE(convertWriteCheck_cpos_LSB, uni),  value);



        /*
         *****************************************************************
            msb--------------------------------------------------------
         *****************************************************************
        */
        testCounter += TEMPLATE(convertTest_WRITE_READ, uni)(TEMPLATE(convertRead_MSB, uni), TEMPLATE(convertWrite_MSB, uni), value);


        // buffer functions check test-----------------------------------------------------
        testCounter += TEMPLATE(convertTest_WRITE_READ_BUFFER_CHK, uni) (TEMPLATE(convertReadCheck_MSB, uni), TEMPLATE(convertWriteCheck_MSB, uni),  value);

        // cpos
        testCounter += TEMPLATE(convertTest_WRITE_READ_CPOS, uni) (TEMPLATE(convertRead_cpos_LSB, uni), TEMPLATE(convertWrite_cpos_LSB, uni), value);

        // buffer functions check test cpos-----------------------------------------------------
        testCounter += TEMPLATE(convertTest_WRITE_READ_CPOS_BUFFER_CHK, uni) (TEMPLATE(convertReadCheck_cpos_MSB, uni), TEMPLATE(convertWriteCheck_cpos_MSB, uni) ,  value);

        valueLast = value;
        value = (u64)(rand());
        while((value == 0) || (valueLast == value)) {
            value = (u64)(rand());
        }
    }

    return testCounter;
}


int convertTest(int testCount, int randomSeed)
{
    int convertTestcnt = 0;

    printf("\n\nCONVERT TEST START--> randon cnt: %d------------------------------------------\n", testCount);
    int test = TEMPLATE(convertTest, u8)(testCount, randomSeed);
    printf("u8 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, u16)(testCount, randomSeed);
    printf("u16 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, u24)(testCount, randomSeed);
    printf("u24 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, u32)(testCount, randomSeed);
    printf("u32 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, u64)(testCount, randomSeed);
    printf("u64 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, c8)(testCount, randomSeed);
    printf("c8 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, i8)(testCount, randomSeed);
    printf("i8 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, i16)(testCount, randomSeed);
    printf("i16 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, i24)(testCount, randomSeed);
    printf("i24 test exit with error: %d\n", test);
    convertTestcnt += test;


    test = TEMPLATE(convertTest, i32)(testCount, randomSeed);
    printf("i32 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, i64)(testCount, randomSeed);
    printf("i64 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, f32)(testCount, randomSeed);
    printf("f32 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, f64)(testCount, randomSeed);
    printf("f64 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, f128)(testCount, randomSeed);
    printf("f128 test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, b)(testCount, randomSeed);
    printf("b test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, reg)(testCount, randomSeed);
    printf("reg test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, sreg)(testCount, randomSeed);
    printf("sreg test exit with error: %d\n", test);
    convertTestcnt += test;

    test = TEMPLATE(convertTest, uni)(testCount, randomSeed);
    printf("universal test exit with error: %d\n", test);
    convertTestcnt += test;
    printf("CONVERT TEST FINISHED!!!------------------------------------------\n");


    printf("CONVERT TEST EXIT WITH ERROR: %d\n", convertTestcnt);

    ///////////////////////////////////////////////////
    printf("CONVERT SIMPLE: -------------------------\n");
    u8 data[100];
    reg pos = 0;

    TEMPLATE(convertWrite_MSB, f32)(data, &pos, 123.456);
    TEMPLATE(convertWrite_MSB, u16)(data, &pos, 853);
    TEMPLATE(CAT_ENDIAN(convertWrite), u64)(data, &pos, 1122334455);

    u64 tmpuni = 1234567890ULL;
    TEMPLATE(CAT_ENDIAN(convertWrite), uni)(sizeof(u64), data, &pos, &tmpuni);


    pos = 0;
    f32 tmp = TEMPLATE(convertRead_MSB, f32)(data, &pos);
    u16 tmp16 = TEMPLATE(convertRead_MSB, u16)(data, &pos);
    u64 tmp64 = TEMPLATE(CAT_ENDIAN(convertRead), u64)(data, &pos);

    u64 tmpunitest;
    TEMPLATE(CAT_ENDIAN(convertRead), uni)(sizeof(u64), data, &pos, &tmpunitest);

    printf("%f\n", tmp);
    printf("%d\n", tmp16);
    printf("%llu\n", tmp64);
    printf("%llu\n", tmpunitest);
    printf("%d\n", pos);

    fflush(stdout);

    return convertTestcnt;
}
#endif /* CONVERT_TEST_DISABLE */


