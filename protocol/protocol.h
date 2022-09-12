#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__ 1

#ifdef __cplusplus
extern "C" {
#endif

// PREPROCESSOR LIB--------------------------------------
#include "preprocessor.h"

// UNIVERSAL INLINE LIB--------------------------------------
#include "inline.h"

// MY TYPES LIB--------------------------------------
#include "my_ctype_id.h"
#include "my_ctypes.h"
#include "my_ctype_cast.h"

// BYTE ORDER LIB--------------------------------------
#ifndef BYTE_ORDER_TEST_DISABLE
    //#define BYTE_ORDER_TEST_DISABLE 1
#endif /* BYTE_ORDER_TEST_DISABLE */

#include "byte_order.h"

// CONVERTER LIB--------------------------------------
#ifndef CONVERT_TEST_DISABLE
    //#define CONVERT_TEST_DISABLE 1
#endif /* CONVERT_TEST_DISABLE */

#include "convert.h"

// CRC8/16/32/64 LIB--------------------------------------
#ifndef _MY_CRC_TEST_DISABLE
    //#define _MY_CRC_TEST_DISABLE 1
#endif /* _MY_CRC_TEST_DISABLE */

#include "my_crc_port.h"

#include "crc8.h"
#include "crc16.h"
#include "crc32.h"
#include "crc64.h"

#include "crc_test.h"


/****************************************************************************************
 * RAW PARSER LIBS
 */

#ifndef D_RAW_P_TEST_DISABLE
    //#define D_RAW_P_TEST_DISABLE 1
#endif /* D_RAW_P_TEST_DISABLE */

#include "rawparser_port.h"
#include "rawparser_macro.h"

// RAW PARSER DMA LIB--------------------------------------
#include "rawparser_dma.h"
#include "raw_parser_dma_test.h"


/****************************************************************************************
 * REED - SOLOMON ECC LIB
 */

#ifndef RSCODE_TEST_DISABLE
    //#define RSCODE_TEST_DISABLE 1
#endif /* RSCODE_TEST_DISABLE */

#include "rscode-config.h"

#include "rs_ecc.h"
#include "rs.h"
#include "berlekamp.h"

#include "rscode_test.h"


/****************************************************************************************
 * CALLBACK MANAGER LIB
 */

#ifndef CALL_B_MAN_TEST_DISABLE
    //#define CALL_B_MAN_TEST_DISABLE
#endif /* CALL_B_MAN_TEST_DISABLE */

#include "callback_manager.h"
#include "callback_manager_test.h"


#ifdef __cplusplus
}
#endif


#endif // __PROTOCOL_H__
