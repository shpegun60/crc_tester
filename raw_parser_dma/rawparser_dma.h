#ifndef __RAW_PARSER_DMA_FUSION_H__
#define __RAW_PARSER_DMA_FUSION_H__

#include "rawparser_port.h"
#include "smart_assert.h"

/**
  * @brief RawParser_DescriptorTypeDef structure definition
  */
typedef struct {
    // packet form -----------------------------------------
    rawP_start_t      m_startByte;          // Specifies the value of start-byte.
    rawP_size_t       m_receivePackLen;
    rawP_data_t       m_receiveBuffer[D_RAW_P_RX_BUF_SIZE];        // received raw byte array
    rawP_data_t       m_receiveFrameBuffer[D_RAW_P_RX_BUF_SIZE];   // frame buffer to proceed into user logic`s

    rawP_data_t       m_sendBuffer[D_RAW_P_TX_BUF_SIZE];    // array for save tx buffer

    // if crc enable -> crc data variable
#ifdef D_RAW_P_CRC_ENA
    rawP_crc_t   m_receiveCalcCRC;            // rx crc calc data
    rawP_crc_t   m_transmittCalcCRC;          // tx crc calc data

    #if defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32)
        rawP_crc_t   m_receiveCRC;            // rx crc
    #endif /* defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) */

#endif /* D_RAW_P_CRC_ENA */
    // ----------------------------------------------------

    uint8_t         m_triggerSB;            // trigger for read start byte
    uint32_t        m_receivePos;           // receive raw position
    uint32_t        m_receiveReadPos;       // receive read position
    rawP_size_t     m_receiveHandlePos;     // receive handler position

    uint32_t        m_transmittPos;           // transmitt raw position

    uint8_t receiveState;

    RawParser_Frame_t TX;
    RawParser_Frame_t RX;
} RawParser_dma_t;

RawParser_dma_t* rawParser_dma_new(rawP_start_t packStart);
int rawParser_dma_delete(RawParser_dma_t** data);

extern inline void RawParser_dma_receiveByte(RawParser_dma_t *self, rawP_data_t byte);
extern inline void RawParser_dma_receiveArray(RawParser_dma_t *self, rawP_data_t *arr, rawP_size_t len);

RawParser_Frame_t* RawParser_dma_proceed(RawParser_dma_t * const self);

extern inline void RawParser_dma_addTxByte(RawParser_dma_t * const self, rawP_data_t byte);
#ifdef D_RAW_P_CRC_ENA
    extern inline void RawParser_dma_addTxByteCRC(RawParser_dma_t * const self, const rawP_data_t byte);
#endif /* D_RAW_P_CRC_ENA */


RawParser_Frame_t* RawParser_dma_shieldFrame(RawParser_dma_t * const self, rawP_data_t * data, rawP_size_t len);




#endif /* __RAW_PARSER_DMA_FUSION_H__ */
