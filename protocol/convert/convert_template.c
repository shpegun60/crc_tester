#include "templates.h"
#include "my_ctypes.h"

#ifndef CONVERT_TEST_DISABLE
#include <stdlib.h>
#endif /* CONVERT_TEST_DISABLE */

#ifndef T
#define T u32
#endif /* T */

/*
 * ******************************************
 * LSB - first
 * ******************************************
 */
T TEMPLATE(convertReadLSB, T) (u8 *data, u16 *pos)
{
    T value;
    MY_CTYPE_USER_DATA_MEMCPY(sizeof(T), (u8*)(data + *pos), (u8 *)(&value));
    *pos += sizeof(T);
    return value;
}

void TEMPLATE(convertWriteLSB, T) (u8 *data, u16 *pos, T value)
{
    MY_CTYPE_USER_DATA_MEMCPY(sizeof(T), (u8*)(&value), (u8*)(data + *pos));
    *pos += sizeof(T);
}

// position not a pointer
T TEMPLATE(convertReadLSB_cpos, T) (u8 *data, u16 pos)
{
    T value;
    MY_CTYPE_USER_DATA_MEMCPY(sizeof(T), (u8*)(data + pos), (u8 *)(&value));
    return value;
}

void TEMPLATE(convertWriteLSB_cpos, T) (u8 *data, u16 pos, T value)
{
    MY_CTYPE_USER_DATA_MEMCPY(sizeof(T), (u8*)(&value), (u8*)(data + pos));
}


/*
 * ******************************************
 * MSB - first
 * ******************************************
 */

T TEMPLATE(convertReadMSB, T) (u8 *data, u16 *pos)
{
    T value;
    MY_CTYPE_USER_DATA_REVCPY(sizeof(T), (u8*)(data + *pos), (u8 *)(&value));
    *pos += sizeof(T);
    return value;
}

void TEMPLATE(convertWriteMSB, T) (u8 *data, u16 *pos, T value)
{
    MY_CTYPE_USER_DATA_REVCPY(sizeof(T), (u8*)(&value), (u8*)(data + *pos));
    *pos += sizeof(T);
}

// position not a pointer
T TEMPLATE(convertReadMSB_cpos, T) (u8 *data, u16 pos)
{
    T value;
    MY_CTYPE_USER_DATA_REVCPY(sizeof(T), (u8*)(data + pos), (u8 *)(&value));
    return value;
}

void TEMPLATE(convertWriteMSB_cpos, T) (u8 *data, u16 pos, T value)
{
    MY_CTYPE_USER_DATA_REVCPY(sizeof(T), (u8*)(&value), (u8*)(data + pos));
}


#ifndef CONVERT_TEST_DISABLE

/*
 * ******************************************
 * test
 * ******************************************
 */
int TEMPLATE(convertTest, T) (int testN)
{
    srand(sizeof(T)); // use type size as seed for random generator

    u8 buff[sizeof(T) + 1];
    u16 pos = 0;

    T value = (T)(sizeof(T));
    T valueLast = value;
    T value_check = 0;


    int testCounter = 0;
    while(testN--) {
        // lsb--------------------------------------------------------
        TEMPLATE(convertWriteLSB, T) (buff, &pos, value);
        if(pos != sizeof(T)) {
            ++testCounter;
        }
        pos = 0;
        value_check = TEMPLATE(convertReadLSB, T) (buff, &pos);
        if(value_check != value) {
            ++testCounter;
        }
        pos = 0;

        // cpos
        TEMPLATE(convertWriteLSB_cpos, T) (buff, pos, value);
        pos = 0;
        value_check = TEMPLATE(convertReadLSB_cpos, T) (buff, pos);
        if(value_check != value) {
            ++testCounter;
        }
        pos = 0;



        // msb---------------------------------------------------------
        TEMPLATE(convertWriteMSB, T) (buff, &pos, value);
        if(pos != sizeof(T)) {
            ++testCounter;
        }
        pos = 0;
        value_check = TEMPLATE(convertReadMSB, T) (buff, &pos);
        if(value_check != value) {
            ++testCounter;
        }
        pos = 0;

        // cpos
        TEMPLATE(convertWriteMSB_cpos, T) (buff, pos, value);
        pos = 0;
        value_check = TEMPLATE(convertReadMSB_cpos, T) (buff, pos);
        if(value_check != value) {
            ++testCounter;
        }
        pos = 0;

        valueLast = value;
        value = (T)(rand());
        while((value == 0) || (valueLast == value)) {
            value = (T)(rand());
        }
    }

    return testCounter;
}
#endif /* CONVERT_TEST_DISABLE */


#undef T

