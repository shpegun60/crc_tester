#include "templateTest.h"
#include "preprocessor_map.h"
#include "all_possible_sums.h"

#include <stdio.h>
#include <stdint.h>

#define Arr 1

//An example of using this macro
#define M(i, ...) i
#define GREET(x) Hello_##x

#define PRED(state, ...) BOOL(state)
#define OP(state, ...) MAP_DEC(state), state, __VA_ARGS__


#define foo(x) x
#define bar(x) x

#define PRINT_STREAM(x, stream) fprintf (stream, "%d\n", x);


#define TRANSLATE(name) CAT(name, MSB)


///////////////////////////////////////////////////////////////////////////////////////////////////
#define MAP_MERGE_OPERATION_ONE_PAR(op, par, cast, sep, ...) \
    IF(HAS_ARGS(__VA_ARGS__))(EVAL(MAP_MERGE_OPERATION_ONE_PAR_INNER(op, par, cast, sep, ##__VA_ARGS__)))
#define MAP_MERGE_OPERATION_ONE_PAR_INNER(op, par, cast, sep, cur_val, ...) \
    op(par(cur_val), (cast)&cur_val) \
    IF(HAS_ARGS(__VA_ARGS__))( \
        sep()\
        DEFER2(_MAP_MERGE_OPERATION_ONE_PAR_INNER)()(op, par, cast, sep, ##__VA_ARGS__) \
    )
#define _MAP_MERGE_OPERATION_ONE_PAR_INNER() MAP_MERGE_OPERATION_ONE_PAR_INNER

void writeFoo(size_t len, uint8_t *data)
{
    printf("writeFoo size: %d\n", len);
    (void)data;
}


#define WRITE_DATA_MACRO(foo, afterExpr, ...)\
    do{\
        size_t totalSize = MAP_MERGE_OPERATION(sizeof, PLUS, ##__VA_ARGS__);\
        MAP_MERGE_OPERATION_ONE_PAR(foo, sizeof, uint8_t*, COMMA_POINT, ##__VA_ARGS__);\
        afterExpr;\
    }while(0U)



void templatetest()
{
    int ai[3] = {1,2,3};
    int bi[3] = {4,5,6};
    double af[3] = {1.0,2.0,3.0};
    double bf[3] = {1.5,2.5,3.5};

    TEMPLATE(sum, int)(3,ai,bi);
    TEMPLATE(sum, double)(3,af,bf);
    //TEMPLATE2(sum,double,int)(3,af,bf);

    //IF_ELSE(Arr)(printf("%s\n", TO_TXT2(RECURSE())))(printf("------------ cond -->%d\n", 0));
    printf("%s\n", TO_TXT2(MAP(GREET, COMMA, Mum, Dad, Adam, Joe)));
    printf("%s\n", TO_TXT2(MAP_ALL_TO_ONE(sum,__,int, float, double)));

    printf("%s\n", TO_TXT2(REPEAT_MACRO(8, M)));

    printf("%s\n", TO_TXT2(WHILE_MACRO(PRED, OP, 7)));

    printf("bitand: %s\n", TO_TXT2(BITAND(1)(123, blablabla)));

    printf("%s\n", TO_TXT2(EQUAL(foo, foo)));



    printf("%s\n", TO_TXT2(MAP_LIST_UD_I(f, x, a, b, c)));

    MAP_UD(PRINT_STREAM, stdout, 1, 2, 3, 4, 5);

    printf("%s\n", TO_TXT2(TEMPLATE(sum, bla1, bla2, bla3, bla4)));

    printf("%s\n", TO_TXT2(TEMPLATE(TRANSLATE(convertWrite), sum)));

    printf("%s\n", TO_TXT2(MAP_MERGE_OPERATION(sizeof, PLUS, int, int, int, bool, char)));
    printf("%s\n", TO_TXT2(MAP_MERGE_OPERATION_ONE_PAR(sizeof,sizeof,u8*, COMMA_POINT, int, int, int, bool, char)));
    printf("%s\n", TO_TXT2(    WRITE_DATA_MACRO(writeFoo, {
                                                    printf("size: %d\n", totalSize);
                                                }, a, b, c, d, POINTER(arr,2))));

    int a;
    size_t b;
    char c;
    uint16_t d;
    uint16_t e;

    uint8_t arr[10] = {0,0,0,0,0,0,0,0,0,0};


    WRITE_DATA_MACRO(writeFoo, {
                         printf("total size: %d\n", totalSize);
                     }, a, b, c, d, e/*, POINTER(arr, 10), STATIC_ARRAY(arr)*/);




    fflush(stdout);
    fflush(stderr);
}
