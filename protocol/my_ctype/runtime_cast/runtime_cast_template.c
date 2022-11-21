#include "preprocessor_template.h"
#include "my_ctype_cast.h"



#ifndef T
#   define T int
#endif /* T */

// read from (void *) to some type
T TEMPLATE(readRuntimeCast, T)(const void* const data, const reg type, reg* const ok)
{

    T value;
    (*ok) = 1;

    switch(type) {

    // unsigned type---------------------------------------------------------------------------------------------
    case UINT8_TYPE:
        value = (T)( *UINT8_TYPE_DC(data) );
        break;

    case UINT16_TYPE:
        value = (T)( *UINT16_TYPE_DC(data) );
        break;

    case UINT32_TYPE:
        value = (T)( *UINT32_TYPE_DC(data) );
        break;

    case UINT64_TYPE:
        value = (T)( *UINT64_TYPE_DC(data) );
        break;

    // signed type-----------------------------------------------------------------------------------------------
    case CHAR_TYPE:
        value = (T)( *CHAR_TYPE_DC(data) );
        break;

    case INT8_TYPE:
        value = (T)( *INT8_TYPE_DC(data) );
        break;

    case INT16_TYPE:
        value = (T)( *INT16_TYPE_DC(data) );
        break;

    case INT32_TYPE:
        value = (T)( *INT32_TYPE_DC(data) );
        break;

    case INT64_TYPE:
        value = (T)( *INT64_TYPE_DC(data) );
        break;

    // floating point type-------------------------------------------------------------------------------------
    case FLOAT_TYPE:
        value = (T)( *FLOAT_TYPE_DC(data) );
        break;

    case DOUBLE_TYPE:
        value = (T)( *DOUBLE_TYPE_DC(data) );
        break;

    case LONG_DOUBLE_TYPE:
        value = (T)( *LONG_DOUBLE_TYPE_DC(data) );
        break;

    // other types --------------------------------------------------------------------------------------------
    case BOOL_TYPE:
        value = (T)( *BOOL_TYPE_DC(data) );
        break;

    case UINT24_TYPE:
        value = (T)( *UINT24_TYPE_DC(data) );
        break;

    case INT24_TYPE:
        value = (T)( *INT24_TYPE_DC(data) );
        break;

    case REG_TYPE:
        value = (T)( *REG_TYPE_DC(data) );
        break;

    case SREG_TYPE:
        value = (T)( *SREG_TYPE_DC(data) );
        break;

    default:
        value = (T)(0);
        (*ok) = 0;
        break;
    }

    return value;
}

// write from some type to (void *)
reg TEMPLATE(writeRuntimeCast, T)(void* const data, const reg type, const T value)
{

    reg ok = 1;

    switch(type) {

    // unsigned type---------------------------------------------------------------------------------------------
    case UINT8_TYPE:
        *UINT8_TYPE_DC(data) = UINT8_TYPE_C(value);
        break;

    case UINT16_TYPE:
        *UINT16_TYPE_DC(data) = UINT16_TYPE_C(value);
        break;

    case UINT32_TYPE:
        *UINT32_TYPE_DC(data) = UINT32_TYPE_C(value);
        break;

    case UINT64_TYPE:
        *UINT64_TYPE_DC(data) = UINT64_TYPE_C(value);
        break;

    // signed type-----------------------------------------------------------------------------------------------
    case CHAR_TYPE:
        *CHAR_TYPE_DC(data) = CHAR_TYPE_C(value);
        break;

    case INT8_TYPE:
        *INT8_TYPE_DC(data) = INT8_TYPE_C(value);
        break;

    case INT16_TYPE:
        *INT16_TYPE_DC(data) = INT16_TYPE_C(value);
        break;

    case INT32_TYPE:
        *INT32_TYPE_DC(data) = INT32_TYPE_C(value);
        break;

    case INT64_TYPE:
        *INT64_TYPE_DC(data) = INT64_TYPE_C(value);
        break;

    // floating point type-------------------------------------------------------------------------------------
    case FLOAT_TYPE:
        *FLOAT_TYPE_DC(data) = FLOAT_TYPE_C(value);
        break;

    case DOUBLE_TYPE:
        *DOUBLE_TYPE_DC(data) = DOUBLE_TYPE_C(value);
        break;

    case LONG_DOUBLE_TYPE:
        *LONG_DOUBLE_TYPE_DC(data) = LONG_DOUBLE_TYPE_C(value);
        break;

    // other types --------------------------------------------------------------------------------------------
    case BOOL_TYPE:
        *BOOL_TYPE_DC(data) = BOOL_TYPE_C(value);
        break;

    case UINT24_TYPE:
        *UINT24_TYPE_DC(data) = UINT24_TYPE_C(value);
        break;

    case INT24_TYPE:
        *INT24_TYPE_DC(data) = INT24_TYPE_C(value);
        break;

    case REG_TYPE:
        *REG_TYPE_DC(data) = REG_TYPE_C(value);
        break;

    case SREG_TYPE:
        *SREG_TYPE_DC(data) = SREG_TYPE_C(value);
        break;

    default:
        ok = 0;
        break;
    }

    return ok;
}


#undef T

