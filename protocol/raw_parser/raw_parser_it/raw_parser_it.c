#include "raw_parser_it.h"
#include "smart_assert.h"
#include "my_ctype_cast.h"


#if defined(D_RAW_P_TWO_BYTES_LEN_SUPPORT) || defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
    #include "byte_order.h"
#endif /* defined(D_RAW_P_TWO_BYTES_LEN_SUPPORT) || defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64) */

#include <stdlib.h>


// FSM RECEIVE data fusion -------------------------------------------------------------------------------------
#define RAW_P_IT_RECEIVE_LEN_0                      0x00U

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    #define RAW_P_IT_RECEIVE_LEN_LOW                0x01U
    #define RAW_P_IT_RECEIVE_LEN_HIGH               0x02U
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

#define RAW_P_IT_RECEIVE_DATA                       0x03U

#define RAW_P_IT_RECEIVE_ERR                        0x04U
#define RAW_P_IT_RECEIVE_OK                         0x05U
//---------------------------------------------------------------------------------------------------------------


// FSM TRANSMITT data fusion -------------------------------------------------------------------------------------
#define RAW_P_IT_TRANSMITT_SB                           0x00U

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    #define RAW_P_IT_TRANSMITT_LEN_SEPARATOR            0x01U
    #define RAW_P_IT_TRANSMITT_LEN_LOW                  0x02U
    #define RAW_P_IT_TRANSMITT_LEN_HIGH                 0x03U
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

#define RAW_P_IT_TRANSMITT_LEN                          0x04U
#define RAW_P_IT_TRANSMITT_DATA                         0x05U

//---------------------------------------------------------------------------------------------------------------

RawParser_it_t* rawParser_it_new(const u8 packStart)
{
#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    M_Assert_BreakSaveCheck(packStart == RECEIVE_EXTENDED_LEN_CMD, M_EMPTY, return (RawParser_it_t*)NULL, "rawParser_it_new: start byte: %d must be not equal RECEIVE_EXTENDED_LEN_CMD: %d", packStart, RECEIVE_EXTENDED_LEN_CMD);
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

    RawParser_it_t* self = (RawParser_it_t *)calloc(1, sizeof(RawParser_it_t));
    M_Assert_BreakSaveCheck(self == (RawParser_it_t *)NULL, M_EMPTY, return self, "RawParser_it_t: No memory for allocation ");

    rawParser_it_init(self, packStart);
    return self;
}


void rawParser_it_init(RawParser_it_t * const self, const u8 packStart)
{
#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    M_Assert_BreakSaveCheck(packStart == RECEIVE_EXTENDED_LEN_CMD, M_EMPTY, return, "rawParser_it_init: start byte: %d must be not equal RECEIVE_EXTENDED_LEN_CMD: %d", packStart, RECEIVE_EXTENDED_LEN_CMD);
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

    M_Assert_BreakSaveCheck(self == (RawParser_it_t *)NULL, M_EMPTY, return, "rawParser_it_init: No input data valid ");


    self->m_startByte = packStart;
    self->m_receivePackLen = 0;

#ifdef D_RAW_P_CRC_ENA
    self->m_transmittCalcCRC = D_RAW_P_CRC_INIT;
#endif /* D_RAW_P_CRC_ENA */

    self->m_triggerSB = 0;
    self->m_receivePos = 0;


    self->receiveState = RAW_P_IT_RECEIVE_LEN_0;
    self->transmittState = RAW_P_IT_TRANSMITT_SB;
    self->m_transmittPos = 0;
    self->transmissionRepeat = 0;
    self->m_transmittPackLen = 0;


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


#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
    rs_initialize_ecc(&self->rs_ecc);
#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */
}



#ifdef D_RAW_P_DISABLE_INTERNAL_TX_BUFFER
void rawParser_it_setUserBufferTX(RawParser_it_t * const self, u8 * const txBuffer)
{
    M_Assert_Break((self == (RawParser_it_t*)NULL), M_EMPTY, return, "rawParser_it_setUserBufferTX: No valid input object");
    M_Assert_Break((txBuffer == NULL), M_EMPTY, return, "rawParser_it_setUserBufferTX: No valid input TX buffer");

    self->TX.data = txBuffer;
    self->TX.size = 0;
}
#endif /* D_RAW_P_DISABLE_INTERNAL_TX_BUFFER */

#ifdef D_RAW_P_DISABLE_INTERNAL_RX_BUFFER
void rawParser_it_setUserBufferRX(RawParser_it_t * const self, u8 * const rxBuffer)
{
    M_Assert_Break((self == (RawParser_it_t*)NULL), M_EMPTY, return, "rawParser_it_setUserBufferRX: No valid input object");
    M_Assert_Break((rxBuffer == NULL), M_EMPTY, return, "rawParser_it_setUserBufferRX: No valid input RX buffer");

    self->RX.data = rxBuffer;
    self->RX.size = 0;
}
#endif /* D_RAW_P_DISABLE_INTERNAL_RX_BUFFER */

#if defined(D_RAW_P_DISABLE_INTERNAL_TX_BUFFER) && defined(D_RAW_P_DISABLE_INTERNAL_RX_BUFFER)
void rawParser_it_setUserBuffers(RawParser_it_t * const self, u8 * const rxBuffer, u8 * const txBuffer)
{
    M_Assert_Break((self == (RawParser_it_t*)NULL), M_EMPTY, return, "rawParser_it_setUserBuffers: No valid input object");
    M_Assert_Break((rxBuffer == NULL || txBuffer == NULL), M_EMPTY, return, "rawParser_it_setUserBuffers: No valid input buffers");

    self->TX.data = txBuffer;
    self->TX.size = 0;

    self->RX.data = rxBuffer;
    self->RX.size = 0;
}
#endif /* defined(D_RAW_P_DISABLE_INTERNAL_TX_BUFFER) || defined(D_RAW_P_DISABLE_INTERNAL_RX_BUFFER) */


int rawParser_it_delete(RawParser_it_t** data)
{
    M_Assert_BreakSaveCheck((data == NULL) || (*data == NULL), M_EMPTY, return 1, "rawParser_it_delete: No allocated memory");
    free(*data);
    *data = NULL;

    return 0;
}

//------------------------------RX------------------------------------------------------------------------------------------
static void RawParser_it_proceedByte(RawParser_it_t* const self, const u8 ch, const u8 newFrame)
{
    M_Assert_Break((self == (RawParser_it_t*)NULL), M_EMPTY, return, "RawParser_it_proceedByte: No valid input");
    M_Assert_Break((self->RX.data == NULL || self->TX.data == NULL), M_EMPTY, return, "RawParser_it_proceedByte: No valid RX or/and TX buffer , call function before: -->  rawParser_it_setUserBufferXX, XX = RX for rx buffer, XX = TX for tx buffer, XX = s for tx & rx buffers");

    if (newFrame) {
        self->m_receivePos = 0U;
        self->receiveState = RAW_P_IT_RECEIVE_LEN_0;
    }

    switch(self->receiveState) {

    case RAW_P_IT_RECEIVE_LEN_0:

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
        if(RECEIVE_EXTENDED_LEN_CMD == ch) {
            self->receiveState = RAW_P_IT_RECEIVE_LEN_LOW;
        } else {
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

            self->m_receivePackLen = (ch > self->m_startByte) ? (ch - 1U) : ch;
            self->m_receivePos = 0U;
            self->receiveState = RAW_P_IT_RECEIVE_DATA;

#ifdef D_RAW_P_CRC_ENA
            self->m_receivePackLen += sizeof(rawP_crc_t);

            M_Assert_WarningSaveCheck((self->m_receivePackLen > D_RAW_P_RX_BUF_SIZE || self->m_receivePackLen == sizeof(rawP_crc_t)), M_EMPTY, {
                                              self->receiveState = RAW_P_IT_RECEIVE_ERR;
                                          }, "RawParser_it_proceedByte: No valid receive length, rx_len + crc = %d, max_len = %d", self->m_receivePackLen, D_RAW_P_RX_BUF_SIZE);
#else
            M_Assert_WarningSaveCheck((self->m_receivePackLen > D_RAW_P_RX_BUF_SIZE || self->m_receivePackLen == 0), M_EMPTY, {
                                              self->receiveState = RAW_P_IT_RECEIVE_ERR;
                                          }, "RawParser_it_proceedByte: No valid receive length, rx_len = %d, max_len = %d", self->m_receivePackLen, D_RAW_P_RX_BUF_SIZE);

#endif /* D_RAW_P_CRC_ENA */



#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
        }
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

        break;

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    case RAW_P_IT_RECEIVE_LEN_LOW:

        self->m_receivePackLen = (reg)(ch & 0x000000FFUL);    // read low byte
        self->receiveState = RAW_P_IT_RECEIVE_LEN_HIGH;
        break;

    case RAW_P_IT_RECEIVE_LEN_HIGH:

        self->m_receivePackLen |= (reg)((((reg)ch) << 8U) & 0x0000FF00UL); // read high byte
        self->m_receivePackLen = LittleEndianU16(self->m_receivePackLen);

        self->m_receivePos = 0;
        self->receiveState = RAW_P_IT_RECEIVE_DATA;

#ifdef D_RAW_P_CRC_ENA
        self->m_receivePackLen += sizeof(rawP_crc_t);

        M_Assert_WarningSaveCheck((self->m_receivePackLen > D_RAW_P_RX_BUF_SIZE || self->m_receivePackLen == sizeof(rawP_crc_t)), M_EMPTY, {
                                          self->receiveState = RAW_P_IT_RECEIVE_ERR;
                                      }, "RawParser_it_proceedByte: No valid receive length, rx_len + crc = %d, max_len = %d", self->m_receivePackLen, D_RAW_P_RX_BUF_SIZE);
#else
        M_Assert_WarningSaveCheck((self->m_receivePackLen > D_RAW_P_RX_BUF_SIZE || self->m_receivePackLen == 0), M_EMPTY, {
                                          self->receiveState = RAW_P_IT_RECEIVE_ERR;
                                      }, "RawParser_it_proceedByte: No valid receive length, rx_len = %d, max_len = %d", self->m_receivePackLen, D_RAW_P_RX_BUF_SIZE);

#endif /* D_RAW_P_CRC_ENA */


        break;

#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

    case RAW_P_IT_RECEIVE_DATA:

        self->RX.data[self->m_receivePos++] = ch;

        if (self->m_receivePos == self->m_receivePackLen) {
            self->RX.size = self->m_receivePackLen;
            self->receiveState = RAW_P_IT_RECEIVE_OK;
        }
        break;


    case RAW_P_IT_RECEIVE_ERR:
        M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_it_proceedByte: Receive error, byte: %d, rx_len: %d, max_rxlen: %d", ch, self->m_receivePackLen, D_RAW_P_RX_BUF_SIZE);
        break;

    case RAW_P_IT_RECEIVE_OK:
        M_Assert_Warning(M_ALWAYS, M_EMPTY, M_EMPTY, "RawParser_it_proceedByte: LAST Received OK, byte: %d is not received because no SB", ch);
        break;

    default:
        break;
    }
}




RawParser_Frame_t* RawParser_it_RXproceedIt(RawParser_it_t* const self, const u8 ch)
{
    M_Assert_Break((self == (RawParser_it_t*)NULL), M_EMPTY, return NULL, "RawParser_it_proceed: No valid input");
    M_Assert_Break((self->RX.data == NULL || self->TX.data == NULL), M_EMPTY, return NULL, "RawParser_it_proceedByte: No valid RX or/and TX buffer , call function before: -->  rawParser_it_setUserBufferXX, XX = RX for rx buffer, XX = TX for tx buffer, XX = s for tx & rx buffers");

    if(self->RX.size) { // ignore byte if not proceeded last packet
        return &self->RX;
    }

    if (self->m_triggerSB) {
        if(self->m_startByte == ch) { //{SB}{SB} -> {SB}
            RawParser_it_proceedByte(self, ch, 0);
        } else { //{SB}{!SB} -> {SB} and newframe
            RawParser_it_proceedByte(self, ch, 1);
        }
        self->m_triggerSB = 0;
    } else if (self->m_startByte == ch) { //{!SB}{SB} -> set flag and skip step
        self->m_triggerSB = 1;
    } else { //{!SB}{!SB} -> {!SB}
        RawParser_it_proceedByte(self, ch, 0);
    }

    return &self->RX;
}

RawParser_Frame_t* RawParser_it_RXproceedLoop(RawParser_it_t* const self)
{
    M_Assert_Break((self == (RawParser_it_t*)NULL), M_EMPTY, return NULL, "RawParser_it_RXproceedLoop: No valid input");
    M_Assert_Break((self->RX.data == NULL || self->TX.data == NULL), M_EMPTY, return &self->RX, "RawParser_it_RXproceedLoop: No valid RX or/and TX buffer , call function before: -->  rawParser_it_setUserBufferXX, XX = RX for rx buffer, XX = TX for tx buffer, XX = s for tx & rx buffers");

    if(self->RX.size == 0) { // ignore packet because len equal 0
        return &self->RX;
    }

#ifdef D_RAW_P_CRC_ENA

    M_Assert_WarningSaveCheck((self->RX.size < (sizeof(rawP_crc_t) + 1U)), M_EMPTY, {
                                  self->RX.size = 0U;
                                  return &self->RX;
                              }, "RawParser_it_RXproceedLoop: ignore packet because len less than 1 byte + crc size, rx len-->&d, need len-->%d", self->RX.size, (sizeof(rawP_crc_t) + 1U));

    reg m_sizeWithoutCRC = (self->RX.size - sizeof(rawP_crc_t));

    rawP_crc_t m_calcCrc = D_RAW_P_CRC_INIT;
    D_RAW_P_CRC_START(m_calcCrc);

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    if(m_sizeWithoutCRC > D_RAW_P_LEN_SEPARATOR) {
        u16 tmp_len = LittleEndianU16(m_sizeWithoutCRC);
        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, RECEIVE_EXTENDED_LEN_CMD);
        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, (u8)( tmp_len         & 0x000000FFUL));
        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, (u8)((tmp_len >> 8U)  & 0x000000FFUL));
    } else {
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, m_sizeWithoutCRC);

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    }
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

    for(reg i = 0; i < m_sizeWithoutCRC; ++i) {
        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, self->RX.data[i]);
    }
    D_RAW_P_CRC_FINAL(m_calcCrc);

#if defined(D_RAW_P_USE_CRC8)
    rawP_crc_t m_receiveCrc = self->RX.data[m_sizeWithoutCRC];
    M_Assert_WarningSaveCheck((m_calcCrc != m_receiveCrc), M_EMPTY, {
                                  self->RX.size = 0U;
                                  return &self->RX;
                              }, "RawParser_it_RXproceedLoop: not compleate CRC8, calc: %d, receive: %d", m_calcCrc, m_receiveCrc);
#elif defined(D_RAW_P_USE_CRC16)
    rawP_crc_t m_receiveCrc = LittleEndianU16( *UINT16_TYPE_DC(&self->RX.data[m_sizeWithoutCRC]) );
    M_Assert_WarningSaveCheck((m_calcCrc != m_receiveCrc), M_EMPTY, {
                                  self->RX.size = 0U;
                                  return &self->RX;
                              }, "RawParser_it_RXproceedLoop: not compleate CRC16, calc: %d, receive: %d", m_calcCrc, m_receiveCrc);
#elif defined(D_RAW_P_USE_CRC32)
    rawP_crc_t m_receiveCrc = LittleEndianU32( *UINT32_TYPE_DC(&self->RX.data[m_sizeWithoutCRC]) );
    M_Assert_WarningSaveCheck((m_calcCrc != m_receiveCrc), M_EMPTY, {
                                  self->RX.size = 0U;
                                  return &self->RX;
                              }, "RawParser_it_RXproceedLoop: not compleate CRC32, calc: %d, receive: %d", m_calcCrc, m_receiveCrc);
#elif defined(D_RAW_P_USE_CRC64)
    rawP_crc_t m_receiveCrc = LittleEndianU64( *UINT64_TYPE_DC(&self->RX.data[m_sizeWithoutCRC]) );
    M_Assert_WarningSaveCheck((m_calcCrc != m_receiveCrc), M_EMPTY, {
                                  self->RX.size = 0U;
                                  return &self->RX;
                              }, "RawParser_it_RXproceedLoop: not compleate CRC64, calc: %d, receive: %d", m_calcCrc, m_receiveCrc);
#endif /* byte order selection */

    self->RX.size = m_sizeWithoutCRC;

#endif /* D_RAW_P_CRC_ENA */



#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA
    M_Assert_WarningSaveCheck((self->RX.size < (RSCODE_NPAR + 1U)), M_EMPTY, {
                                  self->RX.size = 0U;
                                  return &self->RX;
                              }, "RawParser_it_RXproceedLoop: not compleate len with ECC RS-code, len need: %d, receive: %d", (RSCODE_NPAR + 1U), self->RX.size);

    /* Now decode -- encoded codeword size must be passed */
    rscode_decode(&self->rs_ecc, self->RX.data, self->RX.size);
    self->RX.size -= RSCODE_NPAR;

#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */
    return &self->RX;
}



// -------------------------------TX----------------------------------------------------------------
int RawParser_it_TXpush(RawParser_it_t* const self, reg len)
{
    M_Assert_Break((self == (RawParser_it_t*)NULL), M_EMPTY, return 0, "RawParser_it_TXpush: No valid input object");
    M_Assert_Break((self->RX.data == NULL || self->TX.data == NULL), M_EMPTY, return 0, "RawParser_it_TXpush: No valid RX or/and TX buffer , call function before: -->  rawParser_it_setUserBufferXX, XX = RX for rx buffer, XX = TX for tx buffer, XX = s for tx & rx buffers");
    M_Assert_Break((len == 0), M_EMPTY, return 0, "RawParser_it_TXpush: No valid input length");

#if D_RAW_P_MAX_PROTOCOL_LEN < D_RAW_P_TX_BUF_SIZE

#if defined(D_RAW_P_CRC_ENA) && defined(D_RAW_P_REED_SOLOMON_ECC_CORR_ENA)
    M_Assert_BreakSaveCheck((len + RSCODE_NPAR + sizeof(rawP_crc_t)) > D_RAW_P_MAX_PROTOCOL_LEN, M_EMPTY, return 0, "RawParser_it_TXpush: len more than maximum protocol size, len + rs_code + crc--> %d, protocol--> %d", (len + RSCODE_NPAR + sizeof(rawP_crc_t)), D_RAW_P_MAX_PROTOCOL_LEN);
#elif defined(D_RAW_P_REED_SOLOMON_ECC_CORR_ENA)
    M_Assert_BreakSaveCheck((len + RSCODE_NPAR) > D_RAW_P_MAX_PROTOCOL_LEN, M_EMPTY, return 0, "RawParser_it_TXpush: len more than maximum protocol size, len + rs_code--> %d, protocol--> %d", (len + RSCODE_NPAR), D_RAW_P_MAX_PROTOCOL_LEN);
#elif defined(D_RAW_P_CRC_ENA)
    M_Assert_BreakSaveCheck((len + sizeof(rawP_crc_t)) > D_RAW_P_MAX_PROTOCOL_LEN, M_EMPTY, return 0, "RawParser_it_TXpush: len more than maximum protocol size, len + crc--> %d, protocol--> %d", (len + sizeof(rawP_crc_t), D_RAW_P_MAX_PROTOCOL_LEN);
#else
    M_Assert_BreakSaveCheck((len > D_RAW_P_MAX_PROTOCOL_LEN), M_EMPTY, return 0, "RawParser_it_TXpush: len more than maximum protocol size, len--> %d, protocol--> %d", (len), D_RAW_P_MAX_PROTOCOL_LEN);
#endif /* defined(D_RAW_P_CRC_ENA) && defined(D_RAW_P_REED_SOLOMON_ECC_CORR_ENA) */

#else

#if defined(D_RAW_P_CRC_ENA) && defined(D_RAW_P_REED_SOLOMON_ECC_CORR_ENA)
    M_Assert_BreakSaveCheck((len + RSCODE_NPAR + sizeof(rawP_crc_t)) > D_RAW_P_TX_BUF_SIZE, M_EMPTY, return 0, "RawParser_it_TXpush: len more than buffer, len + rs_code + crc--> %d, buffer--> %d", (len + RSCODE_NPAR + sizeof(rawP_crc_t)), D_RAW_P_TX_BUF_SIZE);
#elif defined(D_RAW_P_CRC_ENA)
    M_Assert_BreakSaveCheck((len + sizeof(rawP_crc_t)) > D_RAW_P_TX_BUF_SIZE, M_EMPTY, return 0, "RawParser_it_TXpush: len more than buffer, len + crc--> %d, buffer--> %d", (len + sizeof(rawP_crc_t)), D_RAW_P_TX_BUF_SIZE);
#elif defined(D_RAW_P_REED_SOLOMON_ECC_CORR_ENA)
    M_Assert_BreakSaveCheck((len + RSCODE_NPAR) > D_RAW_P_TX_BUF_SIZE, M_EMPTY, return 0, "RawParser_it_TXpush: len more than buffer, len + rs_code--> %d, protocol--> %d", (len + RSCODE_NPAR), D_RAW_P_TX_BUF_SIZE);
#else
    M_Assert_BreakSaveCheck((len > D_RAW_P_TX_BUF_SIZE), M_EMPTY, return 0, "RawParser_it_TXpush: len more than buffer, len--> %d, buffer--> %d", len, D_RAW_P_TX_BUF_SIZE);
#endif /* defined(D_RAW_P_CRC_ENA) && defined(D_RAW_P_REED_SOLOMON_ECC_CORR_ENA) */

#endif /* D_RAW_P_MAX_PROTOCOL_LEN < D_RAW_P_TX_BUF_SIZE */



#if defined(D_RAW_P_CRC_ENA) || defined(D_RAW_P_REED_SOLOMON_ECC_CORR_ENA)
    reg i;
#endif /* defined(D_RAW_P_CRC_ENA) || defined(D_RAW_P_REED_SOLOMON_ECC_CORR_ENA) */


#ifdef D_RAW_P_CRC_ENA // crc init
    rawP_crc_t m_calcCrc = D_RAW_P_CRC_INIT;
    D_RAW_P_CRC_START(m_calcCrc);
#endif /* D_RAW_P_CRC_ENA */


#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA // calc reed-solomon parity
    rs_encode_data_onlyParity(&self->rs_ecc, self->TX.data, len);
#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */


#if defined(D_RAW_P_CRC_ENA) && defined(D_RAW_P_REED_SOLOMON_ECC_CORR_ENA) // add len to crc

    reg tmp_len = (len + RSCODE_NPAR);

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT

    if(tmp_len > D_RAW_P_LEN_SEPARATOR) {
        tmp_len = LittleEndianU16(tmp_len);
        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, RECEIVE_EXTENDED_LEN_CMD);
        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, (u8)( tmp_len         & 0x000000FFUL));
        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, (u8)((tmp_len >> 8U)  & 0x000000FFUL));
    } else {
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, tmp_len);

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    }
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

#elif defined(D_RAW_P_CRC_ENA)

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT

    if(len > D_RAW_P_LEN_SEPARATOR) {
        rawP_size_t tmp_len = LittleEndianU16(len);
        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, RECEIVE_EXTENDED_LEN_CMD);
        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, (u8)( tmp_len         & 0x000000FFUL));
        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, (u8)((tmp_len >> 8U)  & 0x000000FFUL));
    } else {
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, len);

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    }
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

#endif /*  defined(D_RAW_P_CRC_ENA) && defined(D_RAW_P_REED_SOLOMON_ECC_CORR_ENA) */


#ifdef D_RAW_P_CRC_ENA // calc crc data
    for (i = 0; i < len; ++i) {
        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, self->TX.data[i]);
    }
#endif /* D_RAW_P_CRC_ENA */


#ifdef D_RAW_P_REED_SOLOMON_ECC_CORR_ENA // add reed-solomon parity to crc and write to data
    for (i = 0; i < RSCODE_NPAR; ++i) {
#ifdef D_RAW_P_CRC_ENA // crc init
        m_calcCrc = D_RAW_P_CRC_UPDATE(m_calcCrc, self->rs_ecc.pBytes[RSCODE_NPAR-1-i]);
#endif /* D_RAW_P_CRC_ENA */
        self->TX.data[len++] = self->rs_ecc.pBytes[RSCODE_NPAR-1-i];
    }
#endif /* D_RAW_P_REED_SOLOMON_ECC_CORR_ENA */



#ifdef D_RAW_P_CRC_ENA // add crc to data
    D_RAW_P_CRC_FINAL(m_calcCrc);

#if defined(D_RAW_P_USE_CRC8)
    self->TX.data[len++] = m_calcCrc;
#elif defined(D_RAW_P_USE_CRC16)
    m_calcCrc = LittleEndianU16(m_calcCrc);
    self->TX.data[len++] = (u8)( m_calcCrc        & 0x000000FFUL);
    self->TX.data[len++] = (u8)((m_calcCrc >> 8U) & 0x000000FFUL);
#elif defined(D_RAW_P_USE_CRC32)
    m_calcCrc = LittleEndianU32(m_calcCrc);
    self->TX.data[len++] = (u8)( m_calcCrc            & 0x000000FFUL);
    self->TX.data[len++] = (u8)((m_calcCrc >> 8U )    & 0x000000FFUL);
    self->TX.data[len++] = (u8)((m_calcCrc >> 16U)    & 0x000000FFUL);
    self->TX.data[len++] = (u8)((m_calcCrc >> 24U)    & 0x000000FFUL);
#elif defined(D_RAW_P_USE_CRC64)
    m_calcCrc = LittleEndianU64(m_calcCrc);
    self->TX.data[len++] = (u8)( m_calcCrc            & 0x00000000000000FFULL);
    self->TX.data[len++] = (u8)((m_calcCrc >> 8U )    & 0x00000000000000FFULL);
    self->TX.data[len++] = (u8)((m_calcCrc >> 16U)    & 0x00000000000000FFULL);
    self->TX.data[len++] = (u8)((m_calcCrc >> 24U)    & 0x00000000000000FFULL);
    self->TX.data[len++] = (u8)((m_calcCrc >> 32U)    & 0x00000000000000FFULL);
    self->TX.data[len++] = (u8)((m_calcCrc >> 40U)    & 0x00000000000000FFULL);
    self->TX.data[len++] = (u8)((m_calcCrc >> 48U)    & 0x00000000000000FFULL);
    self->TX.data[len++] = (u8)((m_calcCrc >> 56U)    & 0x00000000000000FFULL);
#endif /* byte order & crc selection */


#endif /* D_RAW_P_CRC_ENA */

    self->transmittState = RAW_P_IT_TRANSMITT_SB;
    self->TX.size = len;
    return len;
}


int RawParser_it_TXproceedIt(RawParser_it_t* const self, u8 * const ch)
{
    M_Assert_Break((self == (RawParser_it_t*)NULL), M_EMPTY, return 0, "RawParser_it_TXproceedIt: No valid input");
    M_Assert_Break((self->RX.data == NULL || self->TX.data == NULL), M_EMPTY, return 0, "RawParser_it_TXproceedIt: No valid RX or/and TX buffer , call function before: -->  rawParser_it_setUserBufferXX, XX = RX for rx buffer, XX = TX for tx buffer, XX = s for tx & rx buffers");

    if(self->TX.size == 0) { // ignore packet because len equal 0
        return 0;
    }

    switch(self->transmittState) {

    case RAW_P_IT_TRANSMITT_SB:
        (*ch) = self->m_startByte;

        self->transmissionRepeat = 0;
        self->m_transmittPos = 0;
        self->m_transmittPackLen = self->TX.size;

#if defined(D_RAW_P_CRC_ENA)
        self->TX.size -= sizeof(rawP_crc_t);
#endif /* D_RAW_P_CRC_ENA */


#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
        if(self->TX.size > D_RAW_P_LEN_SEPARATOR) {
            self->TX.size = LittleEndianU16(self->TX.size);
            self->transmittState = RAW_P_IT_TRANSMITT_LEN_SEPARATOR;
        } else {
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */
            self->transmittState = RAW_P_IT_TRANSMITT_LEN;
#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
        }
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */
        break;


#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    case RAW_P_IT_TRANSMITT_LEN_SEPARATOR:
        (*ch) = RECEIVE_EXTENDED_LEN_CMD;
        self->transmittState = RAW_P_IT_TRANSMITT_LEN_LOW;
        break;

    case RAW_P_IT_TRANSMITT_LEN_LOW: {
        (*ch) = (u8)(self->TX.size & 0x000000FFU);

        if((self->m_startByte == (*ch)) && !self->transmissionRepeat) {
            self->transmissionRepeat = 1;
        } else {
            self->transmissionRepeat = 0;
            self->transmittState = RAW_P_IT_TRANSMITT_LEN_HIGH;
        }

        break;
    }

    case RAW_P_IT_TRANSMITT_LEN_HIGH: {
        (*ch) = (u8)((self->TX.size >> 8U) & 0x000000FFU);

        if((self->m_startByte == (*ch)) && !self->transmissionRepeat) {
            self->transmissionRepeat = 1;
        } else {
            self->transmissionRepeat = 0;
            self->transmittState = RAW_P_IT_TRANSMITT_DATA;
        }

        break;
    }
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

    case RAW_P_IT_TRANSMITT_LEN:
        (*ch) = (u8)(((self->TX.size >= self->m_startByte) ? (self->TX.size + 1U) : self->TX.size) & 0x000000FFUL);
        self->transmittState = RAW_P_IT_TRANSMITT_DATA;
        break;

    case RAW_P_IT_TRANSMITT_DATA:
        (*ch) = self->TX.data[self->m_transmittPos];

        if((self->m_startByte == (*ch)) && !self->transmissionRepeat) {
            self->transmissionRepeat = 1;
        } else {
            self->transmissionRepeat = 0;
            ++self->m_transmittPos;

            if(self->m_transmittPos == self->m_transmittPackLen) {
                self->TX.size = 0;
                self->transmittState = RAW_P_IT_TRANSMITT_SB;
            }
        }

        break;

    }
    return 1;
}




// FSM RECEIVE data fusion -------------------------------------------------------------------------------------
#undef RAW_P_IT_RECEIVE_LEN_0

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    #undef RAW_P_IT_RECEIVE_LEN_LOW
    #undef RAW_P_IT_RECEIVE_LEN_HIGH
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

#undef RAW_P_IT_RECEIVE_DATA

#undef RAW_P_IT_RECEIVE_ERR
#undef RAW_P_IT_RECEIVE_OK
//---------------------------------------------------------------------------------------------------------------


// FSM TRANSMITT data fusion -------------------------------------------------------------------------------------
#undef RAW_P_IT_TRANSMITT_SB

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    #undef RAW_P_IT_TRANSMITT_LEN_SEPARATOR
    #undef RAW_P_IT_TRANSMITT_LEN_LOW
    #undef RAW_P_IT_TRANSMITT_LEN_HIGH
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

#undef RAW_P_IT_TRANSMITT_LEN
#undef RAW_P_IT_TRANSMITT_DATA

//---------------------------------------------------------------------------------------------------------------



