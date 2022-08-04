#ifndef __RAW_PARSER_DMA_FUSION_H__
#define __RAW_PARSER_DMA_FUSION_H__

#include "rawparser_port.h"
#include "smart_assert.h"

/**
  * @brief RawParser_dma_DescriptorTypeDef structure definition
  */
typedef struct {
    // packet form -----------------------------------------
    u8           m_startByte;          // Specifies the value of start-byte.
    rawP_size_t  m_receivePackLen;
    u8           m_receiveBuffer[D_RAW_P_RX_BUF_SIZE];        // received raw byte array
    u8           m_receiveFrameBuffer[D_RAW_P_RX_BUF_SIZE];   // frame buffer to proceed into user logic`s

    u8           m_sendBuffer[D_RAW_P_TX_BUF_SIZE];    // array for save tx buffer

    // if crc enable -> crc data variable
#ifdef D_RAW_P_CRC_ENA
    rawP_crc_t   m_receiveCalcCRC;            // rx crc calc data
    rawP_crc_t   m_transmittCalcCRC;          // tx crc calc data

    #if defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
        rawP_crc_t   m_receiveCRCBuf;            // buffer for rx crc
    #endif /* defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) */

#endif /* D_RAW_P_CRC_ENA */
    // ----------------------------------------------------

    u8              m_triggerSB;            // trigger for read start byte
    u32             m_receivePos;           // receive raw position
    u32             m_receiveReadPos;       // receive read position
    rawP_size_t     m_receiveHandlePos;     // receive handler position

    u32             m_transmittPos;           // transmitt raw position

    u8 receiveState;

    RawParser_Frame_t TX;
    RawParser_Frame_t RX;
} RawParser_dma_t;

// new / delete functions----------------------------------------------
RawParser_dma_t* rawParser_dma_new(const u8 packStart);
int rawParser_dma_delete(RawParser_dma_t** data);


// receive functions-----------------------------------------------------------------------------------------
forceinline void RawParser_dma_receiveByte(RawParser_dma_t* const self, const u8 byte)
{
    M_Assert_Break((self == NULL), M_EMPTY, return, "RawParser_dma_receiveByte: No valid input");
    self->m_receiveBuffer[(self->m_receivePos++) & (D_RAW_P_RX_BUF_SIZE - 1U)] = byte;
}

forceinline void RawParser_dma_receiveArray(RawParser_dma_t* const self, u8* arr, rawP_size_t len)
{
    M_Assert_Break((self == NULL || arr == NULL), M_EMPTY, return, "RawParser_dma_receiveArray: No valid input");
    M_Assert_Break( ((u32)(len) + 1) > (D_RAW_P_RX_BUF_SIZE + 1), M_EMPTY, return, "RawParser_dma_receiveArray: No valid input length, len: %d, max_len: %d", len, (D_RAW_P_RX_BUF_SIZE - 1));

    while(len--) {
        self->m_receiveBuffer[(self->m_receivePos++) & (D_RAW_P_RX_BUF_SIZE - 1U)] = *arr++;
    }
}


RawParser_Frame_t* RawParser_dma_proceed(RawParser_dma_t* const self);

// slow shield functions (slow & more copy)-----------------------------------------------------------------------------------------
RawParser_Frame_t* RawParser_dma_shieldFrame(RawParser_dma_t* const self, u8* data, rawP_size_t len); // shield data array before transmitting
// fast shield functions (no copy)-----------------------------------------------------------------------------------------
void RawParser_dma_startTransmittPacket(RawParser_dma_t* const self, rawP_size_t predictedLen);
RawParser_Frame_t* RawParser_dma_finishTransmittPacket(RawParser_dma_t* const self);


// elementary byte adding functions ----------------------------------------------------------------------------
void RawParser_dma_addTxByte(RawParser_dma_t* const self, const u8 byte);
#ifdef D_RAW_P_CRC_ENA
void RawParser_dma_addTxByteCRC(RawParser_dma_t* const self, const u8 byte);
#else
    #define RawParser_dma_addTxByteCRC(self, byte) RawParser_dma_addTxByte((self), (byte))
#endif /* D_RAW_P_CRC_ENA */




#endif /* __RAW_PARSER_DMA_FUSION_H__ */
