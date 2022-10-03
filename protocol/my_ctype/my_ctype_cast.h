#ifndef _MY_CTYPE_CAST_H_
#define _MY_CTYPE_CAST_H_ 

#include "my_ctypes.h"

// type literals ------------------------------------------
#define S8_C(x)     x
#define U8_C(x)     x ## U
#define S16_C(x)    x
#define U16_C(x)    x ## U
#define S32_C(x)    x
#define U32_C(x)    x ## U
#define S64_C(x)    x ## LL
#define U64_C(x)    x ## ULL
#define F32_C(x)    x ## f
#define F64_C(x)    x
#define F128_C(x)   x ## L

// type cast ------------------------------------------
#define VOID_TYPE_C(x)              ((uni)(x))
// unsigned type
#define UINT8_TYPE_C(x)             ((u8)(x))
#define UINT16_TYPE_C(x)            ((u16)(x))
#define UINT32_TYPE_C(x)            ((u32)(x))
#define UINT64_TYPE_C(x)            ((u64)(x))
//signed type
#define CHAR_TYPE_C(x)              ((c8)(x))
#define INT8_TYPE_C(x)              ((i8)(x))
#define INT16_TYPE_C(x) 			((i16)(x))
#define INT32_TYPE_C(x) 			((i32)(x))
#define INT64_TYPE_C(x) 			((i64)(x))
// floating point type
#define FLOAT_TYPE_C(x) 			((f32)(x))
#define DOUBLE_TYPE_C(x)            ((f64)(x))
#define LONG_DOUBLE_TYPE_C(x)       ((f128)(x)) // platform depend type
// other types
#define BOOL_TYPE_C(x)              ((b)(x))
#define UINT24_TYPE_C(x)            ((u24)(x))
#define INT24_TYPE_C(x)             ((i24)(x))
#define REG_TYPE_C(x)               ((reg)(x))  // platform depend type
#define SREG_TYPE_C(x)              ((sreg)(x)) // platform depend type
//--------------------------------------------------------


// type dynamic cast ------------------------------------------
#define VOID_TYPE_DC(x)                 ((uni*)(x))
// unsigned type
#define UINT8_TYPE_DC(x)                ((u8*)(x))
#define UINT16_TYPE_DC(x)               ((u16*)(x))
#define UINT32_TYPE_DC(x)               ((u32*)(x))
#define UINT64_TYPE_DC(x)               ((u64*)(x))
//signed type
#define CHAR_TYPE_DC(x)                 ((c8*)(x))
#define INT8_TYPE_DC(x)                 ((i8*)(x))
#define INT16_TYPE_DC(x)                ((i16*)(x))
#define INT32_TYPE_DC(x)                ((i32*)(x))
#define INT64_TYPE_DC(x)                ((i64*)(x))
// floating point type
#define FLOAT_TYPE_DC(x)                ((f32*)(x))
#define DOUBLE_TYPE_DC(x)               ((f64*)(x))
#define LONG_DOUBLE_TYPE_DC(x)          ((f128*)(x)) // platform depend type
// other types
#define BOOL_TYPE_DC(x)                 ((b*)(x))
#define UINT24_TYPE_DC(x)               ((u24*)(x))
#define INT24_TYPE_DC(x)                ((i24*)(x))
#define REG_TYPE_DC(x)                  ((reg*)(x)) // platform depend type
#define SREG_TYPE_DC(x)                 ((sreg*)(x)) // platform depend type
//--------------------------------------------------------


#endif /* _MY_CTYPE_CAST_H_ */
