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

/*
 * ***********************************************************************
 *  smart asserts settings
 * ***********************************************************************
 */

//#define NDEBUG                              // uncommit this if program in release version
#define M_MESSAGE_ALWAYS_ENABLE         1   // enabling runtime assert message filter and additon library information (see additional information macro)
#define M_ASSERT_MSG_TEXT_DISABLE       1   // disabling sending assert message text and do not saving this in .text section
#define M_ASSERT_EXPR_DISABLE           1   // disabling sending assert expression text, value and do not saving this in .text section
#define M_ASSERT_FILE_LINE_TEXT_DISABLE 1   // disabling sending assert file, line text and do not saving this in .text section

/*
 * ***********************************************************************
 */


#if defined(M_ASSERT_FILE_LINE_TEXT_DISABLE)
#   define ASSERT_FILE(...)     0 //"FILE"
#   define ASSERT_LINE(...)     0 //"LINE"
#else
#   define ASSERT_FILE(...)     __VA_ARGS__
#   define ASSERT_LINE(...)     __VA_ARGS__
#endif /* defined(M_ASSERT_FILE_LINE_TEXT_DISABLE) */
#
#
#if defined(M_ASSERT_EXPR_DISABLE)
#   define ASSERT_EXPR_TXT(...) 0 //"EXPR"
#   define ASSERT_EXPR(...)     1
#else
#   define ASSERT_EXPR_TXT(...) __VA_ARGS__
#   define ASSERT_EXPR(...)     __VA_ARGS__
#endif /* defined(M_ASSERT_EXPR_TEXT_DISABLE) */
#
#
#if defined(M_ASSERT_MSG_TEXT_DISABLE)
#   define ASSERT_ERROR(Expr_txt, Expr, File, Line, Msg, ...)   __M_Error(ASSERT_EXPR_TXT(Expr_txt), ASSERT_EXPR(Expr), ASSERT_FILE(File), ASSERT_LINE(Line), 0)
#   define ASSERT_WARNING(Expr_txt, Expr, File, Line, Msg, ...) __M_Warning(ASSERT_EXPR_TXT(Expr_txt), ASSERT_EXPR(Expr), ASSERT_FILE(File), ASSERT_LINE(Line), 0)
#else
#   define ASSERT_ERROR(Expr_txt, Expr, File, Line, Msg, ...)   __M_Error(ASSERT_EXPR_TXT(Expr_txt), ASSERT_EXPR(Expr), ASSERT_FILE(File), ASSERT_LINE(Line), (Msg), ##__VA_ARGS__)
#   define ASSERT_WARNING(Expr_txt, Expr, File, Line, Msg, ...) __M_Warning(ASSERT_EXPR_TXT(Expr_txt), ASSERT_EXPR(Expr), ASSERT_FILE(File), ASSERT_LINE(Line), (Msg), ##__VA_ARGS__)
#endif /* defined(M_ASSERT_MSG_TEXT_DISABLE) */

/*
 * ***********************************************************************
 *  smart asserts additional information macro
 * ***********************************************************************
 */
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
            ASSERT_ERROR((#Expr), (Expr), (__FILE__), (__LINE__), (Msg), ##__VA_ARGS__);                                                \
            afterExpr; /* ignored because programm is break, but compiler not known it */                                               \
        }                                                                                                                               \
    }while(0L)

#   define M_Assert_BreakSaveCheck(Expr, beforeExpr, afterExpr, Msg, ...)                                                               \
    do{                                                                                                                                 \
        if (Expr) {                                                                                                                     \
            beforeExpr;                                                                                                                 \
            ASSERT_ERROR((#Expr), (Expr), (__FILE__), (__LINE__), (Msg), ##__VA_ARGS__);                                                \
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
            ASSERT_ERROR("if " #Expr " not confirmed: passed to else", (Expr), (__FILE__), (__LINE__), (Msg), ##__VA_ARGS__);           \
            falseAfterExpr;                                                                                                             \
        }                                                                                                                               \
    }while(0L)

#   define M_Assert_BreakElseSaveCheck(Expr, trueExpr, falseBeforeExpr, falseAfterExpr, Msg, ...)                                       \
    do{                                                                                                                                 \
        if(Expr) {                                                                                                                      \
            trueExpr;                                                                                                                   \
        } else {  /* ignored else expression if NDEBUG */                                                                               \
            falseBeforeExpr;                                                                                                            \
            ASSERT_ERROR("if " #Expr " not confirmed: passed to else", (Expr), (__FILE__), (__LINE__), (Msg), ##__VA_ARGS__);           \
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
            ASSERT_WARNING((#Expr), (Expr), (__FILE__), (__LINE__), (Msg), ##__VA_ARGS__);                                              \
            afterExpr;                                                                                                                  \
        }                                                                                                                               \
    }while(0L)

#   define M_Assert_WarningSaveCheck(Expr, beforeExpr, afterExpr, Msg, ...)                                                             \
    do{                                                                                                                                 \
        if (Expr) {                                                                                                                     \
            beforeExpr;                                                                                                                 \
            ASSERT_WARNING((#Expr), (Expr), (__FILE__), (__LINE__), (Msg), ##__VA_ARGS__);                                              \
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
            ASSERT_WARNING("if " #Expr " not confirmed: passed to else", (Expr), (__FILE__), (__LINE__), (Msg), ##__VA_ARGS__);         \
            falseAfterExpr;                                                                                                             \
        }                                                                                                                               \
    }while(0L)

#   define M_Assert_WarningElseSaveCheck(Expr, trueExpr, falseBeforeExpr, falseAfterExpr, Msg, ...)                                     \
    do{                                                                                                                                 \
        if(Expr) {                                                                                                                      \
            trueExpr;                                                                                                                   \
        } else { /* ignored else expression if NDEBUG */                                                                                \
            falseBeforeExpr;                                                                                                            \
            ASSERT_WARNING("if " #Expr " not confirmed: passed to else", (Expr), (__FILE__), (__LINE__), (Msg), ##__VA_ARGS__);         \
            falseAfterExpr;                                                                                                             \
        }                                                                                                                               \
    }while(0L)


/*
 * ***********************************************************************************************************************************************
 *  functions
 * ***********************************************************************************************************************************************
 */

#   define M_Assert_SafeFunctionCall(foo, true_expression)                                                                              \
    do{                                                                                                                                 \
        if(foo) {                                                                                                                       \
            true_expression;                                                                                                            \
        } else {                                                                                                                        \
            ASSERT_WARNING((#foo), (0), (__FILE__), (__LINE__), ("NO exists function"));                                                \
        }                                                                                                                               \
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


#define PRINT_ONCE(...)                                                                                                             \
    do{                                                                                                                             \
        static int __printed = 0;                                                                                                   \
        if(!__printed) {                                                                                                            \
            printf(__VA_ARGS__);                                                                                                    \
            __printed = 1;                                                                                                          \
        }                                                                                                                           \
    }while(0L)


//#define TO_TXT(a) #a

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SMART_ASSERT_H_ */
