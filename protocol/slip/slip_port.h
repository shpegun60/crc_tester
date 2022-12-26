#ifndef __SLIP_PORT_H__
#define __SLIP_PORT_H__

/*
***************************************************************************************************
    CRC SETTINGS
***************************************************************************************************
*/

#define SLIP_P_CRC_ENA

#ifdef SLIP_P_CRC_ENA

    //#define SLIP_USE_CRC8           // enable crc8, check if multiple use crc then error
    //#define SLIP_USE_CRC16          // enable crc16, check if multiple use crc then error
    //#define SLIP_USE_CRC32          // enable crc32, check if multiple use crc then error
    #define SLIP_USE_CRC64            // enable crc64, check if multiple use crc then error

#endif /* SLIP_P_CRC_ENA */

#endif /* __SLIP_PORT_H__ */
