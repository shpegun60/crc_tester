#include "raw_parser_it.h"
#include "smart_assert.h"


#if defined(D_RAW_P_TWO_BYTES_LEN_SUPPORT) || defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
    #include "byte_order.h"
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */


#include <stdlib.h>


#ifndef NULL
#define NULL (void *)0
#endif /* NULL */


// FSM RECEIVE data fusion -------------------------------------------------------------------------------------
#define RAW_P_IT_RECEIVE_LEN_0           0x00U

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
#define RAW_P_IT_RECEIVE_LEN_LOW         0x01U
#define RAW_P_IT_RECEIVE_LEN_HIGH        0x02U
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

#define RAW_P_IT_RECEIVE_DATA            0x03U

#ifdef D_RAW_P_CRC_ENA
#define RAW_P_IT_RECEIVE_CRC_0           0x04U

#if defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
#define RAW_P_IT_RECEIVE_CRC_1           0x05U
#endif /* defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) */

#if defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
#define RAW_P_IT_RECEIVE_CRC_2           0x06U
#define RAW_P_IT_RECEIVE_CRC_3           0x07U
#endif /* defined(D_RAW_P_USE_CRC32) */

#if defined(D_RAW_P_USE_CRC64)
#define RAW_P_IT_RECEIVE_CRC_4           0x08U
#define RAW_P_IT_RECEIVE_CRC_5           0x09U
#define RAW_P_IT_RECEIVE_CRC_6           0x0AU
#define RAW_P_IT_RECEIVE_CRC_7           0x0BU
#endif /* defined(D_RAW_P_USE_CRC64) */

#endif /* D_RAW_P_CRC_ENA */

#define RAW_P_IT_RECEIVE_ERR             0x0CU
#define RAW_P_IT_RECEIVE_OK              0x0DU
//---------------------------------------------------------------------------------------------------------------

RawParser_it_t* rawParser_it_new(const u8 packStart)
{
#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    M_Assert_BreakSaveCheck(packStart == RECEIVE_EXTENDED_LEN_CMD, M_EMPTY, return (RawParser_it_t*)NULL, "rawParser_it_new: start byte: %d must be not equal RECEIVE_EXTENDED_LEN_CMD: %d", packStart, RECEIVE_EXTENDED_LEN_CMD);
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

    RawParser_it_t* self = (RawParser_it_t *)calloc(1, sizeof(RawParser_it_t));
    M_Assert_BreakSaveCheck(self == (RawParser_it_t *)NULL, M_EMPTY, return self, "RawParser_it_t: No memory for allocation ");


    rawParser_it_init(self, packStart);
    return self;
}

void rawParser_it_init(RawParser_it_t * const self, const u8 packStart)
{
#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    M_Assert_BreakSaveCheck(packStart == RECEIVE_EXTENDED_LEN_CMD, M_EMPTY, return, "rawParser_it_init: start byte: %d must be not equal RECEIVE_EXTENDED_LEN_CMD: %d", packStart, RECEIVE_EXTENDED_LEN_CMD);
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

    M_Assert_BreakSaveCheck(self == (RawParser_it_t *)NULL, M_EMPTY, return, "rawParser_it_init: No input data valid ");


    self->m_startByte = packStart;
    self->m_receivePackLen = (rawP_size_t)0;
    self->m_rx_pack_ready = 0;

#ifdef D_RAW_P_CRC_ENA
    self->m_receiveCalcCRC = D_RAW_P_CRC_INIT;
    self->m_transmittCalcCRC = D_RAW_P_CRC_INIT;

    #if defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
        self->m_receiveCRCBuf = D_RAW_P_CRC_INIT;
    #endif /* defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64) */

#endif /* D_RAW_P_CRC_ENA */

    self->m_triggerSB = 0;
    self->m_receivePos = 0;

    self->m_transmittPos = 0;
    self->receiveState = RAW_P_IT_RECEIVE_LEN_0;
    //self->transmittState = xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx;

    self->TX.data = self->m_sendBuffer;
    self->TX.size = (rawP_size_t)0;

    self->RX.data = self->m_receiveFrameBuffer;
    self->RX.size = (rawP_size_t)0;

    self->uniRXPosition = 0;


#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
    rs_initialize_ecc(&self->rs_ecc);
#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */
}

int rawParser_it_delete(RawParser_it_t** data)
{
    M_Assert_BreakSaveCheck((data == NULL) || (*data == NULL), M_EMPTY, return 1, "rawParser_it_delete: No allocated memory");
    free(*data);
    *data = NULL;

    return 0;
}

static void RawParser_it_proceedByte(RawParser_it_t* const self, const u8 ch, const u8 newFrame)
{
    M_Assert_Break((self == (RawParser_it_t*)NULL), M_EMPTY, return, "RawParser_it_proceedByte: No valid input");

    if (newFrame) {

#ifdef D_RAW_P_CRC_ENA
        self->m_receiveCalcCRC = D_RAW_P_CRC_INIT;
        D_RAW_P_CRC_START(self->m_receiveCalcCRC);
#endif /* D_RAW_P_CRC_ENA */

        self->m_receivePos = 0U;
        self->receiveState = RAW_P_IT_RECEIVE_LEN_0;
    }

    switch(self->receiveState) {

    case RAW_P_IT_RECEIVE_LEN_0:

#ifdef D_RAW_P_CRC_ENA
    self->m_receiveCalcCRC = D_RAW_P_CRC_UPDATE(self->m_receiveCalcCRC, ch);
#endif /* D_RAW_P_CRC_ENA */

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
        if(RECEIVE_EXTENDED_LEN_CMD == ch) {
            self->receiveState = RAW_P_IT_RECEIVE_LEN_LOW;
        } else {
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */
            if (ch > self->m_startByte) {
                self->m_receivePackLen = (ch - 1U);
            } else {
                self->m_receivePackLen = ch;
            }

            self->receiveState = RAW_P_IT_RECEIVE_DATA;
            M_Assert_WarningSaveCheck(((u32)self->m_receivePackLen > D_RAW_P_RX_BUF_SIZE || self->m_receivePackLen == 0), M_EMPTY, {
                                              self->receiveState = RAW_P_IT_RECEIVE_ERR;
                                          }, "RawParser_it_proceedByte: No valid receive length, rx_len = %d, max_len = %d", self->m_receivePackLen, D_RAW_P_RX_BUF_SIZE);

            self->m_receivePos = 0;

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
        }
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

        break;

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    case RAW_P_IT_RECEIVE_LEN_LOW:

#ifdef D_RAW_P_CRC_ENA
    self->m_receiveCalcCRC = D_RAW_P_CRC_UPDATE(self->m_receiveCalcCRC, ch);
#endif /* D_RAW_P_CRC_ENA */

        self->m_receivePackLen = (rawP_size_t)(ch & 0x000000FFUL);    // read low byte
        self->receiveState = RAW_P_IT_RECEIVE_LEN_HIGH;
        break;

    case RAW_P_IT_RECEIVE_LEN_HIGH:

#ifdef D_RAW_P_CRC_ENA
    self->m_receiveCalcCRC = D_RAW_P_CRC_UPDATE(self->m_receiveCalcCRC, ch);
#endif /* D_RAW_P_CRC_ENA */

        self->m_receivePackLen |= (rawP_size_t)((((rawP_size_t)ch) << 8U) & 0x0000FF00UL); // read high byte
        self->m_receivePackLen = LittleEndianU16(self->m_receivePackLen);

        self->receiveState = RAW_P_IT_RECEIVE_DATA;
        M_Assert_WarningSaveCheck((self->m_receivePackLen > D_RAW_P_RX_BUF_SIZE || self->m_receivePackLen == 0), M_EMPTY, {
                                          self->receiveState = RAW_P_IT_RECEIVE_ERR;
                                      }, "RawParser_it_proceedByte: No valid receive length, rx_len = %d, max_len = %d", self->m_receivePackLen, D_RAW_P_RX_BUF_SIZE);

        self->m_receivePos = 0;
        break;
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

    case RAW_P_IT_RECEIVE_DATA:

#ifdef D_RAW_P_CRC_ENA
        self->m_receiveCalcCRC = D_RAW_P_CRC_UPDATE(self->m_receiveCalcCRC, ch);
#endif /* D_RAW_P_CRC_ENA */

        self->m_receiveFrameBuffer[self->m_receivePos++] = ch;

        if (self->m_receivePos == self->m_receivePackLen) {

#ifdef D_RAW_P_CRC_ENA
            D_RAW_P_CRC_FINAL(self->m_receiveCalcCRC);
            self->receiveState = RAW_P_IT_RECEIVE_CRC_0;
#else
            self->RX.size = self->m_receivePackLen;
            self->receiveState = RAW_P_DMA_RECEIVE_OK;
#endif /* D_RAW_P_CRC_ENA */
        }
        break;


#ifdef D_RAW_P_CRC_ENA

    case RAW_P_IT_RECEIVE_CRC_0:

    #ifdef D_RAW_P_USE_CRC8

        if(self->m_receiveCalcCRC == ch) {
            self->RX.size = self->m_receivePackLen;
            self->receiveState = RAW_P_IT_RECEIVE_OK;
        } else {
            M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_it_proceedByte: Receive CRC8 error, rx_crc: %d, calc_crc: %d", ch, self->m_receiveCalcCRC);
            self->receiveState = RAW_P_IT_RECEIVE_ERR;
        }


    #elif defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)

        self->m_receiveCRCBuf = (rawP_crc_t)(ch & 0x000000FFUL); // read 0 byte
        self->receiveState = RAW_P_IT_RECEIVE_CRC_1;

    #endif /* CRC 0b SWITCH LOGIC */

        break;



    #if defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
        case RAW_P_IT_RECEIVE_CRC_1:
            self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)ch) << 8U) & 0x0000FF00UL); // read 1 byte

        #if defined(D_RAW_P_USE_CRC16)
            self->m_receiveCRCBuf = LittleEndianU16(self->m_receiveCRCBuf);

            if(self->m_receiveCalcCRC == self->m_receiveCRCBuf) {
                self->RX.size = self->m_receivePackLen;
                self->receiveState = RAW_P_IT_RECEIVE_OK;
            } else {
                M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_it_proceedByte: Receive CRC16 error, rx_crc: %d, calc_crc: %d", self->m_receiveCRCBuf, self->m_receiveCalcCRC);
                self->receiveState = RAW_P_IT_RECEIVE_ERR;
            }

        #elif defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
            self->receiveState = RAW_P_IT_RECEIVE_CRC_2;
        #endif /* CRC16 - CRC32 - CRC64 SWITCH */
            break;

    #endif /* defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64) */



    #if defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
        case RAW_P_IT_RECEIVE_CRC_2:
            self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)ch) << 16U) & 0x00FF0000UL); // read 2 byte
            self->receiveState = RAW_P_IT_RECEIVE_CRC_3;
            break;

        case RAW_P_IT_RECEIVE_CRC_3:
            self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)ch) << 24U) & 0xFF000000UL); // read 3 byte

        #if defined(D_RAW_P_USE_CRC32)
            self->m_receiveCRCBuf = LittleEndianU32(self->m_receiveCRCBuf);

            if(self->m_receiveCalcCRC == self->m_receiveCRCBuf) {
                self->RX.size = self->m_receivePackLen;
                self->receiveState = RAW_P_IT_RECEIVE_OK;
            } else {
                M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_it_proceedByte: Receive CRC32 error, rx data: %d, crc calc:%d", self->m_receiveCRCBuf, self->m_receiveCalcCRC);
                self->receiveState = RAW_P_IT_RECEIVE_ERR;
            }
        #elif defined(D_RAW_P_USE_CRC64)
            self->receiveState = RAW_P_IT_RECEIVE_CRC_4;
        #endif /* defined(D_RAW_P_USE_CRC32) */
            break;

    #endif /* defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64) */

    #if defined(D_RAW_P_USE_CRC64)
        case RAW_P_IT_RECEIVE_CRC_4:
            self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)ch) << 32U) & 0x000000FF00000000ULL); // read 4 byte
            self->receiveState = RAW_P_IT_RECEIVE_CRC_5;
            break;

        case RAW_P_IT_RECEIVE_CRC_5:
            self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)ch) << 40U) & 0x0000FF0000000000ULL); // read 5 byte
            self->receiveState = RAW_P_IT_RECEIVE_CRC_6;
            break;

        case RAW_P_IT_RECEIVE_CRC_6:
            self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)ch) << 48U) & 0x00FF000000000000ULL); // read 6 byte
            self->receiveState = RAW_P_IT_RECEIVE_CRC_7;
            break;

        case RAW_P_IT_RECEIVE_CRC_7:
            self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)ch) << 56U) & 0xFF00000000000000ULL); // read 7 byte
            self->m_receiveCRCBuf = LittleEndianU64(self->m_receiveCRCBuf);

            if(self->m_receiveCalcCRC == self->m_receiveCRCBuf) {
                self->RX.size = self->m_receivePackLen;
                self->receiveState = RAW_P_IT_RECEIVE_OK;
            } else {
                M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_it_proceedByte: Receive CRC64 error, rx data: %d, crc calc:%d", self->m_receiveCRCBuf, self->m_receiveCalcCRC);
                self->receiveState = RAW_P_IT_RECEIVE_ERR;
            }
            break;
    #endif /* defined(D_RAW_P_USE_CRC64) */

#endif /* D_RAW_P_CRC_ENA */


    case RAW_P_IT_RECEIVE_ERR:
        M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_it_proceedByte: Receive error, byte: %d, rx_len: %d, max_rxlen: %d", ch, self->m_receivePackLen, D_RAW_P_RX_BUF_SIZE);
        break;

    case RAW_P_IT_RECEIVE_OK:
        M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_it_proceedByte: LAST Received OK, byte: %d is not received because no SB", ch);
        break;

    default:
        break;
    }
}



void RawParser_it_proceed(RawParser_it_t* const self, const u8 ch)
{
    M_Assert_Break((self == (RawParser_it_t*)NULL), M_EMPTY, return, "RawParser_it_proceed: No valid input");

    if(!self->m_rx_pack_ready) {

        self->RX.size = 0;
        if (self->m_triggerSB) {
            if(self->m_startByte == ch) { //{SB}{SB} -> {SB}
                RawParser_it_proceedByte(self, ch, 0);
            } else { //{SB}{!SB} -> {SB} and newframe
                RawParser_it_proceedByte(self, ch, 1);
            }
            self->m_triggerSB = 0;
        } else if (self->m_startByte == ch) { //{!SB}{SB} -> set flag and skip step
            self->m_triggerSB = 1;
        } else { //{!SB}{!SB} -> {!SB}
            RawParser_it_proceedByte(self, ch, 0);
        }

        if(self->RX.size != 0) {
            self->m_rx_pack_ready = 1;
        }
    }
}

//void RawParser_it_proceed_loop(RawParser_it_t* const self)
//{
//    if(self->m_rx_pack_ready) {
//#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
//        if(self->RX.size > RSCODE_NPAR) {
//            /* Now decode -- encoded codeword size must be passed */
//            rscode_decode(&self->rs_ecc, self->RX.data, self->RX.size);
//            self->RX.size -= RSCODE_NPAR;
//        } else {
//            self->RX.size = 0;
//        }
//#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */
//        //return &self->RX;
//    }

//}


