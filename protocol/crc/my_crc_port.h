#ifndef __MY_CRC_PORT_H__
#define __MY_CRC_PORT_H__ 1

#ifndef _MY_CRC_TEST_DISABLE
    //#define _MY_CRC_TEST_DISABLE
#endif /* _MY_CRC_TEST_DISABLE */

/*
 * ******************************************************************************
 * CRC8 SETTINGS (USER OVERWRITE) commit define if do not use future
 * ******************************************************************************
 */
#ifndef _MY_CRC8_ENA
    #define _MY_CRC8_ENA                    // CRC8 enable/disable
#endif /* _MY_CRC8_ENA */

#ifndef _MY_CRC8_TABLE_CALC_ENA
    #define _MY_CRC8_TABLE_CALC_ENA         // CRC8 table fast calculation enable/disable
#endif /* _MY_CRC8_TABLE_CALC_ENA */

#ifndef _MY_CRC8_GENERIC_CALC_ENA
    #define _MY_CRC8_GENERIC_CALC_ENA       // CRC8 generic slow calculation enable/disable
#endif /* _MY_CRC8_GENERIC_CALC_ENA */



/*
 * ******************************************************************************
 * CRC16 SETTINGS (USER OVERWRITE)  commit define if do not use future
 * ******************************************************************************
 */
#ifndef _MY_CRC16_ENA
    #define _MY_CRC16_ENA                   // CRC16 enable/disable
#endif /* _MY_CRC16_ENA */

#ifndef _MY_CRC16_TABLE_CALC_ENA
    #define _MY_CRC16_TABLE_CALC_ENA        // CRC16 table fast calculation enable/disable
#endif /* _MY_CRC16_TABLE_CALC_ENA */

#ifndef _MY_CRC16_GENERIC_CALC_ENA
    #define _MY_CRC16_GENERIC_CALC_ENA      // CRC16 generic slow calculation enable/disable
#endif /* _MY_CRC16_GENERIC_CALC_ENA */



/*
 * ******************************************************************************
 * CRC32 SETTINGS (USER OVERWRITE)  commit define if do not use future
 * ******************************************************************************
 */
#ifndef _MY_CRC32_ENA
    #define _MY_CRC32_ENA                   // CRC32 enable/disable
#endif /* _MY_CRC32_ENA */

#ifndef _MY_CRC32_TABLE_CALC_ENA
    #define _MY_CRC32_TABLE_CALC_ENA        // CRC32 table fast calculation enable/disable
#endif /* _MY_CRC32_TABLE_CALC_ENA */

#ifndef _MY_CRC32_GENERIC_CALC_ENA
    #define _MY_CRC32_GENERIC_CALC_ENA      // CRC32 generic slow calculation enable/disable
#endif /* _MY_CRC32_GENERIC_CALC_ENA */



/*
 * ******************************************************************************
 * CRC64 SETTINGS (USER OVERWRITE)  commit define if do not use future
 * ******************************************************************************
 */
#ifndef _MY_CRC64_ENA
    #define _MY_CRC64_ENA                   // CRC64 enable/disable
#endif /* _MY_CRC64_ENA */

#ifndef _MY_CRC64_TABLE_CALC_ENA
    #define _MY_CRC64_TABLE_CALC_ENA        // CRC64 table fast calculation enable/disable
#endif /* _MY_CRC64_TABLE_CALC_ENA */

#ifndef _MY_CRC64_GENERIC_CALC_ENA
    #define _MY_CRC64_GENERIC_CALC_ENA      // CRC64 generic slow calculation enable/disable
#endif /* _MY_CRC64_GENERIC_CALC_ENA */



/*
 * ******************************************************************************
 * check user input (DO NOT CHANGE THIS SECTION !!!!!!!)
 * ******************************************************************************
 */



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

#ifdef _MY_CRC64_ENA

    #if !defined (_MY_CRC64_TABLE_CALC_ENA) && !defined(_MY_CRC64_GENERIC_CALC_ENA)
        #error At least one implementation CRC64 required
    #endif // !defined (_MY_CRC64_TABLE_CALC_ENA) && !defined(_MY_CRC64_GENERIC_CALC_ENA)

#endif /* _MY_CRC64_ENA */


#endif /* __MY_CRC_PORT_H__*/

