/*
 *	Preprocessor Library by Parra Studios
 *	Copyright (C) 2016 - 2022 Vicente Eduardo Ferrer Garcia <vic798@gmail.com>
 *
 *	A generic header-only preprocessor metaprogramming library.
 *
 */

#ifndef PREPROCESSOR_DETECTION_H
#define PREPROCESSOR_DETECTION_H 1

/* -- Headers -- */

#include <preprocessor/preprocessor_arguments.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -- Macros -- */

/************************************************************************************************************************************************************
 * Expects a single input (not containing commas). Returns 1 if the input is
 * PREPROCESSOR_PROBE() and otherwise returns 0.
 *
 * This can be useful as the basis of a NOT function.
 *
 * This macro abuses the fact that PREPROCESSOR_PROBE() contains a comma while other valid
 * inputs must not.
 *
 * PREPROCESSOR_IS_PROBE(PREPROCESSOR_PROBE())      // Expands to 1
 * PREPROCESSOR_IS_PROBE(xxx)                       // Expands to 0
 */
#define PREPROCESSOR_IS_PROBE(...) _PREPROCESSOR_IS_PROBE(__VA_ARGS__) // expand all arguments
#define _PREPROCESSOR_IS_PROBE(...) PREPROCESSOR_ARGS_SECOND(__VA_ARGS__, 0)
#define PREPROCESSOR_PROBE() ~, 1

/************************************************************************************************************************************************************
 * Detection for parenthesis --> ()
 *
 *      PREPROCESSOR_IS_PAREN(())  // Expands to 1
 *      PREPROCESSOR_IS_PAREN(xxx) // Expands to 0
 */
#define PREPROCESSOR_IS_PAREN(x) _PREPROCESSOR_IS_PAREN(x) // expand x
#define _PREPROCESSOR_IS_PAREN(x) PREPROCESSOR_IS_PROBE(PREPROCESSOR_IS_PAREN_PROBE x)
#define PREPROCESSOR_IS_PAREN_PROBE(...) PREPROCESSOR_PROBE()

/************************************************************************************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* PREPROCESSOR_DETECTION_H */
