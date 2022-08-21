#include "my_ctypes.h"

/*
 * *************************************************
 * Define user copy
 * *************************************************
 */
C_INLINE void MY_CTYPE_USER_DATA_MEMCPY(reg n, u8* from, u8* to);
C_INLINE void MY_CTYPE_USER_DATA_REVCPY(reg n, u8* from, u8* to);
C_INLINE void MY_CTYPE_USER_DATA_COPY_REGISTER(u8* from, u8* to);

C_INLINE const u8* myCTypeGetTablePointer();
C_INLINE u8 getMYCTypeLen(u8 type);


const u8 typeLengthMappingArray[TYPE_ARRAY_LENGTH] = {
    (u8)0x00U,                   //VOID_TYPE          ((u8) 0)
    // unsigned type
    (u8)sizeof(u8),             //UINT8_TYPE          ((u8) 1)
    (u8)sizeof(u16),            //UINT16_TYPE         ((u8) 2)
    (u8)sizeof(u32),            //UINT32_TYPE         ((u8) 3)
    (u8)sizeof(u64),            //UINT64_TYPE         ((u8) 4)
    //signed type
    (u8)sizeof(c8),             //CHAR_TYPE           ((u8) 5)
    (u8)sizeof(i8),             //INT8_TYPE           ((u8) 6)
    (u8)sizeof(i16),            //INT16_TYPE          ((u8) 7)
    (u8)sizeof(i32),            //INT32_TYPE          ((u8) 8)
    (u8)sizeof(i64),            //INT64_TYPE          ((u8) 9)
    // floating point type
    (u8)sizeof(f32),            //FLOAT_TYPE          ((u8) 10)
    (u8)sizeof(f64),            //DOUBLE_TYPE         ((u8) 11)
    (u8)sizeof(f128),           //LONG_DOUBLE_TYPE    ((u8) 12)
    // other types
    (u8)sizeof(b),              //BOOL_TYPE           ((u8) 13)
    (u8)sizeof(i24),            //UINT24_TYPE         ((u8) 14)
    (u8)sizeof(u24),            //INT24_TYPE          ((u8) 15)
    (u8)sizeof(reg),            //REG_TYPE            ((u8) 16) // platform depend type
    (u8)sizeof(sreg)            //SREG_TYPE           ((u8) 17) // platform depend type
};

// copy types -------------------------------------------------
void cTypeMemcpy(u8 type, u8* from, u8* to)
{
    u8 len = getMYCTypeLen(type);
    MY_CTYPE_USER_DATA_MEMCPY(len, from, to);
}

void cTypeRevcpy(u8 type, u8* from, u8* to)
{
    u8 len = getMYCTypeLen(type);
    MY_CTYPE_USER_DATA_REVCPY(len, from, to);
}

// init data type ---------------------------------------
void cTypePointerInit(u8 type, u8* ptr)
{
    u8 len = getMYCTypeLen(type);

    while(len--) {
        *ptr++ = ((u8)0);
    }
}

// init data sizeof ---------------------------------------
void pointerInit(reg n, u8* ptr)
{
    while(n--) {
        *ptr++ = ((u8)0);
    }
}


// string compleate------------------------------------------
u8 cTypeStrnCmp(reg n, const c8* str1, const c8* str2)
{
    while(n--) {
        if(*str1++ != *str2++) {
            return ((u8)1);
        }
    }
    return ((u8)0);
}

u8 cTypeStrnRevCmp(reg n, const c8* str1, const c8* str2)
{
    while(n--) {
        if(*str1++ != *(str2 + n)) {
            return ((u8)1);
        }
    }
    return ((u8)0);
}


