#ifndef __RAW_PARSER_IT_H__
#define __RAW_PARSER_IT_H__ 1

#include "rawparser_port.h"

/**
  * @brief RawParser_it_DescriptorTypeDef structure definition
  */
typedef struct {
    // packet form -----------------------------------------
    u8           m_startByte;                                   // Specifies the value of start-byte.
    rawP_size_t  m_receivePackLen;
    u8           m_receiveFrameBuffer[D_RAW_P_RX_BUF_SIZE];     // frame buffer to proceed into user logic`s
    volatile u8  m_rx_pack_ready;
    u8           m_sendBuffer[D_RAW_P_TX_BUF_SIZE];             // array for save tx buffer

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

    u32             m_transmittPos;           // transmitt raw position

    u8 receiveState;
    u8 transmittState;

    RawParser_Frame_t TX;
    RawParser_Frame_t RX;
    u32 uniRXPosition; // value for universal read position function


#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
    rscode_driver rs_ecc;
#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */

} RawParser_it_t;



RawParser_it_t* rawParser_it_new(const u8 packStart);
void rawParser_it_init(RawParser_it_t * const self, const u8 packStart);
int rawParser_it_delete(RawParser_it_t** data);


void RawParser_it_proceed(RawParser_it_t* const self, const u8 ch);


#endif /* RAW_PARSER_IT_H */
