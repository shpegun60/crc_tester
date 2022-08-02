#include "my_ctype_cast.h"

// conversation types to one type
f32 getDynamicCastToFloat(void* data, u8 type)
{

    f32 value;

    switch(type) {

    case UINT8_TYPE:
        value = FLOAT_TYPE_C( *UINT8_TYPE_DC(data) );
        break;

    case UINT16_TYPE:
        value = FLOAT_TYPE_C( *UINT16_TYPE_DC(data) );
        break;

    case UINT32_TYPE:
        value = FLOAT_TYPE_C( *UINT32_TYPE_DC(data) );
        break;

    case UINT64_TYPE:
        value = FLOAT_TYPE_C( *UINT64_TYPE_DC(data) );
        break;

    case CHAR_TYPE:
        value = FLOAT_TYPE_C( *CHAR_TYPE_DC(data) );
        break;

    case INT8_TYPE:
        value = FLOAT_TYPE_C( *INT8_TYPE_DC(data) );
        break;

    case INT16_TYPE:
        value = FLOAT_TYPE_C( *INT16_TYPE_DC(data) );
        break;

    case INT32_TYPE:
        value = FLOAT_TYPE_C( *INT32_TYPE_DC(data) );
        break;

    case INT64_TYPE:
        value = FLOAT_TYPE_C( *INT64_TYPE_DC(data) );
        break;

    case FLOAT_TYPE:
        value = FLOAT_TYPE_C( *FLOAT_TYPE_DC(data) );
        break;

    case DOUBLE_TYPE:
        value = FLOAT_TYPE_C( *DOUBLE_TYPE_DC(data) );
        break;

    case LONG_DOUBLE_TYPE:
        value = FLOAT_TYPE_C( *LONG_DOUBLE_TYPE_DC(data) );
        break;

    case UINT24_TYPE:
        value = FLOAT_TYPE_C( *UINT24_TYPE_DC(data) );
        break;

    case INT24_TYPE:
        value = FLOAT_TYPE_C( *INT24_TYPE_DC(data) );
        break;

    case REG_TYPE:
        value = FLOAT_TYPE_C( *REG_TYPE_DC(data) );
        break;

    case SREG_TYPE:
        value = FLOAT_TYPE_C( *SREG_TYPE_DC(data) );
        break;

    case BOOL_TYPE:
        value = FLOAT_TYPE_C( *BOOL_TYPE_DC(data) );
        break;

    default:
        value = FLOAT_TYPE_C(0.0f);
        break;
    }

    return value;
}

// conversation types to one type
f64 getDynamicCastToDouble(void* data, u8 type)
{

    f64 value;

    switch(type) {

    case UINT8_TYPE:
        value = DOUBLE_TYPE_C( *UINT8_TYPE_DC(data) );
        break;

    case UINT16_TYPE:
        value = DOUBLE_TYPE_C( *UINT16_TYPE_DC(data) );
        break;

    case UINT32_TYPE:
        value = DOUBLE_TYPE_C( *UINT32_TYPE_DC(data) );
        break;

    case UINT64_TYPE:
        value = DOUBLE_TYPE_C( *UINT64_TYPE_DC(data) );
        break;

    case CHAR_TYPE:
        value = DOUBLE_TYPE_C( *CHAR_TYPE_DC(data) );
        break;

    case INT8_TYPE:
        value = DOUBLE_TYPE_C( *INT8_TYPE_DC(data) );
        break;

    case INT16_TYPE:
        value = DOUBLE_TYPE_C( *INT16_TYPE_DC(data) );
        break;

    case INT32_TYPE:
        value = DOUBLE_TYPE_C( *INT32_TYPE_DC(data) );
        break;

    case INT64_TYPE:
        value = DOUBLE_TYPE_C( *INT64_TYPE_DC(data) );
        break;

    case FLOAT_TYPE:
        value = DOUBLE_TYPE_C( *FLOAT_TYPE_DC(data) );
        break;

    case DOUBLE_TYPE:
        value = DOUBLE_TYPE_C( *DOUBLE_TYPE_DC(data) );
        break;

    case LONG_DOUBLE_TYPE:
        value = DOUBLE_TYPE_C( *LONG_DOUBLE_TYPE_DC(data) );
        break;

    case UINT24_TYPE:
        value = DOUBLE_TYPE_C( *UINT24_TYPE_DC(data) );
        break;

    case INT24_TYPE:
        value = DOUBLE_TYPE_C( *INT24_TYPE_DC(data) );
        break;

    case REG_TYPE:
        value = DOUBLE_TYPE_C( *REG_TYPE_DC(data) );
        break;

    case SREG_TYPE:
        value = DOUBLE_TYPE_C( *SREG_TYPE_DC(data) );
        break;

    case BOOL_TYPE:
        value = DOUBLE_TYPE_C( *BOOL_TYPE_DC(data) );
        break;

    default:
        value = DOUBLE_TYPE_C(0.0);
        break;
    }

    return value;
}


void writeFloatToDynamicCast(void* data, u8 type, f32 value)
{
    switch(type) {

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

    case FLOAT_TYPE:
        *FLOAT_TYPE_DC(data) = FLOAT_TYPE_C(value);
        break;

    case DOUBLE_TYPE:
        *DOUBLE_TYPE_DC(data) = DOUBLE_TYPE_C(value);
        break;

    case LONG_DOUBLE_TYPE:
        *LONG_DOUBLE_TYPE_DC(data) = LONG_DOUBLE_TYPE_C(value);
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

    case BOOL_TYPE:
        *BOOL_TYPE_DC(data) = SREG_TYPE_C(value);
        break;

    default:
        break;
    }
}

void writeDoubleToDynamicCast(void* data, u8 type, f64 value)
{
    switch(type) {

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

    case FLOAT_TYPE:
        *FLOAT_TYPE_DC(data) = FLOAT_TYPE_C(value);
        break;

    case DOUBLE_TYPE:
        *DOUBLE_TYPE_DC(data) = DOUBLE_TYPE_C(value);
        break;

    case LONG_DOUBLE_TYPE:
        *LONG_DOUBLE_TYPE_DC(data) = LONG_DOUBLE_TYPE_C(value);
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

    case BOOL_TYPE:
        *BOOL_TYPE_DC(data) = SREG_TYPE_C(value);
        break;

    default:
        break;
    }
}

