#ifndef __RAW_PARSER_DMA_PORT_H__
#define __RAW_PARSER_DMA_PORT_H__

#include <stddef.h>
#include <stdint.h>

#include "crc8.h"
#include "crc16.h"
#include "crc32.h"
#include "byte_order.h"

//--------------------------------TYPES--------------------------------
typedef uint8_t rawP_start_t; // start byte type, sizeof(rawP_start_t) must be equal 1 else error
typedef uint8_t rawP_data_t;  // data type, sizeof(rawP_data_t) must be equal 1 else error

/*
***************************************************************************************************
    INTERNAL BUFFER SIZE (number of rawP_data_t values)
***************************************************************************************************
*/
#define D_RAW_P_RX_BUF_SIZE 256U // rx buffer must be is power of 2, else error
#define D_RAW_P_TX_BUF_SIZE 256U // tx buffer must be is power of 2, else error

/*
***************************************************************************************************
   SUPPORT TWO BYTE LENGTH OF PACKETS
***************************************************************************************************
*/

//#define D_RAW_P_TWO_BYTES_LEN_SUPPORT

#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    typedef uint16_t rawP_size_t;
#else // use one byte of length
    typedef uint8_t rawP_size_t;
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */


#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
    #define RECEIVE_EXTENDED_LEN_CMD (rawP_data_t)(0xFFU)
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

#define D_RAW_P_LEN_SEPARATOR 0xFBU

/*
***************************************************************************************************
   frame data structure definition
***************************************************************************************************
*/

typedef struct {
    rawP_data_t *data;
    rawP_size_t size;
} RawParser_Frame_t;

/*
***************************************************************************************************
    CRC SETTINGS
***************************************************************************************************
*/

#define D_RAW_P_CRC_ENA

#ifdef D_RAW_P_CRC_ENA

    #define D_RAW_P_USE_CRC8                // enable crc8, check if multiple use crc then error
    //#define D_RAW_P_USE_CRC16               // enable crc16, check if multiple use crc then error
    //#define D_RAW_P_USE_CRC32               // enable crc32, check if multiple use crc then error

#endif /* D_RAW_P_CRC_ENA */


//CRC TABLE SIZE & TYPE -----------------
#ifdef D_RAW_P_CRC_ENA

    #if defined(D_RAW_P_USE_CRC8) && defined(_MY_CRC8_ENA)
        typedef my_crc8_t rawP_crc_t;

        #if defined(_MY_CRC8_TABLE_CALC_ENA)
            #define D_RAW_P_CRC_ARRAY(data, len)  fast_crc8_maxim_array((data), (len))
            #define D_RAW_P_CRC_UPDATE(crc, data) fast_crc8_maxim_byte((crc), (data))
        #else
            #define D_RAW_P_CRC_ARRAY(data, len)  slow_crc8_maxim_array((data), (len))
            #define D_RAW_P_CRC_UPDATE(crc, data) slow_crc8_maxim_byte((crc), (data))
        #endif /* D_RAW_P_USE_CRC_TABLE_FAST */

        #define D_RAW_P_CRC_INIT CRC8INIT
        #define D_RAW_P_CRC_FINAL(crc) CRC8FINAL(crc)

    #elif defined(D_RAW_P_USE_CRC16) && defined(_MY_CRC16_ENA)
        typedef my_crc16_t rawP_crc_t;

        #if defined(_MY_CRC16_TABLE_CALC_ENA)
            #define D_RAW_P_CRC_ARRAY(data, len)  fast_crc16_t10_dif_array((data), (len))
            #define D_RAW_P_CRC_UPDATE(crc, data) fast_crc16_t10_dif_byte((crc), (data))
        #else
            #define D_RAW_P_CRC_ARRAY(data, len)  slow_crc16_t10_dif_array((data), (len))
            #define D_RAW_P_CRC_UPDATE(crc, data) slow_crc16_t10_dif_byte((crc), (data))
        #endif /* D_RAW_P_USE_CRC_TABLE_FAST */

        #define D_RAW_P_CRC_INIT CRC16INIT
        #define D_RAW_P_CRC_FINAL(crc) CRC16FINAL(crc)

    #elif defined(D_RAW_P_USE_CRC32) && defined(_MY_CRC32_ENA)
        typedef my_crc32_t rawP_crc_t;

        #if defined(_MY_CRC32_TABLE_CALC_ENA)
            #define D_RAW_P_CRC_ARRAY(data, len)  fast_crc32b_array((data), (len))
            #define D_RAW_P_CRC_UPDATE(crc, data) fast_crc32b_byte((crc), (data))
        #else
            #define D_RAW_P_CRC_ARRAY(data, len)  slow_crc32b_array((data), (len))
            #define D_RAW_P_CRC_UPDATE(crc, data) slow_crc32b_byte((crc), (data))
        #endif /* D_RAW_P_USE_CRC_TABLE_FAST */

        #define D_RAW_P_CRC_INIT CRC32INIT
        #define D_RAW_P_CRC_FINAL(crc) CRC32FINAL(crc)

    #endif /* CRC CHECK DEFINES */

#else /* no crc functions */

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

    static_assert((sizeof(rawP_start_t) == 1), "MY_RAW_PARSER: size of start byte type must be equal 1, change --> raw_parser_port.h: typedef rawP_start_t");
    static_assert((sizeof(rawP_data_t) == 1), "MY_RAW_PARSER: size of data type must be equal 1, change --> raw_parser_port.h: typedef rawP_data_t");

    #ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
        static_assert((sizeof(rawP_size_t) == 2), "MY_RAW_PARSER: size of length type must be equal 2, change --> raw_parser_port.h: typedef rawP_size_t");
    #else // use one byte of length
        static_assert((sizeof(rawP_size_t) == 1), "MY_RAW_PARSER: size of length type must be equal 1, change --> raw_parser_port.h: typedef rawP_size_t");
    #endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

    #ifdef D_RAW_P_CRC_ENA
        static_assert((sizeof(rawP_data_t) == sizeof(my_crc_byte_t)), "MY_RAW_PARSER: sizeof CRC my_crc_byte_t  must be equal to raw parser data type rawP_data_t, change --> raw_parser_port.h: typedef rawP_data_t, or my_crc_port.h --> typedef my_crc_byte_t");
    #endif /* D_RAW_P_CRC_ENA */

#else // if old version C
    #define C99_D_RAW_P_STATIC_ASSERTION_CREATE(COND,MSG) typedef int my_crc_static_assertion_##MSG[(COND)? 1 : -1] // define custom static assertion if version C less than C11
    //--------------------------------------------------------------------------------------------------------------

    C99_D_RAW_P_STATIC_ASSERTION_CREATE((sizeof(rawP_start_t) == 1), size_of_start_byte_type_must_be_equal_1_change_typedef_rawP_start_t);
    C99_D_RAW_P_STATIC_ASSERTION_CREATE((sizeof(rawP_data_t) == 1), size_of_data_type_type_must_be_equal_1_change_typedef_rawP_data_t);

    #ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
        C99_D_RAW_P_STATIC_ASSERTION_CREATE((sizeof(rawP_size_t) == 2), size_of_length_type_type_must_be_equal_2_change_typedef_rawP_size_t);
    #else // use one byte of length
        C99_D_RAW_P_STATIC_ASSERTION_CREATE((sizeof(rawP_size_t) == 1), size_of_length_type_type_must_be_equal_1_change_typedef_rawP_size_t);
    #endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */

    #ifdef D_RAW_P_CRC_ENA
        C99_D_RAW_P_STATIC_ASSERTION_CREATE((sizeof(rawP_data_t) == sizeof(my_crc_byte_t)), size_of_crc_type_must_be_equal_to_rawParser_data_type_change_typedef_rawP_size_t_or_my_crc_byte_t);
    #endif /* D_RAW_P_CRC_ENA */

    //--------------------------------------------------------------------------------------------------------------
    #undef C99_D_RAW_P_STATIC_ASSERTION_CREATE
#endif /* (__STDC_VERSION__ >= 201112L) */


// crc user check functions-----------------------
#ifdef D_RAW_P_CRC_ENA

    #if !defined(D_RAW_P_USE_CRC8) && !defined(D_RAW_P_USE_CRC16) && !defined(D_RAW_P_USE_CRC32)
        #error At least one implementation CRC required if CRC is ENABLED, or you must disable CRC than commit D_RAW_P_CRC_ENA
    #elif defined(D_RAW_P_USE_CRC8) && (defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32))
        #error only one realization CRC8, 16 or 32 supported
    #elif defined(D_RAW_P_USE_CRC16) && (defined(D_RAW_P_USE_CRC8) || defined(D_RAW_P_USE_CRC32))
        #error only one realization CRC8, 16 or 32 supported
    #elif defined(D_RAW_P_USE_CRC32) && (defined(D_RAW_P_USE_CRC8) || defined(D_RAW_P_USE_CRC16))
        #error only one realization CRC8, 16 or 32 supported
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

    #endif

#endif /* D_RAW_P_CRC_ENA */


// buffer rx/tx size check ----------------------------------------------------------------------------------------------------
#if ((D_RAW_P_RX_BUF_SIZE - 1) & D_RAW_P_RX_BUF_SIZE) || (D_RAW_P_RX_BUF_SIZE == 0) // rx buffer is power of 2 size check
    #error D_RAW_P_RX_BUF_SIZE must be is power of 2
#endif //check if power of 2 D_RAW_P_RX_BUF_SIZE

#if ((D_RAW_P_TX_BUF_SIZE - 1) & D_RAW_P_TX_BUF_SIZE) || (D_RAW_P_TX_BUF_SIZE == 0) // tx buffer is power of 2 size check
    #error D_RAW_P_TX_BUF_SIZE must be is power of 2
#endif //check if power of 2 D_RAW_P_TX_BUF_SIZE

#if ((D_RAW_P_RX_BUF_SIZE > 256) || (D_RAW_P_TX_BUF_SIZE > 256)) && !defined(D_RAW_P_TWO_BYTES_LEN_SUPPORT)
    #error BUFFER`s must be less than 256 or uncommit D_RAW_P_TWO_BYTES_LEN_SUPPORT
#endif // two bytes len support buffer check


#if ((D_RAW_P_RX_BUF_SIZE > 8192) || (D_RAW_P_TX_BUF_SIZE > 8192))
    #error BUFFER`s must be less or equal than 8192 bytes per one packet (MTU)
#endif // two bytes len support buffer check

#endif /* __RAW_PARSER_PORT_H__*/
