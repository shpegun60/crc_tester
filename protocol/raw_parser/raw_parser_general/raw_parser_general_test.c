#include "raw_parser_general_test.h"
#include "raw_parser_general.h"
#include <stdio.h>
#include <string.h>

#ifndef D_RAW_P_TEST_DISABLE

static void recv_message_test_general(u8* data, reg size, PREPROCESSOR_CTX_TYPE(ctx));
static u8 write_byte_test_general(const u8 byte, PREPROCESSOR_CTX_TYPE(ctx));

static void raw_p_gen_send(RawParser_gen_t* const proto, u8 *to_send, reg size, PREPROCESSOR_CTX_TYPE(ctx))
{
    PREPROCESSOR_CTX_GET(ctx,
                         reg* const loopback_buf_index,
                         const u8 * const loopback_buf,
                         const u8 * const tmpBuff,
                         const reg* const len,
                         const int* const testNotPassed);


    (*loopback_buf_index) = 0;
    //printf("GENERAL SEND: %s\n", to_send);
    RawParser_gen_send_message(proto, to_send, size, ctx);

    for(u16 i = 0; i < (*loopback_buf_index); ++i) {
        const u8 rx = loopback_buf[i];
        RawParser_gen_read_byte(proto, rx, ctx);
    }

    UNUSED(tmpBuff);
    UNUSED(len);
    UNUSED(testNotPassed);
}

int rawParserGenTest(unsigned int randomSeed, int randTestCount)
{
    int testNotPassed = 0;

    //------------------------------------------------------------------------------------------------------
    reg loopback_buf_index = 0;
    u8  loopback_buf[D_RAW_P_TX_BUF_SIZE << 1] = {};
    u8  buf[D_RAW_P_TX_BUF_SIZE] = {};
    u8 tmpBuff[D_RAW_P_TX_BUF_SIZE] = {};

    const RawParser_gen_descr_t general_descriptor = {
        .m_startByte = 0x1A,
        .RX_buf = buf,
        .RX_buf_size = D_RAW_P_TX_BUF_SIZE,
        .recv_message = recv_message_test_general,
        .write_byte = write_byte_test_general,
    };
    RawParser_gen_t* proto =  RawParser_gen_new(&general_descriptor);

    if(proto == NULL) {
        return 1;
    }

    srand(randomSeed); // use current time as seed for random generator


    int testCnt = 0;
    reg lenLast = 0;

    reg size_modulus = D_RAW_P_TX_BUF_SIZE;

#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA // calc reed-solomon parity
    size_modulus -= (RSCODE_NPAR + 1);
#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */

    while(testCnt != randTestCount) {

        // check length----------------------------
        reg len = rand() % size_modulus;
        while((len == lenLast) || len == 0) {
            len = rand() % size_modulus;
        }
        lenLast = len;
        //------------------------------------------

        for(reg i = 0; i < len; ++i) {
            tmpBuff[i] = rand() & 0xFFU;
        }


        PREPROCESSOR_CTX_TYPE(ctx) = PREPROCESSOR_CTX_CAPTURE({&loopback_buf_index,
                                                               loopback_buf,
                                                               tmpBuff,
                                                               &len,
                                                               &testNotPassed});
        raw_p_gen_send(proto, tmpBuff, len, ctx);

        ++testCnt;
    }


    if(RawParser_gen_delete(&proto) != D_RAW_P_OK) {
        ++testNotPassed;
    }

    if(proto != NULL) {
        ++testNotPassed;
    }
    //------------------------------------------------------------------------------------------------------

    return testNotPassed;
}

static void recv_message_test_general(u8* data, reg size, PREPROCESSOR_CTX_TYPE(ctx))
{
    PREPROCESSOR_CTX_GET(ctx,
                         reg* const loopback_buf_index,
                         const u8 * const loopback_buf,
                         const u8 * const tmpBuff,
                         const reg* const len,
                         int* const testNotPassed);

    int error = 0;
    if((*len) != size) {
        ++error;
    }

    error += cTypeStrnCmp(size, (c8*)data, (c8*)tmpBuff);

    printf("len1 = %d, len2 = %d, GENERAL RECV: 0x", size, (*len));
    for(reg i = 0; i < size; ++i) {
        printf("%x.", data[i]);
    }
    printf("\n");
    printf("data error: %d\n", error);
    printf("\n");

    (*testNotPassed) = error;

    UNUSED(loopback_buf_index);
    UNUSED(loopback_buf);
}

static u8 write_byte_test_general(const u8 byte, PREPROCESSOR_CTX_TYPE(ctx))
{
    PREPROCESSOR_CTX_GET(ctx,
                         reg* const loopback_buf_index,
                         u8 * const loopback_buf,
                         const u8 * const tmpBuff,
                         const reg* const len,
                         const int* const testNotPassed);

    loopback_buf[(*loopback_buf_index)++] = byte;
    return D_RAW_P_OK;

    UNUSED(tmpBuff);
    UNUSED(len);
    UNUSED(testNotPassed);
}

#endif /* D_RAW_P_TEST_DISABLE */

