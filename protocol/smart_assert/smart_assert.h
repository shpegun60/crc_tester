/**
 * @file    smart_assert.h
 * @brief   This library for check expressions and send message if expressions is true on RUNTIME and on DEBUG version.
 *          If NDEBUG is not defined some expressions has change to empty define. (programmer must uncommit NDEBUG if code version released)
 *
 * @date
 */

#ifndef __SMART_ASSERT_H_
#define __SMART_ASSERT_H_


// this library recomended include only .c or .cpp file!!!!!!!!!!!!!!!!

//   C++ linking for mixed C++/C code
#ifdef __cplusplus
extern "C" {
#endif


//#define NDEBUG

#define M_EMPTY /* ignored expression */
#define M_ALWAYS 1 /* always proceed expression */


void __M_SEND_DEBUG_INFO(const char* const msg, ...);
void __M_assert_test();

#ifndef NDEBUG /* --------------------------------------------------------------------------------------------------------- */

void __M_Error(const char* expr_str, unsigned char expr, const char* file, int line, const char* msg, ...);
void __M_Warning(const char* expr_str, unsigned char expr, const char* file, int line, const char* msg, ...);

/*
 * ***********************************************************************************************************************************************
 *  defines simple if NDEBUG disable expr
 * ***********************************************************************************************************************************************
 */
// this macros will disable if NDEBUG is defined
#define M_Assert_disableExpr(...) __VA_ARGS__

/*
 * ***********************************************************************************************************************************************
 *  defines for break program
 * ***********************************************************************************************************************************************
 */

#   define M_Assert_Break(Expr, beforeExpr, afterExpr, Msg, arg...)\
    do{\
        if (Expr) {\
            beforeExpr;\
            __M_Error((#Expr), (Expr), (__FILE__), (__LINE__), (Msg), ##arg);\
            afterExpr;/* ignored if NDEBUG because programm is break, but compiler not known it*/\
        }\
    }while(0L)

#   define M_Assert_BreakSaveCheck(Expr, beforeExpr, afterExpr, Msg, arg...)\
    do{\
        if (Expr) {\
            beforeExpr;\
            __M_Error((#Expr), (Expr), (__FILE__), (__LINE__), (Msg), ##arg);\
            afterExpr;/* ignored if NDEBUG because programm is break, but compiler not known it*/\
        }\
    }while(0L)

/*
 * ***********************************************************************************************************************************************
 *  defines no break program
 * ***********************************************************************************************************************************************
 */

#   define M_Assert_Warning(Expr, beforeExpr, afterExpr, Msg, arg...)\
    do{\
        if (Expr) {\
            beforeExpr;\
            __M_Warning((#Expr), (Expr), (__FILE__), (__LINE__), (Msg), ##arg);\
            afterExpr;\
        }\
    }while(0L)

#   define M_Assert_WarningSaveCheck(Expr, beforeExpr, afterExpr, Msg, arg...)\
    do{\
        if (Expr) {\
            beforeExpr;\
            __M_Warning((#Expr), (Expr), (__FILE__), (__LINE__), (Msg), ##arg);\
            afterExpr;\
        }\
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
    }while(0L);

#else
/*
 * ***********************************************************************************************************************************************
 *  defines simple if NDEBUG disable expr
 * ***********************************************************************************************************************************************
 */

#define M_Assert_disableExpr(...)
/*
 * ***********************************************************************************************************************************************
 *  defines for break program
 * ***********************************************************************************************************************************************
 */

#   define M_Assert_Break(Expr, beforeExpr, afterExpr, Msg, arg...)

#   define M_Assert_BreakSaveCheck(Expr, beforeExpr, afterExpr, Msg, arg...)\
    do{\
        if (Expr) {\
            beforeExpr;\
            afterExpr;\
        }\
    }while(0L)

/*
 * ***********************************************************************************************************************************************
 *  defines no break program
 * ***********************************************************************************************************************************************
 */

#   define M_Assert_Warning(Expr, beforeExpr, afterExpr, Msg, arg...)

#   define M_Assert_WarningSaveCheck(Expr, beforeExpr, afterExpr, Msg, arg...)\
    do{\
        if (Expr) {\
            beforeExpr;\
            afterExpr;\
        }\
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
        }\
    }while(0L);



#endif /* NDEBUG */



#define PRINT_ONCE(...) \
    do{ \
        static int __printed = 0; \
        if(!__printed) { \
            printf(__VA_ARGS__); \
            __printed=1; \
        } \
    }while(0L)


//#define TO_TXT(a) #a

#ifdef __cplusplus
}
#endif

#endif // __SMART_ASSERT
