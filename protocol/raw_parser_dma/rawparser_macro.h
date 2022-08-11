#ifndef __RAWPARSER_MACRO_H__
#define __RAWPARSER_MACRO_H__

#include "my_ctypes.h"
#include "preprocessor_base.h"
#include "byte_order.h"

#include <preprocessor/preprocessor.h>
#include <stdio.h>



#define foo(x) x
#define bar(x) x

#define $POINTER(x) x
#define $STATIC_ARRAY(x) x
#define $CONST(x) x

//sizeof merge macro -----------------------------------------------------------------------------------------------
#define RAW_P_MERGE_OPERATION(op, sep, ...) \
 IF(HAS_ARGS(__VA_ARGS__))(EVAL(RAW_P_MERGE_OPERATION_INNER(op, sep, ##__VA_ARGS__)))
#define RAW_P_MERGE_OPERATION_INNER(op, sep, cur_val, ...) \
    IF_ELSE(EQUAL(cur_val, $POINTER))( \
        RAW_P_MERGE_OPERATION_POINTER_INNER(op, sep, ##__VA_ARGS__),\
        IF_ELSE(EQUAL(cur_val, $STATIC_ARRAY))(\
            RAW_P_MERGE_OPERATION_STATIC_ARRAY_INNER(op, sep, ##__VA_ARGS__),\
            IF_ELSE(EQUAL(cur_val, $CONST))(\
                RAW_P_MERGE_OPERATION_CONST_INNER(op, sep, ##__VA_ARGS__),\
                op(cur_val)\
                RAW_P_MERGE_OPERATION_ELSE_INNER(op, sep, ##__VA_ARGS__)\
            )\
        )\
    )

#define _RAW_P_MERGE_OPERATION_INNER() RAW_P_MERGE_OPERATION_INNER

#define RAW_P_MERGE_OPERATION_ELSE_INNER(op, sep, ...)\
    IF(HAS_ARGS(__VA_ARGS__))( \
      sep() DEFER5(_RAW_P_MERGE_OPERATION_INNER)()(op, sep, ##__VA_ARGS__) \
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
    IF(HAS_ARGS(__VA_ARGS__))(EVAL(RAW_P_MERGE_WRITE_OPERATION_INNER(foo, par1, op, cast, sep, 0, ##__VA_ARGS__)))
#define RAW_P_MERGE_WRITE_OPERATION_INNER(foo, par1, op, cast, sep, count, cur_val, ...) \
    IF_ELSE(EQUAL(cur_val, $POINTER))( \
        RAW_P_MERGE_WRITE_OPERATION_POINTER_INNER(foo, par1, op, cast, sep, count, ##__VA_ARGS__),\
        IF_ELSE(EQUAL(cur_val, $STATIC_ARRAY))( \
            RAW_P_MERGE_WRITE_OPERATION_STATIC_ARRAY_INNER(foo, par1, op, cast, sep, count, ##__VA_ARGS__),\
            IF_ELSE(EQUAL(cur_val, $CONST))(\
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
    type CAT(const_,count) = val;\
    foo(par1, op(type), op(type), (cast)&CAT(const_,count))\
    RAW_P_MERGE_WRITE_OPERATION_ELSE(foo, par1, op, cast, sep, count, ##__VA_ARGS__)


#define RAW_P_MERGE_WRITE_OPERATION_ELSE(foo, par1, op, cast, sep, count, ...)\
    IF(HAS_ARGS(__VA_ARGS__))( \
        sep() DEFER5(_RAW_P_MERGE_WRITE_OPERATION_INNER)()(foo, par1, op, cast, sep, MAP_INC(count), ##__VA_ARGS__) \
    )



//-----------------------------------------------------------------------------------------------------------------
#define WRITE_PAYLOAD_MACRO(foo, par1, beforeExpr, afterExpr, ...)\
    do{\
        reg totalSize = RAW_P_MERGE_OPERATION(sizeof, PLUS, ##__VA_ARGS__);\
        beforeExpr;\
        RAW_P_MERGE_WRITE_OPERATION(foo, (u8*)par1, sizeof, u8*, COMMA_POINT, ##__VA_ARGS__);\
        afterExpr;\
        (void)totalSize;\
    }while(0U)



//**************************************************************************************************
// TEST
//**************************************************************************************************
static int pos_i = 0; // into descriptor type position saved
void rawPTestWriteUni(u8* desc, reg totalLenInByte, reg typelenInByte, u8 *data)
{
    u8* to = (u8*)(desc + pos_i);
    pos_i += totalLenInByte;


#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
    while(totalLenInByte--) {
        *to++ = *data++;
    }

    (void)typelenInByte;
#else /* MY_ENDIAN_ORDER == MY_BIG_ENDIAN */
    for(reg i = 0; i < totalLenInByte; i+= typelenInByte) {
        reg n = typelenInByte;
        while(n--) {
            *to++ = *(data + n + i);
        }
    }
#endif /* MY_ENDIAN_ORDER == MY_BIG_ENDIAN */

}

void rawPTestWriteType(u8* desc, reg totalLenInByte, reg typelenInByte, u8 *data)
{
    printf("writeTypeFoo size: %d\n", totalLenInByte);
    (void)data;
    (void)desc;
    (void)typelenInByte;
}



void rawParserMacroTest()
{
    printf("\n------------- RAW_PARSER_MACRO_TEST -----------------------------\n");
    printf("%s\n\n", TO_TXT2(RAW_P_MERGE_OPERATION(sizeof, PLUS, int, int, int, bool, char)));
    printf("%s\n\n", TO_TXT2(RAW_P_MERGE_WRITE_OPERATION(sizeof,par1, sizeof,u8*, COMMA_POINT, int, int, int, bool, char)));
    printf("%s\n\n", PREPROCESSOR_STRINGIFY_VARIADIC(            WRITE_PAYLOAD_MACRO(rawPTestWriteType, NULL, EMPTY(), {
                                                                 printf("total size: %d\n", totalSize);
                                                                 printf("const: %d\n", const_5);
                                                             }, a, b, c, $STATIC_ARRAY, arr, $POINTER, 11, arr3_ptr, $CONST, 123, i32)));

    i32 a = 1;
    reg b = 2;
    u16 c = 6;

    u32 arr16[10] = {0x01020304,2,3,4,5,6,7,8,9,10};
    u32 arr16_tmp[100] = {0,0,0,0,0,0,0,0,0,0};



    printf("\n ------------------- before data --------------------------------\n");
    for(reg i = 0; i < sizeof(arr16)/sizeof(arr16[0]); ++i) {
        printf("\n data[%d]: %08x\n",i, arr16[i]);
    }

    pos_i = 0;
    WRITE_PAYLOAD_MACRO(rawPTestWriteUni, arr16_tmp, EMPTY(), EMPTY(), $STATIC_ARRAY, arr16, a, b, c);

    printf("\n ------------------- after data -------------------------------- %d\n", pos_i);
    for(reg i = 0; i < (10 + 9); ++i) {
        printf("\n data[%d]: %08x\n",i, arr16_tmp[i]);
    }


    u8 arr[10] = {0,0,0,0,0,0,0,0,0,0};
    u8 arr2[11] = {0,0,0,0,0,0,0,0,0,0};
    u8 * arr3_ptr = arr2;


    WRITE_PAYLOAD_MACRO(rawPTestWriteType, NULL, EMPTY(), {
                            printf("total size: %d\n", totalSize);
                            printf("const: %d\n", const_5);
                        }, a, b, c, $STATIC_ARRAY, arr, $POINTER, 11, arr3_ptr, $CONST, 123, i32);

    printf("\n------------- END OF RAW_PARSER_MACRO_TEST -----------------------------\n");


    printf("%s \n",         TO_TXT2(  PREPROCESSOR_SIMPLE_HAS_ARGS(,))       );


//    *
//    *    // Expands to 1
//    *   PREPROCESSOR_PRIMITIVE_COMPARE(foo, foo) // Expands to 0



    fflush(stdout);
    fflush(stderr);
}

#endif // __RAWPARSER_MACRO_H__
