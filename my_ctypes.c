#include "my_ctypes.h"

const u8 typeLengthMappingArray[TYPE_ARRAY_LENGTH] = {
    (u8)0x00,                   //VOID_TYPE 			((u8) 0)
    // unsigned type
    (u8)sizeof(u8),             //UINT8_TYPE            ((u8) 1)
    (u8)sizeof(u16),            //UINT16_TYPE           ((u8) 2)
    (u8)sizeof(u32),            //UINT32_TYPE    		((u8) 3)
    (u8)sizeof(u64),            //UINT64_TYPE    		((u8) 4)
    //signed type
    (u8)sizeof(c),              //CHAR_TYPE 			((u8) 5)
    (u8)sizeof(i8),             //INT8_TYPE 			((u8) 6)
    (u8)sizeof(i16),            //INT16_TYPE 			((u8) 7)
    (u8)sizeof(i32),            //INT32_TYPE 			((u8) 8)
    (u8)sizeof(i64),            //INT64_TYPE 			((u8) 9)
    // floating point type
    (u8)sizeof(f32),            //FLOAT_TYPE 			((u8) 10)
    (u8)sizeof(f64),            //DOUBLE_TYPE           ((u8) 11)
    (u8)sizeof(f128),           //LONG_DOUBLE_TYPE      ((u8) 12)
    // other types
    (u8)sizeof(b),              //BOOL_TYPE 			((u8) 13)
    (u8)(sizeof(i24) - 0x01),   //UINT24_TYPE           ((u8) 14)
    (u8)(sizeof(u24) - 0x01),   //INT24_TYPE 			((u8) 15)
    (u8)sizeof(reg)             //REG_TYPE              ((u8) 16)
};
