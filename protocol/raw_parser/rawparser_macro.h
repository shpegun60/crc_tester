#ifndef __RAWPARSER_MACRO_H__
#define __RAWPARSER_MACRO_H__ 1

#include "rawparser_port.h"
#include "preprocessor.h"
#include "my_ctypes.h"

#include <stdio.h>


#define $POINTER(x) x
#define $STATIC_ARRAY(x) x
#define $CONST(x) x

//sizeof merge macro -----------------------------------------------------------------------------------------------
#define RAW_P_MERGE_OPERATION(op, sep, ...) \
 PREPROCESSOR_IF(PREPROCESSOR_ARGS_NOT_EMPTY(__VA_ARGS__))(PREPROCESSOR_EVAL(RAW_P_MERGE_OPERATION_INNER(op, sep, ##__VA_ARGS__)))

#define RAW_P_MERGE_OPERATION_INNER(op, sep, cur_val, ...) \
    PREPROCESSOR_IF_ELSE(PREPROCESSOR_EQUAL(cur_val, $POINTER))( \
        RAW_P_MERGE_OPERATION_POINTER_INNER(op, sep, ##__VA_ARGS__),\
        PREPROCESSOR_IF_ELSE(PREPROCESSOR_EQUAL(cur_val, $STATIC_ARRAY))(\
            RAW_P_MERGE_OPERATION_STATIC_ARRAY_INNER(op, sep, ##__VA_ARGS__),\
            PREPROCESSOR_IF_ELSE(PREPROCESSOR_EQUAL(cur_val, $CONST))(\
                RAW_P_MERGE_OPERATION_CONST_INNER(op, sep, ##__VA_ARGS__),\
                op(cur_val)\
                RAW_P_MERGE_OPERATION_ELSE_INNER(op, sep, ##__VA_ARGS__)\
            )\
        )\
    )

#define _RAW_P_MERGE_OPERATION_INNER() RAW_P_MERGE_OPERATION_INNER

#define RAW_P_MERGE_OPERATION_ELSE_INNER(op, sep, ...)\
    PREPROCESSOR_IF(PREPROCESSOR_ARGS_NOT_EMPTY(__VA_ARGS__))( \
      sep() PREPROCESSOR_DEFER5(_RAW_P_MERGE_OPERATION_INNER)()(op, sep, ##__VA_ARGS__) \
    )

#define RAW_P_MERGE_OPERATION_POINTER_INNER(op, sep, len, arr, ...)\
    len\
    RAW_P_MERGE_OPERATION_ELSE_INNER(op, sep, ##__VA_ARGS__)

#define RAW_P_MERGE_OPERATION_STATIC_ARRAY_INNER(op, sep, arr, ...)\
    op(arr)\
    RAW_P_MERGE_OPERATION_ELSE_INNER(op, sep, ##__VA_ARGS__)

#define RAW_P_MERGE_OPERATION_CONST_INNER(op, sep, val, type, ...)\
    op(type)\
    RAW_P_MERGE_OPERATION_ELSE_INNER(op, sep, ##__VA_ARGS__)


// write merge macro------------------------------------------------------------------------------------------------
#define RAW_P_MERGE_WRITE_OPERATION(foo, par1, op, cast, sep, ...) \
    PREPROCESSOR_IF(PREPROCESSOR_ARGS_NOT_EMPTY(__VA_ARGS__))(PREPROCESSOR_EVAL(RAW_P_MERGE_WRITE_OPERATION_INNER(foo, par1, op, cast, sep, 0, ##__VA_ARGS__)))
#define RAW_P_MERGE_WRITE_OPERATION_INNER(foo, par1, op, cast, sep, count, cur_val, ...) \
    PREPROCESSOR_IF_ELSE(PREPROCESSOR_EQUAL(cur_val, $POINTER))( \
        RAW_P_MERGE_WRITE_OPERATION_POINTER_INNER(foo, par1, op, cast, sep, count, ##__VA_ARGS__),\
        PREPROCESSOR_IF_ELSE(PREPROCESSOR_EQUAL(cur_val, $STATIC_ARRAY))( \
            RAW_P_MERGE_WRITE_OPERATION_STATIC_ARRAY_INNER(foo, par1, op, cast, sep, count, ##__VA_ARGS__),\
            PREPROCESSOR_IF_ELSE(PREPROCESSOR_EQUAL(cur_val, $CONST))(\
                RAW_P_MERGE_WRITE_OPERATION_CONST_INNER(foo, par1, op, cast, sep, count, ##__VA_ARGS__),\
                foo(par1, op(cur_val), op(cur_val), (cast)&cur_val) \
                RAW_P_MERGE_WRITE_OPERATION_ELSE(foo, par1, op, cast, sep, count, ##__VA_ARGS__)\
            )\
        )\
    )
#define _RAW_P_MERGE_WRITE_OPERATION_INNER() RAW_P_MERGE_WRITE_OPERATION_INNER


#define RAW_P_MERGE_WRITE_OPERATION_POINTER_INNER(foo, par1, op, cast, sep, count, len, arr, ...)\
        foo(par1, len, op(arr[0]), (cast)arr)\
        RAW_P_MERGE_WRITE_OPERATION_ELSE(foo, par1, op, cast, sep, count, ##__VA_ARGS__)

#define RAW_P_MERGE_WRITE_OPERATION_STATIC_ARRAY_INNER(foo, par1, op, cast, sep, count, arr, ...)\
        foo(par1, op(arr), op(arr[0]), (cast)arr)\
        RAW_P_MERGE_WRITE_OPERATION_ELSE(foo, par1, op, cast, sep, count, ##__VA_ARGS__)

#define RAW_P_MERGE_WRITE_OPERATION_CONST_INNER(foo, par1, op, cast, sep, count, val, type, ...)\
    type PREPROCESSOR_CONCAT(const_,count) = val;\
    foo(par1, op(type), op(type), (cast)&PREPROCESSOR_CONCAT(const_,count))\
    RAW_P_MERGE_WRITE_OPERATION_ELSE(foo, par1, op, cast, sep, count, ##__VA_ARGS__)


#define RAW_P_MERGE_WRITE_OPERATION_ELSE(foo, par1, op, cast, sep, count, ...)\
    PREPROCESSOR_IF(PREPROCESSOR_ARGS_NOT_EMPTY(__VA_ARGS__))( \
        sep() PREPROCESSOR_DEFER5(_RAW_P_MERGE_WRITE_OPERATION_INNER)()(foo, par1, op, cast, sep, PREPROCESSOR_INC(count), ##__VA_ARGS__) \
    )


//-----------------------------------------------------------------------------------------------------------------
#define WRITE_PAYLOAD_MACRO(foo, par1, beforeExpr, afterExpr, ...)\
    do{\
        reg totalSize = RAW_P_MERGE_OPERATION(sizeof, PREPROCESSOR_PLUS, ##__VA_ARGS__);\
        beforeExpr;\
        RAW_P_MERGE_WRITE_OPERATION(foo, par1, sizeof, u8*, PREPROCESSOR_COMMA_POINT, ##__VA_ARGS__);\
        afterExpr;\
        (void)totalSize;\
    }while(0U)

//-----------------------------------------------------------------------------------------------------------------
#define READ_PAYLOAD_MACRO(foo, par1, beforeExpr, afterExpr, ...) WRITE_PAYLOAD_MACRO(foo, par1, beforeExpr, afterExpr, __VA_ARGS__)



#ifndef D_RAW_P_TEST_DISABLE
int rawParserMacroTest();
#endif /* D_RAW_P_TEST_DISABLE */

#endif // __RAWPARSER_MACRO_H__
