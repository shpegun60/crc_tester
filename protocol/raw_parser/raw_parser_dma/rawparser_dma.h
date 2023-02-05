#ifndef __RAW_PARSER_DMA_FUSION_H__
#define __RAW_PARSER_DMA_FUSION_H__ 1

#include "rawparser_port.h"
#include "smart_assert.h"

/**
  * @brief RawParser_dma_DescriptorTypeDef structure definition
  */
typedef struct {
    // packet form -----------------------------------------
    u8           m_startByte;          // Specifies the value of start-byte.
    reg          m_receivePackLen;
    u8           m_receiveBuffer[D_RAW_P_RX_BUF_SIZE];        // received raw byte array

#ifndef D_RAW_P_DISABLE_INTERNAL_RX_BUFFER
    u8           m_receiveFrameBuffer[D_RAW_P_RX_BUF_SIZE];   // frame buffer to proceed into user logic`s
#endif /* D_RAW_P_DISABLE_INTERNAL_RX_BUFFER */

#ifndef D_RAW_P_DISABLE_INTERNAL_TX_BUFFER
    u8           m_sendBuffer[D_RAW_P_TX_BUF_SIZE];    // array for save tx buffer
#endif /* D_RAW_P_DISABLE_INTERNAL_TX_BUFFER */

    // if crc enable -> crc data variable
#ifdef D_RAW_P_CRC_ENA
    rawP_crc_t   m_receiveCalcCRC;            // rx crc calc data
    rawP_crc_t   m_transmittCalcCRC;          // tx crc calc data

#   if defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
    rawP_crc_t   m_receiveCRCBuf;            // buffer for rx crc
#   endif /* defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) */

#endif /* D_RAW_P_CRC_ENA */
    // ----------------------------------------------------

    u8              m_triggerSB;            // trigger for read start byte
    reg             m_receivePos;           // receive raw position
    reg             m_receiveReadPos;       // receive read position
    reg             m_receiveHandlePos;     // receive handler position

    u8 receiveState;

    RawParser_Frame_t TX;
    RawParser_Frame_t RX;
    reg uniRXPosition; // value for universal read position function


#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
    rscode_driver rs_ecc;
#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */

} RawParser_dma_t;

/* CREATE/DELETE FUNCTIONS *************************************************************************************************
 *
 */
RawParser_dma_t* rawParser_dma_new(const u8 packStart);
int rawParser_dma_init(RawParser_dma_t * const self, const u8 packStart);
int rawParser_dma_delete(RawParser_dma_t** data);

// set user buffers
#ifdef D_RAW_P_DISABLE_INTERNAL_TX_BUFFER
void rawParser_dma_setUserBufferTX(RawParser_dma_t * const self, u8 * const txBuffer);
#endif /* D_RAW_P_DISABLE_INTERNAL_TX_BUFFER */

#ifdef D_RAW_P_DISABLE_INTERNAL_RX_BUFFER
void rawParser_dma_setUserBufferRX(RawParser_dma_t * const self, u8 * const rxBuffer);
#endif /* D_RAW_P_DISABLE_INTERNAL_RX_BUFFER */

#if defined(D_RAW_P_DISABLE_INTERNAL_TX_BUFFER) && defined(D_RAW_P_DISABLE_INTERNAL_RX_BUFFER)
void rawParser_dma_setUserBuffers(RawParser_dma_t * const self, u8 * const rxBuffer, u8 * const txBuffer);
#endif /* defined(D_RAW_P_DISABLE_INTERNAL_TX_BUFFER) || defined(D_RAW_P_DISABLE_INTERNAL_RX_BUFFER) */


/* PROCEED FUNCTIONS *************************************************************************************************
 *
 */

// receive functions-----------------------------------------------------------------------------------------
STATIC_FORCEINLINE void RawParser_dma_receiveByte(RawParser_dma_t* const self, const u8 byte)
{
    M_Assert_Break((self == NULL), M_EMPTY, return, "RawParser_dma_receiveByte: No valid input");
    u8* const m_receiveBuffer   = self->m_receiveBuffer;
    reg m_receivePos            = self->m_receivePos;

    m_receiveBuffer[m_receivePos & (D_RAW_P_RX_BUF_SIZE - 1U)] = byte;
    ++m_receivePos;

    self->m_receivePos = m_receivePos;
}

STATIC_FORCEINLINE void RawParser_dma_receiveArray(RawParser_dma_t* const self, u8* arr, reg len)
{
    M_Assert_Break((self == NULL || arr == NULL), M_EMPTY, return, "RawParser_dma_receiveArray: No valid input");
    M_Assert_Break( (len > D_RAW_P_RX_BUF_SIZE), M_EMPTY, return, "RawParser_dma_receiveArray: No valid input length, len: %d, max_len: %d", len, D_RAW_P_RX_BUF_SIZE);

    u8* const m_receiveBuffer   = self->m_receiveBuffer;
    reg m_receivePos            = self->m_receivePos;

    while(len--) {
        m_receiveBuffer[m_receivePos & (D_RAW_P_RX_BUF_SIZE - 1U)] = *arr++;
        ++m_receivePos;
    }

    self->m_receivePos = m_receivePos;
}


RawParser_Frame_t* RawParser_dma_proceed(RawParser_dma_t* const self);

// slow shield functions (slow & more copy)-----------------------------------------------------------------------------------------
RawParser_Frame_t* RawParser_dma_shieldFrame(RawParser_dma_t* const self, const u8* data, reg len); // shield data array before transmitting
// fast shield functions (no copy)-----------------------------------------------------------------------------------------
void RawParser_dma_startTransmittPacket(RawParser_dma_t* const self, reg predictedLen);
RawParser_Frame_t* RawParser_dma_finishTransmittPacket(RawParser_dma_t* const self);


// elementary byte adding functions ----------------------------------------------------------------------------
STATIC_FORCEINLINE void RawParser_dma_addTxByte(RawParser_dma_t* const self, const u8 byte)
{
    M_Assert_Break((self == (RawParser_dma_t*)NULL), M_EMPTY, return, "RawParser_dma_addTxByte: No valid input");

#ifdef D_RAW_P_DISABLE_INTERNAL_RX_BUFFER
    M_Assert_Break((self->RX.data == NULL), M_EMPTY, return, "RawParser_dma_addTxByte: No valid RX buffer, call function before: -->  rawParser_dma_setUserBufferXX, XX = RX for rx buffer, XX = TX for tx buffer, XX = s for tx & rx buffers");
#endif /* D_RAW_P_DISABLE_INTERNAL_RX_BUFFER */

#ifdef D_RAW_P_DISABLE_INTERNAL_TX_BUFFER
    M_Assert_Break((self->TX.data == NULL), M_EMPTY, return, "RawParser_dma_addTxByte: No valid TX buffer, call function before: -->  rawParser_dma_setUserBufferXX, XX = RX for rx buffer, XX = TX for tx buffer, XX = s for tx & rx buffers");
#endif /* D_RAW_P_DISABLE_INTERNAL_TX_BUFFER */

    // move to cash
    u8* const   TX_data   = self->TX.data;
    reg         TX_size   = self->TX.size;


#if D_RAW_P_MAX_PROTOCOL_LEN < D_RAW_P_TX_BUF_SIZE
    M_Assert_Break((TX_size == (D_RAW_P_MAX_PROTOCOL_LEN - 1)), M_EMPTY, return, "RawParser_dma_addTxByte: LEN packet: %d,  more than more than protocol maximum len: %d", TX_size + 1, D_RAW_P_MAX_PROTOCOL_LEN - 1);
#else
    M_Assert_Break((TX_size == (D_RAW_P_TX_BUF_SIZE - 1)), M_EMPTY, return, "RawParser_dma_addTxByte: LEN packet: %d,  more than buffer size: %d", TX_size + 1, D_RAW_P_TX_BUF_SIZE - 1);
#endif /* D_RAW_P_MAX_PROTOCOL_LEN < D_RAW_P_TX_BUF_SIZE */

    TX_data[TX_size++] = byte;

    if(byte == self->m_startByte) {
#if D_RAW_P_MAX_PROTOCOL_LEN < D_RAW_P_TX_BUF_SIZE
        M_Assert_Break((TX_size == (D_RAW_P_MAX_PROTOCOL_LEN - 1)), M_EMPTY, return, "RawParser_dma_addTxByte: LEN packet: %d,  more than more than protocol maximum len: %d", TX_size + 1, D_RAW_P_MAX_PROTOCOL_LEN - 1);
#else
        M_Assert_Break((TX_size == (D_RAW_P_TX_BUF_SIZE - 1)), M_EMPTY, return, "RawParser_dma_addTxByte: LEN packet: %d,  more than buffer size: %d", TX_size + 1, D_RAW_P_TX_BUF_SIZE - 1);
#endif /* D_RAW_P_MAX_PROTOCOL_LEN < D_RAW_P_TX_BUF_SIZE */

        TX_data[TX_size++] = byte;
    }

    self->TX.size = TX_size;
}

#ifdef D_RAW_P_CRC_ENA
// elementary byte adding functions with calc crc----------------------------------------------------------------------------
STATIC_FORCEINLINE void RawParser_dma_addTxByteCRC(RawParser_dma_t* const self, const u8 byte)
{
    M_Assert_Break((self == (RawParser_dma_t*)NULL), M_EMPTY, return, "RawParser_dma_addTxByteCRC: No valid input");

#   ifdef D_RAW_P_DISABLE_INTERNAL_RX_BUFFER
    M_Assert_Break((self->RX.data == NULL), M_EMPTY, return, "RawParser_dma_addTxByte: No valid RX buffer, call function before: -->  rawParser_dma_setUserBufferXX, XX = RX for rx buffer, XX = TX for tx buffer, XX = s for tx & rx buffers");
#   endif /* D_RAW_P_DISABLE_INTERNAL_RX_BUFFER */

#   ifdef D_RAW_P_DISABLE_INTERNAL_TX_BUFFER
    M_Assert_Break((self->TX.data == NULL), M_EMPTY, return, "RawParser_dma_addTxByte: No valid TX buffer, call function before: -->  rawParser_dma_setUserBufferXX, XX = RX for rx buffer, XX = TX for tx buffer, XX = s for tx & rx buffers");
#   endif /* D_RAW_P_DISABLE_INTERNAL_TX_BUFFER */

    // move to cash
    u8* const   TX_data   = self->TX.data;
    reg         TX_size   = self->TX.size;


#   if D_RAW_P_MAX_PROTOCOL_LEN < D_RAW_P_TX_BUF_SIZE
    M_Assert_Break((TX_size == (D_RAW_P_MAX_PROTOCOL_LEN - 1)), M_EMPTY, return, "RawParser_dma_addTxByteCRC: LEN packet: %d,  more than protocol maximum len: %d", TX_size + 1, D_RAW_P_MAX_PROTOCOL_LEN - 1);
#   else
    M_Assert_Break((TX_size == (D_RAW_P_TX_BUF_SIZE - 1)), M_EMPTY, return, "RawParser_dma_addTxByteCRC: LEN packet: %d,  more than buffer size: %d", TX_size + 1, D_RAW_P_TX_BUF_SIZE - 1);
#   endif /* D_RAW_P_MAX_PROTOCOL_LEN < D_RAW_P_TX_BUF_SIZE */


    self->m_transmittCalcCRC = D_RAW_P_CRC_UPDATE(self->m_transmittCalcCRC, byte);

    TX_data[TX_size++] = byte;
    if(byte == self->m_startByte) {

#   if D_RAW_P_MAX_PROTOCOL_LEN < D_RAW_P_TX_BUF_SIZE
        M_Assert_Break((TX_size == (D_RAW_P_MAX_PROTOCOL_LEN - 1)), M_EMPTY, return, "RawParser_dma_addTxByteCRC: LEN packet: %d,  more than protocol maximum len: %d", TX_size + 1, D_RAW_P_MAX_PROTOCOL_LEN - 1);
#   else
        M_Assert_Break((TX_size == (D_RAW_P_TX_BUF_SIZE - 1)), M_EMPTY, return, "RawParser_dma_addTxByteCRC: LEN packet: %d,  more than buffer size: %d", TX_size + 1, D_RAW_P_TX_BUF_SIZE - 1);
#   endif /* D_RAW_P_MAX_PROTOCOL_LEN < D_RAW_P_TX_BUF_SIZE */

        TX_data[TX_size++] = byte;
    }

    self->TX.size = TX_size;
}
#else
#   define RawParser_dma_addTxByteCRC(self, byte) RawParser_dma_addTxByte(self, byte)
#endif /* D_RAW_P_CRC_ENA */


// function for use universal macro ---------------------------------------------------------------------------------------------------------
void RawParser_dma_universalWrite(RawParser_dma_t* const self, reg totalLenInByte, const reg typelenInByte, const u8 *data);
void RawParser_dma_universalRead(RawParser_dma_t* const self, reg totalLenInByte, const reg typelenInByte, u8 *data);


#endif /* __RAW_PARSER_DMA_FUSION_H__ */
