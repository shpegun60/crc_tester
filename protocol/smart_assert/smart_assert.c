#include "smart_assert.h"

#ifndef NDEBUG

#include <stdarg.h>
#include <assert.h>

#define __M_CHECK_LIB_DATA(msg) (((msg)[0] == '[') && ((msg)[1] == 'd') && ((msg)[2] == ']'))

static inline void __M_SEND_MSG(const char* const header,
                                const char* const expr_str, const unsigned char expr,
                                const char* const file, const int line,
                                const char* const msg, va_list args)
{
    if(__M_CHECK_LIB_DATA(msg)) {
        // get library info
        int assertEna = va_arg(args, const int);
        char* descr = va_arg(args, char*);

        // send message if enabled message
        if(assertEna) {
            fprintf(stderr, "\n%s\n", header);
            fprintf(stderr, "Library Name: %s\n", descr);
            fprintf(stderr, "Assert failed:\t");
            vfprintf(stderr, &msg[3], args);
            fprintf(stderr, "\n");

            fprintf(stderr, "Expression:\t %s, value: %d\n", expr_str, expr);
            fprintf(stderr, "Source:\t\t %s, line: %d\n", file, line);
            fflush(stderr);
        }
    } else {
        fprintf(stderr, "\n%s\n", header);
        fprintf(stderr, "Assert failed:\t");
        vfprintf(stderr, msg, args);
        fprintf(stderr, "\n");

        fprintf(stderr, "Expression:\t %s, value: %d\n", expr_str, expr);
        fprintf(stderr, "Source:\t\t %s, line: %d\n", file, line);
        fflush(stderr);
    }
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
    va_list args;
    va_start(args, msg);
    __M_SEND_MSG("WARNING!!!", expr_str, expr, file, line, msg, args);
    va_end(args);
}

#undef __M_CHECK_LIB_DATA

#endif /* NDEBUG */

//--------------------------------------------------------------------------------------------------------
void __M_DBG(const char* const msg, ...)
{
    va_list args;
    va_start(args, msg);
    vfprintf(stdout, msg, args);
    fprintf(stdout, "\n");
    fflush(stdout);
    va_end(args);
}

void __M_DBG_ERR(const char* const msg, ...)
{
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n");
    fflush(stderr);
    va_end(args);
}

void __M_DBG_FILE(FILE * file, const char* const msg, ...)
{
    va_list args;
    va_start(args, msg);
    vfprintf(file, msg, args);
    fprintf(file, "\n");
    fflush(file);
    va_end(args);
}


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



