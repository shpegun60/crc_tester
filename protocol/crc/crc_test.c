/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include "crc_test.h"
#include <inttypes.h>

#ifndef _MY_CRC_TEST_DISABLE

#include <stdlib.h>
#include <stdio.h>

#include "crc8.h"
#include "crc16.h"
#include "crc32.h"
#include "crc64.h"


int crc_test(unsigned int randomSeed, int randomTestCnt)
{
    u8 data[] = "123456789";

    int not_passed8 = 0;
    int not_passed16 = 0;
    int not_passed32 = 0;
    int not_passed64 = 0;

    u8 crc8_matched_with_test = 1;
    u8 crc16_matched_with_test = 1;
    u8 crc32_matched_with_test = 1;
    u8 crc64_matched_with_test = 1;

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
    printf("\nCRC32 is matched with test 0x%x: %d", CRC32CHECK, crc32_matched_with_test);

    if(!crc32_matched_with_test) {
        ++not_passed32;
    }
#else
    printf("\nCRC32 is disabled\n");
#endif /* _MY_CRC32_ENA */



#ifdef _MY_CRC64_ENA
    u64 resCrc64 = CRC64INIT;

    not_passed64 = crc64_test(data, (sizeof (data) - 1), &resCrc64);
    printf("\ncheck test CRC64 exit with error: %d", not_passed64);

    crc64_matched_with_test = (resCrc64 == CRC64CHECK);
    printf("\nCRC64 is matched with test 0x%" PRIx64 ": %d", CRC64CHECK, crc64_matched_with_test);

    if(!crc64_matched_with_test) {
        ++not_passed64;
    }
#else
    printf("\nCRC64 is disabled\n");
#endif /* _MY_CRC64_ENA */

    fflush(stdout);

    if(not_passed8 != 0 || not_passed16 != 0 || not_passed32 != 0 || not_passed64 != 0) {
        printf("\n\nEXIT, SOME CRC is NOT MATCHED WITH TEST VALUE \n");
        printf("CRC TEST PASSED: 0\n");
        fflush(stdout);
        return 1;
    }

    not_passed8 = 0;
    not_passed16 = 0;
    not_passed32 = 0;
    not_passed64 = 0;

    printf("\n\n-------------------------------\nRANDOM TEST");
    printf("\n-------------------------------\n");

    u8 randomData[4096*2];
    srand(randomSeed); // use current time as seed for random generator

#ifdef _MY_CRC8_ENA

    // crc 8 random test ------------------------------------------------
    for(reg i = 0; i < (reg)randomTestCnt; ++i) {

        reg len = 0;

        while(len == 0) {
            len = rand() % 16; // crc 8 maximum 15 bytes
        }

        for(reg j = 0; j < len; ++j) {
            randomData[j] = rand() % 256;
        }

        not_passed8 += crc8_test(randomData, len, &resCrc8);
        fflush(stdout);
    }

#endif /* _MY_CRC8_ENA */


#ifdef _MY_CRC16_ENA
    // crc 16 random test ------------------------------------------------
    for(reg i = 0; i < (reg)randomTestCnt; ++i) {

        reg len = 0;

        while(len == 0) {
            len = rand() % 4096; // crc 16 maximum 4095 bytes
        }

        for(reg j = 0; j < len; ++j) {
            randomData[j] = rand() % 256;
        }

        not_passed16 += crc16_test(randomData, len, &resCrc16);
        fflush(stdout);
    }

#endif /* _MY_CRC16_ENA */


#ifdef _MY_CRC32_ENA

    // crc 32 random test ------------------------------------------------
    for(reg i = 0; i < (reg)randomTestCnt; ++i) {

        reg len = 0;

        while(len == 0) {
            len = rand() % (4096*2); // crc 32 maximum over 4096 bytes
        }

        for(reg j = 0; j < len; ++j) {
            randomData[j] = rand() % 256;
        }

        not_passed32 += crc32_test(randomData, len, &resCrc32);
        fflush(stdout);
    }

#endif /* _MY_CRC32_ENA */

#ifdef _MY_CRC64_ENA

    // crc 64 random test ------------------------------------------------
    for(reg i = 0; i < (reg)randomTestCnt; ++i) {

        reg len = 0;

        while(len == 0) {
            len = rand() % (4096*2); // crc64 maximum over 4096 bytes
        }

        for(reg j = 0; j < len; ++j) {
            randomData[j] = rand() % 256;
        }

        not_passed64 += crc64_test(randomData, len, &resCrc64);
        fflush(stdout);
    }

#endif /* _MY_CRC32_ENA */

    printf("\n-----------------------> END OF CRC TEST <-----------------------");
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

#ifdef _MY_CRC64_ENA
    printf("\nCRC64 is matched with test 0x%" PRIx64 ": %d", CRC64CHECK, crc64_matched_with_test);
    printf("\nrandom test CRC64 exit with error: %d\n", not_passed64);
#else
    printf("\nCRC64 is disabled\n");
#endif /* _MY_CRC32_ENA */

    printf("\n");

    int testPassed = (not_passed8 == 0) && (not_passed16 == 0) && (not_passed32 == 0) && (not_passed64 == 0)
            && (crc8_matched_with_test == 1) && (crc16_matched_with_test == 1) && (crc32_matched_with_test == 1) && (crc64_matched_with_test == 1);

    printf("\nCRC TEST PASSED: %d\n", testPassed);

    fflush(stdout);


    (void)randomData;
    (void)data;
    return !testPassed;
}




#endif /* _MY_CRC_TEST_DISABLE */

