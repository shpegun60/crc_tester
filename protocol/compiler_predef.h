#ifndef COMPILER_PREDEF_H
#define COMPILER_PREDEF_H

/*
 * *********************************************************************
 * COMPILER MACRO
 * *********************************************************************
 */

#if defined(__STDC__)
#  if defined(__STDC_VERSION__)
#    if (__STDC_VERSION__ >= 201710L)
#      define PREDEF_C_LANGUAGE_STANDARD 2018
#    elif (__STDC_VERSION__ >= 201112L)
#      define PREDEF_C_LANGUAGE_STANDARD 2011
#    elif (__STDC_VERSION__ >= 199901L)
#      define PREDEF_C_LANGUAGE_STANDARD 1999
#    elif (__STDC_VERSION__ >= 199409L)
#      define PREDEF_C_LANGUAGE_STANDARD 1995
#    endif
#  else
#      define PREDEF_C_LANGUAGE_STANDARD 1990
#  endif
#else
#      define PREDEF_C_LANGUAGE_STANDARD 1972
#endif /* compiler version selector */



#if defined(__clang__)
    /* Clang/LLVM. ---------------------------------------------- */
#   define PREDEF_COMPILER_NAME "Clang/LLVM"

#elif defined(__ICC) || defined(__INTEL_COMPILER)
    /* Intel ICC/ICPC. ------------------------------------------ */
#   define PREDEF_COMPILER_NAME "Intel ICC/ICPC"

#elif (defined(__GNUC__) || defined(__GNUG__)) && !(defined(__clang__) || defined(__INTEL_COMPILER))
    /* GNU GCC/G++. --------------------------------------------- */
#   define PREDEF_COMPILER_NAME "GNU GCC/G++"

#elif defined(__HP_cc) || defined(__HP_aCC)
    /* Hewlett-Packard C/aC++. ---------------------------------- */
#   define PREDEF_COMPILER_NAME "Hewlett-Packard C/aC++"

#elif defined(__IBMC__) || defined(__IBMCPP__)
    /* IBM XL C/C++. -------------------------------------------- */
#   define PREDEF_COMPILER_NAME "IBM XL C/C++"

#elif defined(_MSC_VER)
    /* Microsoft Visual Studio. --------------------------------- */
#   define PREDEF_COMPILER_NAME "Microsoft Visual Studio"

#elif defined(__PGI)
    /* Portland Group PGCC/PGCPP. ------------------------------- */
#   define PREDEF_COMPILER_NAME "Portland Group PGCC/PGCPP"

#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
    /* Oracle Solaris Studio. ----------------------------------- */
#   define PREDEF_COMPILER_NAME "Oracle Solaris Studio"

#else
#   define PREDEF_COMPILER_NAME "Undefined"
#endif /* compiler name selector */

#endif // COMPILER_PREDEF_H
