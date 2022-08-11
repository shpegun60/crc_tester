/*
 *	Preprocessor Library by Parra Studios
 *	Copyright (C) 2016 - 2022 Vicente Eduardo Ferrer Garcia <vic798@gmail.com>
 *
 *	A generic header-only preprocessor metaprogramming library.
 *
 */

#ifndef PREPROCESSOR_CONCATENATION_H
#define PREPROCESSOR_CONCATENATION_H 1

/* -- Headers -- */

#ifdef __cplusplus
extern "C" {
#endif

/* -- Macros -- */


/************************************************************************************************************************************************************
 * Indirection around the standard ## concatenation operator. This simply
 * ensures that the arguments are expanded (once) before concatenation.
 */

#define PREPROCESSOR_EXPAND(x) x // neeed cut to core --------------------------------------------

#define PREPROCESSOR_CONCAT(a, ...) PREPROCESSOR_EXPAND(a ## __VA_ARGS__)
#define PREPROCESSOR_CONCAT3(a, b, ...) PREPROCESSOR_EXPAND(a ## b ## __VA_ARGS__)
#define PREPROCESSOR_CONCAT4(a, b, c, ...) PREPROCESSOR_EXPAND(a ## b ## c ## __VA_ARGS__)
#define PREPROCESSOR_CONCAT5(a, b, c, d, ...) PREPROCESSOR_EXPAND(a ## b ## c ## d ## __VA_ARGS__)
#define PREPROCESSOR_CONCAT6(a, b, c, d, e, ...) PREPROCESSOR_EXPAND(a ## b ## c ## d ## e ## __VA_ARGS__)
#define PREPROCESSOR_CONCAT7(a, b, c, d, e, f, ...) PREPROCESSOR_EXPAND(a ## b ## c ## d ## e ## f ## __VA_ARGS__)
#define PREPROCESSOR_CONCAT8(a, b, c, d, e, f, g, ...) PREPROCESSOR_EXPAND(a ## b ## c ## d ## e ## f ## g ## __VA_ARGS__)
#define PREPROCESSOR_CONCAT9(a, b, c, d, e, f, g, h, ...) PREPROCESSOR_EXPAND(a ## b ## c ## d ## e ## f ## g ## h ## __VA_ARGS__)
#define PREPROCESSOR_CONCAT10(a, b, c, d, e, f, g, h, i, ...) PREPROCESSOR_EXPAND(a ## b ## c ## d ## e ## f ## g ## h ## i ## __VA_ARGS__)
#define PREPROCESSOR_CONCAT11(a, b, c, d, e, f, g, h, i, j, ...) PREPROCESSOR_EXPAND(a ## b ## c ## d ## e ## f ## g ## h ## i ## j ## __VA_ARGS__)
#define PREPROCESSOR_CONCAT12(a, b, c, d, e, f, g, h, i, j, k, ...) PREPROCESSOR_EXPAND(a ## b ## c ## d ## e ## f ## g ## h ## i ## j ## k ## __VA_ARGS__)
#define PREPROCESSOR_CONCAT13(a, b, c, d, e, f, g, h, i, j, k, l, ...) PREPROCESSOR_EXPAND(a ## b ## c ## d ## e ## f ## g ## h ## i ## j ## k ## l ## __VA_ARGS__)
#define PREPROCESSOR_CONCAT14(a, b, c, d, e, f, g, h, i, j, k, l, m, ...) PREPROCESSOR_EXPAND(a ## b ## c ## d ## e ## f ## g ## h ## i ## j ## k ## l ## m ## __VA_ARGS__)
#define PREPROCESSOR_CONCAT15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, ...) PREPROCESSOR_EXPAND(a ## b ## c ## d ## e ## f ## g ## h ## i ## j ## k ## l ## m ## n ## __VA_ARGS__)
#define PREPROCESSOR_CONCAT16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, ...) PREPROCESSOR_EXPAND(a ## b ## c ## d ## e ## f ## g ## h ## i ## j ## k ## l ## m ## n ## o ## __VA_ARGS__)

/************************************************************************************************************************************************************/


#ifdef __cplusplus
}
#endif

#endif /* PREPROCESSOR_CONCATENATION_H */
