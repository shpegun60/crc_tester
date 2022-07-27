#include "templates.h"
#include "my_ctypes.h"

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

// position not pointer
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

// position not pointer
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


#undef T

