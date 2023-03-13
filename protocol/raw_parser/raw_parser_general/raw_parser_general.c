#include "raw_parser_general.h"
#include "smart_assert.h"
#include "my_ctype_cast.h"


#if defined(D_RAW_P_TWO_BYTES_LEN_SUPPORT) || defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
    #include "byte_order.h"
#endif /* defined(D_RAW_P_TWO_BYTES_LEN_SUPPORT) || defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64) */

#include <stdlib.h>


// FSM data fusion -----------------------------
#define RAW_P_GEN_RECEIVE_LEN_0                     0x00U

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
#   define RAW_P_GEN_RECEIVE_LEN_LOW                0x01U
#   define RAW_P_GEN_RECEIVE_LEN_HIGH               0x02U
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

#define RAW_P_GEN_RECEIVE_DATA                      0x03U

#ifdef D_RAW_P_CRC_ENA
#   define RAW_P_GEN_RECEIVE_CRC_0                  0x04U
#
#   if defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
#       define RAW_P_GEN_RECEIVE_CRC_1              0x05U
#   endif /* defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) */
#
#   if defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
#       define RAW_P_GEN_RECEIVE_CRC_2              0x06U
#       define RAW_P_GEN_RECEIVE_CRC_3              0x07U
#   endif /* defined(D_RAW_P_USE_CRC32) */
#
#   if defined(D_RAW_P_USE_CRC64)
#       define RAW_P_GEN_RECEIVE_CRC_4              0x08U
#       define RAW_P_GEN_RECEIVE_CRC_5              0x09U
#       define RAW_P_GEN_RECEIVE_CRC_6              0x0AU
#       define RAW_P_GEN_RECEIVE_CRC_7              0x0BU
#   endif /* defined(D_RAW_P_USE_CRC64) */
#
#endif /* D_RAW_P_CRC_ENA */

#define RAW_P_GEN_RECEIVE_ERR                       0x0CU
#define RAW_P_GEN_RECEIVE_OK                        0x0DU
//----------------------------------------------


RawParser_gen_t* RawParser_gen_new(const RawParser_gen_descr_t* const descriptor)
{
    RawParser_gen_t* self = (RawParser_gen_t *)calloc(1, sizeof(RawParser_gen_t));
    M_Assert_BreakSaveCheck(self == (RawParser_gen_t *)NULL, M_EMPTY, return self, "RawParser_gen_new: No memory for allocation ");

    if(RawParser_gen_init(self, descriptor) == D_RAW_P_ERROR) {
        free(self);
        self = NULL;
    }
    return self;
}

int RawParser_gen_init(RawParser_gen_t* const self, const RawParser_gen_descr_t* const descriptor)
{
    M_Assert_BreakSaveCheck(self == NULL || descriptor == NULL, M_EMPTY, return D_RAW_P_ERROR, "RawParser_gen_init: data is null");

    M_Assert_BreakSaveCheck((descriptor->RX_buf == NULL) || (descriptor->RX_buf_size == 0) || (descriptor->recv_message == NULL) || (descriptor->write_byte == NULL),
                            M_EMPTY, return D_RAW_P_ERROR, "RawParser_gen_init: invalid input");

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    const u8 packStart = descriptor->m_startByte;
    M_Assert_BreakSaveCheck(packStart == RECEIVE_EXTENDED_LEN_CMD, M_EMPTY, return D_RAW_P_ERROR, "RawParser_gen_init: start byte: %d must be not equal RECEIVE_EXTENDED_LEN_CMD: %d", packStart, RECEIVE_EXTENDED_LEN_CMD);
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

    self->descriptor = descriptor;

#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
    rs_initialize_ecc(&self->rs_ecc);
#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */
    return D_RAW_P_OK;
}


int RawParser_gen_delete(RawParser_gen_t** data)
{
    M_Assert_BreakSaveCheck((data == NULL) || (*data == NULL), M_EMPTY, return D_RAW_P_ERROR, "rawParser_dma_delete: No allocated memory");
    free(*data);
    (*data) = NULL;

    return D_RAW_P_OK;
}



/* PROCEED FUNCTIONS *************************************************************************************************
 *
 */

static void RawParser_gen_proceedByte(RawParser_gen_t* const self, const u8 byte, const u8 newFrame, PREPROCESSOR_CTX_TYPE(ctx))
{
    const RawParser_gen_descr_t* const descriptor = self->descriptor;


    if (newFrame) {

#ifdef D_RAW_P_CRC_ENA
        self->m_receiveCalcCRC = D_RAW_P_CRC_INIT;
        D_RAW_P_CRC_START(self->m_receiveCalcCRC);
#endif /* D_RAW_P_CRC_ENA */

        self->RXpos = 0;
        self->receiveState = RAW_P_GEN_RECEIVE_LEN_0;
    }

    switch(self->receiveState) {

    case RAW_P_GEN_RECEIVE_LEN_0: {

#ifdef D_RAW_P_CRC_ENA
        self->m_receiveCalcCRC = D_RAW_P_CRC_UPDATE(self->m_receiveCalcCRC, byte);
#endif /* D_RAW_P_CRC_ENA */

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
        if(RECEIVE_EXTENDED_LEN_CMD == byte) {
            self->receiveState = RAW_P_GEN_RECEIVE_LEN_LOW;
        } else {
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

            const reg rx_len = self->m_receivePackLen = ((byte > descriptor->m_startByte) ? (byte - 1U) : byte);
            self->receiveState = RAW_P_GEN_RECEIVE_DATA;

            M_Assert_WarningSaveCheck((rx_len > descriptor->RX_buf_size || rx_len == 0), M_EMPTY, {
                                          self->receiveState = RAW_P_GEN_RECEIVE_ERR;
                                      }, "RawParser_gen_proceedByte: No valid receive length, rx_len = %d, max_len = %d", rx_len, descriptor->RX_buf_size);

#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
            M_Assert_WarningSaveCheck((rx_len < (RSCODE_NPAR + 1U)), M_EMPTY, {
                                        self->receiveState = RAW_P_GEN_RECEIVE_ERR;
                                      }, "RawParser_gen_proceedByte: not compleate len with ECC RS-code, len need: %d, receive: %d", (RSCODE_NPAR + 1U), rx_len);

#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
        }
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

        break;}

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    case RAW_P_GEN_RECEIVE_LEN_LOW: {

#   ifdef D_RAW_P_CRC_ENA
        self->m_receiveCalcCRC = D_RAW_P_CRC_UPDATE(self->m_receiveCalcCRC, byte);
#   endif /* D_RAW_P_CRC_ENA */

        self->m_receivePackLen = (reg)(byte & 0x000000FFUL);    // read low byte
        self->receiveState = RAW_P_GEN_RECEIVE_LEN_HIGH;
        break;}

    case RAW_P_GEN_RECEIVE_LEN_HIGH: {

#   ifdef D_RAW_P_CRC_ENA
        self->m_receiveCalcCRC = D_RAW_P_CRC_UPDATE(self->m_receiveCalcCRC, byte);
#   endif /* D_RAW_P_CRC_ENA */


        reg rx_len = (reg)((((reg)byte) << 8U) & 0x0000FF00UL) | self->m_receivePackLen; // read high byte
        self->m_receivePackLen = rx_len = LittleEndianU16(rx_len);

        self->receiveState = RAW_P_GEN_RECEIVE_DATA;
        M_Assert_WarningSaveCheck((rx_len > descriptor->RX_buf_size || rx_len == 0), M_EMPTY, {
                                      self->receiveState = RAW_P_GEN_RECEIVE_ERR;
                                  }, "RawParser_gen_proceedByte: No valid receive length, rx_len = %d, max_len = %d", rx_len, descriptor->RX_buf_size);

#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
            M_Assert_WarningSaveCheck((rx_len < (RSCODE_NPAR + 1U)), M_EMPTY, {
                                        self->receiveState = RAW_P_GEN_RECEIVE_ERR;
                                      }, "RawParser_gen_proceedByte: not compleate len with ECC RS-code, len need: %d, receive: %d", (RSCODE_NPAR + 1U), rx_len);

#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */

        break;}
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */


    case RAW_P_GEN_RECEIVE_DATA: {
        reg rx_pos = self->RXpos;

#ifdef D_RAW_P_CRC_ENA
        self->m_receiveCalcCRC = D_RAW_P_CRC_UPDATE(self->m_receiveCalcCRC, byte);
#endif /* D_RAW_P_CRC_ENA */

        descriptor->RX_buf[rx_pos] = byte;
        ++rx_pos;

        if (rx_pos == self->m_receivePackLen) {

#ifdef D_RAW_P_CRC_ENA
            D_RAW_P_CRC_FINAL(self->m_receiveCalcCRC);
            self->receiveState = RAW_P_GEN_RECEIVE_CRC_0;
#else
            const TYPEOF_STRUCT(RawParser_gen_descr_t, recv_message) __recv_message = descriptor->recv_message;
            M_Assert_SafeFunctionCall(__recv_message, {
                                        __recv_message(descriptor->RX_buf, rx_pos, ctx);
                                      });
            self->receiveState = RAW_P_GEN_RECEIVE_OK;
#endif /* D_RAW_P_CRC_ENA */
        }
        self->RXpos = rx_pos;
        break;}

#ifdef D_RAW_P_CRC_ENA

    case RAW_P_GEN_RECEIVE_CRC_0: {

#   ifdef D_RAW_P_USE_CRC8

        if(self->m_receiveCalcCRC == byte) {

#       ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
            /* Now decode -- encoded codeword size must be passed */
            rscode_decode(&self->rs_ecc, descriptor->RX_buf, self->RXpos);
            self->RXpos -= RSCODE_NPAR;
#       endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */

            const TYPEOF_STRUCT(RawParser_gen_descr_t, recv_message) __recv_message = descriptor->recv_message;
            M_Assert_SafeFunctionCall(__recv_message, {
                                        __recv_message(descriptor->RX_buf, self->RXpos, ctx);
                                      });
            self->receiveState = RAW_P_GEN_RECEIVE_OK;
        } else {
            M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_dma_proceedByte: Receive CRC8 error, rx data: %d, crc calc:%d", byte, self->m_receiveCalcCRC);
            self->receiveState = RAW_P_GEN_RECEIVE_ERR;
        }


#   elif defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)

        self->m_receiveCRCBuf = (rawP_crc_t)(byte & 0x000000FFUL); // read 0 byte
        self->receiveState = RAW_P_GEN_RECEIVE_CRC_1;

#   endif /* CRC 0b SWITCH LOGIC */

        break;}



#   if defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
    case RAW_P_GEN_RECEIVE_CRC_1: {

#       if defined(D_RAW_P_USE_CRC16)
        rawP_crc_t RX_crc = (rawP_crc_t)((((rawP_crc_t)byte) << 8U) & 0x0000FF00UL) | self->m_receiveCRCBuf; // read 1 byte
        RX_crc = LittleEndianU16(RX_crc);

        if(self->m_receiveCalcCRC == RX_crc) {

#           ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
            /* Now decode -- encoded codeword size must be passed */
            rscode_decode(&self->rs_ecc, descriptor->RX_buf, self->RXpos);
            self->RXpos -= RSCODE_NPAR;
#           endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */

            const TYPEOF_STRUCT(RawParser_gen_descr_t, recv_message) __recv_message = descriptor->recv_message;
            M_Assert_SafeFunctionCall(__recv_message, {
                                        __recv_message(descriptor->RX_buf, self->RXpos, ctx);
                                      });
            self->receiveState = RAW_P_GEN_RECEIVE_OK;
        } else {
            M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_dma_proceedByte: Receive CRC16 error, rx data: %d, crc calc:%d", RX_crc, self->m_receiveCalcCRC);
            self->receiveState = RAW_P_GEN_RECEIVE_ERR;
        }

#       elif defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)

        self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)byte) << 8U) & 0x0000FF00UL); // read 1 byte
        self->receiveState = RAW_P_GEN_RECEIVE_CRC_2;

#       endif /* CRC16 - CRC32 - CRC64 SWITCH */

        break;}

#   endif /* defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64) */



#   if defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
    case RAW_P_GEN_RECEIVE_CRC_2: {
        self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)byte) << 16U) & 0x00FF0000UL); // read 2 byte
        self->receiveState = RAW_P_GEN_RECEIVE_CRC_3;
        break;}

    case RAW_P_GEN_RECEIVE_CRC_3: {

#       if defined(D_RAW_P_USE_CRC32)
        rawP_crc_t RX_crc = (rawP_crc_t)((((rawP_crc_t)byte) << 24U) & 0xFF000000UL) | self->m_receiveCRCBuf; // read 3 byte
        RX_crc = LittleEndianU32(RX_crc);

        if(self->m_receiveCalcCRC == RX_crc) {

#           ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
            /* Now decode -- encoded codeword size must be passed */
            rscode_decode(&self->rs_ecc, descriptor->RX_buf, self->RXpos);
            self->RXpos -= RSCODE_NPAR;
#           endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */

            const TYPEOF_STRUCT(RawParser_gen_descr_t, recv_message) __recv_message = descriptor->recv_message;
            M_Assert_SafeFunctionCall(__recv_message, {
                                        __recv_message(descriptor->RX_buf, self->RXpos, ctx);
                                      });
            self->receiveState = RAW_P_GEN_RECEIVE_OK;
        } else {
            M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_dma_proceedByte: Receive CRC32 error, rx data: %d, crc calc:%d", RX_crc, self->m_receiveCalcCRC);
            self->receiveState = RAW_P_GEN_RECEIVE_ERR;
        }

#       elif defined(D_RAW_P_USE_CRC64)
        self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)byte) << 24U) & 0xFF000000UL); // read 3 byte
        self->receiveState = RAW_P_GEN_RECEIVE_CRC_4;
#       endif /* defined(D_RAW_P_USE_CRC32) */

        break;}

#   endif /* defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64) */

#   if defined(D_RAW_P_USE_CRC64)
    case RAW_P_GEN_RECEIVE_CRC_4: {
        self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)byte) << 32U) & 0x000000FF00000000ULL); // read 4 byte
        self->receiveState = RAW_P_GEN_RECEIVE_CRC_5;
        break;}

    case RAW_P_GEN_RECEIVE_CRC_5: {
        self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)byte) << 40U) & 0x0000FF0000000000ULL); // read 5 byte
        self->receiveState = RAW_P_GEN_RECEIVE_CRC_6;
        break;}

    case RAW_P_GEN_RECEIVE_CRC_6: {
        self->m_receiveCRCBuf |= (rawP_crc_t)((((rawP_crc_t)byte) << 48U) & 0x00FF000000000000ULL); // read 6 byte
        self->receiveState = RAW_P_GEN_RECEIVE_CRC_7;
        break;}

    case RAW_P_GEN_RECEIVE_CRC_7: {
        rawP_crc_t RX_crc = (rawP_crc_t)((((rawP_crc_t)byte) << 56U) & 0xFF00000000000000ULL) | self->m_receiveCRCBuf; // read 7 byte
        RX_crc = LittleEndianU64(RX_crc);

        if(self->m_receiveCalcCRC == RX_crc) {

#       ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
            /* Now decode -- encoded codeword size must be passed */
            rscode_decode(&self->rs_ecc, descriptor->RX_buf, self->RXpos);
            self->RXpos -= RSCODE_NPAR;
#       endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */

            const TYPEOF_STRUCT(RawParser_gen_descr_t, recv_message) __recv_message = descriptor->recv_message;
            M_Assert_SafeFunctionCall(__recv_message, {
                                        __recv_message(descriptor->RX_buf, self->RXpos, ctx);
                                      });
            self->receiveState = RAW_P_GEN_RECEIVE_OK;
        } else {
            M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_dma_proceedByte: Receive CRC64 error, rx data: %d, crc calc:%d", RX_crc, self->m_receiveCalcCRC);
            self->receiveState = RAW_P_GEN_RECEIVE_ERR;
        }
        break;}
#   endif /* defined(D_RAW_P_USE_CRC64) */

#endif /* D_RAW_P_CRC_ENA */

    case RAW_P_GEN_RECEIVE_ERR: {
        M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_gen_proceedByte: Receive error, byte: %d, rx_len: %d, max_rxlen: %d", byte, self->m_receivePackLen, descriptor->RX_buf_size);
        break;}

    case RAW_P_GEN_RECEIVE_OK: {
        M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_gen_proceedByte: LAST Received OK, byte: %d is not received because no SB", byte);
        break;}

    default:
        break;
    }
}



void RawParser_gen_read_byte(RawParser_gen_t* const self, const u8 byte, PREPROCESSOR_CTX_TYPE(ctx))
{
    M_Assert_Break((self == (RawParser_gen_t*)NULL), M_EMPTY, return, "RawParser_gen_read_byte: No valid input");

    const RawParser_gen_descr_t* const descriptor = self->descriptor;
    M_Assert_Break((descriptor == NULL || descriptor->recv_message == NULL), M_EMPTY, return, "RawParser_gen_read_byte: No valid descriptor");

    if (self->m_triggerSB) {
        if(descriptor->m_startByte == byte) { //{SB}{SB} -> {SB}
            RawParser_gen_proceedByte(self, byte, 0, ctx);
        } else { //{SB}{!SB} -> {SB} and newframe
            RawParser_gen_proceedByte(self, byte, 1, ctx);
        }
        self->m_triggerSB = 0;
    } else if (descriptor->m_startByte == byte) { //{!SB}{SB} -> set flag and skip step
        self->m_triggerSB = 1;
    } else { //{!SB}{!SB} -> {!SB}
        RawParser_gen_proceedByte(self, byte, 0, ctx);
    }
}

// elementary byte adding functions----------------------------------------------------------------------------
STATIC_FORCEINLINE int RawParser_general_addTxByte( const u8 byte,
                                                    const u8 startByte,
                                                    const TYPEOF_STRUCT(RawParser_gen_descr_t, write_byte) __write_byte,
                                                    PREPROCESSOR_CTX_TYPE(ctx))
{
    if(__write_byte(byte, ctx) == D_RAW_P_ERROR) {
        return D_RAW_P_ERROR;
    }

    if(startByte == byte) {
        if(__write_byte(byte, ctx) == D_RAW_P_ERROR) {
            return D_RAW_P_ERROR;
        }
    }

    return D_RAW_P_OK;
}




void RawParser_gen_send_message(RawParser_gen_t* const self, const u8* const data, reg size, PREPROCESSOR_CTX_TYPE(ctx))
{
    M_Assert_Break((self == (RawParser_gen_t*)NULL || data == NULL), M_EMPTY, return, "RawParser_gen_send_message: No valid input");
    M_Assert_WarningSaveCheck(size == 0, M_EMPTY, return, "RawParser_gen_send_message: No valid input size");

    // move to cash
    reg i;
    const RawParser_gen_descr_t* const descriptor = self->descriptor;
    const AUTO_VAL(__write_byte, descriptor->write_byte);
    const AUTO_VAL(__startByte, descriptor->m_startByte);
    M_Assert_Break((descriptor == NULL || __write_byte == NULL || (size > descriptor->RX_buf_size)), M_EMPTY, return, "RawParser_gen_read_byte: No valid descriptor");


#define RAW_P_GEN_WRITE_CHECK(expr)     \
    if((expr) == D_RAW_P_ERROR) {       \
        return;                         \
    }

#ifdef D_RAW_P_CRC_ENA // crc init
    rawP_crc_t m_calcCrc = D_RAW_P_CRC_INIT;
    D_RAW_P_CRC_START(m_calcCrc);
#endif /* D_RAW_P_CRC_ENA */

#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA // calc reed-solomon parity
    rs_encode_data_onlyParity(&self->rs_ecc, data, size);
#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */

    RAW_P_GEN_WRITE_CHECK(__write_byte(__startByte, ctx));


#if defined(D_RAW_P_CRC_ENA) && defined(D_RAW_P_REED_SOLOMON_ECC_CORR_ENA) // add len to crc

    reg tmp_len = (size + RSCODE_NPAR);

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT

    if(tmp_len > D_RAW_P_LEN_SEPARATOR) {
        tmp_len = LittleEndianU16(tmp_len);
        const u8 tmp_len_LO = (u8)( tmp_len         & 0x000000FFUL);
        const u8 tmp_len_HI = (u8)((tmp_len >> 8U)  & 0x000000FFUL);

        RAW_P_GEN_WRITE_CHECK(__write_byte(RECEIVE_EXTENDED_LEN_CMD, ctx));
        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, RECEIVE_EXTENDED_LEN_CMD);

        RAW_P_GEN_WRITE_CHECK(RawParser_general_addTxByte(tmp_len_LO, __startByte, __write_byte, ctx));
        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, tmp_len_LO);

        RAW_P_GEN_WRITE_CHECK(RawParser_general_addTxByte(tmp_len_HI, __startByte, __write_byte, ctx));
        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, tmp_len_HI);
    } else {
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

        if(tmp_len >= __startByte) {
            ++tmp_len;
        }

        RAW_P_GEN_WRITE_CHECK(__write_byte(tmp_len, ctx));
        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, tmp_len);

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    }
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

#elif defined(D_RAW_P_CRC_ENA)

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT

    if(size > D_RAW_P_LEN_SEPARATOR) {
        reg tmpSize = LittleEndianU16(size);
        const u8 tmp_len_LO = (u8)( tmpSize         & 0x000000FFUL);
        const u8 tmp_len_HI = (u8)((tmpSize >> 8U)  & 0x000000FFUL);

        RAW_P_GEN_WRITE_CHECK(__write_byte(RECEIVE_EXTENDED_LEN_CMD, ctx));
        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, RECEIVE_EXTENDED_LEN_CMD);

        RAW_P_GEN_WRITE_CHECK(RawParser_general_addTxByte(tmp_len_LO, __startByte, __write_byte, ctx));
        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, tmp_len_LO);

        RAW_P_GEN_WRITE_CHECK(RawParser_general_addTxByte(tmp_len_HI, __startByte, __write_byte, ctx));
        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, tmp_len_HI);
    } else {
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

        if(size >= __startByte) {
            ++size;
        }

        RAW_P_GEN_WRITE_CHECK(__write_byte(size, ctx));
        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, size);

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    }
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */


#else
#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT

    if(size > D_RAW_P_LEN_SEPARATOR) {
        reg tmpSize = LittleEndianU16(size);
        const u8 tmp_len_LO = (u8)( tmpSize         & 0x000000FFUL);
        const u8 tmp_len_HI = (u8)((tmpSize >> 8U)  & 0x000000FFUL);

        RAW_P_GEN_WRITE_CHECK(__write_byte(RECEIVE_EXTENDED_LEN_CMD, ctx));
        RAW_P_GEN_WRITE_CHECK(RawParser_general_addTxByte(tmp_len_LO, __startByte, __write_byte, ctx));
        RAW_P_GEN_WRITE_CHECK(RawParser_general_addTxByte(tmp_len_HI, __startByte, __write_byte, ctx));
    } else {
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

        if(size >= __startByte) {
            ++size;
        }

        RAW_P_GEN_WRITE_CHECK(__write_byte(size, ctx));

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    }
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

#endif /*  defined(D_RAW_P_CRC_ENA) && defined(D_RAW_P_REED_SOLOMON_ECC_CORR_ENA) */


    for (i = 0; i < size; ++i) {
        const u8 byte = data[i];

        RAW_P_GEN_WRITE_CHECK(RawParser_general_addTxByte(byte, __startByte, __write_byte, ctx));

#ifdef D_RAW_P_CRC_ENA // calc crc data
        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, byte);
#endif /* D_RAW_P_CRC_ENA */
    }

#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA // add reed-solomon parity to crc and write to data
    const TYPEOF_DATA(self->rs_ecc.pBytes[0])* const pBytes = self->rs_ecc.pBytes;

    for (i = 0; i < RSCODE_NPAR; ++i) {
        const TYPEOF_DATA(self->rs_ecc.pBytes[0]) pByte = pBytes[RSCODE_NPAR-1-i];

        RAW_P_GEN_WRITE_CHECK(RawParser_general_addTxByte(pByte, __startByte, __write_byte, ctx));

#ifdef D_RAW_P_CRC_ENA // crc init
        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, pByte);
#endif /* D_RAW_P_CRC_ENA */

    }
#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */



#ifdef D_RAW_P_CRC_ENA // calc crc data
    D_RAW_P_CRC_FINAL(m_calcCrc);

#if defined(D_RAW_P_USE_CRC16)
    m_calcCrc = LittleEndianU16(m_calcCrc);
#elif defined(D_RAW_P_USE_CRC32)
    m_calcCrc = LittleEndianU32(m_calcCrc);
#elif defined(D_RAW_P_USE_CRC64)
    m_calcCrc = LittleEndianU64(m_calcCrc);
#endif /* byte order & crc selection */

#if defined(D_RAW_P_USE_CRC8)
    RAW_P_GEN_WRITE_CHECK(RawParser_general_addTxByte(m_calcCrc, __startByte, __write_byte, ctx));
#else
    const u8* const crc_bytes = UINT8_TYPE_DC( &m_calcCrc );
    for (i = 0; i < sizeof(rawP_crc_t); ++i) {
        RAW_P_GEN_WRITE_CHECK(RawParser_general_addTxByte(crc_bytes[i], __startByte, __write_byte, ctx));
    }
#endif /* defined(D_RAW_P_USE_CRC8) */


#endif /* D_RAW_P_CRC_ENA */

#undef RAW_P_GEN_WRITE_CHECK
}


// FSM data fusion -----------------------------
#undef RAW_P_GEN_RECEIVE_LEN_0

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
#   undef RAW_P_GEN_RECEIVE_LEN_LOW
#   undef RAW_P_GEN_RECEIVE_LEN_HIGH
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

#undef RAW_P_GEN_RECEIVE_DATA

#ifdef D_RAW_P_CRC_ENA
#   undef RAW_P_GEN_RECEIVE_CRC_0
#
#   if defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
#       undef RAW_P_GEN_RECEIVE_CRC_1
#   endif /* defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) */
#
#   if defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
#       undef RAW_P_GEN_RECEIVE_CRC_2
#       undef RAW_P_GEN_RECEIVE_CRC_3
#   endif /* defined(D_RAW_P_USE_CRC32) */
#
#   if defined(D_RAW_P_USE_CRC64)
#       undef RAW_P_GEN_RECEIVE_CRC_4
#       undef RAW_P_GEN_RECEIVE_CRC_5
#       undef RAW_P_GEN_RECEIVE_CRC_6
#       undef RAW_P_GEN_RECEIVE_CRC_7
#   endif /* defined(D_RAW_P_USE_CRC64) */
#
#endif /* D_RAW_P_CRC_ENA */

#undef RAW_P_GEN_RECEIVE_ERR
#undef RAW_P_GEN_RECEIVE_OK
//----------------------------------------------





