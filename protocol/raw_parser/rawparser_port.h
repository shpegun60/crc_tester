#ifndef __RAW_PARSER_DMA_PORT_H__
#define __RAW_PARSER_DMA_PORT_H__ 1

#include "my_ctypes.h"

#ifndef D_RAW_P_TEST_DISABLE
    //#define D_RAW_P_TEST_DISABLE
#endif /* D_RAW_P_TEST_DISABLE */


/*
***************************************************************************************************
    INTERNAL BUFFER SIZE (number of rawP_data_t values)
***************************************************************************************************
*/
#define D_RAW_P_RX_BUF_SIZE 512U // rx buffer must be is power of 2, else error
#define D_RAW_P_TX_BUF_SIZE 512U // tx buffer must be is power of 2, else error

/*
***************************************************************************************************
   SUPPORT TWO BYTE LENGTH OF PACKETS
***************************************************************************************************
*/

#define D_RAW_P_TWO_BYTES_LEN_SUPPORT

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    typedef u16 rawP_size_t;
    #define D_RAW_P_MAX_PROTOCOL_LEN 8192U
#else // use one byte of length
    typedef u8 rawP_size_t;
    #define D_RAW_P_MAX_PROTOCOL_LEN 254U
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */


#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    #define RECEIVE_EXTENDED_LEN_CMD (u8)(0xFFU)
    #define D_RAW_P_LEN_SEPARATOR (u8)(0xFBU)
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */



/*
***************************************************************************************************
   frame data structure definition
***************************************************************************************************
*/

typedef struct {
    u8 *data;
    rawP_size_t size;
} RawParser_Frame_t;

/*
***************************************************************************************************
    CRC SETTINGS
***************************************************************************************************
*/

#define D_RAW_P_CRC_ENA

#ifdef D_RAW_P_CRC_ENA

    //#define D_RAW_P_USE_CRC8                // enable crc8, check if multiple use crc then error
    //#define D_RAW_P_USE_CRC16               // enable crc16, check if multiple use crc then error
    //#define D_RAW_P_USE_CRC32               // enable crc32, check if multiple use crc then error
    #define D_RAW_P_USE_CRC64               // enable crc64, check if multiple use crc then error

#endif /* D_RAW_P_CRC_ENA */


//CRC TABLE SIZE & TYPE -----------------
#ifdef D_RAW_P_CRC_ENA
    #if defined(D_RAW_P_USE_CRC8)
        #include "crc8.h"
    #elif defined(D_RAW_P_USE_CRC16)
        #include "crc16.h"
    #elif defined(D_RAW_P_USE_CRC32)
        #include "crc32.h"
    #elif defined(D_RAW_P_USE_CRC64)
        #include "crc64.h"
    #endif

    #if defined(D_RAW_P_USE_CRC8) && defined(_MY_CRC8_ENA)
        typedef u8 rawP_crc_t;

        #if defined(_MY_CRC8_TABLE_CALC_ENA)
            #define D_RAW_P_CRC_ARRAY(data, len)  fast_crc8_maxim_array((data), (len))
            #define D_RAW_P_CRC_UPDATE(crc, data) fast_crc8_maxim_byte((crc), (data))
        #else
            #define D_RAW_P_CRC_ARRAY(data, len)  slow_crc8_maxim_array((data), (len))
            #define D_RAW_P_CRC_UPDATE(crc, data) slow_crc8_maxim_byte((crc), (data))
        #endif /* D_RAW_P_USE_CRC_TABLE_FAST */

        #define D_RAW_P_CRC_START(crc) CRC8START(crc)
        #define D_RAW_P_CRC_INIT CRC8INIT
        #define D_RAW_P_CRC_FINAL(crc) CRC8FINAL(crc)

    #elif defined(D_RAW_P_USE_CRC16) && defined(_MY_CRC16_ENA)
        typedef u16 rawP_crc_t;

        #if defined(_MY_CRC16_TABLE_CALC_ENA)
            #define D_RAW_P_CRC_ARRAY(data, len)  fast_crc16_t10_dif_array((data), (len))
            #define D_RAW_P_CRC_UPDATE(crc, data) fast_crc16_t10_dif_byte((crc), (data))
        #else
            #define D_RAW_P_CRC_ARRAY(data, len)  slow_crc16_t10_dif_array((data), (len))
            #define D_RAW_P_CRC_UPDATE(crc, data) slow_crc16_t10_dif_byte((crc), (data))
        #endif /* D_RAW_P_USE_CRC_TABLE_FAST */

        #define D_RAW_P_CRC_START(crc) CRC16START(crc)
        #define D_RAW_P_CRC_INIT CRC16INIT
        #define D_RAW_P_CRC_FINAL(crc) CRC16FINAL(crc)

    #elif defined(D_RAW_P_USE_CRC32) && defined(_MY_CRC32_ENA)
        typedef u32 rawP_crc_t;

        #if defined(_MY_CRC32_TABLE_CALC_ENA)
            #define D_RAW_P_CRC_ARRAY(data, len)  fast_crc32b_array((data), (len))
            #define D_RAW_P_CRC_UPDATE(crc, data) fast_crc32b_byte((crc), (data))
        #else
            #define D_RAW_P_CRC_ARRAY(data, len)  slow_crc32b_array((data), (len))
            #define D_RAW_P_CRC_UPDATE(crc, data) slow_crc32b_byte((crc), (data))
        #endif /* defined(_MY_CRC32_TABLE_CALC_ENA) */

        #define D_RAW_P_CRC_START(crc) CRC32START(crc)
        #define D_RAW_P_CRC_INIT CRC32INIT
        #define D_RAW_P_CRC_FINAL(crc) CRC32FINAL(crc)

    #elif defined(D_RAW_P_USE_CRC64) && defined(_MY_CRC64_ENA)
        typedef u64 rawP_crc_t;

        #if defined(_MY_CRC64_TABLE_CALC_ENA)
            #define D_RAW_P_CRC_ARRAY(data, len)  fast_crc64jones_array((data), (len))
            #define D_RAW_P_CRC_UPDATE(crc, data) fast_crc64jones_byte((crc), (data))
        #else
            #define D_RAW_P_CRC_ARRAY(data, len)  slow_crc64jones_array((data), (len))
            #define D_RAW_P_CRC_UPDATE(crc, data) slow_crc64jones_byte((crc), (data))
        #endif /* defined(_MY_CRC64_TABLE_CALC_ENA) */

        #define D_RAW_P_CRC_START(crc) CRC64START(crc)
        #define D_RAW_P_CRC_INIT CRC64INIT
        #define D_RAW_P_CRC_FINAL(crc) CRC64FINAL(crc)

    #endif /* CRC CHECK DEFINES */

#else /* no crc functions */

    #define D_RAW_P_CRC_START(crc)
    #define D_RAW_P_CRC_INIT 0
    #define D_RAW_P_CRC_UPDATE(crc, data)
    #define D_RAW_P_CRC_ARRAY(data, len)
    #define D_RAW_P_CRC_FINAL(crc)

#endif /* D_RAW_P_CRC_ENA */



/*
***************************************************************************************************
   Check user input (DO NOT OWERWRITE THIS!!!!!!!)
***************************************************************************************************
*/

// strict type control (PRECOMPILE)------------------------------------------------
#if (__STDC_VERSION__ >= 201112L) // if C version equal or more than C11
    #include "assert.h"

    #ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
        static_assert((sizeof(rawP_size_t) == 2), "MY_RAW_PARSER: size of length type must be equal 2, change --> raw_parser_port.h: typedef rawP_size_t");
    #else // use one byte of length
        static_assert((sizeof(rawP_size_t) == 1), "MY_RAW_PARSER: size of length type must be equal 1, change --> raw_parser_port.h: typedef rawP_size_t");
    #endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

#else // if old version C
    #define C99_D_RAW_P_STATIC_ASSERTION_CREATE(COND,MSG) typedef int my_crc_static_assertion_##MSG[(COND)? 1 : -1] // define custom static assertion if version C less than C11
    //--------------------------------------------------------------------------------------------------------------

    #ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
        C99_D_RAW_P_STATIC_ASSERTION_CREATE((sizeof(rawP_size_t) == 2), size_of_length_type_type_must_be_equal_2_change_typedef_rawP_size_t);
    #else // use one byte of length
        C99_D_RAW_P_STATIC_ASSERTION_CREATE((sizeof(rawP_size_t) == 1), size_of_length_type_type_must_be_equal_1_change_typedef_rawP_size_t);
    #endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

    //--------------------------------------------------------------------------------------------------------------
    #undef C99_D_RAW_P_STATIC_ASSERTION_CREATE
#endif /* (__STDC_VERSION__ >= 201112L) */


// crc user check functions-----------------------
#ifdef D_RAW_P_CRC_ENA

    #if !defined(D_RAW_P_USE_CRC8) && !defined(D_RAW_P_USE_CRC16) && !defined(D_RAW_P_USE_CRC32) && !defined(D_RAW_P_USE_CRC64)
        #error At least one implementation CRC required if CRC is ENABLED, or you must disable CRC than commit D_RAW_P_CRC_ENA
    #elif defined(D_RAW_P_USE_CRC8) && (defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64))
        #error only one realization CRC8, 16, 32 or 64 supported
    #elif defined(D_RAW_P_USE_CRC16) && (defined(D_RAW_P_USE_CRC8) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64))
        #error only one realization CRC8, 16, 32 or 64 supported
    #elif defined(D_RAW_P_USE_CRC32) && (defined(D_RAW_P_USE_CRC8) || defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC64))
        #error only one realization CRC8, 16, 32 or 64 supported
    #elif defined(D_RAW_P_USE_CRC64) && (defined(D_RAW_P_USE_CRC8) || defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32))
        #error only one realization CRC8, 16, 32 or 64 supported
    #else

        #if defined(D_RAW_P_USE_CRC8) && !defined(_MY_CRC8_ENA)
            #error you must enable CRC8 in CRC library first (uncommit _MY_CRC8_ENA)
        #endif /* defined(D_RAW_P_USE_CRC8) && !defined(_MY_CRC8_ENA) */

        #if defined(D_RAW_P_USE_CRC16) && !defined(_MY_CRC16_ENA)
            #error you must enable CRC16 in CRC library first (uncommit _MY_CRC16_ENA)
        #endif /* defined(D_RAW_P_USE_CRC16) && !defined(_MY_CRC16_ENA) */

        #if defined(D_RAW_P_USE_CRC32) && !defined(_MY_CRC32_ENA)
            #error you must enable CRC32 in CRC library first (uncommit _MY_CRC32_ENA)
        #endif /* defined(D_RAW_P_USE_CRC32) && !defined(_MY_CRC32_ENA) */

        #if defined(D_RAW_P_USE_CRC64) && !defined(_MY_CRC64_ENA)
            #error you must enable CRC64 in CRC library first (uncommit _MY_CRC64_ENA)
        #endif /* defined(D_RAW_P_USE_CRC64) && !defined(_MY_CRC64_ENA) */

    #endif

#endif /* D_RAW_P_CRC_ENA */


// buffer rx/tx size check ----------------------------------------------------------------------------------------------------
#if ((D_RAW_P_RX_BUF_SIZE - 1) & D_RAW_P_RX_BUF_SIZE) || (D_RAW_P_RX_BUF_SIZE == 0) // rx buffer is power of 2 size check
    #error D_RAW_P_RX_BUF_SIZE must be is power of 2
#endif //check if power of 2 D_RAW_P_RX_BUF_SIZE

#if ((D_RAW_P_TX_BUF_SIZE - 1) & D_RAW_P_TX_BUF_SIZE) || (D_RAW_P_TX_BUF_SIZE == 0) // tx buffer is power of 2 size check
    #error D_RAW_P_TX_BUF_SIZE must be is power of 2
#endif //check if power of 2 D_RAW_P_TX_BUF_SIZE

#if ((D_RAW_P_RX_BUF_SIZE > 512U) || (D_RAW_P_TX_BUF_SIZE > 512U)) && !defined(D_RAW_P_TWO_BYTES_LEN_SUPPORT)
    #error BUFFER`s must be less than 512 or uncommit D_RAW_P_TWO_BYTES_LEN_SUPPORT
#endif // two bytes len support buffer check


#if ((D_RAW_P_RX_BUF_SIZE > 16384U) || (D_RAW_P_TX_BUF_SIZE > 16384U))
    #error BUFFER`s must be less or equal than 16384U bytes per one packet (MTU)
#endif // two bytes len support buffer check

#endif /* __RAW_PARSER_PORT_H__*/
