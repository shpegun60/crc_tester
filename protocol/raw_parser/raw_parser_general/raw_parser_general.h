#include "smart_assert.h"
#ifndef __RAW_PARSER_GENERAL_H__
#define __RAW_PARSER_GENERAL_H__ 1

#include "rawparser_port.h"
#include "preprocessor_ctx.h"
/**
  * @brief RawParser_general_DescriptorTypeDef structure definition
  */
typedef struct {
    const u8 m_startByte;
    u8* const RX_buf;
    const reg RX_buf_size;

    void (* const recv_message)(u8* data, reg size, PREPROCESSOR_CTX_TYPE(ctx));
    u8 (* const write_byte)(const u8 byte, PREPROCESSOR_CTX_TYPE(ctx));
} RawParser_gen_descr_t;


typedef struct {
    const RawParser_gen_descr_t* descriptor;

    u8              m_triggerSB;            // trigger for read start byte

    // if crc enable -> crc data variable
#ifdef D_RAW_P_CRC_ENA
    rawP_crc_t      m_receiveCalcCRC;

#   if defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
    rawP_crc_t   m_receiveCRCBuf;            // buffer for rx crc
#   endif /* defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) */

#endif /* D_RAW_P_CRC_ENA */

    reg             RXpos;
    u8              receiveState;

    reg m_receivePackLen;

#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
    rscode_driver rs_ecc;
#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */
} RawParser_gen_t;

RawParser_gen_t* RawParser_gen_new(const RawParser_gen_descr_t* const descriptor);
int RawParser_gen_init(RawParser_gen_t* const self, const RawParser_gen_descr_t* const descriptor);
int RawParser_gen_delete(RawParser_gen_t** self);

void RawParser_gen_read_byte(RawParser_gen_t* const self, const u8 byte, PREPROCESSOR_CTX_TYPE(ctx));
void RawParser_gen_send_message(RawParser_gen_t* const self, const u8 * const data, reg size, PREPROCESSOR_CTX_TYPE(ctx));


#endif /* __RAW_PARSER_GENERAL_H__ */
