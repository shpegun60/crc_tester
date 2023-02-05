#include "raw_parser_it_test.h"
#include <stdio.h>

#ifndef D_RAW_P_TEST_DISABLE

#include "raw_parser_it.h"
#include <stdlib.h>

#ifdef D_RAW_P_DISABLE_INTERNAL_RX_BUFFER
static u8           m_receiveFrameBuffer_external_it[D_RAW_P_RX_BUF_SIZE];
#endif /* D_RAW_P_DISABLE_INTERNAL_RX_BUFFER */

#ifdef D_RAW_P_DISABLE_INTERNAL_TX_BUFFER
static u8           m_sendBuffer_external_it[D_RAW_P_TX_BUF_SIZE];    // array for save tx buffer
#endif /* D_RAW_P_DISABLE_INTERNAL_TX_BUFFER */

#define D_RAW_P_IT_TEST_TRYING 2

static int receiveTransmittSimpleItTest(RawParser_it_t* desc, u8 * data, reg size)
{
    if(desc == NULL || data == NULL) {
        return 1;
    }

    int conterNotvalid = 0;
    int it = 0;
    //--------------------------------------------------------------------------------------------------
    RawParser_it_RXproceedLoop(desc); // flush last data

    while(it < D_RAW_P_IT_TEST_TRYING) {
        for(reg i = 0; i < size; ++i) {
            data[i] += it;                  // shake data
            desc->TX.data[i] = data[i];
        }

        RawParser_it_TXpush(desc, size);

        if(desc->TX.size == 0) {
            ++conterNotvalid;
            desc->RX.size = 0;
            desc->TX.size = 0;
            return conterNotvalid;
        }


        RawParser_Frame_t* RX = NULL;
        u8 ch;
        while(desc->TX.size) {
            if(RawParser_it_TXproceedIt(desc, &ch)) {
                RawParser_it_RXproceedIt(desc, ch);
            }

            RX = RawParser_it_RXproceedLoop(desc);

             if(RX == NULL) {
                 ++conterNotvalid;
                 desc->RX.size = 0;
                 desc->TX.size = 0;
                 return conterNotvalid;
             } else if(RX->size) {
                if(RX->size != size) {
                    ++conterNotvalid;
                    desc->RX.size = 0;
                    desc->TX.size = 0;
                    return conterNotvalid;
                }
            }
        }


        if(RX != NULL && RX->size != 0) {
            conterNotvalid += cTypeStrnCmp(size, (c8*)data, (c8*)RX->data);
        } else {
            ++conterNotvalid;
        }
        desc->RX.size = 0;
        ++it;
    }
    //--------------------------------------------------------------------------------------------------
    return conterNotvalid;
}

static int receiveTransmittCollisionItTest(RawParser_it_t* desc, u8 * data, reg size)
{
    if(desc == NULL || data == NULL) {
        return 1;
    }

    int conterNotvalid = 0;
    //-----------------------------------------------------------------------------------------------------------------------------
    RawParser_it_RXproceedLoop(desc);
    for(reg i = 0; i < size; ++i) {
        desc->TX.data[i] = data[i];
    }
    RawParser_it_TXpush(desc, size);

    if(desc->TX.size == 0) {
        ++conterNotvalid;
        desc->RX.size = 0;
        desc->TX.size = 0;
        return conterNotvalid;
    }

    // get pos to crash byte
    reg pos = 0;
    while(pos == 0) {
         pos = rand() % desc->TX.size;
    }

    u8 randdata = desc->TX.data[pos];
    while(randdata == desc->TX.data[pos]) {
        randdata = rand() % 256;
    }
    desc->TX.data[pos] = randdata;


    RawParser_Frame_t* RX = NULL;
    u8 ch;
    while(desc->TX.size) {
        if(RawParser_it_TXproceedIt(desc, &ch)) {
            RawParser_it_RXproceedIt(desc, ch);
        }

        RX = RawParser_it_RXproceedLoop(desc);

         if(RX == NULL) {
             ++conterNotvalid;
             desc->RX.size = 0;
             desc->TX.size = 0;
             return conterNotvalid;
         } else if(RX->size) {
            printf("RAW PARSER IT: collision\n");
            conterNotvalid += cTypeStrnCmp(size, (c8*)data, (c8*)RX->data);
            desc->RX.size = 0;
            desc->TX.size = 0;
            return conterNotvalid;
        }
    }

    desc->RX.size = 0;
    desc->TX.size = 0;

    //-----------------------------------------------------------------------------------------------------------------------------
    return conterNotvalid;
}



int rawParserItTest(unsigned int randomSeed, int randTestCount, int collisionTestEna)
{
    int conterNotvalid = 0;
    int collisionsCounter = 0;
    srand(randomSeed); // use current time as seed for random generator

    //--------------------------------------------------------------------------------------------------
    RawParser_it_t * prot = rawParser_it_new(0x1A);

    if(prot == NULL) {
        return 1;
    }

#if defined(D_RAW_P_DISABLE_INTERNAL_TX_BUFFER) && defined(D_RAW_P_DISABLE_INTERNAL_RX_BUFFER)
 // if disabled internal rx & tx buffers than set external
    rawParser_it_setUserBuffers(prot, m_receiveFrameBuffer_external_it, m_sendBuffer_external_it);
#elif defined(D_RAW_P_DISABLE_INTERNAL_RX_BUFFER)
    // if disabled internal rx buffer than set external
    rawParser_it_setUserBufferRX(prot, m_receiveFrameBuffer_external_it);

#elif defined(D_RAW_P_DISABLE_INTERNAL_TX_BUFFER)
    // if disabled internal tx buffer than set external
    rawParser_it_setUserBufferTX(prot, m_sendBuffer_external_it);

#endif /* D_RAW_P_DISABLE_INTERNAL_TX_BUFFER */

    u8 * data = (u8*)calloc(D_RAW_P_TX_BUF_SIZE, sizeof(u8));
    for(u32 i = 0; i < D_RAW_P_TX_BUF_SIZE; ++i) {
        data[i] = i;
    }

    conterNotvalid += receiveTransmittSimpleItTest(prot, data, 30);

    while(randTestCount--) {
        reg len = 0;

        while(len == 0) {
#if D_RAW_P_MAX_PROTOCOL_LEN < D_RAW_P_TX_BUF_SIZE

    #if defined(D_RAW_P_CRC_ENA) && defined(D_RAW_P_REED_SOLOMON_ECC_CORR_ENA)
            len = rand() % (D_RAW_P_MAX_PROTOCOL_LEN - RSCODE_NPAR - sizeof(rawP_crc_t));
    #elif defined(D_RAW_P_CRC_ENA)
            len = rand() % (D_RAW_P_MAX_PROTOCOL_LEN - sizeof(rawP_crc_t));
    #elif defined(D_RAW_P_REED_SOLOMON_ECC_CORR_ENA)
            len = rand() % (D_RAW_P_MAX_PROTOCOL_LEN - RSCODE_NPAR);
    #else
            len = rand() % D_RAW_P_MAX_PROTOCOL_LEN;
    #endif /* defined(D_RAW_P_CRC_ENA) && defined(D_RAW_P_REED_SOLOMON_ECC_CORR_ENA) */

#else

    #if defined(D_RAW_P_CRC_ENA) && defined(D_RAW_P_REED_SOLOMON_ECC_CORR_ENA)
            len = rand() % (D_RAW_P_TX_BUF_SIZE - RSCODE_NPAR - sizeof(rawP_crc_t));
    #elif defined(D_RAW_P_CRC_ENA)
            len = rand() % (D_RAW_P_TX_BUF_SIZE - sizeof(rawP_crc_t));
    #elif defined(D_RAW_P_REED_SOLOMON_ECC_CORR_ENA)
            len = rand() % (D_RAW_P_TX_BUF_SIZE - RSCODE_NPAR);
    #else
            len = rand() % D_RAW_P_TX_BUF_SIZE;
    #endif /* defined(D_RAW_P_CRC_ENA) && defined(D_RAW_P_REED_SOLOMON_ECC_CORR_ENA) */

#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */
        }

        for(reg j = 0; j < len; ++j) {
            data[j] = rand() % 256;
        }

        conterNotvalid += receiveTransmittSimpleItTest(prot, data, len);
        if(collisionTestEna) {
            collisionsCounter += receiveTransmittCollisionItTest(prot, data, len);
        }
    }

    printf("\n------------------->RAW PARSER IT RESULT<--------------------------------------------------------------\n");
    printf("RAW PARSER IT: collisions: %d, %s\n", collisionsCounter, collisionTestEna ? "ENABLE" : "DISABLE");
    printf("RAW PARSER IT: errors: %d\n", conterNotvalid);
    printf("---------------------------------------------------------------------------------\n");
    fflush(stdout);
    free(data);
    conterNotvalid += (rawParser_it_delete(&prot) != D_RAW_P_OK);
    //--------------------------------------------------------------------------------------------------
    return conterNotvalid;
}

#undef D_RAW_P_IT_TEST_TRYING

#endif /* D_RAW_P_TEST_DISABLE */
