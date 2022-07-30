

#define INLINE
#include "rawparser_dma.h"
#include "byte_order.h"
#include <stdlib.h>


#ifndef NULL
#define NULL (void *)0
#endif /* NULL */


// FSM data fusion -----------------------------
#define RECEIVE_LEN_0           0x00U

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
#define RECEIVE_LEN_LOW         0x01U
#define RECEIVE_LEN_HIGH        0x02U
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

#define RECEIVE_DATA            0x03U

#ifdef D_RAW_P_CRC_ENA
#define RECEIVE_CRC_0           0x04U

#if defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32)
#define RECEIVE_CRC_1           0x05U
#endif /* defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) */

#if defined(D_RAW_P_USE_CRC32)
#define RECEIVE_CRC_2           0x06U
#define RECEIVE_CRC_3           0x07U
#endif /* defined(D_RAW_P_USE_CRC32) */

#endif /* D_RAW_P_CRC_ENA */

#define RECEIVE_ERR             0x08U
#define RECEIVE_OK              0x09U
//----------------------------------------------

RawParser_dma_t* rawParser_dma_new(u8 packStart)
{
#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    M_Assert_BreakSaveCheck(packStart == RECEIVE_EXTENDED_LEN_CMD, M_EMPTY, return (RawParser_dma_t*)NULL, "rawParser_new: start byte must be not equal RECEIVE_EXTENDED_LEN_CMD");
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

    RawParser_dma_t* self = (RawParser_dma_t *)calloc(1, sizeof(RawParser_dma_t));
    M_Assert_BreakSaveCheck(self == (RawParser_dma_t *)NULL, M_EMPTY, return self, "rawParser_dma_new: No memory for allocation ");

    self->m_startByte = packStart;
    self->m_receivePackLen = (rawP_size_t)0;

#ifdef D_RAW_P_CRC_ENA
    self->m_receiveCalcCRC = D_RAW_P_CRC_INIT;
    self->m_transmittCalcCRC = D_RAW_P_CRC_INIT;

    #if defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32)
        self->m_receiveCRCBuf = D_RAW_P_CRC_INIT;
    #endif /* defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) */

#endif /* D_RAW_P_CRC_ENA */
    
    self->m_triggerSB = 0;
    self->m_receivePos = 0;
    self->m_receiveReadPos = 0;
    self->m_receiveHandlePos = 0;

    self->m_transmittPos = 0;
    self->receiveState = RECEIVE_LEN_0;

    self->TX.data = self->m_sendBuffer;
    self->TX.size = (rawP_size_t)0;

    self->RX.data = self->m_receiveFrameBuffer;
    self->RX.size = (rawP_size_t)0;

    return self;
}


int rawParser_dma_delete(RawParser_dma_t** data)
{
    M_Assert_BreakSaveCheck((data == NULL) || (*data == NULL), M_EMPTY, return 0, "rawParser_dma_new: No allocated memory");
    free(*data);
    *data = NULL;

    return 1;
}


static void RawParser_dma_proceedByte(RawParser_dma_t * const self, const u8 ch, const unsigned char newFrame)
{
    M_Assert_Break((self == (RawParser_dma_t*)NULL), M_EMPTY, return, "RawParser_dma_proceedByte: No valid input");

    if (newFrame) {

#ifdef D_RAW_P_CRC_ENA
        self->m_receiveCalcCRC = D_RAW_P_CRC_INIT;
#endif /* D_RAW_P_CRC_ENA */

        self->m_receiveHandlePos = 0U;
        self->receiveState = RECEIVE_LEN_0;
    }

    switch(self->receiveState) {

    case RECEIVE_LEN_0:

#ifdef D_RAW_P_CRC_ENA
    self->m_receiveCalcCRC = D_RAW_P_CRC_UPDATE(self->m_receiveCalcCRC, ch);
#endif /* D_RAW_P_CRC_ENA */

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
        if(RECEIVE_EXTENDED_LEN_CMD == ch) {
            self->receiveState = RECEIVE_LEN_LOW;
        } else {
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */
            if (ch > self->m_startByte) {
                self->m_receivePackLen = (ch - 1U);
            } else {
                self->m_receivePackLen = ch;
            }

            self->receiveState = RECEIVE_DATA;
            M_Assert_WarningSaveCheck(((u32)self->m_receivePackLen > D_RAW_P_RX_BUF_SIZE || self->m_receivePackLen == 0), M_EMPTY, {
                                              self->receiveState = RECEIVE_ERR;
                                          }, "RawParser_proceedByte: No valid receive length, rx_len = %d, max_len = %d\n", self->m_receivePackLen, D_RAW_P_RX_BUF_SIZE);

            self->m_receiveHandlePos = 0;

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
        }
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

        break;

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    case RECEIVE_LEN_LOW:

#ifdef D_RAW_P_CRC_ENA
    self->m_receiveCalcCRC = D_RAW_P_CRC_UPDATE(self->m_receiveCalcCRC, ch);
#endif /* D_RAW_P_CRC_ENA */

        self->m_receivePackLen = (rawP_size_t)(ch & 0x000000FFUL);    // read low byte
        self->receiveState = RECEIVE_LEN_HIGH;
        break;

    case RECEIVE_LEN_HIGH:

#ifdef D_RAW_P_CRC_ENA
    self->m_receiveCalcCRC = D_RAW_P_CRC_UPDATE(self->m_receiveCalcCRC, ch);
#endif /* D_RAW_P_CRC_ENA */

        self->m_receivePackLen |= (rawP_size_t)((((rawP_size_t)ch) << 8U) & 0x0000FF00UL); // read high byte
        self->m_receivePackLen = LittleEndianU16(self->m_receivePackLen);

        self->receiveState = RECEIVE_DATA;
        M_Assert_WarningSaveCheck((self->m_receivePackLen > D_RAW_P_RX_BUF_SIZE || self->m_receivePackLen == 0), M_EMPTY, {
                                          self->receiveState = RECEIVE_ERR;
                                      }, "RawParser_proceedByte: No valid receive length, rx_len = %d, max_len = %d\n", self->m_receivePackLen, D_RAW_P_RX_BUF_SIZE);

        self->m_receiveHandlePos = 0;
        break;
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

    case RECEIVE_DATA:

#ifdef D_RAW_P_CRC_ENA
    self->m_receiveCalcCRC = D_RAW_P_CRC_UPDATE(self->m_receiveCalcCRC, ch);
#endif /* D_RAW_P_CRC_ENA */

        self->m_receiveFrameBuffer[self->m_receiveHandlePos++] = ch;

        if (self->m_receiveHandlePos == self->m_receivePackLen) {

#ifdef D_RAW_P_CRC_ENA
            self->receiveState = RECEIVE_CRC_0;
#else
            self->RX.size = self->m_receivePackLen;
            self->receiveState = RECEIVE_OK;
#endif /* D_RAW_P_CRC_ENA */
        }
        break;


#ifdef D_RAW_P_CRC_ENA

    case RECEIVE_CRC_0:

    #ifdef D_RAW_P_USE_CRC8
        D_RAW_P_CRC_FINAL(self->m_receiveCalcCRC);

        if(self->m_receiveCalcCRC == ch) {
            self->RX.size = self->m_receivePackLen;
            self->receiveState = RECEIVE_OK;
        } else {
            M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_proceedByte: Receive CRC8 error, rx_crc: %d, calc_crc: %d\n", ch, self->m_receiveCalcCRC);
            self->receiveState = RECEIVE_ERR;
        }


    #elif defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32)

        self->m_receiveCRCBuf = (rawP_crc_t)(ch & 0x000000FFUL); // read 0 byte
        self->receiveState = RECEIVE_CRC_1;

    #endif /* CRC 0b SWITCH LOGIC */

        break;



    #if defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32)
        case RECEIVE_CRC_1:
            self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)ch) << 8U) & 0x0000FF00UL); // read 1 byte

        #if defined(D_RAW_P_USE_CRC16)
            self->m_receiveCRCBuf = LittleEndianU16(self->m_receiveCRCBuf);
            D_RAW_P_CRC_FINAL(self->m_receiveCalcCRC);

            if(self->m_receiveCalcCRC == self->m_receiveCRCBuf) {
                self->RX.size = self->m_receivePackLen;
                self->receiveState = RECEIVE_OK;
            } else {
                M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_proceedByte: Receive CRC16 error, rx_crc: %d, calc_crc: %d\n", self->m_receiveCRCBuf, self->m_receiveCalcCRC);
                self->receiveState = RECEIVE_ERR;
            }

        #elif defined(D_RAW_P_USE_CRC32)
            self->receiveState = RECEIVE_CRC_2;
        #endif /* CRC16 - CRC32 SWITCH */
            break;

    #endif /* defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) */



    #if defined(D_RAW_P_USE_CRC32)
        case RECEIVE_CRC_2:
            self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)ch) << 16U) & 0x00FF0000UL); // read 2 byte
            self->receiveState = RECEIVE_CRC_3;
            break;

        case RECEIVE_CRC_3:
            self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)ch) << 24U) & 0xFF000000UL); // read 3 byte
            self->m_receiveCRCBuf = LittleEndianU32(self->m_receiveCRCBuf);
            D_RAW_P_CRC_FINAL(self->m_receiveCalcCRC);

            if(self->m_receiveCalcCRC == self->m_receiveCRCBuf) {
                self->RX.size = self->m_receivePackLen;
                self->receiveState = RECEIVE_OK;
            } else {
                M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_proceedByte: Receive CRC32 error, rx data: %d, crc calc:%d\n", self->m_receiveCRCBuf, self->m_receiveCalcCRC);
                self->receiveState = RECEIVE_ERR;
            }
            break;

    #endif /* defined(D_RAW_P_USE_CRC32) */

#endif /* D_RAW_P_CRC_ENA */


    case RECEIVE_ERR:
        M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_proceedByte: Receive error, byte: %d, rx_len: %d, max_rxlen: %d\n", ch, self->m_receivePackLen, D_RAW_P_RX_BUF_SIZE);
        break;

    case RECEIVE_OK:
        M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_proceedByte: LAST Received OK, byte: %d is not received because no SB", ch);
        break;

    default:
        break;
    }
}



RawParser_Frame_t* RawParser_dma_proceed(RawParser_dma_t * const self)
{
    M_Assert_Break((self == (RawParser_dma_t*)NULL), M_EMPTY, return (RawParser_Frame_t*)NULL, "RawParser_proceed: No valid input");
    self->RX.size = 0;

    while ((self->m_receivePos & (D_RAW_P_RX_BUF_SIZE - 1U)) != (self->m_receiveReadPos & (D_RAW_P_RX_BUF_SIZE - 1U))) {

        u8 ch = self->m_receiveBuffer[self->m_receiveReadPos & (D_RAW_P_RX_BUF_SIZE - 1U)];
        ++self->m_receiveReadPos;

        if (self->m_triggerSB) {
            if(self->m_startByte == ch) { //{SB}{SB} -> {SB}
                RawParser_dma_proceedByte(self, ch, 0);
            } else { //{SB}{!SB} -> {SB} and newframe
                RawParser_dma_proceedByte(self, ch, 1);
            }
            self->m_triggerSB = 0;
        } else if (self->m_startByte == ch) { //{!SB}{SB} -> set flag and skip step
            self->m_triggerSB = 1;
        } else { //{!SB}{!SB} -> {!SB}
            RawParser_dma_proceedByte(self, ch, 0);
        }

        if(self->RX.size != 0) {
            return &self->RX;
        }
    }

    return &self->RX;
}

// slow shield functions (slow & more copy)-----------------------------------------------------------------------------------------
RawParser_Frame_t* RawParser_dma_shieldFrame(RawParser_dma_t * const self, u8 * data, rawP_size_t len)
{
    M_Assert_Break((self == (RawParser_dma_t*)NULL || len == 0), M_EMPTY, return (RawParser_Frame_t*)NULL, "RawParser_dma_shieldFrame: No valid input");
    M_Assert_Break((data == (u8*)NULL), M_EMPTY, {
                       self->TX.size = 0;
                       return &self->TX;
                   }, "RawParser_dma_shieldFrame: No valid input");

    RawParser_dma_startTransmittPacket(self, len);
    while(len--) {
        RawParser_dma_addTxByteCRC(self, *data++);
    }
    return RawParser_dma_finishTransmittPacket(self);
}

// fast shield functions (no copy)-----------------------------------------------------------------------------------------
void RawParser_dma_startTransmittPacket(RawParser_dma_t * const self, rawP_size_t predictedLen)
{
    M_Assert_Break(((self == (RawParser_dma_t*)NULL) || (predictedLen == 0) || ((u32)predictedLen > (D_RAW_P_TX_BUF_SIZE - 1))), M_EMPTY, return , "RawParser_dma_startTransmittPacket: No valid input");

#ifdef D_RAW_P_CRC_ENA
    self->m_transmittCalcCRC = D_RAW_P_CRC_INIT;
#endif /* D_RAW_P_CRC_ENA */

    self->m_transmittPos = 0;
    self->m_sendBuffer[self->m_transmittPos++] = self->m_startByte;

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    if(predictedLen > D_RAW_P_LEN_SEPARATOR) {
        predictedLen = LittleEndianU16(predictedLen);

        RawParser_dma_addTxByteCRC(self, RECEIVE_EXTENDED_LEN_CMD);
        RawParser_dma_addTxByteCRC(self, (u8)(predictedLen & 0x000000FFU));
        RawParser_dma_addTxByteCRC(self, (u8)((predictedLen >> 8U) & 0x000000FFU));

    } else {
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

        RawParser_dma_addTxByteCRC(self, (u8)(((predictedLen >= self->m_startByte) ? (predictedLen + 1U) : predictedLen) & 0x000000FFU));

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    }
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */
}


RawParser_Frame_t* RawParser_dma_finishTransmittPacket(RawParser_dma_t * const self)
{
    M_Assert_Break((self == (RawParser_dma_t*)NULL), M_EMPTY, return (RawParser_Frame_t*)NULL, "RawParser_dma_startTransmittPacket: No valid input");


#ifdef D_RAW_P_CRC_ENA
    D_RAW_P_CRC_FINAL(self->m_transmittCalcCRC);

    #ifdef D_RAW_P_USE_CRC8
        RawParser_dma_addTxByte(self, self->m_transmittCalcCRC);
    #elif defined(D_RAW_P_USE_CRC16)
        self->m_transmittCalcCRC = LittleEndianU16(self->m_transmittCalcCRC);

        RawParser_dma_addTxByte(self, (u8)((self->m_transmittCalcCRC      ) & 0x000000FFU));
        RawParser_dma_addTxByte(self, (u8)((self->m_transmittCalcCRC >> 8U) & 0x000000FFU));
    #elif defined(D_RAW_P_USE_CRC32)
        self->m_transmittCalcCRC = LittleEndianU32(self->m_transmittCalcCRC);

        RawParser_dma_addTxByte(self, (u8)((self->m_transmittCalcCRC       ) & 0x000000FFU));
        RawParser_dma_addTxByte(self, (u8)((self->m_transmittCalcCRC >> 8U ) & 0x000000FFU));
        RawParser_dma_addTxByte(self, (u8)((self->m_transmittCalcCRC >> 16U) & 0x000000FFU));
        RawParser_dma_addTxByte(self, (u8)((self->m_transmittCalcCRC >> 24U) & 0x000000FFU));
    #endif /* CRC SWITCH LOGIC */

#endif /* D_RAW_P_CRC_ENA */

    self->TX.size = (rawP_size_t)self->m_transmittPos;
    return &self->TX;
}




// elementary byte adding functions ----------------------------------------------------------------------------
inline void RawParser_dma_addTxByte(RawParser_dma_t * const self, const u8 byte)
{
    M_Assert_Break((self->m_transmittPos == D_RAW_P_TX_BUF_SIZE), M_EMPTY, return, "RawParser_dma_addTxByte: LEN packet more than buffer");

    self->m_sendBuffer[self->m_transmittPos++] = byte;
    if(byte == self->m_startByte) {
        M_Assert_Break((self->m_transmittPos == D_RAW_P_TX_BUF_SIZE), M_EMPTY, return, "RawParser_dma_addTxByte: LEN packet more than buffer");

        self->m_sendBuffer[self->m_transmittPos++] = byte;
    }
}

#ifdef D_RAW_P_CRC_ENA
inline void RawParser_dma_addTxByteCRC(RawParser_dma_t * const self, const u8 byte)
{
    M_Assert_Break((self->m_transmittPos == D_RAW_P_TX_BUF_SIZE), M_EMPTY, return, "RawParser_dma_addTxByte: LEN packet more than buffer");

    self->m_transmittCalcCRC = D_RAW_P_CRC_UPDATE(self->m_transmittCalcCRC, byte);

    self->m_sendBuffer[self->m_transmittPos++] = byte;
    if(byte == self->m_startByte) {
        M_Assert_Break((self->m_transmittPos == D_RAW_P_TX_BUF_SIZE), M_EMPTY, return, "RawParser_dma_addTxByte: LEN packet more than buffer");

        self->m_sendBuffer[self->m_transmittPos++] = byte;
    }
}
#endif /* D_RAW_P_CRC_ENA */


// FSM data fusion delete -----------------------------
#undef RECEIVE_LEN_0

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    #undef RECEIVE_LEN_LOW
    #undef RECEIVE_LEN_HIGH
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

#undef RECEIVE_DATA

#ifdef D_RAW_P_CRC_ENA
    #undef RECEIVE_CRC_0

    #if defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32)
        #undef RECEIVE_CRC_1
    #endif /* defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) */

    #if defined(D_RAW_P_USE_CRC32)
        #undef RECEIVE_CRC_2
        #undef RECEIVE_CRC_3
    #endif /* defined(D_RAW_P_USE_CRC32) */
#else
    #undef RawParser_dma_addTxByteCRC
#endif /* D_RAW_P_CRC_ENA */

#undef RECEIVE_ERR
#undef RECEIVE_OK
//----------------------------------------------

#undef INLINE
