#include "rawparser_dma.h"

#if defined(D_RAW_P_TWO_BYTES_LEN_SUPPORT) || defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
    #include "byte_order.h"
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */


#include <stdlib.h>


// FSM data fusion -----------------------------
#define RAW_P_DMA_RECEIVE_LEN_0           0x00U

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
#define RAW_P_DMA_RECEIVE_LEN_LOW         0x01U
#define RAW_P_DMA_RECEIVE_LEN_HIGH        0x02U
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

#define RAW_P_DMA_RECEIVE_DATA            0x03U

#ifdef D_RAW_P_CRC_ENA
#define RAW_P_DMA_RECEIVE_CRC_0           0x04U

#if defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
#define RAW_P_DMA_RECEIVE_CRC_1           0x05U
#endif /* defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) */

#if defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
#define RAW_P_DMA_RECEIVE_CRC_2           0x06U
#define RAW_P_DMA_RECEIVE_CRC_3           0x07U
#endif /* defined(D_RAW_P_USE_CRC32) */

#if defined(D_RAW_P_USE_CRC64)
#define RAW_P_DMA_RECEIVE_CRC_4           0x08U
#define RAW_P_DMA_RECEIVE_CRC_5           0x09U
#define RAW_P_DMA_RECEIVE_CRC_6           0x0AU
#define RAW_P_DMA_RECEIVE_CRC_7           0x0BU
#endif /* defined(D_RAW_P_USE_CRC64) */

#endif /* D_RAW_P_CRC_ENA */

#define RAW_P_DMA_RECEIVE_ERR             0x0CU
#define RAW_P_DMA_RECEIVE_OK              0x0DU
//----------------------------------------------

// receive functions-----------------------------------------------------------------------------------------
C_INLINE void RawParser_dma_receiveByte(RawParser_dma_t* const self, const u8 byte);
C_INLINE void RawParser_dma_receiveArray(RawParser_dma_t* const self, u8* arr, reg len);

// elementary byte adding functions ----------------------------------------------------------------------------
C_INLINE void RawParser_dma_addTxByte(RawParser_dma_t* const self, const u8 byte);
C_INLINE void RawParser_dma_addTxByteCRC(RawParser_dma_t* const self, const u8 byte);


/* CREATE/DELETE FUNCTIONS *************************************************************************************************
 *
 */
RawParser_dma_t* rawParser_dma_new(const u8 packStart)
{
#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    M_Assert_BreakSaveCheck(packStart == RECEIVE_EXTENDED_LEN_CMD, M_EMPTY, return (RawParser_dma_t*)NULL, "rawParser_dma_new: start byte: %d must be not equal RECEIVE_EXTENDED_LEN_CMD: %d", packStart, RECEIVE_EXTENDED_LEN_CMD);
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

    RawParser_dma_t* self = (RawParser_dma_t *)calloc(1, sizeof(RawParser_dma_t));
    M_Assert_BreakSaveCheck(self == (RawParser_dma_t *)NULL, M_EMPTY, return self, "rawParser_dma_new: No memory for allocation ");

    rawParser_dma_init(self, packStart);
    return self;
}

void rawParser_dma_init(RawParser_dma_t * const self, const u8 packStart)
{
#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    M_Assert_BreakSaveCheck(packStart == RECEIVE_EXTENDED_LEN_CMD, M_EMPTY, return, "rawParser_dma_init: start byte: %d must be not equal RECEIVE_EXTENDED_LEN_CMD: %d", packStart, RECEIVE_EXTENDED_LEN_CMD);
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

    M_Assert_BreakSaveCheck(self == (RawParser_dma_t *)NULL, M_EMPTY, return, "rawParser_dma_init: No input data valid ");

    self->m_startByte = packStart;
    self->m_receivePackLen = 0;

#ifdef D_RAW_P_CRC_ENA
    self->m_receiveCalcCRC = D_RAW_P_CRC_INIT;
    self->m_transmittCalcCRC = D_RAW_P_CRC_INIT;

    #if defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
        self->m_receiveCRCBuf = D_RAW_P_CRC_INIT;
    #endif /* defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64) */

#endif /* D_RAW_P_CRC_ENA */

    self->m_triggerSB = 0;
    self->m_receivePos = 0;
    self->m_receiveReadPos = 0;
    self->m_receiveHandlePos = 0;

    self->m_transmittPos = 0;
    self->receiveState = RAW_P_DMA_RECEIVE_LEN_0;


#ifndef D_RAW_P_DISABLE_INTERNAL_TX_BUFFER
    self->TX.data = self->m_sendBuffer;
#else
    self->TX.data = NULL;
#endif /* D_RAW_P_DISABLE_INTERNAL_TX_BUFFER */

    self->TX.size = 0;

#ifndef D_RAW_P_DISABLE_INTERNAL_RX_BUFFER
    self->RX.data = self->m_receiveFrameBuffer;
#else
     self->RX.data = NULL;
#endif /* D_RAW_P_DISABLE_INTERNAL_RX_BUFFER */

    self->RX.size = 0;

    self->uniRXPosition = 0;


#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
    rs_initialize_ecc(&self->rs_ecc);
#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */
}


#ifdef D_RAW_P_DISABLE_INTERNAL_TX_BUFFER
void rawParser_dma_setUserBufferTX(RawParser_dma_t * const self, u8 * const txBuffer)
{
    M_Assert_Break((self == (RawParser_dma_t*)NULL), M_EMPTY, return, "rawParser_dma_setUserBuffer: No valid input object");
    M_Assert_Break((txBuffer == NULL), M_EMPTY, return, "rawParser_dma_setUserBuffer: No valid input TX buffer");

    self->TX.data = txBuffer;
    self->TX.size = 0;
}
#endif /* D_RAW_P_DISABLE_INTERNAL_TX_BUFFER */

#ifdef D_RAW_P_DISABLE_INTERNAL_RX_BUFFER
void rawParser_dma_setUserBufferRX(RawParser_dma_t * const self, u8 * const rxBuffer)
{
    M_Assert_Break((self == (RawParser_dma_t*)NULL), M_EMPTY, return, "rawParser_dma_setUserBuffer: No valid input object");
    M_Assert_Break((rxBuffer == NULL), M_EMPTY, return, "rawParser_dma_setUserBuffer: No valid input RX buffer");

    self->RX.data = rxBuffer;
    self->RX.size = 0;
}
#endif /* D_RAW_P_DISABLE_INTERNAL_RX_BUFFER */

#if defined(D_RAW_P_DISABLE_INTERNAL_TX_BUFFER) && defined(D_RAW_P_DISABLE_INTERNAL_RX_BUFFER)
void rawParser_dma_setUserBuffers(RawParser_dma_t * const self, u8 * const rxBuffer, u8 * const txBuffer)
{
    M_Assert_Break((self == (RawParser_dma_t*)NULL), M_EMPTY, return, "rawParser_dma_setUserBuffer: No valid input object");
    M_Assert_Break((rxBuffer == NULL || txBuffer == NULL), M_EMPTY, return, "rawParser_dma_setUserBuffer: No valid input buffers");

    self->TX.data = txBuffer;
    self->TX.size = 0;

    self->RX.data = rxBuffer;
    self->RX.size = 0;
}
#endif /* defined(D_RAW_P_DISABLE_INTERNAL_TX_BUFFER) || defined(D_RAW_P_DISABLE_INTERNAL_RX_BUFFER) */

int rawParser_dma_delete(RawParser_dma_t** data)
{
    M_Assert_BreakSaveCheck((data == NULL) || (*data == NULL), M_EMPTY, return 1, "rawParser_dma_delete: No allocated memory");
    free(*data);
    *data = NULL;

    return 0;
}

/* PROCEED FUNCTIONS *************************************************************************************************
 *
 */


static void RawParser_dma_proceedByte(RawParser_dma_t* const self, const u8 ch, const u8 newFrame)
{
    M_Assert_Break((self == (RawParser_dma_t*)NULL), M_EMPTY, return, "RawParser_dma_proceedByte: No valid input");
    M_Assert_Break((self->RX.data == NULL || self->TX.data == NULL), M_EMPTY, return, "RawParser_dma_proceedByte: No valid RX or/and TX buffer , call function before: -->  rawParser_dma_setUserBufferXX, XX = RX for rx buffer, XX = TX for tx buffer, XX = s for tx & rx buffers");

    if (newFrame) {

#ifdef D_RAW_P_CRC_ENA
        self->m_receiveCalcCRC = D_RAW_P_CRC_INIT;
        D_RAW_P_CRC_START(self->m_receiveCalcCRC);
#endif /* D_RAW_P_CRC_ENA */

        self->m_receiveHandlePos = 0;
        self->receiveState = RAW_P_DMA_RECEIVE_LEN_0;
    }

    switch(self->receiveState) {

    case RAW_P_DMA_RECEIVE_LEN_0:

#ifdef D_RAW_P_CRC_ENA
        self->m_receiveCalcCRC = D_RAW_P_CRC_UPDATE(self->m_receiveCalcCRC, ch);
#endif /* D_RAW_P_CRC_ENA */

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
        if(RECEIVE_EXTENDED_LEN_CMD == ch) {
            self->receiveState = RAW_P_DMA_RECEIVE_LEN_LOW;
        } else {
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */
            self->m_receivePackLen = (ch > self->m_startByte) ? (ch - 1U) : ch;
            self->m_receiveHandlePos = 0;
            self->receiveState = RAW_P_DMA_RECEIVE_DATA;

            M_Assert_WarningSaveCheck((self->m_receivePackLen > D_RAW_P_RX_BUF_SIZE || self->m_receivePackLen == 0), M_EMPTY, {
                                              self->receiveState = RAW_P_DMA_RECEIVE_ERR;
                                          }, "RawParser_dma_proceedByte: No valid receive length, rx_len = %d, max_len = %d", self->m_receivePackLen, D_RAW_P_RX_BUF_SIZE);

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
        }
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

        break;

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    case RAW_P_DMA_RECEIVE_LEN_LOW:

#ifdef D_RAW_P_CRC_ENA
        self->m_receiveCalcCRC = D_RAW_P_CRC_UPDATE(self->m_receiveCalcCRC, ch);
#endif /* D_RAW_P_CRC_ENA */

        self->m_receivePackLen = (rawP_size_t)(ch & 0x000000FFUL);    // read low byte
        self->receiveState = RAW_P_DMA_RECEIVE_LEN_HIGH;
        break;

    case RAW_P_DMA_RECEIVE_LEN_HIGH:

#ifdef D_RAW_P_CRC_ENA
    self->m_receiveCalcCRC = D_RAW_P_CRC_UPDATE(self->m_receiveCalcCRC, ch);
#endif /* D_RAW_P_CRC_ENA */

        self->m_receivePackLen |= (rawP_size_t)((((rawP_size_t)ch) << 8U) & 0x0000FF00UL); // read high byte
        self->m_receivePackLen = LittleEndianU16(self->m_receivePackLen);

        self->m_receiveHandlePos = 0;
        self->receiveState = RAW_P_DMA_RECEIVE_DATA;
        M_Assert_WarningSaveCheck((self->m_receivePackLen > D_RAW_P_RX_BUF_SIZE || self->m_receivePackLen == 0), M_EMPTY, {
                                          self->receiveState = RAW_P_DMA_RECEIVE_ERR;
                                      }, "RawParser_dma_proceedByte: No valid receive length, rx_len = %d, max_len = %d", self->m_receivePackLen, D_RAW_P_RX_BUF_SIZE);
        break;
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

    case RAW_P_DMA_RECEIVE_DATA:

#ifdef D_RAW_P_CRC_ENA
        self->m_receiveCalcCRC = D_RAW_P_CRC_UPDATE(self->m_receiveCalcCRC, ch);
#endif /* D_RAW_P_CRC_ENA */

        self->RX.data[self->m_receiveHandlePos++] = ch;

        if (self->m_receiveHandlePos == self->m_receivePackLen) {

#ifdef D_RAW_P_CRC_ENA
            D_RAW_P_CRC_FINAL(self->m_receiveCalcCRC);
            self->receiveState = RAW_P_DMA_RECEIVE_CRC_0;
#else
            self->RX.size = self->m_receivePackLen;
            self->receiveState = RAW_P_DMA_RECEIVE_OK;
#endif /* D_RAW_P_CRC_ENA */
        }
        break;


#ifdef D_RAW_P_CRC_ENA

    case RAW_P_DMA_RECEIVE_CRC_0:

    #ifdef D_RAW_P_USE_CRC8

        if(self->m_receiveCalcCRC == ch) {
            self->RX.size = self->m_receivePackLen;
            self->receiveState = RAW_P_DMA_RECEIVE_OK;
        } else {
            M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_dma_proceedByte: Receive CRC8 error, rx_crc: %d, calc_crc: %d", ch, self->m_receiveCalcCRC);
            self->receiveState = RAW_P_DMA_RECEIVE_ERR;
        }


    #elif defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)

        self->m_receiveCRCBuf = (rawP_crc_t)(ch & 0x000000FFUL); // read 0 byte
        self->receiveState = RAW_P_DMA_RECEIVE_CRC_1;

    #endif /* CRC 0b SWITCH LOGIC */

        break;



    #if defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
        case RAW_P_DMA_RECEIVE_CRC_1:
            self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)ch) << 8U) & 0x0000FF00UL); // read 1 byte

        #if defined(D_RAW_P_USE_CRC16)
            self->m_receiveCRCBuf = LittleEndianU16(self->m_receiveCRCBuf);

            if(self->m_receiveCalcCRC == self->m_receiveCRCBuf) {
                self->RX.size = self->m_receivePackLen;
                self->receiveState = RAW_P_DMA_RECEIVE_OK;
            } else {
                M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_dma_proceedByte: Receive CRC16 error, rx_crc: %d, calc_crc: %d", self->m_receiveCRCBuf, self->m_receiveCalcCRC);
                self->receiveState = RAW_P_DMA_RECEIVE_ERR;
            }

        #elif defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
            self->receiveState = RAW_P_DMA_RECEIVE_CRC_2;
        #endif /* CRC16 - CRC32 - CRC64 SWITCH */
            break;

    #endif /* defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64) */



    #if defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
        case RAW_P_DMA_RECEIVE_CRC_2:
            self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)ch) << 16U) & 0x00FF0000UL); // read 2 byte
            self->receiveState = RAW_P_DMA_RECEIVE_CRC_3;
            break;

        case RAW_P_DMA_RECEIVE_CRC_3:
            self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)ch) << 24U) & 0xFF000000UL); // read 3 byte

        #if defined(D_RAW_P_USE_CRC32)
            self->m_receiveCRCBuf = LittleEndianU32(self->m_receiveCRCBuf);

            if(self->m_receiveCalcCRC == self->m_receiveCRCBuf) {
                self->RX.size = self->m_receivePackLen;
                self->receiveState = RAW_P_DMA_RECEIVE_OK;
            } else {
                M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_dma_proceedByte: Receive CRC32 error, rx data: %d, crc calc:%d", self->m_receiveCRCBuf, self->m_receiveCalcCRC);
                self->receiveState = RAW_P_DMA_RECEIVE_ERR;
            }
        #elif defined(D_RAW_P_USE_CRC64)
            self->receiveState = RAW_P_DMA_RECEIVE_CRC_4;
        #endif /* defined(D_RAW_P_USE_CRC32) */
            break;

    #endif /* defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64) */

    #if defined(D_RAW_P_USE_CRC64)
        case RAW_P_DMA_RECEIVE_CRC_4:
            self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)ch) << 32U) & 0x000000FF00000000ULL); // read 4 byte
            self->receiveState = RAW_P_DMA_RECEIVE_CRC_5;
            break;

        case RAW_P_DMA_RECEIVE_CRC_5:
            self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)ch) << 40U) & 0x0000FF0000000000ULL); // read 5 byte
            self->receiveState = RAW_P_DMA_RECEIVE_CRC_6;
            break;

        case RAW_P_DMA_RECEIVE_CRC_6:
            self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)ch) << 48U) & 0x00FF000000000000ULL); // read 6 byte
            self->receiveState = RAW_P_DMA_RECEIVE_CRC_7;
            break;

        case RAW_P_DMA_RECEIVE_CRC_7:
            self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)ch) << 56U) & 0xFF00000000000000ULL); // read 7 byte
            self->m_receiveCRCBuf = LittleEndianU64(self->m_receiveCRCBuf);

            if(self->m_receiveCalcCRC == self->m_receiveCRCBuf) {
                self->RX.size = self->m_receivePackLen;
                self->receiveState = RAW_P_DMA_RECEIVE_OK;
            } else {
                M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_dma_proceedByte: Receive CRC64 error, rx data: %d, crc calc:%d", self->m_receiveCRCBuf, self->m_receiveCalcCRC);
                self->receiveState = RAW_P_DMA_RECEIVE_ERR;
            }
            break;
    #endif /* defined(D_RAW_P_USE_CRC64) */

#endif /* D_RAW_P_CRC_ENA */


    case RAW_P_DMA_RECEIVE_ERR:
        M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_dma_proceedByte: Receive error, byte: %d, rx_len: %d, max_rxlen: %d", ch, self->m_receivePackLen, D_RAW_P_RX_BUF_SIZE);
        break;

    case RAW_P_DMA_RECEIVE_OK:
        M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_dma_proceedByte: LAST Received OK, byte: %d is not received because no SB", ch);
        break;

    default:
        break;
    }
}



RawParser_Frame_t* RawParser_dma_proceed(RawParser_dma_t* const self)
{
    M_Assert_Break((self == (RawParser_dma_t*)NULL), M_EMPTY, return (RawParser_Frame_t*)NULL, "RawParser_dma_proceed: No valid input");
    M_Assert_Break((self->RX.data == NULL || self->TX.data == NULL), M_EMPTY, return(RawParser_Frame_t*)NULL, "RawParser_dma_proceed: No valid RX or/and TX buffer , call function before: -->  rawParser_dma_setUserBufferXX, XX = RX for rx buffer, XX = TX for tx buffer, XX = s for tx & rx buffers");
    self->RX.size = 0;

    while ((self->m_receivePos & (D_RAW_P_RX_BUF_SIZE - 1U)) != (self->m_receiveReadPos & (D_RAW_P_RX_BUF_SIZE - 1U))) {

        u8 ch = self->m_receiveBuffer[self->m_receiveReadPos & (D_RAW_P_RX_BUF_SIZE - 1U)];

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

        ++self->m_receiveReadPos;

        if(self->RX.size != 0) {
#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
    M_Assert_WarningSaveCheck((self->RX.size < (RSCODE_NPAR + 1U)), M_EMPTY, {
                                  self->RX.size = 0U;
                                  return &self->RX;
                              }, "RawParser_dma_proceed: not compleate len with ECC RS-code, len need: %d, receive: %d", (RSCODE_NPAR + 1U), self->RX.size);


    /* Now decode -- encoded codeword size must be passed */
    rscode_decode(&self->rs_ecc, self->RX.data, self->RX.size);
    self->RX.size -= RSCODE_NPAR;

#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */
            return &self->RX;
        }
    }

    return &self->RX;
}

// slow shield functions (slow & more copy)-----------------------------------------------------------------------------------------
RawParser_Frame_t* RawParser_dma_shieldFrame(RawParser_dma_t* const self, u8* data, reg len)
{
    M_Assert_Break((self == (RawParser_dma_t*)NULL), M_EMPTY, return (RawParser_Frame_t*)NULL, "RawParser_dma_shieldFrame: No valid input");
    M_Assert_Break((data == (u8*)NULL) || len == 0, M_EMPTY, {
                       self->TX.size = 0;
                       return &self->TX;
                   }, "RawParser_dma_shieldFrame: No valid input");


#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
    RawParser_dma_startTransmittPacket(self, (len + RSCODE_NPAR));
    rs_encode_data_onlyParity(&self->rs_ecc, data, len);
#else
    RawParser_dma_startTransmittPacket(self, len);

#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */

    while(len--) {
        RawParser_dma_addTxByteCRC(self, *data++);
    }

#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
    for (int i = 0; i < RSCODE_NPAR; ++i) {
        RawParser_dma_addTxByteCRC(self, self->rs_ecc.pBytes[RSCODE_NPAR-1-i]);
    }
#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */

    return RawParser_dma_finishTransmittPacket(self);
}

// fast shield functions (no copy)-----------------------------------------------------------------------------------------
void RawParser_dma_startTransmittPacket(RawParser_dma_t* const self, reg predictedLen) /////////////////////////////////////////////////////
{
    M_Assert_Break((self == (RawParser_dma_t*)NULL), M_EMPTY, return, "RawParser_dma_startTransmittPacket: No valid input");
    M_Assert_Break((self->RX.data == NULL || self->TX.data == NULL), M_EMPTY, return, "RawParser_dma_startTransmittPacket: No valid RX or/and TX buffer , call function before: -->  rawParser_dma_setUserBufferXX, XX = RX for rx buffer, XX = TX for tx buffer, XX = s for tx & rx buffers");
    M_Assert_Break((predictedLen == 0), M_EMPTY, return, "RawParser_dma_startTransmittPacket: No valid input length");

#if D_RAW_P_MAX_PROTOCOL_LEN < D_RAW_P_TX_BUF_SIZE // control undefined behavior
    M_Assert_Break( ((predictedLen + 4) > (D_RAW_P_MAX_PROTOCOL_LEN - 1)), M_EMPTY, return, "RawParser_dma_startTransmittPacket: No valid input length: %d, maxTxLen: %d", predictedLen, D_RAW_P_MAX_PROTOCOL_LEN);
    M_Assert_Warning((predictedLen > (D_RAW_P_MAX_PROTOCOL_LEN >> 1)), M_EMPTY, M_EMPTY, "RawParser_dma_startTransmittPacket: UNDEFINED BEHAVIOR!!! recomended packet length must be less than half - maximun protocol size !!!, Txlen: %d, recomendedLen: %d", predictedLen, (D_RAW_P_MAX_PROTOCOL_LEN >> 1));
#else
    M_Assert_Break( (predictedLen + 4) > (D_RAW_P_TX_BUF_SIZE - 1), M_EMPTY, return, "RawParser_dma_startTransmittPacket: No valid input length: %d, buffer: %d", predictedLen, D_RAW_P_TX_BUF_SIZE);
    M_Assert_Warning((predictedLen > (D_RAW_P_TX_BUF_SIZE >> 1)), M_EMPTY, M_EMPTY, "RawParser_dma_startTransmittPacket: UNDEFINED BEHAVIOR!!! recomended packet length must be less than half - buffer size !!!, Txlen: %d, recomendedLen: %d", predictedLen, (D_RAW_P_TX_BUF_SIZE >> 1));
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */


#ifdef D_RAW_P_CRC_ENA
    self->m_transmittCalcCRC = D_RAW_P_CRC_INIT;
    D_RAW_P_CRC_START(self->m_transmittCalcCRC);
#endif /* D_RAW_P_CRC_ENA */

    self->m_transmittPos = 0;
    self->TX.data[self->m_transmittPos++] = self->m_startByte;

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    if(predictedLen > D_RAW_P_LEN_SEPARATOR) {
        predictedLen = LittleEndianU16(predictedLen);

        RawParser_dma_addTxByteCRC(self, RECEIVE_EXTENDED_LEN_CMD);
        RawParser_dma_addTxByteCRC(self, (u8)(predictedLen & 0x000000FFUL));
        RawParser_dma_addTxByteCRC(self, (u8)((predictedLen >> 8U) & 0x000000FFUL));

    } else {
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

        RawParser_dma_addTxByteCRC(self, (u8)(((predictedLen >= self->m_startByte) ? (predictedLen + 1U) : predictedLen) & 0x000000FFUL));

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    }
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */
}


RawParser_Frame_t* RawParser_dma_finishTransmittPacket(RawParser_dma_t* const self)
{
    M_Assert_Break((self == (RawParser_dma_t*)NULL), M_EMPTY, return (RawParser_Frame_t*)NULL, "RawParser_dma_startTransmittPacket: No valid input");


#ifdef D_RAW_P_CRC_ENA
    D_RAW_P_CRC_FINAL(self->m_transmittCalcCRC);

    #ifdef D_RAW_P_USE_CRC8
        RawParser_dma_addTxByte(self, self->m_transmittCalcCRC);
    #elif defined(D_RAW_P_USE_CRC16)
        self->m_transmittCalcCRC = LittleEndianU16(self->m_transmittCalcCRC);

        RawParser_dma_addTxByte(self, (u8)((self->m_transmittCalcCRC      ) & 0x000000FFUL));
        RawParser_dma_addTxByte(self, (u8)((self->m_transmittCalcCRC >> 8U) & 0x000000FFUL));
    #elif defined(D_RAW_P_USE_CRC32)
        self->m_transmittCalcCRC = LittleEndianU32(self->m_transmittCalcCRC);

        RawParser_dma_addTxByte(self, (u8)((self->m_transmittCalcCRC       ) & 0x000000FFUL));
        RawParser_dma_addTxByte(self, (u8)((self->m_transmittCalcCRC >> 8U ) & 0x000000FFUL));
        RawParser_dma_addTxByte(self, (u8)((self->m_transmittCalcCRC >> 16U) & 0x000000FFUL));
        RawParser_dma_addTxByte(self, (u8)((self->m_transmittCalcCRC >> 24U) & 0x000000FFUL));

    #elif defined(D_RAW_P_USE_CRC64)
        self->m_transmittCalcCRC = LittleEndianU64(self->m_transmittCalcCRC);

        RawParser_dma_addTxByte(self, (u8)((self->m_transmittCalcCRC       ) & 0x00000000000000FFULL));
        RawParser_dma_addTxByte(self, (u8)((self->m_transmittCalcCRC >> 8U ) & 0x00000000000000FFULL));
        RawParser_dma_addTxByte(self, (u8)((self->m_transmittCalcCRC >> 16U) & 0x00000000000000FFULL));
        RawParser_dma_addTxByte(self, (u8)((self->m_transmittCalcCRC >> 24U) & 0x00000000000000FFULL));
        RawParser_dma_addTxByte(self, (u8)((self->m_transmittCalcCRC >> 32U) & 0x00000000000000FFULL));
        RawParser_dma_addTxByte(self, (u8)((self->m_transmittCalcCRC >> 40U) & 0x00000000000000FFULL));
        RawParser_dma_addTxByte(self, (u8)((self->m_transmittCalcCRC >> 48U) & 0x00000000000000FFULL));
        RawParser_dma_addTxByte(self, (u8)((self->m_transmittCalcCRC >> 56U) & 0x00000000000000FFULL));
    #endif /* CRC SWITCH LOGIC */

#endif /* D_RAW_P_CRC_ENA */

    self->TX.size = (rawP_size_t)self->m_transmittPos;
    return &self->TX;
}


// function for use universal macro ---------------------------------------------------------------------------------------------------------
void RawParser_dma_universalWrite(RawParser_dma_t* const self, reg totalLenInByte, reg typelenInByte, u8 *data)
{
    M_Assert_Break((self == (RawParser_dma_t*)NULL || data == NULL), M_EMPTY, return, "RawParser_dma_universalWrite: No valid input");

#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN

#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
    for(reg i = 0; i < totalLenInByte; ++i) {
        RawParser_dma_addTxByteCRC(self, data[i]);
        rs_encode_data_continious_proceed(&self->rs_ecc, data[i]);
    }
#else
    while(totalLenInByte--) {
        RawParser_dma_addTxByteCRC(self, *data++);
    }
#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */

    (void)typelenInByte;
#else /* MY_ENDIAN_ORDER == MY_BIG_ENDIAN */
    for(reg i = 0; i < totalLenInByte; i += typelenInByte) {
        reg n = typelenInByte;
        while(n--) {
            RawParser_dma_addTxByteCRC(self, *(data + n + i));

#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
            rs_encode_data_continious_proceed(&self->rs_ecc, *(data + n + i));
#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */

        }
    }
#endif /* MY_ENDIAN_ORDER == MY_BIG_ENDIAN */

}

void RawParser_dma_universalRead(RawParser_dma_t* const self, reg totalLenInByte, reg typelenInByte, u8 *data)
{
    M_Assert_Break((self == (RawParser_dma_t*)NULL || data == NULL), M_EMPTY, return, "RawParser_dma_universalRead: No valid input");
    M_Assert_Break((self->RX.data == NULL || self->TX.data == NULL), M_EMPTY, return, "RawParser_dma_universalRead: No valid RX or/and TX buffer , call function before: -->  rawParser_dma_setUserBufferXX, XX = RX for rx buffer, XX = TX for tx buffer, XX = s for tx & rx buffers");
    M_Assert_BreakSaveCheck(((self->uniRXPosition + totalLenInByte) > self->RX.size), M_EMPTY, return, "RawParser_dma_universalRead: no length for reading");

#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    while(totalLenInByte--) {
        *data++ = self->RX.data[self->uniRXPosition];
        ++self->uniRXPosition;
    }

    (void)typelenInByte;
#else /* MY_ENDIAN_ORDER == MY_BIG_ENDIAN */
    for(reg i = 0; i < totalLenInByte; i+= typelenInByte) {
        reg n = typelenInByte;
        while(n--) {
            *(data + n + i) = self->RX.data[self->uniRXPosition];
            ++self->uniRXPosition;
        }
    }
#endif /* MY_ENDIAN_ORDER == MY_BIG_ENDIAN */

}



// FSM data fusion delete -----------------------------
#undef RAW_P_DMA_RECEIVE_LEN_0

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    #undef RAW_P_DMA_RECEIVE_LEN_LOW
    #undef RAW_P_DMA_RECEIVE_LEN_HIGH
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

#undef RAW_P_DMA_RECEIVE_DATA

#ifdef D_RAW_P_CRC_ENA
    #undef RAW_P_DMA_RECEIVE_CRC_0

    #if defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
        #undef RAW_P_DMA_RECEIVE_CRC_1
    #endif /* defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) */

    #if defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
        #undef RAW_P_DMA_RECEIVE_CRC_2
        #undef RAW_P_DMA_RECEIVE_CRC_3
    #endif /* defined(D_RAW_P_USE_CRC32) */

    #if defined(D_RAW_P_USE_CRC64)
        #undef RAW_P_DMA_RECEIVE_CRC_4
        #undef RAW_P_DMA_RECEIVE_CRC_5
        #undef RAW_P_DMA_RECEIVE_CRC_6
        #undef RAW_P_DMA_RECEIVE_CRC_7
    #endif /* defined(D_RAW_P_USE_CRC64) */
#else
    #undef RawParser_dma_addTxByteCRC
#endif /* D_RAW_P_CRC_ENA */

#undef RAW_P_DMA_RECEIVE_ERR
#undef RAW_P_DMA_RECEIVE_OK
//----------------------------------------------
