#include "smart_assert.h"
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>


void __M_SEND_DEBUG_INFO(const char* const msg, ...)
{
    va_list args;
    va_start(args, msg);
    vfprintf(stdout, msg, args);
    fprintf(stdout, "\n");
    fflush(stdout);
    va_end(args);
}

#ifndef NDEBUG
static inline void __M_SEND_MSG(const char* const header, const char* const expr_str, const unsigned char expr, const char* const file, const int line, const char* const msg, const va_list args)
{
    (void)expr;
    fprintf(stderr, "\n%s\n", header);
    fprintf(stderr, "Assert failed:\t");
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n");

    fprintf(stderr, "Expression:\t %s \n", expr_str);
    fprintf(stderr, "Source:\t\t %s, line: %d\n", file, line);
    fflush(stderr);
}

void __M_Error(const char* const expr_str, const unsigned char expr, const char* const file, const int line, const char* const msg, ...)
{
    va_list args;
    va_start(args, msg);
    __M_SEND_MSG("PROGRAMM EXIT WITH ERROR!!!", expr_str, expr, file, line, msg, args);
    va_end(args);
    abort(); // exit programm
}

void __M_Warning(const char* const expr_str, const unsigned char expr, const char* const file, const int line, const char* const msg, ...)
{
    (void)expr;

    va_list args;
    va_start(args, msg);
    __M_SEND_MSG("WARNING!!!", expr_str, expr, file, line, msg, args);
    va_end(args);
}

#endif /* NDEBUG */

void __M_assert_test()
{
    int i = 1;
    //M_Assert_Break(1, M_EMPTY, M_EMPTY, "RUNTIME ERROR Assert test: M_Assert_Break without parameters");
    //M_Assert_Break(1, M_EMPTY, M_EMPTY, "RUNTIME ERROR Assert test: M_Assert_Break with parameter: %d", 123);
    //M_Assert_Break(1, M_EMPTY, M_EMPTY, "RUNTIME ERROR Assert test:" "M_Assert_Break with two parameter: %d, %d", 123, 456);


    M_Assert_Warning(1, M_EMPTY, M_EMPTY, "RUNTIME WARNING Assert test: M_Assert_Break without parameters");
    M_Assert_Warning(1, ++i, M_EMPTY, "RUNTIME WARNING Assert test: M_Assert_Break with parameter: %d", i);
    M_Assert_Warning(1, M_EMPTY, ++i, "RUNTIME WARNING Assert test: M_Assert_Break with two parameter: %d, %d", i, 456);
    M_Assert_Warning(1, M_EMPTY, M_EMPTY, "RUNTIME WARNING Assert test:" "M_Assert_Break with two parameter: %d, %d", i, 456);
    M_Assert_Break(1, ++i, M_EMPTY, "RUNTIME ERROR Assert test: M_Assert_Break with two parameter: %d, %d", i, 456);

    (void)i;
}


