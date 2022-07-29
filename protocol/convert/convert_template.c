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
T TEMPLATE(convertRead_LSB, T) (u8 *data, u16 *pos)
{
    T value;
    MY_CTYPE_USER_DATA_MEMCPY(sizeof(T), (u8*)(data + *pos), (u8 *)(&value));
    *pos += sizeof(T);
    return value;
}

void TEMPLATE(convertWrite_LSB, T) (u8 *data, u16 *pos, T value)
{
    MY_CTYPE_USER_DATA_MEMCPY(sizeof(T), (u8*)(&value), (u8*)(data + *pos));
    *pos += sizeof(T);
}

// position not a pointer
T TEMPLATE(convertRead_cpos_LSB, T) (u8 *data, u16 pos)
{
    T value;
    MY_CTYPE_USER_DATA_MEMCPY(sizeof(T), (u8*)(data + pos), (u8 *)(&value));
    return value;
}

void TEMPLATE(convertWrite_cpos_LSB, T) (u8 *data, u16 pos, T value)
{
    MY_CTYPE_USER_DATA_MEMCPY(sizeof(T), (u8*)(&value), (u8*)(data + pos));
}


/*
 * ******************************************
 * MSB - first
 * ******************************************
 */

T TEMPLATE(convertRead_MSB, T) (u8 *data, u16 *pos)
{
    T value;
    MY_CTYPE_USER_DATA_REVCPY(sizeof(T), (u8*)(data + *pos), (u8 *)(&value));
    *pos += sizeof(T);
    return value;
}

void TEMPLATE(convertWrite_MSB, T) (u8 *data, u16 *pos, T value)
{
    MY_CTYPE_USER_DATA_REVCPY(sizeof(T), (u8*)(&value), (u8*)(data + *pos));
    *pos += sizeof(T);
}

// position not a pointer
T TEMPLATE(convertRead_cpos_MSB, T) (u8 *data, u16 pos)
{
    T value;
    MY_CTYPE_USER_DATA_REVCPY(sizeof(T), (u8*)(data + pos), (u8 *)(&value));
    return value;
}

void TEMPLATE(convertWrite_cpos_MSB, T) (u8 *data, u16 pos, T value)
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
    T valueLast = 0;
    T value_check = 0;


    int testCounter = 0;
    while(testN--) {
        // lsb--------------------------------------------------------
        TEMPLATE(convertWrite_LSB, T) (buff, &pos, value);
        if(pos != sizeof(T)) {
            ++testCounter;
        }
        pos = 0;
        value_check = TEMPLATE(convertRead_LSB, T) (buff, &pos);
        if(value_check != value) {
            ++testCounter;
        }
        pos = 0;

        // cpos
        TEMPLATE(convertWrite_cpos_LSB, T) (buff, pos, value);
        pos = 0;
        value_check = TEMPLATE(convertRead_cpos_LSB, T) (buff, pos);
        if(value_check != value) {
            ++testCounter;
        }
        pos = 0;



        // msb---------------------------------------------------------
        TEMPLATE(convertWrite_MSB, T) (buff, &pos, value);
        if(pos != sizeof(T)) {
            ++testCounter;
        }
        pos = 0;
        value_check = TEMPLATE(convertRead_MSB, T) (buff, &pos);
        if(value_check != value) {
            ++testCounter;
        }
        pos = 0;

        // cpos
        TEMPLATE(convertWrite_cpos_MSB, T) (buff, pos, value);
        pos = 0;
        value_check = TEMPLATE(convertRead_cpos_MSB, T) (buff, pos);
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

