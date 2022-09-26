#include "raw_parser_it_test.h"

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

static int receiveTransmittSimpleItTest(RawParser_it_t* desc, u8 * data, rawP_size_t size)
{
    if(desc == NULL || data == NULL) {
        return 1;
    }

    int conterNotvalid = 0;
    int it = 0;
    //--------------------------------------------------------------------------------------------------


    while(it < D_RAW_P_IT_TEST_TRYING) {
        for(reg i = 0; i < size; ++i) {
            data[i] += it;
            desc->TX.data[i] = data[i];
        }

        RawParser_it_TXpush(desc, size);

        if(desc->TX.size == 0) {
            ++conterNotvalid;
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
                 return conterNotvalid;
             } else if(RX->size) {
                if(RX->size != size) {
                    ++conterNotvalid;
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



int rawParserItTest(unsigned int randomSeed, int randTestCount)
{
    int conterNotvalid = 0;
    srand(randomSeed); // use current time as seed for random generator

    //--------------------------------------------------------------------------------------------------
    RawParser_it_t * prot = rawParser_it_new(0x1A);

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


    conterNotvalid += receiveTransmittSimpleItTest(prot, data, 300);





    free(data);
    conterNotvalid += rawParser_it_delete(&prot);
    //--------------------------------------------------------------------------------------------------
    return conterNotvalid;
}

#undef D_RAW_P_IT_TEST_TRYING

#endif /* D_RAW_P_TEST_DISABLE */
