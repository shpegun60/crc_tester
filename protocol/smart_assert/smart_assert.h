/**
 * @file    smart_assert.h
 * @brief   This library for check expressions and send message if expressions is true on RUNTIME and on DEBUG version.
 *          If NDEBUG is not defined some expressions has change to empty define. (programmer must uncommit NDEBUG if code version released)
 *
 * @date
 * @author Shpegun60
 */

#ifndef __SMART_ASSERT_H_
#define __SMART_ASSERT_H_


// this library recomended include only .c or .cpp file!!!!!!!!!!!!!!!!

//   C++ linking for mixed C++/C code

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdio.h>

//#define NDEBUG

#define M_EMPTY     /* ignored expression */
#define M_ALWAYS 1  /* always proceed expression */
#define M_LIB_DATA_DEF          "[d]" /* adds text for some library data, user must adds to message list 0--> messageEna, 1-->libDescr  (0|1, "descr") */
#define M_LIB_NAME_DEF          "[s]" /* adds text for some library data, user must adds to message list 0--> libDescr ("descr") */
#define M_LIB_ENA_DEF           "[e]" /* adds text for some library data, user must adds to message list 0--> messageEna (0|1) */
#define M_LIB_ALWAYS_DIS_DEF    "[0]" /* always disable message in smart assert, nothing to adds */
#define M_LIB_ALWAYS_ENA_DEF    "[1]" /* always enable message in smart assert, nothing to adds */


#ifndef NDEBUG /* --------------------------------------------------------------------------------------------------------- */

void __M_Error(const char* const expr_str, const unsigned char expr, const char* const file, const int line, const char* const msg, ...);
void __M_Warning(const char* const expr_str, const unsigned char expr, const char* const file, const int line, const char* const msg, ...);

/*
 * ***********************************************************************************************************************************************
 *  defines simple if NDEBUG disable this expression expr
 * ***********************************************************************************************************************************************
 */
// this macros will disable if NDEBUG is defined
#   define M_Assert_disableExpr(...) __VA_ARGS__

/*
 * ***********************************************************************************************************************************************
 *  defines for break program
 * ***********************************************************************************************************************************************
 */

#   define M_Assert_Break(Expr, beforeExpr, afterExpr, Msg, ...)                                                                        \
    do{                                                                                                                                 \
        if (Expr) {                                                                                                                     \
            beforeExpr;                                                                                                                 \
            __M_Error((#Expr), (Expr), (__FILE__), (__LINE__), (Msg), ##__VA_ARGS__);                                                   \
            afterExpr; /* ignored because programm is break, but compiler not known it */                                               \
        }                                                                                                                               \
    }while(0L)

#   define M_Assert_BreakSaveCheck(Expr, beforeExpr, afterExpr, Msg, ...)                                                               \
    do{                                                                                                                                 \
        if (Expr) {                                                                                                                     \
            beforeExpr;                                                                                                                 \
            __M_Error((#Expr), (Expr), (__FILE__), (__LINE__), (Msg), ##__VA_ARGS__);                                                   \
            afterExpr; /* ignored because programm is break, but compiler not known it */                                               \
        }                                                                                                                               \
    }while(0L)


// else breaking asserts
#   define M_Assert_BreakElse(Expr, trueExpr, falseBeforeExpr, falseAfterExpr, Msg, ...)                                                \
    do{                                                                                                                                 \
        if(Expr) {                                                                                                                      \
            trueExpr;                                                                                                                   \
        } else { /* ignored else expression if NDEBUG */                                                                                \
            falseBeforeExpr;                                                                                                            \
            __M_Error("if " #Expr " not confirmed: passed to else", (Expr), (__FILE__), (__LINE__), (Msg), ##__VA_ARGS__);              \
            falseAfterExpr;                                                                                                             \
        }                                                                                                                               \
    }while(0L)

#   define M_Assert_BreakElseSaveCheck(Expr, trueExpr, falseBeforeExpr, falseAfterExpr, Msg, ...)                                       \
    do{                                                                                                                                 \
        if(Expr) {                                                                                                                      \
            trueExpr;                                                                                                                   \
        } else {  /* ignored else expression if NDEBUG */                                                                               \
            falseBeforeExpr;                                                                                                            \
            __M_Error("if " #Expr " not confirmed: passed to else", (Expr), (__FILE__), (__LINE__), (Msg), ##__VA_ARGS__);              \
            falseAfterExpr;                                                                                                             \
        }                                                                                                                               \
    }while(0L)

/*
 * ***********************************************************************************************************************************************
 *  defines no break program
 * ***********************************************************************************************************************************************
 */

#   define M_Assert_Warning(Expr, beforeExpr, afterExpr, Msg, ...)                                                                      \
    do{                                                                                                                                 \
        if (Expr) {                                                                                                                     \
            beforeExpr;                                                                                                                 \
            __M_Warning((#Expr), (Expr), (__FILE__), (__LINE__), (Msg), ##__VA_ARGS__);                                                 \
            afterExpr;                                                                                                                  \
        }                                                                                                                               \
    }while(0L)

#   define M_Assert_WarningSaveCheck(Expr, beforeExpr, afterExpr, Msg, ...)                                                             \
    do{                                                                                                                                 \
        if (Expr) {                                                                                                                     \
            beforeExpr;                                                                                                                 \
            __M_Warning((#Expr), (Expr), (__FILE__), (__LINE__), (Msg), ##__VA_ARGS__);                                                 \
            afterExpr;                                                                                                                  \
        }                                                                                                                               \
    }while(0L)


// else warning asserts
#   define M_Assert_WarningElse(Expr, trueExpr, falseBeforeExpr, falseAfterExpr, Msg, ...)                                              \
    do{                                                                                                                                 \
        if(Expr) {                                                                                                                      \
            trueExpr;                                                                                                                   \
        } else { /* ignored else expression if NDEBUG */                                                                                \
            falseBeforeExpr;                                                                                                            \
            __M_Warning("if " #Expr " not confirmed: passed to else", (Expr), (__FILE__), (__LINE__), (Msg), ##__VA_ARGS__);            \
            falseAfterExpr;                                                                                                             \
        }                                                                                                                               \
    }while(0L)

#   define M_Assert_WarningElseSaveCheck(Expr, trueExpr, falseBeforeExpr, falseAfterExpr, Msg, ...)                                     \
    do{                                                                                                                                 \
        if(Expr) {                                                                                                                      \
            trueExpr;                                                                                                                   \
        } else { /* ignored else expression if NDEBUG */                                                                                \
            falseBeforeExpr;                                                                                                            \
            __M_Warning("if " #Expr " not confirmed: passed to else", (Expr), (__FILE__), (__LINE__), (Msg), ##__VA_ARGS__);            \
            falseAfterExpr;                                                                                                             \
        }                                                                                                                               \
    }while(0L)


/*
 * ***********************************************************************************************************************************************
 *  functions
 * ***********************************************************************************************************************************************
 */

#   define M_Assert_SafeFunctionCall(foo, true_expression)\
    do{\
        if(foo) {\
            true_expression;\
        } else {\
            __M_Warning((#foo), (0), (__FILE__), (__LINE__), ("NO exists function"));\
        }\
    }while(0L)

#else
/*
 * ***********************************************************************************************************************************************
 *  defines simple if NDEBUG disable expr
 * ***********************************************************************************************************************************************
 */

#   define M_Assert_disableExpr(...)
/*
 * ***********************************************************************************************************************************************
 *  defines for break program
 * ***********************************************************************************************************************************************
 */

#   define M_Assert_Break(Expr, beforeExpr, afterExpr, Msg, ...)

#   define M_Assert_BreakSaveCheck(Expr, beforeExpr, afterExpr, Msg, ...)                                                           \
    do{                                                                                                                             \
        if (Expr) {                                                                                                                 \
            beforeExpr;                                                                                                             \
            afterExpr;                                                                                                              \
        }                                                                                                                           \
    }while(0L)

// else breaking asserts
#   define M_Assert_BreakElse(Expr, trueExpr, falseBeforeExpr, falseAfterExpr, Msg, ...)                                            \
    do{                                                                                                                             \
        trueExpr;                                                                                                                   \
    }while(0L)

#   define M_Assert_BreakElseSaveCheck(Expr, trueExpr, falseBeforeExpr, falseAfterExpr, Msg, ...)                                   \
    do{                                                                                                                             \
        if(Expr) {                                                                                                                  \
            trueExpr;                                                                                                               \
        }                                                                                                                           \
    }while(0L)

/*
 * ***********************************************************************************************************************************************
 *  defines no break program
 * ***********************************************************************************************************************************************
 */

#   define M_Assert_Warning(Expr, beforeExpr, afterExpr, Msg, ...)

#   define M_Assert_WarningSaveCheck(Expr, beforeExpr, afterExpr, Msg, ...)                                                         \
    do{                                                                                                                             \
        if (Expr) {                                                                                                                 \
            beforeExpr;                                                                                                             \
            afterExpr;                                                                                                              \
        }                                                                                                                           \
    }while(0L)

// else warning asserts
#   define M_Assert_WarningElse(Expr, trueExpr, falseBeforeExpr, falseAfterExpr, Msg, ...)                                          \
    do{                                                                                                                             \
        trueExpr;                                                                                                                   \
    }while(0L)

#   define M_Assert_WarningElseSaveCheck(Expr, trueExpr, falseBeforeExpr, falseAfterExpr, Msg, ...)                                 \
    do{                                                                                                                             \
        if(Expr) {                                                                                                                  \
            trueExpr;                                                                                                               \
        }                                                                                                                           \
    }while(0L)


/*
 * ***********************************************************************************************************************************************
 *  functions
 * ***********************************************************************************************************************************************
 */

#   define M_Assert_SafeFunctionCall(foo, true_expression)                                                                          \
    do{                                                                                                                             \
        if(foo) {                                                                                                                   \
            true_expression;                                                                                                        \
        }                                                                                                                           \
    }while(0L)



#endif /* NDEBUG */


void __M_DBG(const char* const msg, ...);
void __M_DBG_ERR(const char* const msg, ...);
void __M_DBG_FILE(FILE * file, const char* const msg, ...);


void __M_assert_test();


#define PRINT_ONCE(...) \
    do{ \
        static int __printed = 0; \
        if(!__printed) { \
            printf(__VA_ARGS__); \
            __printed = 1; \
        } \
    }while(0L)


//#define TO_TXT(a) #a

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SMART_ASSERT_H_ */
