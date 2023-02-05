#ifndef __RAW_PARSER_IT_H__
#define __RAW_PARSER_IT_H__ 1

#include "rawparser_port.h"

/**
  * @brief RawParser_it_DescriptorTypeDef structure definition
  */
typedef struct {
    // packet form -----------------------------------------
    u8           m_startByte;                                   // Specifies the value of start-byte.
    reg          m_receivePackLen;

#ifndef D_RAW_P_DISABLE_INTERNAL_RX_BUFFER
    u8           m_receiveFrameBuffer[D_RAW_P_RX_BUF_SIZE];   // frame buffer to proceed into user logic`s
#endif /* D_RAW_P_DISABLE_INTERNAL_RX_BUFFER */

#ifndef D_RAW_P_DISABLE_INTERNAL_TX_BUFFER
    u8           m_sendBuffer[D_RAW_P_TX_BUF_SIZE];    // array for save tx buffer
#endif /* D_RAW_P_DISABLE_INTERNAL_TX_BUFFER */

    // if crc enable -> crc data variable
#ifdef D_RAW_P_CRC_ENA
    rawP_crc_t   m_transmittCalcCRC;          // tx crc calc data
#endif /* D_RAW_P_CRC_ENA */
    // ----------------------------------------------------

    u8              m_triggerSB;            // trigger for read start byte
    reg             m_receivePos;           // receive raw position

    reg             m_transmittPos;           // transmitt raw position

    u8  receiveState;
    u8  transmittState;
    u8  transmissionRepeat;
    reg m_transmittPackLen;

    RawParser_Frame_t TX;
    RawParser_Frame_t RX;

#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
    rscode_driver rs_ecc;
#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */

} RawParser_it_t;


RawParser_it_t* rawParser_it_new(const u8 packStart);
int rawParser_it_init(RawParser_it_t * const self, const u8 packStart);

// set external buffers
#ifdef D_RAW_P_DISABLE_INTERNAL_TX_BUFFER
void rawParser_it_setUserBufferTX(RawParser_it_t * const self, u8 * const txBuffer);
#endif /* D_RAW_P_DISABLE_INTERNAL_TX_BUFFER */

#ifdef D_RAW_P_DISABLE_INTERNAL_RX_BUFFER
void rawParser_it_setUserBufferRX(RawParser_it_t * const self, u8 * const rxBuffer);
#endif /* D_RAW_P_DISABLE_INTERNAL_RX_BUFFER */

#if defined(D_RAW_P_DISABLE_INTERNAL_TX_BUFFER) && defined(D_RAW_P_DISABLE_INTERNAL_RX_BUFFER)
void rawParser_it_setUserBuffers(RawParser_it_t * const self, u8 * const rxBuffer, u8 * const txBuffer);
#endif /* defined(D_RAW_P_DISABLE_INTERNAL_TX_BUFFER) || defined(D_RAW_P_DISABLE_INTERNAL_RX_BUFFER) */


int rawParser_it_delete(RawParser_it_t** data);

//------------------------------RX------------------------------------------------------------------------------------------
RawParser_Frame_t* RawParser_it_RXproceedIt(RawParser_it_t* const self, const u8 ch);
RawParser_Frame_t* RawParser_it_RXproceedLoop(RawParser_it_t* const self);

// -------------------------------TX----------------------------------------------------------------
int RawParser_it_TXpush(RawParser_it_t* const self, reg len);
int RawParser_it_TXproceedIt(RawParser_it_t* const self, u8 * const ch);


#endif /* RAW_PARSER_IT_H */
