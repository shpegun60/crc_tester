/*
 * Copyright (C) 2012 William Swanson
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the names of the authors or
 * their institutions shall not be used in advertising or otherwise to
 * promote the sale, use or other dealings in this Software without
 * prior written authorization from the authors.
 */

#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include "preprocessor_base_old.h"

#define MAP_END(...)
#define MAP_OUT

#define MAP_GET_END2() 0, MAP_END
#define MAP_GET_END1(...) MAP_GET_END2
#define MAP_GET_END(...) MAP_GET_END1
#define MAP_NEXT0(test, next, ...) next MAP_OUT
#define MAP_NEXT1(test, next) DEFER1 ( MAP_NEXT0 ) ( test, next, 0)
#define MAP_NEXT(test, next)  MAP_NEXT1(MAP_GET_END test, next)

#define MAP0(f, x, peek, ...) f(x) DEFER1 ( MAP_NEXT(peek, MAP1) ) ( f, peek, __VA_ARGS__ )
#define MAP1(f, x, peek, ...) f(x) DEFER1 ( MAP_NEXT(peek, MAP0) ) ( f, peek, __VA_ARGS__ )

#define MAP0_UD(f, userdata, x, peek, ...) f(x,userdata) DEFER1 ( MAP_NEXT(peek, MAP1_UD) ) ( f, userdata, peek, __VA_ARGS__ )
#define MAP1_UD(f, userdata, x, peek, ...) f(x,userdata) DEFER1 ( MAP_NEXT(peek, MAP0_UD) ) ( f, userdata, peek, __VA_ARGS__ )

#define MAP0_UD_I(f, userdata, index, x, peek, ...) f(x,userdata,index) DEFER1 ( MAP_NEXT(peek, MAP1_UD_I) ) ( f, userdata, MAP_INC(index), peek, __VA_ARGS__ )
#define MAP1_UD_I(f, userdata, index, x, peek, ...) f(x,userdata,index) DEFER1 ( MAP_NEXT(peek, MAP0_UD_I) ) ( f, userdata, MAP_INC(index), peek, __VA_ARGS__ )

#define MAP_LIST0(f, x, peek, ...) , f(x) DEFER1 ( MAP_NEXT(peek, MAP_LIST1) ) ( f, peek, __VA_ARGS__ )
#define MAP_LIST1(f, x, peek, ...) , f(x) DEFER1 ( MAP_NEXT(peek, MAP_LIST0) ) ( f, peek, __VA_ARGS__ )
#define MAP_LIST2(f, x, peek, ...)   f(x) DEFER1 ( MAP_NEXT(peek, MAP_LIST1) ) ( f, peek, __VA_ARGS__ )

#define MAP_LIST0_UD(f, userdata, x, peek, ...) , f(x, userdata) DEFER1 ( MAP_NEXT(peek, MAP_LIST1_UD) ) ( f, userdata, peek, __VA_ARGS__ )
#define MAP_LIST1_UD(f, userdata, x, peek, ...) , f(x, userdata) DEFER1 ( MAP_NEXT(peek, MAP_LIST0_UD) ) ( f, userdata, peek, __VA_ARGS__ )
#define MAP_LIST2_UD(f, userdata, x, peek, ...)   f(x, userdata) DEFER1 ( MAP_NEXT(peek, MAP_LIST1_UD) ) ( f, userdata, peek, __VA_ARGS__ )

#define MAP_LIST0_UD_I(f, userdata, index, x, peek, ...) , f(x, userdata, index) DEFER1 ( MAP_NEXT(peek, MAP_LIST1_UD_I) ) ( f, userdata, MAP_INC(index), peek, __VA_ARGS__ )
#define MAP_LIST1_UD_I(f, userdata, index, x, peek, ...) , f(x, userdata, index) DEFER1 ( MAP_NEXT(peek, MAP_LIST0_UD_I) ) ( f, userdata, MAP_INC(index), peek, __VA_ARGS__ )
#define MAP_LIST2_UD_I(f, userdata, index, x, peek, ...)   f(x, userdata, index) DEFER1 ( MAP_NEXT(peek, MAP_LIST0_UD_I) ) ( f, userdata, MAP_INC(index), peek, __VA_ARGS__ )

/**********************************************************************************************************************
 * Applies the function macro `f` to each of the remaining parameters.
 */
#define MAP_PAR(f, ...) EVAL(MAP1(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

/**********************************************************************************************************************
 * Applies the function macro `f` to each of the remaining parameters and
 * inserts commas between the results.
 */
#define MAP_LIST(f, ...) EVAL(MAP_LIST2(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

/**********************************************************************************************************************
 * Applies the function macro `f` to each of the remaining parameters and passes userdata as the second parameter to each invocation,
 * e.g. MAP_UD(f, x, a, b, c) evaluates to f(a, x) f(b, x) f(c, x)
 */
#define MAP_UD(f, userdata, ...) EVAL(MAP1_UD(f, userdata, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

/**********************************************************************************************************************
 * Applies the function macro `f` to each of the remaining parameters, inserts commas between the results,
 * and passes userdata as the second parameter to each invocation,
 * e.g. MAP_LIST_UD(f, x, a, b, c) evaluates to f(a, x), f(b, x), f(c, x)
 */
#define MAP_LIST_UD(f, userdata, ...) EVAL(MAP_LIST2_UD(f, userdata, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

/**********************************************************************************************************************
 * Applies the function macro `f` to each of the remaining parameters, passes userdata as the second parameter to each invocation,
 * and the index of the invocation as the third parameter,
 * e.g. MAP_UD_I(f, x, a, b, c) evaluates to f(a, x, 0) f(b, x, 1) f(c, x, 2)
 */
#define MAP_UD_I(f, userdata, ...) EVAL(MAP1_UD_I(f, userdata, 0, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

/**********************************************************************************************************************
 * Applies the function macro `f` to each of the remaining parameters, inserts commas between the results,
 * passes userdata as the second parameter to each invocation, and the index of the invocation as the third parameter,
 * e.g. MAP_LIST_UD_I(f, x, a, b, c) evaluates to f(a, x, 0), f(b, x, 1), f(c, x, 2)
 */
#define MAP_LIST_UD_I(f, userdata, ...) EVAL(MAP_LIST2_UD_I(f, userdata, 0, __VA_ARGS__, ()()(), ()()(), ()()(), 0))


#endif
