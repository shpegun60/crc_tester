#include "rawparser_dma.h"
#include "smart_assert.h"

//#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
//    #define RECEIVE_EXTENDED_LEN (rawP_data_t)(0xFF)
//#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */


//// FSM data fusion -----------------------------
//#define RECEIVE_LEN_0           0

//#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
//    #define RECEIVE_LEN_LOW     1
//    #define RECEIVE_LEN_HIGH    2
//#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

//#define RECEIVE_DATA            3

//#ifdef D_RAW_P_CRC_ENA
//    #define RECEIVE_CRC         4
//#endif /* D_RAW_P_CRC_ENA */

//#define RECEIVE_ERR             5
////----------------------------------------------

//RawParser_t* rawParser_new(RawParser_Init_t *init_desc)
//{
//    M_Assert_BreakSaveCheck((init_desc == NULL) || (init_desc->maxFrameSize == 0), "rawParser_new: No valid init", return NULL);

//    RawParser_t* self = (RawParser_t *)calloc(1, sizeof(RawParser_t));
//    M_Assert_BreakSaveCheck(self == NULL, "rawParser_new: No allocation memory", return NULL);


//    self->init = *init_desc;
//    self->m_receivePackLen = (rawP_size_t)0;

//    #ifdef D_RAW_P_CRC_ENA
//        self->m_frameCrc = (rawP_crc_t)(0xFFFFFFFFFFFFFFFFULL);
//    #endif /* D_RAW_P_CRC_ENA */
    
//    self->m_triggerSB = 0;
//    self->m_receivePos = 0;
//    self->m_receiveReadPos = 0;
//    self->m_receiveHandlePos = 0;

//    self-receiveState = RECEIVE_LEN_0;

//    self->TX.data = self->m_sendBuffer;
//    self->TX.size = (rawP_size_t)0;

//    self->RX.data = self->m_frameBuffer;
//    self->RX.size = (rawP_size_t)0;
//}

//int rawParser_delete(RawParser_t** data)
//{
//    M_Assert_BreakSaveCheck((data == NULL) || (*data->maxFrameSize == NULL), "rawParser_delete: No valid input", return 0);
//    free(*data);
//    *data = NULL;
//}


//void RawParser_receiveByte(RawParser_t *self, rawP_data_t byte)
//{
//    M_Assert_Break((self == NULL), "RawParser_PushWord: No valid input", return);

//    self->m_recBuffer[self->m_receivePos++] = byte;
//}

//void RawParser_receiveByte_ptr(RawParser_t *self, rawP_data_t *byte)
//{
//    M_Assert_Break((self == NULL || byte == NULL), "RawParser_PushWord: No valid input", return);

//    self->m_recBuffer[self->m_receivePos++] = *byte;
//}


//RawParser_Frame_t* RawParser_proceed(RawParser_t *self)
//{
//    M_Assert_Break((self == NULL), "RawParser_proceed: No valid input", return);
//    self->RX.size = 0;

//    while (self->m_receivePos != self->m_receiveReadPos) {
//        rawP_data_t ch = self->m_recBuffer[self->m_receiveReadPos & (D_RAW_P_RX_BUF_SIZE - 1U)];
//        ++self->m_receiveReadPos;

//        if (self->m_triggerSB) {
//            if(ch == self->m_startByte) { //{SB}{SB} -> {SB}
//                _proceedByte(self, ch, 0);
//            } else { //{SB}{!SB} -> {SB} and newframe
//                _proceedByte(self, ch, 1);
//            }
//            self->m_triggerSB = 0;
//        } else if (ch == self->m_startByte) { //{!SB}{SB} -> set flag and skip step
//            self->m_triggerSB = 1;
//        } else { //{!SB}{!SB} -> {!SB}
//            _proceedByte(self, ch, 0);
//        }

//        if(self->RX.size != 0) {
//            return self->&RX;
//        }
//    }

//    return self->&RX;
//}


//static void RawParser_proceedByte(RawParser_t *self, rawP_data_t ch, unsigned char newFrame)
//{
//    if (newFrame) {

//#ifdef D_RAW_P_CRC_ENA
//        self->m_frameCrc = (rawP_crc_t)(0xFFFFFFFFFFFFFFFFULL);
//#endif /* D_RAW_P_CRC_ENA */

//        self->m_receiveHandlePos = 0;
//        self-receiveState = RECEIVE_LEN_0;
//    }

//    switch(self-receiveState) {

//        case RECEIVE_LEN_0:

//#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
//            if(ch == RECEIVE_EXTENDED_LEN) {
//                self->receiveState = RECEIVE_LEN_LOW;
//            } else {
//#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */
//                self->m_receivePackLen = ch;

//                if (self->m_receivePackLen > self->m_startByte) {
//                    self->m_receivePackLen -= 1;
//                }
//                self->m_receiveHandlePos = 0;
//                self->receiveState = RECEIVE_DATA;
//#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
//            }
//#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

//            break;
//#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
//        case RECEIVE_LEN_LOW:
//            self->m_receivePackLen = ch;
//            self->receiveState = RECEIVE_LEN_HIGH;
//            break;

//        case RECEIVE_LEN_HIGH:
//            self->m_receivePackLen |= (rawP_size_t)((((rawP_size_t)ch) << 8) & 0xFF00UL);
//            self->m_receiveHandlePos = 0;
//            self->receiveState = RECEIVE_DATA;
//            break;
//#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

//        case RECEIVE_DATA:
//            M_Assert_WarningSaveCheck((self->m_receivePackLen > D_RAW_P_RX_BUF_SIZE), "RawParser_proceedByte: No valid input length", self->receiveState = RECEIVE_ERR);

//            self->m_frameBuffer[self->m_receiveHandlePos++] = ch;

//            if (self->m_receiveHandlePos == self->m_receivePackLen) {

//                #ifdef D_RAW_P_CRC_ENA
//                    self->receiveState = RECEIVE_CRC;
//                #else
//                    self->RX.size = self->m_receivePackLen;
//                #endif /* D_RAW_P_CRC_ENA */
//            }
//            break;

// #ifdef D_RAW_P_CRC_ENA
//        case RECEIVE_CRC:
//            if(self->m_frameCrc == ch) {
//                self->RX.size = self->m_receivePackLen;
//            }
//            break;
//#endif /* D_RAW_P_CRC_ENA */


//        case RECEIVE_ERR:
//            M_Assert_Warning((self->m_receivePackLen > D_RAW_P_BUF_SIZE), "RawParser_proceedByte: No valid input length state changed, need new pack");
//            break;
//    }

//    #ifdef D_RAW_P_CRC_ENA
//        self->m_frameCrc =  _proceedCrc(m_frameCrc, ch);
//    #endif /* D_RAW_P_CRC_ENA */
//}


//// RawParser_Frame_t *RawParser_ShieldFrame(RawParser_t *self, uint8_t *data, uint8_t dataSize)
//// {
////     uint8_t crc = 0xFF;
////     uint8_t writePos = 0;

////     desc->outputFrameBuff[writePos++] = desc->startByte;

////     if (dataSize >= desc->startByte) {
////         crc = _RawParser_proceedCrc(crc, (dataSize + 1));
////         desc->outputFrameBuff[writePos++] = (dataSize + 1);

////         if ((dataSize + 1) == desc->startByte)
////             desc->outputFrameBuff[writePos++] = (dataSize + 1);

////     } else {
////         crc = _RawParser_proceedCrc(crc, dataSize);
////         desc->outputFrameBuff[writePos++] = dataSize;

////         if (dataSize == desc->startByte)
////             desc->outputFrameBuff[writePos++] = dataSize;
////     }


////     for (uint8_t i = 0; i < dataSize; i++) {
////         crc = _RawParser_proceedCrc(crc, data[i]);
////         desc->outputFrameBuff[writePos++] = data[i];

////         if (data[i] == desc->startByte)
////             desc->outputFrameBuff[writePos++] = data[i];
////     }


////     desc->outputFrameBuff[writePos++] = crc;
////     if (crc == desc->startByte)
////         desc->outputFrameBuff[writePos++] = crc;

////     desc->frameTX.data = desc->outputFrameBuff;
////     desc->frameTX.size = writePos;
////     return &desc->frameTX;
//// }

    














///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void TcpClient::proceed()
//{
//    int len  = m_client.read(m_tmp, 10);
    
//    //return;
//    for (int  i = 0; i < len; ++i) {
//        auto ch = m_tmp[i];

//        if (m_triggerSB) {
//            if(ch == m_startByte) { //{SB}{SB} -> {SB}
//                _proceedByte(ch, false);
//            } else { //{SB}{!SB} -> {SB} and newframe
//                _proceedByte(ch, true);
//            }
//            m_triggerSB = false;
//        } else if (ch == m_startByte) { //{!SB}{SB} -> set flag and skip step
//            m_triggerSB = true;
//        } else { //{!SB}{!SB} -> {!SB}
//            _proceedByte(ch, false);
//        }
//    }
//}


//#ifdef CLIENT_AUTO
//int TcpClient::clientAutoProceedNonBlock(unsigned int timeMs, const uint16_t port, const char * host)
//{
//    switch (clientAutoState)
//    {
//        case 0:
//            if (connected()) {
//                proceed();
//                return CLIENT_OK;
//            }
//            ++clientAutoState;
//            return CLIENT_TRY_CONNECT;
//            break;

//        case 1:
//            if (connect(host, port)) {
//                clientAutoState = 0;
//                return CLIENT_CONNECTED;
//            }
//            ++clientAutoState;
//            clientAutolastTime = timeMs;
//            break;

//        case 2:
//            if((timeMs - clientAutolastTime) > CLIENT_TIMEOUT) {
//                clientAutoState = 1;
//                clientAutolastTime = timeMs;
//                return CLIENT_TRY_CONNECT;
//            }
//            break;

//        default:
//            clientAutoState = 1;
//            break;
//    }

//    return CLIENT_ERROR_CONNECTION;
//}

//#endif /* CLIENT_AUTO */




//void TcpClient::_proceedByte(uint8_t ch, bool newFrame)
//{
//    //Serial.print("PROCEED BYTE: "); Serial.print(ch); Serial.print("  ");Serial.print(newFrame); Serial.println("");
//    if (newFrame) {
//        m_frameCrc = 0xFF;
//        m_receivePos = 0;
//    }

//    if (m_receivePos == 0) {
//        m_receivePackLen = ch;
//        if (m_receivePackLen > m_startByte) {
//            m_receivePackLen -= 1;
//        }
//    } else if ((m_receivePos - 1) < m_receivePackLen) {
//        m_recBuffer[m_receivePos-1] = ch;
//    } else if ((m_receivePos - 1) == m_receivePackLen && m_frameCrc == ch) {
//            _proceedPack();
//    } else {
//        return;
//    }

//    m_receivePos++;
//    m_frameCrc =  _proceedCrc(m_frameCrc, ch);
//}

//void TcpClient::_proceedPack()
//{
//    //Serial.println("PACK received: ");
    
//    if (m_receivePackLen > 0) {
//        auto s = m_handlers.find(m_recBuffer[0]);
//        if (s != m_handlers.end()) {
//            s->second(m_receivePackLen - 1, (m_recBuffer+1));
//        }
//    }
//}

//void TcpClient::on(uint8_t cmd, std::function<void(int len, uint8_t*)> foo)
//{
//    m_handlers.insert({cmd, foo});
//}

//void TcpClient::write(int len, unsigned char *ptr)
//{
//    char crc = static_cast<char>(0xFF);
//    m_sendBuffer[0] = m_startByte;
//    m_sendBuffer[1] = len >= m_startByte ? len + 1 : len;

//    int pos = 2;
//    auto addByte = [&](uint8_t b) {
//        crc = _proceedCrc(crc, b);
//        m_sendBuffer[pos++] = b;
//        if (b == m_startByte) {
//            m_sendBuffer[pos++] = b;
//        }
//    };

//    for (uint8_t i = 0; i < len; i++) {
//        addByte(ptr[i]);
//    }
//    addByte(crc);

//    m_client.write(m_sendBuffer, pos);
//}

//uint8_t TcpClient::_proceedCrc(uint8_t crc, uint8_t ch) {
//    crc ^= ch;
//    for (int i = 0; i < 8; i++)
//        crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
//    return crc;
//}


//#undef CLIENT_AUTO
