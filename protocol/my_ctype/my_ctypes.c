#include "my_ctypes.h"

/*
 * *************************************************
 * Define user copy
 * *************************************************
 */

const reg typeLengthMappingArray[TYPE_ARRAY_LENGTH] = {
    0x00U,                  //VOID_TYPE           (0)
    // unsigned type
    sizeof(u8),             //UINT8_TYPE          (1)
    sizeof(u16),            //UINT16_TYPE         (2)
    sizeof(u32),            //UINT32_TYPE         (3)
    sizeof(u64),            //UINT64_TYPE         (4)
    //signed type
    sizeof(c8),             //CHAR_TYPE           (5)
    sizeof(i8),             //INT8_TYPE           (6)
    sizeof(i16),            //INT16_TYPE          (7)
    sizeof(i32),            //INT32_TYPE          (8)
    sizeof(i64),            //INT64_TYPE          (9)
    // floating point type
    sizeof(f32),            //FLOAT_TYPE          (10)
    sizeof(f64),            //DOUBLE_TYPE         (11)
    sizeof(f128),           //LONG_DOUBLE_TYPE    (12)
    // other types
    sizeof(b),              //BOOL_TYPE           (13)
    sizeof(i24),            //UINT24_TYPE         (14)
    sizeof(u24),            //INT24_TYPE          (15)
    sizeof(reg),            //REG_TYPE            (16) // platform depend type
    sizeof(sreg)            //SREG_TYPE           (17) // platform depend type
};

// copy types -------------------------------------------------
void cTypeMemcpy(reg type, u8* from, u8* to)
{
    reg len = getMYCTypeLen(type);
    MY_CTYPE_USER_DATA_MEMCPY(len, from, to);
}

void cTypeRevcpy(reg type, u8* from, u8* to)
{
    reg len = getMYCTypeLen(type);
    MY_CTYPE_USER_DATA_REVCPY(len, from, to);
}

// init data type ---------------------------------------
void cTypePointerInit(reg type, u8* ptr)
{
    reg len = getMYCTypeLen(type);

    while(len--) {
        *ptr++ = 0;
    }
}

// init data sizeof ---------------------------------------
void pointerInit(reg n, u8* ptr)
{
    while(n--) {
        *ptr++ = 0;
    }
}

// fill data sizeof ---------------------------------------
void pointerFill(reg n, u8* ptr, u8 data)
{
    while(n--) {
        *ptr++ = data;
    }
}


// string compleate------------------------------------------
int cTypeStrnCmp(reg n, const c8* str1, const c8* str2)
{
    while(n--) {
        if(*str1++ != *str2++) {
            return 1;
        }
    }
    return 0;
}

int cTypeStrnRevCmp(reg n, const c8* str1, const c8* str2)
{
    while(n--) {
        if(*str1++ != *(str2 + n)) {
            return 1;
        }
    }
    return 0;
}


