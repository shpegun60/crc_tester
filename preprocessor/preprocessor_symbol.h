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
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either xess or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 *
 */

#ifndef PREPROCESSOR_SYMBOL_H
#define PREPROCESSOR_SYMBOL_H 

/* -- Headers -- */

#include <preprocessor/preprocessor_if.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -- Macros-- */

/************************************************************************************************************************************************************
 * Macros which expand to common values
 */
#define PREPROCESSOR_PASS(...) __VA_ARGS__
#define PREPROCESSOR_EMPTY()
#define PREPROCESSOR_EAT(...)
#define PREPROCESSOR_COMMA() ,
#define PREPROCESSOR_COMMA_VARIADIC(...) ,
#define PREPROCESSOR_PLUS() +
#define PREPROCESSOR_ZERO() 0
#define PREPROCESSOR_ONE() 1
#define PREPROCESSOR_UNDERSCORE() _
#define PREPROCESSOR_COMMA_POINT() ;



/************************************************************************************************************************************************************
 * CommaIf implementation.
 *
 *      PREPROCESSOR_COMMA_IF_IMPL(1)() // expand to , (PREPROCESSOR_COMMA())
 *      PREPROCESSOR_COMMA_IF_IMPL(0)() // expand to nothing (PREPROCESSOR_EMPTY())
 */

#define PREPROCESSOR_COMMA_IF_IMPL(x) \
    PREPROCESSOR_IF_ELSE(x)(PREPROCESSOR_COMMA, PREPROCESSOR_EMPTY)

#define PREPROCESSOR_COMMA_IF(x) \
    PREPROCESSOR_COMMA_IF_IMPL(x)

/************************************************************************************************************************************************************
 * SymbolIf implementation.
 *
 *      PREPROCESSOR_SYMBOL_IF(1, PREPROCESSOR_PLUS)()      // expand to + (PREPROCESSOR_PLUS())
 *      PREPROCESSOR_SYMBOL_IF(0, PREPROCESSOR_PLUS)()      // expand to nothing (PREPROCESSOR_EMPTY())
 */

#define PREPROCESSOR_SYMBOL_IF_IMPL(x, symbol) \
    PREPROCESSOR_IF_ELSE(x)(symbol, PREPROCESSOR_EMPTY)

#define PREPROCESSOR_SYMBOL_IF(x, symbol) \
    PREPROCESSOR_SYMBOL_IF_IMPL(x, symbol)


#ifdef __cplusplus
}
#endif

#endif /* PREPROCESSOR_SYMBOL_H */
