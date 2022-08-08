#ifndef __RAWPARSER_MACRO_H__
#define __RAWPARSER_MACRO_H__

#include "my_ctypes.h"
#include "preprocessor_base.h"
#include <stdio.h>

#define $POINTER(x) x
#define $STATIC_ARRAY(x) x
#define $CONST(x) x

//sizeof merge macro -----------------------------------------------------------------------------------------------
#define MAP_MERGE_OPERATION(op, sep, ...) \
 IF(HAS_ARGS(__VA_ARGS__))(EVAL(MAP_MERGE_OPERATION_INNER(op, sep, ##__VA_ARGS__)))
#define MAP_MERGE_OPERATION_INNER(op, sep, cur_val, ...) \
    IF_ELSE(EQUAL(cur_val, $POINTER))( \
        MAP_MERGE_OPERATION_INNER_POINTER(op, sep, ##__VA_ARGS__),\
        IF_ELSE(EQUAL(cur_val, $STATIC_ARRAY))(\
            MAP_MERGE_OPERATION_INNER_STATIC_ARRAY(op, sep, ##__VA_ARGS__),\
            IF_ELSE(EQUAL(cur_val, $CONST))(\
                MAP_MERGE_OPERATION_INNER_CONST(op, sep, ##__VA_ARGS__),\
                op(cur_val)\
                MAP_MERGE_OPERATION_INNER_ELSE(op, sep, ##__VA_ARGS__)\
            )\
        )\
    )

#define _MAP_MERGE_OPERATION_INNER() MAP_MERGE_OPERATION_INNER

#define MAP_MERGE_OPERATION_INNER_ELSE(op, sep, ...)\
    IF(HAS_ARGS(__VA_ARGS__))( \
      sep() DEFER5(_MAP_MERGE_OPERATION_INNER)()(op, sep, ##__VA_ARGS__) \
    )

#define MAP_MERGE_OPERATION_INNER_POINTER(op, sep, len, arr, ...)\
    len\
    MAP_MERGE_OPERATION_INNER_ELSE(op, sep, ##__VA_ARGS__)

#define MAP_MERGE_OPERATION_INNER_STATIC_ARRAY(op, sep, arr, ...)\
    op(arr)\
    MAP_MERGE_OPERATION_INNER_ELSE(op, sep, ##__VA_ARGS__)

#define MAP_MERGE_OPERATION_INNER_CONST(op, sep, val, type, ...)\
    op(type)\
    MAP_MERGE_OPERATION_INNER_ELSE(op, sep, ##__VA_ARGS__)


// write merge macro------------------------------------------------------------------------------------------------
#define MAP_MERGE_OPERATION_ONE_PAR(op, par, cast, sep, ...) \
    IF(HAS_ARGS(__VA_ARGS__))(EVAL(MAP_MERGE_OPERATION_ONE_PAR_INNER(op, par, cast, sep, 0, ##__VA_ARGS__)))
#define MAP_MERGE_OPERATION_ONE_PAR_INNER(op, par, cast, sep, count, cur_val, ...) \
    IF_ELSE(EQUAL(cur_val, $POINTER))( \
        INNER_EXEC_POINTER(op, par, cast, sep, count, ##__VA_ARGS__),\
        IF_ELSE(EQUAL(cur_val, $STATIC_ARRAY))( \
            INNER_EXEC_STATIC_ARRAY(op, par, cast, sep, count, ##__VA_ARGS__),\
            IF_ELSE(EQUAL(cur_val, $CONST))(\
                INNER_EXEC_CONST(op, par, cast, sep, count, ##__VA_ARGS__),\
                op(par(cur_val), (cast)&cur_val) \
                INNER_EXEC_ELSE(op, par, cast, sep, count, ##__VA_ARGS__)\
            )\
        )\
    )
#define _MAP_MERGE_OPERATION_ONE_PAR_INNER() MAP_MERGE_OPERATION_ONE_PAR_INNER


#define INNER_EXEC_POINTER(op, par, cast, sep, count, len, arr, ...)\
        op(len, (cast)arr)\
        INNER_EXEC_ELSE(op, par, cast, sep, count, ##__VA_ARGS__)

#define INNER_EXEC_STATIC_ARRAY(op, par, cast, sep, count, arr, ...)\
        op(par(arr), (cast)arr)\
        INNER_EXEC_ELSE(op, par, cast, sep, count, ##__VA_ARGS__)

#define INNER_EXEC_CONST(op, par, cast, sep, count, val, type, ...)\
    type CAT(const_,count) = val;\
    op(par(type), (cast)&CAT(const_,count))\
    INNER_EXEC_ELSE(op, par, cast, sep, count, ##__VA_ARGS__)


#define INNER_EXEC_ELSE(op, par, cast, sep, count, ...)\
    IF(HAS_ARGS(__VA_ARGS__))( \
        sep() DEFER5(_MAP_MERGE_OPERATION_ONE_PAR_INNER)()(op, par, cast, sep, MAP_INC(count), ##__VA_ARGS__) \
    )
//-----------------------------------------------------------------------------------------------------------------


#define WRITE_DATA_MACRO(foo, beforeExpr, afterExpr, ...)\
    do{\
        reg totalSize = MAP_MERGE_OPERATION(sizeof, PLUS, ##__VA_ARGS__);\
        beforeExpr;\
        MAP_MERGE_OPERATION_ONE_PAR(foo, sizeof, u8*, COMMA_POINT, ##__VA_ARGS__);\
        afterExpr;\
    }while(0U)




void rawPTestWriteType(reg typelen, u8 *data)
{
    printf("writeTypeFoo size: %d\n", typelen);
    (void)data;
}

void rawPTestWriteArray(reg typelen, u8 *data, reg typeLen)
{
    printf("writeArrayFoo size: %d\n", typelen);
    (void)data;
}


void rawParserMacroTest()
{
    printf("\n------------- RAW_PARSER_MACRO_TEST -----------------------------\n");
    printf("%s\n\n", TO_TXT2(MAP_MERGE_OPERATION(sizeof, PLUS, int, int, int, bool, char)));
    printf("%s\n\n", TO_TXT2(MAP_MERGE_OPERATION_ONE_PAR(sizeof,sizeof,u8*, COMMA_POINT, int, int, int, bool, char)));
    printf("%s\n\n", TO_TXT2(        WRITE_DATA_MACRO(writeFoo, EMPTY(), {
                                                        printf("total size: %d\n", totalSize);
                                                    }, a, b, c, $STATIC_ARRAY, arr, $POINTER, 10, arr2, $CONST, 123, i32, $CONST, 123.123, f32, br)));

    i32 a;
    reg b;
    c8 c;

    u8 arr[10] = {0,0,0,0,0,0,0,0,0,0};
    u8 arr2[11] = {0,0,0,0,0,0,0,0,0,0};
    u8 * arr3_ptr = arr2;


    WRITE_DATA_MACRO(rawPTestWriteType, EMPTY(),{
                         printf("total size: %d\n", totalSize);
                         printf("const: %d\n", const_5);
                     }, a, b, c, $STATIC_ARRAY, arr, $POINTER, 11, arr3_ptr, $CONST, 123, i32);

    printf("\n------------- END OF RAW_PARSER_MACRO_TEST -----------------------------\n");
    fflush(stdout);
    fflush(stderr);
}


#endif // __RAWPARSER_MACRO_H__
