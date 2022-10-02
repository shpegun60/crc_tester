#include "preprocessor_template.h"
#include "my_ctypes.h"


#ifndef T
#   define T int
#endif /* T */

// reed from (void *) to some type
T TEMPLATE(reedRuntimeCast, T)(void* data, u8 type)
{

    T value;

    switch(type) {

    // unsigned type---------------------------------------------------------------------------------------------
    case UINT8_TYPE:
        value = (T)( *((u8*)data) );
        break;

    case UINT16_TYPE:
        value = (T)( *((u16*)data) );
        break;

    case UINT32_TYPE:
        value = (T)( *((u32*)data) );
        break;

    case UINT64_TYPE:
        value = (T)( *((u64*)data) );
        break;

    // signed type-----------------------------------------------------------------------------------------------
    case CHAR_TYPE:
        value = (T)( *((c8*)data) );
        break;

    case INT8_TYPE:
        value = (T)( *((i8*)data) );
        break;

    case INT16_TYPE:
        value = (T)( *((i16*)data) );
        break;

    case INT32_TYPE:
        value = (T)( *((i32*)data) );
        break;

    case INT64_TYPE:
        value = (T)( *((i64*)data) );
        break;

    // floating point type-------------------------------------------------------------------------------------
    case FLOAT_TYPE:
        value = (T)( *((f32*)data) );
        break;

    case DOUBLE_TYPE:
        value = (T)( *((f64*)data) );
        break;

    case LONG_DOUBLE_TYPE:
        value = (T)( *((f128*)data) );
        break;

    // other types --------------------------------------------------------------------------------------------
    case BOOL_TYPE:
        value = (T)( *((b*)data) );
        break;

    case UINT24_TYPE:
        value = (T)( *((u24*)data) );
        break;

    case INT24_TYPE:
        value = (T)( *((i24*)data) );
        break;

    case REG_TYPE:
        value = (T)( *((reg*)data) );
        break;

    case SREG_TYPE:
        value = (T)( *((sreg*)data) );
        break;

    default:
        value = (T)(0);
        break;
    }

    return value;
}

// write from some type to (void *)
int TEMPLATE(writeRuntimeCast, T)(void* data, u8 type, T value)
{
    int ok = 1;

    switch(type) {

    // unsigned type---------------------------------------------------------------------------------------------
    case UINT8_TYPE:
        *((u8 *)data) = (u8)(value);
        break;

    case UINT16_TYPE:
        *((u16 *)data) = (u16)(value);
        break;

    case UINT32_TYPE:
        *((u32 *)data) = (u32)(value);
        break;

    case UINT64_TYPE:
        *((u64 *)data) = (u64)(value);
        break;

    // signed type-----------------------------------------------------------------------------------------------
    case CHAR_TYPE:
        *((c8 *)data) = (c8)(value);
        break;

    case INT8_TYPE:
        *((i8 *)data) = (i8)(value);
        break;

    case INT16_TYPE:
        *((i16 *)data) = (i16)(value);
        break;

    case INT32_TYPE:
        *((i32 *)data) = (i32)(value);
        break;

    case INT64_TYPE:
        *((i64 *)data) = (i64)(value);
        break;

    // floating point type-------------------------------------------------------------------------------------
    case FLOAT_TYPE:
        *((f32 *)data) = (f32)(value);
        break;

    case DOUBLE_TYPE:
        *((f64 *)data) = (f64)(value);
        break;

    case LONG_DOUBLE_TYPE:
        *((f128 *)data) = (f128)(value);
        break;

    // other types --------------------------------------------------------------------------------------------
    case BOOL_TYPE:
        *((b *)data) = (b)(value);
        break;

    case UINT24_TYPE:
        *((u24 *)data) = (u24)(value);
        break;

    case INT24_TYPE:
        *((i24 *)data) = (i24)(value);
        break;

    case REG_TYPE:
        *((reg *)data) = (reg)(value);
        break;

    case SREG_TYPE:
        *((sreg *)data) = (sreg)(value);
        break;

    default:
        ok = 0;
        break;
    }

    return ok;
}


#undef T

