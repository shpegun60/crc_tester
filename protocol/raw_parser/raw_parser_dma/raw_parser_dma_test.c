#include "raw_parser_dma_test.h"


#ifndef D_RAW_P_TEST_DISABLE

#include "rawparser_dma.h"
#include "rawparser_macro.h"
#include "byte_order.h"
#include "convert.h"

#include <stdio.h>
#include <stdlib.h>


#ifdef D_RAW_P_DISABLE_INTERNAL_RX_BUFFER
static u8           m_receiveFrameBuffer_external[D_RAW_P_RX_BUF_SIZE];
#endif /* D_RAW_P_DISABLE_INTERNAL_RX_BUFFER */

#ifdef D_RAW_P_DISABLE_INTERNAL_TX_BUFFER
static u8           m_sendBuffer_external[D_RAW_P_TX_BUF_SIZE];    // array for save tx buffer
#endif /* D_RAW_P_DISABLE_INTERNAL_TX_BUFFER */


/**********************************************************************************************
 *
 * SIMPLE RANDOM TRANSMITT/RECEIVE TEST
 *
 */

static int receiveTransmittSimpleDmaTest(RawParser_dma_t* desc, u8 * data, reg size)
{
    int byteReceiveCompl = 0;
    int arrReceiveCompl = 0;

    RawParser_Frame_t* Txframe = RawParser_dma_shieldFrame(desc, data, size);

    if(Txframe == NULL) {
        return 1;
    }

    for(reg i = 0; i < Txframe->size; ++i) {
        RawParser_dma_receiveByte(desc, Txframe->data[i]);
    }


    // ------------- one byte pushing test---------------------------------------
    RawParser_Frame_t* Rxframe = RawParser_dma_proceed(desc);

    if(Rxframe && Rxframe->size != 0) {
        if(Rxframe->size != size) {
            byteReceiveCompl++;
        } else {
            byteReceiveCompl += cTypeStrnCmp(size, (c8*)data, (c8*)Rxframe->data);
        }
    } else {
        byteReceiveCompl++;
    }

    Txframe->size = 0;



    // ------------- all byte pushing test---------------------------------------
    Txframe = RawParser_dma_shieldFrame(desc, data, size);

    if(Txframe == NULL) {
        return byteReceiveCompl + 1;
    }

    RawParser_dma_receiveArray(desc, Txframe->data, Txframe->size);

    Rxframe = RawParser_dma_proceed(desc);
    if(Rxframe && Rxframe->size != 0) {
        if(Rxframe->size != size) {
            byteReceiveCompl++;
        } else {
            arrReceiveCompl += cTypeStrnCmp(size, (c8*)data, (c8*)Rxframe->data);
        }
    } else {
        arrReceiveCompl++;
    }

    reg last = 0;
    while (last < size) {
        RawParser_dma_proceed(desc);
        ++last;
    }

    return byteReceiveCompl + arrReceiveCompl;
}

/**********************************************************************************************
 *
 * SIMPLE COLLISION TEST (MAY BE DISABLE SMART ASSERSTS ???)
 *
 */

static int receiveTransmittCollisionsDmaTest(RawParser_dma_t* desc, u8 * data, reg size)
{
    int byteCollisisons = 0;
    int arrCollisisons = 0;

    RawParser_Frame_t* Txframe = RawParser_dma_shieldFrame(desc, data, size);

    if(Txframe == NULL) {
        return 1;
    }

    reg lenBreak = rand() % size;

    // replace data (emulate real glitches)
    c8 b_rand = rand() % 256;
    while(Txframe->data[lenBreak] == b_rand) {
        b_rand = rand() % 256;
    }
    Txframe->data[lenBreak] = b_rand;


    for(reg i = 0; i < Txframe->size; ++i) {
        RawParser_dma_receiveByte(desc, Txframe->data[i]);
    }


    // ------------- one byte pushing test---------------------------------------
    RawParser_Frame_t* Rxframe = RawParser_dma_proceed(desc);

    if(Rxframe && Rxframe->size != 0) {
        byteCollisisons++;
    }

    Txframe->size = 0;

    // ------------- all byte pushing test---------------------------------------
    Txframe = RawParser_dma_shieldFrame(desc, data, size);

    if(Txframe == NULL) {
        return 1;
    }

    lenBreak = rand() % size;

    b_rand = rand() % 256;
    while(Txframe->data[lenBreak] == b_rand) {
        b_rand = rand() % 256;
    }
    Txframe->data[lenBreak] = b_rand;


    RawParser_dma_receiveArray(desc, Txframe->data, Txframe->size);

    Rxframe = RawParser_dma_proceed(desc);
    if(Rxframe && Rxframe->size != 0) {
        arrCollisisons++;
    }

    reg last = 0;
    while (last < size) {
        RawParser_dma_proceed(desc);
        ++last;
    }

    return byteCollisisons + arrCollisisons;
}


/**********************************************************************************************
 *
 * MACRO WRITE / READ TEST
 *
 */

static int receiveTransmittMacroWriteReadDmaTest(RawParser_dma_t* desc, int randTestCount)
{
#define RAW_P_DMA_TEST_ARR_SIZE 10
    int byteReceiveCompl = 0;

    printf("\n\n ---------------------------RAW PARSER DMA MACRO (FAST) ------------------------\n");

    while(randTestCount--) {


        // randomization --------------------------------------------------------------------------------------
        i32 a = rand();
        reg b = rand();
        u16 c = rand();
        u8  d = rand();
        i32 inner = rand();
        u32 arr1[RAW_P_DMA_TEST_ARR_SIZE];

        u8 arr2[RAW_P_DMA_TEST_ARR_SIZE];
        u8 * arr3_ptr = arr2;

        for(int i = 0; i < RAW_P_DMA_TEST_ARR_SIZE; ++i) {
            arr1[i] = rand();
            arr2[i] = rand();
        }



        // transmitting packet -------------------------------------------------------------------------------
        RawParser_Frame_t* Txframe = NULL;
        reg totSize;

        WRITE_PAYLOAD_MACRO(RawParser_dma_universalWrite, desc, {
#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
                                rs_encode_data_continious_start(&desc->rs_ecc);
                                RawParser_dma_startTransmittPacket(desc, totalSize + RSCODE_NPAR);
#else
                                RawParser_dma_startTransmittPacket(desc, totalSize);
#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */

                                totSize = totalSize;
                            }, {
#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
                                for (int i = 0; i < RSCODE_NPAR; ++i) {
                                    RawParser_dma_addTxByteCRC(desc, desc->rs_ecc.pBytes[RSCODE_NPAR-1-i]);
                                }
#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */
                                Txframe = RawParser_dma_finishTransmittPacket(desc);
                            }, a, b, c, d, $STATIC_ARRAY, arr1, $CONST, inner, i32, $POINTER, 10, arr3_ptr);

        if(Txframe == NULL) {
            return 1;
        }

        for(reg i = 0; i < Txframe->size; ++i) {
            RawParser_dma_receiveByte(desc, Txframe->data[i]);
        }

        // receiving packet -------------------------------------------------------------------------------
        i32 CHK_a = 0;
        reg CHK_b = 0;
        u16 CHK_c = 0;
        u8  CHK_d = 0;
        i32 CHK_inner = 0;
        u32 CHK_arr1[RAW_P_DMA_TEST_ARR_SIZE];

        u8 CHK_arr2[RAW_P_DMA_TEST_ARR_SIZE];
        u8 * CHK_arr3_ptr = CHK_arr2;

        RawParser_Frame_t* Rxframe = RawParser_dma_proceed(desc);

        if(Rxframe && Rxframe->size != 0) {
            if(totSize != Rxframe->size) {
                byteReceiveCompl++;
            } else {
                READ_PAYLOAD_MACRO(RawParser_dma_universalRead, desc, {
                                       desc->uniRXPosition = 0;
                                   }, {
                                       CHK_inner = const_5;
                                   }, CHK_a, CHK_b, CHK_c, CHK_d, $STATIC_ARRAY, CHK_arr1, $CONST, inner, i32, $POINTER, RAW_P_DMA_TEST_ARR_SIZE, CHK_arr3_ptr);

                if(CHK_a != a || CHK_b != b || CHK_c != c || CHK_d != d || CHK_inner != inner) {
                    byteReceiveCompl++;
                }

                byteReceiveCompl += cTypeStrnCmp(RAW_P_DMA_TEST_ARR_SIZE, (c8*)CHK_arr1, (c8*)arr1);
                byteReceiveCompl += cTypeStrnCmp(RAW_P_DMA_TEST_ARR_SIZE, (c8*)CHK_arr3_ptr, (c8*)arr3_ptr);
            }


        } else {
            byteReceiveCompl++;
        }
    }

#undef RAW_P_DMA_TEST_ARR_SIZE

    printf("\ntest exit with error: %d\n---------------------------RAW PARSER DMA MACRO (FAST) END ------------------------\n", byteReceiveCompl);
    return byteReceiveCompl;
}

/**********************************************************************************************
 *
 * CONVERT WRITE / READ TEST
 *
 */

static int receiveTransmittConvertDmaTest(RawParser_dma_t* desc, int randTestCount)
{
#define RAW_P_DMA_TEST_ARR_SIZE 10
    int byteReceiveCompl = 0;

    printf("\n\n ---------------------------RAW PARSER DMA CONVERT (SLOW) ------------------------\n");

    while(randTestCount--) {
        // randomization --------------------------------------------------------------------------------------
        i32 a = rand();
        reg b = rand();
        u16 c = rand();
        u8  d = rand();
        i32 inner = rand();
        u32 arr1[RAW_P_DMA_TEST_ARR_SIZE];

        for(int i = 0; i < RAW_P_DMA_TEST_ARR_SIZE; ++i) {
            arr1[i] = rand();
        }


        // transmitting packet -------------------------------------------------------------------------------
        u8 buffdata [D_RAW_P_TX_BUF_SIZE];
        reg pos = 0;
        TEMPLATE(CAT_ENDIAN(convertWrite), i32)(buffdata, &pos, a);
        TEMPLATE(CAT_ENDIAN(convertWrite), reg)(buffdata, &pos, b);
        TEMPLATE(CAT_ENDIAN(convertWrite), u16)(buffdata, &pos, c);
        TEMPLATE(CAT_ENDIAN(convertWrite), u8 )(buffdata, &pos, d);
        TEMPLATE(CAT_ENDIAN(convertWrite), i32)(buffdata, &pos, inner);

        for(int i = 0; i < RAW_P_DMA_TEST_ARR_SIZE; ++i) {
            TEMPLATE(CAT_ENDIAN(convertWrite), u32)(buffdata, &pos, arr1[i]);
        }

        RawParser_Frame_t* Txframe = RawParser_dma_shieldFrame(desc, buffdata, pos);

        if(Txframe == NULL) {
            return 1;
        }

        for(reg i = 0; i < Txframe->size; ++i) {
            RawParser_dma_receiveByte(desc, Txframe->data[i]);
        }

        // receiving packet -------------------------------------------------------------------------------
        RawParser_Frame_t* Rxframe = RawParser_dma_proceed(desc);

        if(Rxframe && Rxframe->size != 0) {

            if(pos != Rxframe->size) {
                byteReceiveCompl++;
            } else {

                pos = 0;

                i32 CHK_a = TEMPLATE(CAT_ENDIAN(convertRead), i32)(Rxframe->data, &pos);
                reg CHK_b = TEMPLATE(CAT_ENDIAN(convertRead), reg)(Rxframe->data, &pos);
                u16 CHK_c = TEMPLATE(CAT_ENDIAN(convertRead), u16)(Rxframe->data, &pos);
                u8  CHK_d = TEMPLATE(CAT_ENDIAN(convertRead), u8 )(Rxframe->data, &pos);
                i32 CHK_inner = TEMPLATE(CAT_ENDIAN(convertRead), i32)(Rxframe->data, &pos);
                u32 CHK_arr1[RAW_P_DMA_TEST_ARR_SIZE];

                for(int i = 0; i < RAW_P_DMA_TEST_ARR_SIZE; ++i) {
                    CHK_arr1[i] = TEMPLATE(CAT_ENDIAN(convertRead), u32)(Rxframe->data, &pos);
                }

                if(CHK_a != a || CHK_b != b || CHK_c != c || CHK_d != d || CHK_inner != inner) {
                    byteReceiveCompl++;
                }

                byteReceiveCompl += cTypeStrnCmp(RAW_P_DMA_TEST_ARR_SIZE, (c8*)CHK_arr1, (c8*)arr1);
            }
        } else {
            byteReceiveCompl++;
        }
    }

#undef RAW_P_DMA_TEST_ARR_SIZE

    printf("\ntest exit with error: %d\n---------------------------RAW PARSER DMA CONVERT (SLOW) END ------------------------\n", byteReceiveCompl);
    return byteReceiveCompl;
}

int rawParserDmaTest(unsigned int randomSeed, int randTestCount, int collisionTestEna)
{
    int errorCounter = 0;
    int collisionCounter = 0;
    u8 * data = (u8*)calloc(D_RAW_P_TX_BUF_SIZE, sizeof(u8));

    for(u32 i = 0; i < D_RAW_P_TX_BUF_SIZE; ++i) {
        data[i] = i;
    }

    RawParser_dma_t * prot = rawParser_dma_new(0x1A);

    if(prot == NULL) {
        return 1;
    }


#if defined(D_RAW_P_DISABLE_INTERNAL_TX_BUFFER) && defined(D_RAW_P_DISABLE_INTERNAL_RX_BUFFER)
 // if disabled internal rx & tx buffers than set external
    rawParser_dma_setUserBuffers(prot, m_receiveFrameBuffer_external, m_sendBuffer_external);
#elif defined(D_RAW_P_DISABLE_INTERNAL_RX_BUFFER)
    // if disabled internal rx buffer than set external
    rawParser_dma_setUserBufferRX(prot, m_receiveFrameBuffer_external);

#elif defined(D_RAW_P_DISABLE_INTERNAL_TX_BUFFER)
    // if disabled internal tx buffer than set external
    rawParser_dma_setUserBufferTX(prot, m_sendBuffer_external);

#endif /* D_RAW_P_DISABLE_INTERNAL_TX_BUFFER */


    errorCounter += receiveTransmittSimpleDmaTest(prot, data, 30);

    srand(randomSeed); // use current time as seed for random generator

    // simple random rx/tx test----------------------------------------------------------
    for(int i = 0; i < randTestCount; ++i) {
        reg len = 0;

        while(len == 0) {
#if D_RAW_P_MAX_PROTOCOL_LEN < D_RAW_P_TX_BUF_SIZE
            len = rand() % (D_RAW_P_MAX_PROTOCOL_LEN >> 1);
#else
            len = rand() % (D_RAW_P_TX_BUF_SIZE >> 1);
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */
        }


        for(reg j = 0; j < len; ++j) {
            data[j] = rand() % 256;
        }
        errorCounter += receiveTransmittSimpleDmaTest(prot, data, len);

        if(collisionTestEna) {
            collisionCounter+= receiveTransmittCollisionsDmaTest(prot, data, len);
        }
    }

    errorCounter += receiveTransmittMacroWriteReadDmaTest(prot, randTestCount);
    errorCounter += receiveTransmittConvertDmaTest(prot, randTestCount);

    printf("\n----------RAW PARSER DMA TEST FINISHED!!!-------------------------\nRAW_PARSER_DMA EXIT WITH ERROR: %d\n", errorCounter);
    printf("PACKET COUNTER: %d\n", randTestCount * 2);
    if(collisionTestEna) {
        printf("WITH COLLISIONS: %d\n", collisionCounter);
        printf("PACKED CRC LOOSE percents: %f\n", (f32)((f32)collisionCounter / (f32)randTestCount) * 100.0f * 0.5f);
    }

    free(data);
    printf("RawParser_dma deleted error: %d\n", rawParser_dma_delete(&prot) != D_RAW_P_OK);
    fflush(stdout);

    return errorCounter;
}

#endif /* D_RAW_P_TEST_DISABLE */
