/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/



#include "crc_test.h"

#ifndef _MY_CRC_TEST_DISABLE

#include <stdlib.h>


#include <stdio.h>

#include "crc8.h"
#include "crc16.h"
#include "crc32.h"


#define CRC_RANDOM_TEST_NUM 500

__attribute__((unused)) static uint8_t _proceedCrc_OLD(uint8_t crc, uint8_t ch) { // original crc from old protocol
    crc ^= ch;
    for (int i = 0; i < 8; i++)
        crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
    return crc;
}

#ifdef _MY_CRC8_ENA
static int crc8_test(u8 *data, size_t len, u8 * res)
{
    u8 crc8[5] = {CRC8INIT, CRC8INIT, CRC8INIT, CRC8INIT, CRC8INIT};
    
    printf("\n\n------> crc 8 test <-------------------------------------------\n");
    printf("data: 0x");
    for(size_t i = 0; i < len; ++i) {
        printf("%x.", data[i]);
    }

    printf("\nsizeof buffer: %d\n\n", (int)len);
    

#ifdef _MY_CRC8_TABLE_CALC_ENA
    crc8[0] = fast_crc8_maxim_array(data, len);
    printf("crc8 --> fast_crc8_maxim_array: 0x%x", crc8[0]);

    for(size_t i = 0; i < len; ++i) {
        crc8[1] = fast_crc8_maxim_byte(crc8[1], data[i]);
    }
    printf("\ncrc8 --> fast_crc8_maxim_byte: 0x%x", crc8[1]);
#endif /* _MY_CRC8_TABLE_CALC_ENA */



#ifdef _MY_CRC8_GENERIC_CALC_ENA
    crc8[2] = slow_crc8_maxim_array(data, len);
    printf("\ncrc8 --> slow_crc8_maxim_array: 0x%x", crc8[2]);

    for(size_t i = 0; i < len; ++i) {
        crc8[3] = slow_crc8_maxim_byte(crc8[3], data[i]);
    }
    printf("\ncrc8 --> slow_crc8_maxim_byte: 0x%x", crc8[3]);
#endif /* _MY_CRC8_GENERIC_CALC_ENA */
    
    for(size_t i = 0; i < len; ++i) {
        crc8[4] = _proceedCrc_OLD(crc8[4], data[i]);
    }
    printf("\ncrc8 --> _proceedCrc_OLD: 0x%x", crc8[4]);
    
    int counter_not_valid = 0;

#if defined(_MY_CRC8_TABLE_CALC_ENA) && defined (_MY_CRC8_GENERIC_CALC_ENA)

    for(int i = 1; i < 5; ++i) {
        if(crc8[0] != crc8[i]) {
            ++counter_not_valid;
        }
    }

    *res = crc8[0];
#elif defined(_MY_CRC8_TABLE_CALC_ENA) && !defined (_MY_CRC8_GENERIC_CALC_ENA)

    if(crc8[0] != crc8[1]) {
        ++counter_not_valid;
    }

    if(crc8[0] != crc8[4]) {
        ++counter_not_valid;
    }

    *res = crc8[0];
#else
    if(crc8[2] != crc8[3]) {
        ++counter_not_valid;
    }

    if(crc8[2] != crc8[4]) {
        ++counter_not_valid;
    }

    *res = crc8[2];
#endif /* defined(_MY_CRC8_TABLE_CALC_ENA) && defined (_MY_CRC8_GENERIC_CALC_ENA) */


    
    printf("\nerror counter %d\n", counter_not_valid);

    fflush(stdout);
    return counter_not_valid;
}

#endif /* _MY_CRC8_ENA */



#ifdef _MY_CRC16_ENA

static int crc16_test(u8 *data, size_t len, u16 * res)
{
    u16 crc16[4] = {CRC16INIT, CRC16INIT, CRC16INIT, CRC16INIT};

    printf("\n\n----> crc16 test <-------------------------------------------\n");
    printf("sizeof buffer: %d\n\n", (int)len);

#ifdef _MY_CRC16_TABLE_CALC_ENA

    crc16[0] = fast_crc16_t10_dif_array(data, len);
    printf("crc16 --> fast_crc16_t10_dif_array: 0x%x", crc16[0]);

    for(size_t i = 0; i < len; ++i) {
        crc16[1] = fast_crc16_t10_dif_byte(crc16[1], data[i]);
    }
    printf("\ncrc16 --> fast_crc16_t10_dif_byte: 0x%x", crc16[1]);

#endif /* _MY_CRC16_TABLE_CALC_ENA */


#ifdef _MY_CRC16_GENERIC_CALC_ENA

    crc16[2] = slow_crc16_t10_dif_array(data, len);
    printf("\ncrc16 --> slow_crc16_t10_dif_array: 0x%x", crc16[2]);

    for(size_t i = 0; i < len; ++i) {
        crc16[3] = slow_crc16_t10_dif_byte(crc16[3], data[i]);
    }
    printf("\ncrc16 --> slow_crc16_t10_dif_byte: 0x%x", crc16[3]);

#endif /* _MY_CRC16_GENERIC_CALC_ENA */

    int counter_not_valid = 0;

#if defined(_MY_CRC16_TABLE_CALC_ENA) && defined (_MY_CRC16_GENERIC_CALC_ENA)

    for(int i = 1; i < 4; ++i) {
        if(crc16[0] != crc16[i]) {
            ++counter_not_valid;
        }
    }

    *res = crc16[0];

#elif defined(_MY_CRC16_TABLE_CALC_ENA) && !defined (_MY_CRC16_GENERIC_CALC_ENA)
    if(crc16[0] != crc16[1]) {
        ++counter_not_valid;
    }

    *res = crc16[0];
#else
    if(crc16[2] != crc16[3]) {
        ++counter_not_valid;
    }

    *res = crc16[2];
#endif /* defined(_MY_CRC16_TABLE_CALC_ENA) && defined (_MY_CRC16_GENERIC_CALC_ENA) */


    printf("\nerror counter %d\n", counter_not_valid);
    fflush(stdout);
    return counter_not_valid;
}

#endif /* _MY_CRC16_ENA */


#ifdef _MY_CRC32_ENA

static int crc32_test(u8 *data, size_t len, u32 *res)
{
    u32 crc32[4] = {CRC32INIT, CRC32INIT, CRC32INIT, CRC32INIT};

    printf("\n\n--------> crc32 test <-------------------------------------------\n");
    printf("sizeof buffer: %d\n\n", (int)len);

#ifdef _MY_CRC32_TABLE_CALC_ENA

    crc32[0] = fast_crc32b_array(data, len);
    printf("crc32 --> fast_crc32b_array: 0x%x", (unsigned int)crc32[0]);

    for(size_t i = 0; i < len; ++i) {
        crc32[1] = fast_crc32b_byte(crc32[1], data[i]);
    }
    CRC32FINAL(crc32[1]);
    //crc32[1] = ~crc32[1];
    printf("\ncrc16 --> fast_crc32b_byte: 0x%x", (unsigned int)crc32[1]);

#endif /* _MY_CRC32_TABLE_CALC_ENA */


#ifdef _MY_CRC32_GENERIC_CALC_ENA

    crc32[2] = slow_crc32b_array(data, len);
    printf("\ncrc32 --> slow_crc32b_array: 0x%x", (unsigned int)crc32[2]);

    for(size_t i = 0; i < len; ++i) {
        crc32[3] = slow_crc32b_byte(crc32[3], data[i]);
    }
    CRC32FINAL(crc32[3]);
    //crc32[3] = ~crc32[3];
    printf("\ncrc16 --> slow_crc32b_byte: 0x%x", (unsigned int)crc32[3]);

#endif /* _MY_CRC32_GENERIC_CALC_ENA */


    int counter_not_valid = 0;

#if defined(_MY_CRC32_TABLE_CALC_ENA) && defined (_MY_CRC32_GENERIC_CALC_ENA)

    for(int i = 1; i < 4; ++i) {
        if(crc32[0] != crc32[i]) {
            ++counter_not_valid;
        }
    }

    *res = crc32[0];
#elif defined(_MY_CRC32_TABLE_CALC_ENA) && !defined (_MY_CRC32_GENERIC_CALC_ENA)

    if(crc32[0] != crc32[1]) {
        ++counter_not_valid;
    }

    *res = crc32[0];
#else

    if(crc32[2] != crc32[3]) {
        ++counter_not_valid;
    }

    *res = crc32[2];
#endif /* defined(_MY_CRC32_TABLE_CALC_ENA) && defined (_MY_CRC32_GENERIC_CALC_ENA) */



    printf("\nerror counter %d\n", counter_not_valid);
    fflush(stdout);
    return counter_not_valid;
}

#endif /* _MY_CRC32_ENA */

int crc_test()
{
    u8 data[] = "123456789";

    int not_passed8 = 0;
    int not_passed16 = 0;
    int not_passed32 = 0;

    u8 crc8_matched_with_test = 1;
    u8 crc16_matched_with_test = 1;
    u8 crc32_matched_with_test = 1;

    printf("\n-------------------------------\nMATCH TEST");
    printf("\n-------------------------------\n");


#ifdef _MY_CRC8_ENA
    u8 resCrc8 = CRC8INIT;

    not_passed8 = crc8_test(data, (sizeof (data) - 1), &resCrc8);
    printf("\ncheck test CRC8 exit with error: %d", not_passed8);

    crc8_matched_with_test = (resCrc8 == CRC8CHECK);
    printf("\nCRC8 is matched with test 0x%x: %d\n", CRC8CHECK, crc8_matched_with_test);

    if(!crc8_matched_with_test) {
        ++not_passed8;
    }
#else
    printf("\nCRC8 is disabled\n");
#endif /* _MY_CRC8_ENA */

    printf("\n-------------------------------\n");

#ifdef _MY_CRC16_ENA
    u16 resCrc16 = CRC16INIT;

    not_passed16 = crc16_test(data, (sizeof (data) - 1), &resCrc16);
    printf("\ncheck test CRC16 exit with error: %d", not_passed16);

    crc16_matched_with_test = (resCrc16 == CRC16CHECK);
    printf("\nCRC16 is matched with test 0x%x: %d\n", CRC16CHECK, crc16_matched_with_test);

    if(!crc16_matched_with_test) {
        ++not_passed16;
    }
#else
    printf("\nCRC16 is disabled\n");
#endif /* _MY_CRC16_ENA */

    printf("\n-------------------------------\n");

#ifdef _MY_CRC32_ENA
    u32 resCrc32 = CRC32INIT;

    not_passed32 = crc32_test(data, (sizeof (data) - 1), &resCrc32);
    printf("\ncheck test CRC32 exit with error: %d", not_passed32);

    crc32_matched_with_test = (resCrc32 == CRC32CHECK);
    printf("\nCRC32 is matched with test 0x%x: %d", (unsigned int)CRC32CHECK, crc32_matched_with_test);

    if(!crc32_matched_with_test) {
        ++not_passed32;
    }
#else
    printf("\nCRC32 is disabled\n");
#endif /* _MY_CRC32_ENA */

    fflush(stdout);

    if(not_passed8 != 0 || not_passed16 != 0 || not_passed32 != 0) {
        printf("\n\nEXIT, SOME CRC is NOT MATCHED WITH TEST VALUE \n");
        printf("CRC TEST PASSED: 0\n");
        fflush(stdout);
        return 0;
    }

    not_passed8 = 0;
    not_passed16 = 0;
    not_passed32 = 0;

    printf("\n\n-------------------------------\nRANDOM TEST");
    printf("\n-------------------------------\n");

    u8 randomData[4096*2];
    srand(100); // use current time as seed for random generator

#ifdef _MY_CRC8_ENA

    // crc 8 random test ------------------------------------------------
    for(size_t i = 0; i < CRC_RANDOM_TEST_NUM; ++i) {

        size_t len = 0;

        while(len == 0) {
            len = rand() % 16; // crc 8 maximum 15 bytes
        }

        for(size_t j = 0; j < len; ++j) {
            randomData[j] = rand() % 256;
        }

        not_passed8 += crc8_test(randomData, len, &resCrc8);
        fflush(stdout);
    }

#endif /* _MY_CRC8_ENA */


#ifdef _MY_CRC16_ENA
    // crc 16 random test ------------------------------------------------
    for(size_t i = 0; i < CRC_RANDOM_TEST_NUM; ++i) {

        size_t len = 0;

        while(len == 0) {
            len = rand() % 4096; // crc 16 maximum 4095 bytes
        }

        for(size_t j = 0; j < len; ++j) {
            randomData[j] = rand() % 256;
        }

        not_passed16 += crc16_test(randomData, len, &resCrc16);
        fflush(stdout);
    }

#endif /* _MY_CRC16_ENA */


#ifdef _MY_CRC32_ENA

    // crc 32 random test ------------------------------------------------
    for(size_t i = 0; i < CRC_RANDOM_TEST_NUM; ++i) {

        size_t len = 0;

        while(len == 0) {
            len = rand() % (4096*2); // crc 32 maximum over 4096 bytes
        }

        for(size_t j = 0; j < len; ++j) {
            randomData[j] = rand() % 256;
        }

        not_passed32 += crc32_test(randomData, len, &resCrc32);
        fflush(stdout);
    }

#endif /* _MY_CRC32_ENA */

    printf("\n-----------------------> END OF TEST <-----------------------");
#ifdef _MY_CRC8_ENA
    printf("\nCRC8 is matched with test 0x%x: %d", CRC8CHECK, crc8_matched_with_test);
    printf("\nrandom test CRC8 exit with error: %d\n", not_passed8);
#else
    printf("\nCRC8 is disabled\n");
#endif /* _MY_CRC8_ENA */

#ifdef _MY_CRC16_ENA
    printf("\nCRC16 is matched with test 0x%x: %d", CRC16CHECK, crc16_matched_with_test);
    printf("\nrandom test CRC16 exit with error: %d\n", not_passed16);
#else
    printf("\nCRC16 is disabled\n");
#endif /* _MY_CRC16_ENA */

#ifdef _MY_CRC32_ENA
    printf("\nCRC32 is matched with test 0x%x: %d", (unsigned int)CRC32CHECK, crc32_matched_with_test);
    printf("\nrandom test CRC32 exit with error: %d\n", not_passed32);
#else
    printf("\nCRC32 is disabled\n");
#endif /* _MY_CRC32_ENA */

    printf("\n");

    int testPassed = (not_passed8 == 0) && (not_passed16 == 0) && (not_passed32 == 0)
            && (crc8_matched_with_test == 1) && (crc16_matched_with_test == 1) && (crc32_matched_with_test == 1);

    printf("\nCRC TEST PASSED: %d\n", testPassed);

    fflush(stdout);


    (void)randomData;
    (void)data;
    return testPassed;
}

#undef CRC_RANDOM_TEST_NUM




#endif /* _MY_CRC_TEST_DISABLE */

