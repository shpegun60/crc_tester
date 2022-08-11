/*
 *	Preprocessor Library by Parra Studios
 *	Copyright (C) 2016 - 2022 Vicente Eduardo Ferrer Garcia <vic798@gmail.com>
 *
 *	A generic header-only preprocessor metaprogramming library.
 *
 */

#ifndef PREPROCESSOR_LOGICAL_H
#define PREPROCESSOR_LOGICAL_H 

/* -- Headers -- */

#include <preprocessor/preprocessor_complement.h>
#include <preprocessor/preprocessor_concatenation.h>
#include <preprocessor/preprocessor_boolean.h>


#ifdef __cplusplus
extern "C" {
#endif

/* -- Methods -- */

/************************************************************************************************************************************************************
 * Simple preprocessor BINARY bit operation:
 *
 *      PREPROCESSOR_BIT_AND(1)(x) //Expands to x
 *      PREPROCESSOR_BIT_AND(0)(x) //Expands to 0
 *
 *      PREPROCESSOR_BIT_OR(1)(x) //Expands to 1
 *      PREPROCESSOR_BIT_OR(0)(x) //Expands to x
 *
 *      PREPROCESSOR_BIT_XOR(1)(x) //Expands to !x
 *      PREPROCESSOR_BIT_XOR(0)(x) //Expands to x
 *
 *      x - must be 0 or 1
 *
 */

//BINARY AND
#define PREPROCESSOR_BIT_AND(x) PREPROCESSOR_CONCAT(_PREPROCESSOR_BIT_AND_, x)
#define _PREPROCESSOR_BIT_AND_0(x) 0
#define _PREPROCESSOR_BIT_AND_1(x) x

//BINARY OR
#define PREPROCESSOR_BIT_OR(x) PREPROCESSOR_CONCAT(_PREPROCESSOR_BIT_OR_, x)
#define _PREPROCESSOR_BIT_OR_0(x) x
#define _PREPROCESSOR_BIT_OR_1(x) 1

//BINARY XOR
#define PREPROCESSOR_BIT_XOR(x) PREPROCESSOR_CONCAT(_PREPROCESSOR_BIT_XOR_, x)
#define _PREPROCESSOR_BIT_XOR_0(x) x
#define _PREPROCESSOR_BIT_XOR_1(x) PREPROCESSOR_COMPL(PREPROCESSOR_BOOL(x))



/************************************************************************************************************************************************************
 * Simple preprocessor BINARY operation (other implementation):
 * Logical OR. Simply performs a lookup.
 */
#define PREPROCESSOR_OR(a,b) PREPROCESSOR_CONCAT3(_PREPROCESSOR_OR_, a, b)
#define _PREPROCESSOR_OR_00 0
#define _PREPROCESSOR_OR_01 1
#define _PREPROCESSOR_OR_10 1
#define _PREPROCESSOR_OR_11 1

/************************************************************************************************************************************************************
 * Simple preprocessor BINARY operation (other implementation):
 * Logical AND. Simply performs a lookup.
 */
#define PREPROCESSOR_AND(a,b) PREPROCESSOR_CONCAT3(_PREPROCESSOR_AND_, a, b)
#define _PREPROCESSOR_AND_00 0
#define _PREPROCESSOR_AND_01 0
#define _PREPROCESSOR_AND_10 0
#define _PREPROCESSOR_AND_11 1


/************************************************************************************************************************************************************
 * BITAND - for user logic macro
 *
 *      BITAND(0)(x,y,z) // Expands to 0
 *      BITAND(1)(x,y,z) // Expands to x,y,z
 */
#define PREPROCESSOR_BITAND(x) PREPROCESSOR_CONCAT(PREPROCESSOR_BITAND_, x)
#define PREPROCESSOR_BITAND_0(...) 0
#define PREPROCESSOR_BITAND_1(...) __VA_ARGS__


#ifdef __cplusplus
}
#endif

#endif /* PREPROCESSOR_LOGICAL_H */
