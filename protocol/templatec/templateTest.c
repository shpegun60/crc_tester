#include "templateTest.h"
#include "preprocessor_map.h"
#include "all_possible_sums.h"

#include <stdio.h>

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

    printf("%s\n", TO_TXT2(BITAND(1)(x,y,z)));

    printf("%s\n", TO_TXT2(EQUAL(foo, foo)));



    printf("%s\n", TO_TXT2(MAP_LIST_UD_I(f, x, a, b, c)));

    MAP_UD(PRINT_STREAM, stdout, 1, 2, 3, 4, 5);

    printf("%s\n", TO_TXT2(TEMPLATE(sum, bla1, bla2, bla3, bla4)));

    printf("%s\n", TO_TXT2(TEMPLATE(TRANSLATE(convertWrite), sum)));



    fflush(stdout);
    fflush(stderr);
}
