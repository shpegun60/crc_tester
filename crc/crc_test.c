/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

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

unsigned char data[] = "123456789";

static int crc8_test()
{
    uint8_t crc8[5] = {0x00, 0x00, CRC8INIT, CRC8INIT, CRC8INIT};
    
    printf("\n\n crc 8 test -------------------------------------------\n");
    printf("sizeof buffer: %d\n\n", (int)(sizeof(data) - 1));
    
    crc8[0] = fast_crc8_maxim_array(data, (sizeof(data) - 1));
    printf("crc8 --> fast_crc8_maxim_array: 0x%x", crc8[0]);
    
    crc8[1] = slow_crc8_maxim_array(data, (sizeof(data) - 1));
    printf("\ncrc8 --> slow_crc8_maxim_array: 0x%x", crc8[1]);
    
    for(unsigned long long i = 0; i < (sizeof(data) - 1); ++i) {
        crc8[2] = fast_crc8_maxim_byte(crc8[2], data[i]);
    }
    printf("\ncrc8 --> fast_crc8_maxim_byte: 0x%x", crc8[2]);
    
    for(unsigned long long i = 0; i < (sizeof(data) - 1); ++i) {
        crc8[3] = slow_crc8_maxim_byte(crc8[3], data[i]);
    }
    printf("\ncrc8 --> slow_crc8_maxim_byte: 0x%x", crc8[3]);
    
    for(unsigned long long i = 0; i < (sizeof(data) - 1); ++i) {
        crc8[4] = _proceedCrc_OLD(crc8[4], data[i]);
    }
    printf("\ncrc8 --> _proceedCrc_OLD: 0x%x", crc8[4]);
    
    int counter_not_valid = 0;
    for(int i = 1; i < 5; ++i) {
        if(crc8[0] != crc8[i]) {
            ++counter_not_valid;
        }
    }
    
    printf("\nerror counter %d", counter_not_valid);

    fflush(stdout);
    return counter_not_valid;
}

static int crc16_test()
{
    uint16_t crc16[4] = {0x00, 0x00, CRC16INIT, CRC16INIT};

    printf("\n\n crc16 test -------------------------------------------\n");
    printf("sizeof buffer: %d\n\n", (int)(sizeof(data) - 1));

    crc16[0] = fast_crc16_t10_dif_array(data, (sizeof(data) - 1));
    printf("crc16 --> fast_crc16_t10_dif_array: 0x%x", crc16[0]);

    crc16[1] = slow_crc16_t10_dif_array(data, (sizeof(data) - 1));
    printf("\ncrc16 --> slow_crc16_t10_dif_array: 0x%x", crc16[1]);


    for(unsigned long long i = 0; i < (sizeof(data) - 1); ++i) {
        crc16[2] = fast_crc16_t10_dif_byte(crc16[2], data[i]);
    }
    printf("\ncrc16 --> fast_crc16_t10_dif_byte: 0x%x", crc16[2]);

    for(unsigned long long i = 0; i < (sizeof(data) - 1); ++i) {
        crc16[3] = slow_crc16_t10_dif_byte(crc16[3], data[i]);
    }
    printf("\ncrc16 --> slow_crc16_t10_dif_byte: 0x%x", crc16[3]);

    int counter_not_valid = 0;
    for(int i = 1; i < 4; ++i) {
        if(crc16[0] != crc16[i]) {
            ++counter_not_valid;
        }
    }

    printf("\nerror counter %d\n", counter_not_valid);
    fflush(stdout);
    return counter_not_valid;
}

static int crc32_test()
{
    uint32_t crc32[4] = {0x00, 0x00, CRC32INIT, CRC32INIT};

    printf("\n\n crc32 test -------------------------------------------\n");
    printf("sizeof buffer: %d\n\n", (int)(sizeof(data) - 1));

    crc32[0] = fast_crc32b_array(data, (sizeof(data) - 1));
    printf("crc32 --> fast_crc32b_array: 0x%x", crc32[0]);

    crc32[1] = slow_crc32b_array(data, (sizeof(data) - 1));
    printf("\ncrc32 --> slow_crc32b_array: 0x%x", crc32[1]);

    for(unsigned long long i = 0; i < (sizeof(data) - 1); ++i) {
        crc32[2] = fast_crc32b_byte(crc32[2], data[i]);
    }
    crc32[2] = ~crc32[2];
    printf("\ncrc16 --> fast_crc32b_byte: 0x%x", crc32[2]);

    for(unsigned long long i = 0; i < (sizeof(data) - 1); ++i) {
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

    printf("\nerror counter %d\n", counter_not_valid);
    fflush(stdout);
    return counter_not_valid;
}

void crc_test()
{
    int not_passed;
    not_passed = crc8_test();
    not_passed += crc16_test();
    not_passed += crc32_test();


    printf("\n test exit with error: %d\n", not_passed);
    fflush(stdout);

}


