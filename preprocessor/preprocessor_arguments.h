/*
 *	Preprocessor Library by Parra Studios
 *	A generic header-only preprocessor metaprogramming library.
 *
 *	Copyright (C) 2016 - 2022 Vicente Eduardo Ferrer Garcia <vic798@gmail.com>
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 *
 */

#ifndef PREPROCESSOR_ARGUMENTS_H
#define PREPROCESSOR_ARGUMENTS_H 1

/* -- Headers -- */

#include <preprocessor/preprocessor_arithmetic.h>
#include <preprocessor/preprocessor_boolean.h>
#include <preprocessor/preprocessor_symbol.h>
#include <preprocessor/preprocessor_concatenation.h>
#include <preprocessor/preprocessor_if.h>
#include <preprocessor/preprocessor_tuple.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -- Definitions -- */
/*********************************************************************************************************************
 * MAXIMUM VARIADIC INPUT MACRO
 *      If you want increase number of arguments you must extends next macro:
 *          - PREPROCESSOR_ARGS_COUNT_SEQ_IMPL,
 *          - PREPROCESSOR_ARGS_N_IMPL,
 *          - PREPROCESSOR_ARGS_COMMA_IMPL_0 ... PREPROCESSOR_ARGS_COMMA_IMPL_X,
 *          - PREPROCESSOR_ARGS_COUNT_PREFIX__PREPROCESSOR_ARGS_COUNT_POSTFIX (number of commas)
 *          - PREPROCESSOR_ARGS_COMMA_SEQ_IMPL
 *          - PREPROCESSOR_ARGS_SIZE
 *
 *      WARNING!!! some compilers do not support when number of arguments more than 127
 */

#define PREPROCESSOR_ARGS_SIZE 64
/******************************************************************************************************************/

/* -- Macros -- */

#define PREPROCESSOR_ARGS_FIRST(first, ...) first
#define PREPROCESSOR_ARGS_FIRST_REMOVE(first, ...) __VA_ARGS__

#define PREPROCESSOR_ARGS_SECOND(first, second, ...) second
#define PREPROCESSOR_ARGS_SECOND_REMOVE(first, second, ...) first, __VA_ARGS__

/*********************************************************************************************************************
 * VARIADIC ARGUMENTS IS EMPTY IMPLEMENTATION
 *
 *          PREPROCESSOR_ARGS_EMPTY()           // expand to 1
 *          PREPROCESSOR_ARGS_EMPTY(x)          // expand to 0
 *          PREPROCESSOR_ARGS_EMPTY(x, y, z)    // expand to 0
 *          PREPROCESSOR_ARGS_EMPTY(,,)         // expand to 0
 *          PREPROCESSOR_ARGS_EMPTY(,)          // expand to 0
 *          PREPROCESSOR_ARGS_EMPTY(, a)        // expand to 0
 *
 */

#define PREPROCESSOR_ARGS_EMPTY(...)                                        \
    PREPROCESSOR_ARGS_EMPTY_IMPL(                                           \
        PREPROCESSOR_ARGS_COMMA(__VA_ARGS__),                               \
        PREPROCESSOR_ARGS_COMMA(PREPROCESSOR_COMMA_VARIADIC __VA_ARGS__),   \
        PREPROCESSOR_ARGS_COMMA(__VA_ARGS__ ()),                            \
        PREPROCESSOR_ARGS_COMMA(PREPROCESSOR_COMMA_VARIADIC __VA_ARGS__ ()) \
    )

#define PREPROCESSOR_ARGS_EMPTY_IMPL(_0, _1, _2, _3) PREPROCESSOR_ARGS_EMPTY_IMPL_I(_0, _1, _2, _3)
#define PREPROCESSOR_ARGS_EMPTY_IMPL_I(_0, _1, _2, _3) PREPROCESSOR_ARGS_COMMA(PREPROCESSOR_ARGS_EMPTY_CASE(PREPROCESSOR_ARGS_EMPTY_CASE_IMPL_, _0, _1, _2, _3))
#define PREPROCESSOR_ARGS_EMPTY_CASE(_0, _1, _2, _3, _4) _0 ## _1 ## _2 ## _3 ## _4
#define PREPROCESSOR_ARGS_EMPTY_CASE_IMPL_0001 ,

/*********************************************************************************************************************
 * VARIADIC ARGUMENTS IS NOT EMPTY IMPLEMENTATION
 *
 *          PREPROCESSOR_ARGS_NOT_EMPTY()           // expand to 0
 *          PREPROCESSOR_ARGS_NOT_EMPTY(x)          // expand to 1
 *          PREPROCESSOR_ARGS_NOT_EMPTY(x, y, z)    // expand to 1
 *          PREPROCESSOR_ARGS_NOT_EMPTY(,,)         // expand to 1
 *          PREPROCESSOR_ARGS_NOT_EMPTY(,)          // expand to 1
 *          PREPROCESSOR_ARGS_NOT_EMPTY(, a)        // expand to 1
 *
 */
#define PREPROCESSOR_ARGS_NOT_EMPTY(...) \
    PREPROCESSOR_NOT(PREPROCESSOR_ARGS_EMPTY(__VA_ARGS__))


/*********************************************************************************************************************
 * VARIADIC ARGUMENTS COUNT IMPLEMENTATION
 *
 *      PREPROCESSOR_ARGS_COUNT()                                    // expand to 0
 *      PREPROCESSOR_ARGS_COUNT(x,y,z)                               // expand to 3
 *      PREPROCESSOR_ARGS_COUNT(x, PREPROCESSOR_TUPLE_MAKE(1,2,3))   // expand to 2
 *      PREPROCESSOR_ARGS_COUNT(,,)                                  // expand to 3 (NOT TESTED ON _MSC_VER!!! may be not work)
 *
 */

#if defined(__GNUC__) || defined(__clang__)

    #define PREPROCESSOR_ARGS_COUNT(...) \
        PREPROCESSOR_IF_ELSE(PREPROCESSOR_ARGS_EMPTY(__VA_ARGS__)) ( \
            0, \
            PREPROCESSOR_ARGS_COUNT_IMPL(__VA_ARGS__, PREPROCESSOR_ARGS_COUNT_SEQ_IMPL()) \
        )

    #define PREPROCESSOR_ARGS_COUNT_IMPL(...) PREPROCESSOR_ARGS_N_IMPL(__VA_ARGS__)

#elif defined(_MSC_VER) && !defined(__clang__)

    #define PREPROCESSOR_ARGS_COUNT(...) \
        PREPROCESSOR_ARGS_COUNT_IMPL((PREPROCESSOR_ARGS_COUNT_PREFIX_ ## __VA_ARGS__ ## _PREPROCESSOR_ARGS_COUNT_POSTFIX, PREPROCESSOR_ARGS_COUNT_SEQ_IMPL()))

    #define PREPROCESSOR_ARGS_COUNT_PREFIX__PREPROCESSOR_ARGS_COUNT_POSTFIX ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,0
    #define PREPROCESSOR_ARGS_COUNT_IMPL(expr) PREPROCESSOR_ARGS_N_IMPL expr

#else
#	error "Unkwnown argument counter implementation for variadic arguments. Source: preprocessor_arguments.h"
#endif /* variadic counter implementation switch */


// MACRO COUNT TABLES ------------------------------------------------------------------------
#define PREPROCESSOR_ARGS_COUNT_SEQ_IMPL()  \
    63, 62, 61, 60,                         \
    59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
    49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
    39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
    29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
    19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
    9, 8, 7, 6, 5, 4, 3, 2, 1, 0


#define PREPROCESSOR_ARGS_N_IMPL(                       \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,            \
    _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,   \
    _21, _22, _23, _24, _25, _26, _27, _28, _29, _30,   \
    _31, _32, _33, _34, _35, _36, _37, _38, _39, _40,   \
    _41, _42, _43, _44, _45, _46, _47, _48, _49, _50,   \
    _51, _52, _53, _54, _55, _56, _57, _58, _59, _60,   \
    _61, _62, _63,                                      \
    N, ...) N


/************************************************************************************************************************************************************
 * Macro which checks if it has any arguments. Returns '0' if there are no
 * arguments, '1' otherwise.
 *
 * Limitation: PREPROCESSOR_SIMPLE_HAS_ARGS(,1,2,3) returns 0 -- this check essentially only checks
 * that the first argument exists.
 *
 * This macro works as follows:
 *
 * 1. PREPROCESSOR_END_OF_ARGUMENTS_ is concatenated with the first argument.
 * 2. If the first argument is not present then only "PREPROCESSOR_END_OF_ARGUMENTS_" will
 *    remain, otherwise "PREPROCESSOR_END_OF_ARGUMENTS_ something_here" will remain. This
 *    remaining argument can start with parentheses.
 * 3. In the former case, the PREPROCESSOR_END_OF_ARGUMENTS_(0) macro expands to a
 *    0 when it is expanded. In the latter, a non-zero result remains. If the
 *    first argument started with parentheses these will mostly not contain
 *    only a single 0, but e.g a C cast or some arithmetic operation that will
 *    cause the PREPROCESSOR_BOOL in PREPROCESSOR_END_OF_ARGUMENTS_ to be one.
 * 4. PREPROCESSOR_BOOL is used to force non-zero results into 1 giving the clean 0 or 1
 *    output required.
 *
 *          PREPROCESSOR_SIMPLE_HAS_ARGS()          // expands to 0
 *          PREPROCESSOR_SIMPLE_HAS_ARGS(,)         // expands to 0
 *          PREPROCESSOR_SIMPLE_HAS_ARGS(, a)       // expands to 0
 *          PREPROCESSOR_SIMPLE_HAS_ARGS(a,b,c)     // expands to 1
 *          PREPROCESSOR_SIMPLE_HAS_ARGS(a,b)       // expands to 1
 *          PREPROCESSOR_SIMPLE_HAS_ARGS(a)         // expands to 1
 */
#define PREPROCESSOR_SIMPLE_HAS_ARGS(...) PREPROCESSOR_BOOL(PREPROCESSOR_ARGS_FIRST(PREPROCESSOR_END_OF_ARGUMENTS_ __VA_ARGS__)(0))
#define PREPROCESSOR_END_OF_ARGUMENTS_(...) PREPROCESSOR_BOOL(PREPROCESSOR_ARGS_FIRST(__VA_ARGS__))


/*********************************************************************************************************************
 * VARIADIC ARGUMENT COMMA FINDER IMPLEMENTATION
 *  Check if the next sequence symbol comma (,) is exists
 *
 *      PREPROCESSOR_ARGS_COMMA(x)      // expands to 0
 *      PREPROCESSOR_ARGS_COMMA()       // expands to 0
 *      PREPROCESSOR_ARGS_COMMA(,)      // expands to 1 (NOT TESTED ON _MSC_VER!!! may be not work)
 *      PREPROCESSOR_ARGS_COMMA(x,y)    // expands to 1
 *
 */

#if defined(__GNUC__) || defined(__clang__)
    #if (defined(__cplusplus) && (__cplusplus >= 201103L)) || (defined(__STDC__) && defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L))

        #define PREPROCESSOR_ARGS_COMMA(...) \
                    PREPROCESSOR_ARGS_COMMA_IMPL( \
                        PREPROCESSOR_DEC(PREPROCESSOR_ARGS_COUNT_IMPL(~, __VA_ARGS__, PREPROCESSOR_ARGS_COUNT_SEQ_IMPL())) \
                    )

        #define PREPROCESSOR_ARGS_COMMA_IMPL(value) PREPROCESSOR_NOT(PREPROCESSOR_CONCAT(PREPROCESSOR_ARGS_COMMA_IMPL_, value))


        #define PREPROCESSOR_ARGS_COMMA_IMPL_0 1
        #define PREPROCESSOR_ARGS_COMMA_IMPL_1 1
        #define PREPROCESSOR_ARGS_COMMA_IMPL_2 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_3 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_4 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_5 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_6 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_7 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_8 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_9 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_10 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_11 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_12 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_13 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_14 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_15 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_16 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_17 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_18 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_19 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_20 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_21 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_22 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_23 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_24 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_25 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_26 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_27 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_28 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_29 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_30 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_31 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_32 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_33 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_34 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_35 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_36 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_37 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_38 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_39 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_40 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_41 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_42 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_43 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_44 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_45 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_46 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_47 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_48 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_49 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_50 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_51 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_52 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_53 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_54 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_55 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_56 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_57 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_58 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_59 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_60 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_61 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_62 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_63 0
        #define PREPROCESSOR_ARGS_COMMA_IMPL_64 0


    #else

        #define PREPROCESSOR_ARGS_COMMA(...) PREPROCESSOR_ARGS_COUNT_IMPL(__VA_ARGS__, PREPROCESSOR_ARGS_COMMA_SEQ_IMPL())

    #endif /* (defined(__cplusplus) && (__cplusplus >= 201103L)) || (defined(__STDC__) && defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) */

#elif defined(_MSC_VER) && !defined(__clang__)

    #define PREPROCESSOR_ARGS_COMMA(...) \
        PREPROCESSOR_ARGS_COUNT_IMPL((PREPROCESSOR_ARGS_COUNT_PREFIX_ ## __VA_ARGS__ ## _PREPROCESSOR_ARGS_COUNT_POSTFIX, PREPROCESSOR_ARGS_COMMA_SEQ_IMPL()))
#else
#	error "Unkwnown preprocessor implementation for variadic arguments. Source: preprocessor_arguments.h"
#endif

// MACRO COMMA TABLES ------------------------------------------------------------------------
#define PREPROCESSOR_ARGS_COMMA_SEQ_IMPL()  \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,           \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,           \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,           \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,           \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,           \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,           \
    1, 1, 0


/*********************************************************************************************************************/



#ifdef __cplusplus
}
#endif

#endif /* PREPROCESSOR_ARGUMENTS_H */
