/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/


#include <time.h>
#include <stdlib.h>

#include "crc_test.h"
#include <stdio.h>
#include "crc32.h"
#include "crc16.h"
#include "crc8.h"


static uint8_t _proceedCrc_OLD(uint8_t crc, uint8_t ch) { // original crc from old protocol
    crc ^= ch;
    for (int i = 0; i < 8; i++)
        crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
    return crc;
}

static int crc8_test(unsigned char *data, size_t len, uint8_t * res)
{
    uint8_t crc8[5] = {0x00, 0x00, CRC8INIT, CRC8INIT, CRC8INIT};
    
    printf("\n\n crc 8 test -------------------------------------------\n");
    printf("sizeof buffer: %d\n\n", (int)len);
    
    crc8[0] = fast_crc8_maxim_array(data, len);
    printf("crc8 --> fast_crc8_maxim_array: 0x%x", crc8[0]);
    
    crc8[1] = slow_crc8_maxim_array(data, len);
    printf("\ncrc8 --> slow_crc8_maxim_array: 0x%x", crc8[1]);
    
    for(size_t i = 0; i < len; ++i) {
        crc8[2] = fast_crc8_maxim_byte(crc8[2], data[i]);
    }
    printf("\ncrc8 --> fast_crc8_maxim_byte: 0x%x", crc8[2]);
    
    for(size_t i = 0; i < len; ++i) {
        crc8[3] = slow_crc8_maxim_byte(crc8[3], data[i]);
    }
    printf("\ncrc8 --> slow_crc8_maxim_byte: 0x%x", crc8[3]);
    
    for(size_t i = 0; i < len; ++i) {
        crc8[4] = _proceedCrc_OLD(crc8[4], data[i]);
    }
    printf("\ncrc8 --> _proceedCrc_OLD: 0x%x", crc8[4]);
    
    int counter_not_valid = 0;
    for(int i = 1; i < 5; ++i) {
        if(crc8[0] != crc8[i]) {
            ++counter_not_valid;
        }
    }

    *res = crc8[0];
    
    printf("\nerror counter %d", counter_not_valid);

    fflush(stdout);
    return counter_not_valid;
}

static int crc16_test(unsigned char *data, size_t len, uint16_t * res)
{
    uint16_t crc16[4] = {0x00, 0x00, CRC16INIT, CRC16INIT};

    printf("\n\n crc16 test -------------------------------------------\n");
    printf("sizeof buffer: %d\n\n", (int)len);

    crc16[0] = fast_crc16_t10_dif_array(data, len);
    printf("crc16 --> fast_crc16_t10_dif_array: 0x%x", crc16[0]);

    crc16[1] = slow_crc16_t10_dif_array(data, len);
    printf("\ncrc16 --> slow_crc16_t10_dif_array: 0x%x", crc16[1]);


    for(size_t i = 0; i < len; ++i) {
        crc16[2] = fast_crc16_t10_dif_byte(crc16[2], data[i]);
    }
    printf("\ncrc16 --> fast_crc16_t10_dif_byte: 0x%x", crc16[2]);

    for(size_t i = 0; i < len; ++i) {
        crc16[3] = slow_crc16_t10_dif_byte(crc16[3], data[i]);
    }
    printf("\ncrc16 --> slow_crc16_t10_dif_byte: 0x%x", crc16[3]);

    int counter_not_valid = 0;
    for(int i = 1; i < 4; ++i) {
        if(crc16[0] != crc16[i]) {
            ++counter_not_valid;
        }
    }

    *res = crc16[0];

    printf("\nerror counter %d\n", counter_not_valid);
    fflush(stdout);
    return counter_not_valid;
}

static int crc32_test(unsigned char *data, size_t len, uint32_t *res)
{
    uint32_t crc32[4] = {0x00, 0x00, CRC32INIT, CRC32INIT};

    printf("\n\n crc32 test -------------------------------------------\n");
    printf("sizeof buffer: %d\n\n", (int)len);

    crc32[0] = fast_crc32b_array(data, len);
    printf("crc32 --> fast_crc32b_array: 0x%x", crc32[0]);

    crc32[1] = slow_crc32b_array(data, len);
    printf("\ncrc32 --> slow_crc32b_array: 0x%x", crc32[1]);

    for(unsigned long long i = 0; i < len; ++i) {
        crc32[2] = fast_crc32b_byte(crc32[2], data[i]);
    }
    crc32[2] = ~crc32[2];
    printf("\ncrc16 --> fast_crc32b_byte: 0x%x", crc32[2]);

    for(unsigned long long i = 0; i < len; ++i) {
        crc32[3] = slow_crc32b_byte(crc32[3], data[i]);
    }
    crc32[3] = ~crc32[3];
    printf("\ncrc16 --> slow_crc32b_byte: 0x%x", crc32[3]);

    int counter_not_valid = 0;
    for(int i = 1; i < 4; ++i) {
        if(crc32[0] != crc32[i]) {
            ++counter_not_valid;
        }
    }

    *res = crc32[0];

    printf("\nerror counter %d\n", counter_not_valid);
    fflush(stdout);
    return counter_not_valid;
}

void crc_test()
{
    unsigned char data[] = "123456789";

    int not_passed8 = 0;
    int not_passed16 = 0;
    int not_passed32 = 0;

    uint8_t resCrc8 = CRC8INIT;
    uint16_t resCrc16 = CRC16INIT;
    uint32_t resCrc32 = CRC32INIT;

    not_passed8 = crc8_test(data, (sizeof (data) - 1), &resCrc8);
    not_passed16 = crc16_test(data, (sizeof (data) - 1), &resCrc16);
    not_passed32 = crc32_test(data, (sizeof (data) - 1), &resCrc32);
    printf("\ncheck test CRC8 exit with error: %d\n", not_passed8);
    printf("\ncheck test CRC16 exit with error: %d\n", not_passed16);
    printf("\ncheck test CRC32 exit with error: %d\n", not_passed32);

    if(not_passed8 != 0 || not_passed16 != 0 || not_passed32 != 0 || resCrc8 != CRC8CHECK || resCrc16 != CRC16CHECK || resCrc32 != CRC32CHECK) {
        printf("EXIT NOT PASSED CRC CHECK VALUE \n");
        fflush(stdout);
        return;
    }

    not_passed8 = 0;
    not_passed16 = 0;
    not_passed32 = 0;


#define CRC_TEST_NUM 1000
    unsigned char randomData[4096*2];
    srand(time(NULL)); // use current time as seed for random generator

    // crc 8 test ------------------------------------------------
    for(size_t i = 0; i < CRC_TEST_NUM; ++i) {

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

    // crc 16 test ------------------------------------------------
    for(size_t i = 0; i < CRC_TEST_NUM; ++i) {

        size_t len = 0;

        while(len == 0) {
            len = rand() % 4096*2; // crc 16 maximum 4095 bytes
        }

        for(size_t j = 0; j < len; ++j) {
            randomData[j] = rand() % 256;
        }

        not_passed16 += crc16_test(randomData, len, &resCrc16);
        fflush(stdout);
    }

    // crc 32 test ------------------------------------------------
    for(size_t i = 0; i < CRC_TEST_NUM; ++i) {

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

#undef CRC_TEST_NUM

    printf("\nrandom test CRC8 exit with error: %d\n", not_passed8);
    printf("\nrandom test CRC16 exit with error: %d\n", not_passed16);
    printf("\nrandom test CRC32 exit with error: %d\n", not_passed32);

    fflush(stdout);
}


