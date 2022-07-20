#ifndef __RAW_PARSER_FUSION_H__
#define __RAW_PARSER_FUSION_H__

#include "rawparser_dma_port.h"

///**
//  * @brief RawParser Frame structure definition
//  */
//typedef struct {
//    rawP_data_t *data;
//    rawP_size_t size;
//} RawParser_Frame_t;

///**
//  * @brief RawParser_DescriptorTypeDef structure definition
//  */
//typedef struct {
//  // packet form -----------------------------------------
//    rawP_start_t      m_startByte;          // Specifies the value of start-byte.
//    rawP_size_t       m_receivePackLen;
//    rawP_data_t       m_recBuffer[D_RAW_P_TX_BUF_SIZE];
    
//    rawP_data_t       m_frameBuffer[D_RAW_P_RX_BUF_SIZE];
//    rawP_data_t       m_sendBuffer[D_RAW_P_RX_BUF_SIZE];
//  // ----------------------------------------------------

//    #ifdef D_RAW_P_CRC_ENA
//        rawP_crc_t   m_frameCrc;            // crc data
//    #endif /* D_RAW_P_CRC_ENA */
    
//    unsigned char   m_triggerSB;            // trigger for read start byte
//    unsigned int    m_receivePos;           // receive position
//    unsigned int    m_receiveReadPos;       // receive read position
//    rawP_size_t     m_receiveHandlePos;     // receive handler position

//    unsigned char receiveState;

//    RawParser_Frame_t TX;
//    RawParser_Frame_t RX;
//} RawParser_t;

//RawParser_t* rawParser_new(RawParser_Init_t *init_desc);
//int rawParser_delete(RawParser_t*);


#endif /* __RAW_PARSER_H__ */
