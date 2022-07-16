#ifndef __MY_CRC_PORT_H__
#define __MY_CRC_PORT_H__

#include <stddef.h>
#include <stdint.h>

typedef unsigned char my_crc_byte_t; // byte type, input to crc functions (error when sizeof != 1)
/*
 * ******************************************************************************
 * CRC8 SETTINGS (USER OVERWRITE) commit define if do not use future
 * ******************************************************************************
 */
#define _MY_CRC8_ENA                    // CRC8 enable/disable
#define _MY_CRC8_TABLE_CALC_ENA         // CRC8 table fast calculation enable/disable
#define _MY_CRC8_GENERIC_CALC_ENA       // CRC8 generic slow calculation enable/disable


// type crc8
#ifdef _MY_CRC8_ENA
    typedef unsigned char my_crc8_t; // definition CRC8 type (error when sizeof != 1)
#endif /* _MY_CRC8_ENA */


/*
 * ******************************************************************************
 * CRC16 SETTINGS (USER OVERWRITE)  commit define if do not use future
 * ******************************************************************************
 */
#define _MY_CRC16_ENA                   // CRC16 enable/disable
#define _MY_CRC16_TABLE_CALC_ENA        // CRC16 table fast calculation enable/disable
#define _MY_CRC16_GENERIC_CALC_ENA      // CRC16 generic slow calculation enable/disable


// type crc16
#ifdef _MY_CRC16_ENA
    typedef unsigned short my_crc16_t; // definition CRC16 type (error when sizeof != 2)
#endif /* _MY_CRC16_ENA */


/*
 * ******************************************************************************
 * CRC32 SETTINGS (USER OVERWRITE)  commit define if do not use future
 * ******************************************************************************
 */
#define _MY_CRC32_ENA                   // CRC32 enable/disable
#define _MY_CRC32_TABLE_CALC_ENA        // CRC32 table fast calculation enable/disable
#define _MY_CRC32_GENERIC_CALC_ENA      // CRC32 generic slow calculation enable/disable


// type crc32
#ifdef _MY_CRC32_ENA
    typedef unsigned long my_crc32_t; // definition CRC32 type (error when sizeof != 4)
#endif /* _MY_CRC32_ENA */

/*
 * ******************************************************************************
 * check user input (DO NOT CHANGE THIS SECTION !!!!!!!)
 * ******************************************************************************
 */

// strict type control (PRECOMPILE)------------------------------------------------
#if (__STDC_VERSION__ >= 201112L) // if C version equal or more than C11
    #include "assert.h"

    static_assert((sizeof(my_crc_byte_t) == 1), "MY_CRC: size of crc byte type must be equal 1, change --> my_crc_port.h: typedef my_crc_byte_t ");

    #ifdef _MY_CRC8_ENA
        static_assert((sizeof(my_crc8_t) == 1), "MY_CRC: size of crc8 type must be equal 1, change --> my_crc_port.h: typedef my_crc8_t ");
    #endif /* _MY_CRC8_ENA*/

    #ifdef _MY_CRC16_ENA
        static_assert((sizeof(my_crc16_t) == 2), "MY_CRC: size of crc16 type must be equal 2, change --> my_crc_port.h: typedef my_crc16_t ");
    #endif /* _MY_CRC16_ENA*/

    #ifdef _MY_CRC32_ENA
        static_assert((sizeof(my_crc32_t) == 4), "MY_CRC: size of crc32 type must be equal 4, change --> my_crc_port.h: typedef my_crc32_t ");
    #endif /* _MY_CRC32_ENA*/

#else // if old version C
    #define C99MY_CRC_STATIC_ASSERTION_CREATE(COND,MSG) typedef int my_crc_static_assertion_##MSG[(COND)? 1 : -1] // define custom static assertion if version C less than C11
    //--------------------------------------------------------------------------------------------------------------

    C99MY_CRC_STATIC_ASSERTION_CREATE((sizeof(my_crc_byte_t) == 1), size_of_crc_byte_type_must_be_equal_1_change_typedef_my_crc_byte_t);

    #ifdef _MY_CRC8_ENA
        C99MY_CRC_STATIC_ASSERTION_CREATE((sizeof(my_crc8_t) == 1), size_of_crc8_type_must_be_equal_1_change_typedef_my_crc8_t);
    #endif /* _MY_CRC8_ENA*/

    #ifdef _MY_CRC16_ENA
        C99MY_CRC_STATIC_ASSERTION_CREATE((sizeof(my_crc16_t) == 2), size_of_crc16_type_must_be_equal_2_change_typedef_my_crc16_t);
    #endif /* _MY_CRC16_ENA*/

    #ifdef _MY_CRC32_ENA
        C99MY_CRC_STATIC_ASSERTION_CREATE((sizeof(my_crc32_t) == 4), size_of_crc32_type_must_be_equal_4_change_typedef_my_crc32_t);
    #endif /* _MY_CRC32_ENA*/

    //--------------------------------------------------------------------------------------------------------------
    #undef C99MY_CRC_STATIC_ASSERTION_CREATE // delete define for static assertion if version C less than C11
#endif /* (__STDC_VERSION__ >= 201112L) */



#ifdef _MY_CRC8_ENA

    #if !defined (_MY_CRC8_TABLE_CALC_ENA) && !defined(_MY_CRC8_GENERIC_CALC_ENA)
        #error At least one implementation CRC8 required
    #endif // !defined (_MY_CRC8_TABLE_CALC_ENA) && !defined(_MY_CRC8_GENERIC_CALC_ENA)

#endif /* _MY_CRC8_ENA */

#ifdef _MY_CRC16_ENA

    #if !defined (_MY_CRC16_TABLE_CALC_ENA) && !defined(_MY_CRC16_GENERIC_CALC_ENA)
        #error At least one implementation CRC16 required
    #endif // !defined (_MY_CRC16_TABLE_CALC_ENA) && !defined(_MY_CRC16_GENERIC_CALC_ENA)

#endif /* _MY_CRC16_ENA */

#ifdef _MY_CRC32_ENA

    #if !defined (_MY_CRC32_TABLE_CALC_ENA) && !defined(_MY_CRC32_GENERIC_CALC_ENA)
        #error At least one implementation CRC32 required
    #endif // !defined (_MY_CRC32_TABLE_CALC_ENA) && !defined(_MY_CRC32_GENERIC_CALC_ENA)

#endif /* _MY_CRC32_ENA */


#endif /* __MY_CRC_PORT_H__*/

