#ifndef __MY_CRC_TEST_H__
#define __MY_CRC_TEST_H__

#include "my_crc_port.h"

#ifndef _MY_CRC_TEST_DISABLE
/*
 * ****************************************
 * two stage test:
 *  1) known input data --> "123456789" and compleate with CRCXCHECK
 *  2) random length & random data test
 *
 *      * return 1 if test compleated, 0 - not compleated
 * ****************************************
 */

int crc_test(); // this is a test for your platform to check the performance CRC8/16/32/64


#endif /* _MY_CRC_TEST_DISABLE */

#endif /* __MY_CRC_TEST_H__*/

