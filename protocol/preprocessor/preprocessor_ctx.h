/*
 *	CREATED BY SHPEGUN60
 *
 *  PREPROCESSOR CONTEXT CAPTURE/GETTING LIBRARY
 */
#ifndef __PREPROCESSOR_CTX_H__
#define __PREPROCESSOR_CTX_H__ 1


/*********************************************************************************************************************
 *  PREPROCESSOR CONTEXT TYPE
 *
 *      PREPROCESSOR_CTX_TYPE(ctx)       // expands to --> void* const * const ctx
 */
#define PREPROCESSOR_CTX_TYPE(name) void* const * const name
#define PREPROCESSOR_CTX_TYPE_CAST(name) ((void** const) name)


/*********************************************************************************************************************
 *  PREPROCESSOR CONTEXT CAPTURE
 *
 *      uint8_t val1 = 254;
 *      uint16_t val2 = 12896;
 *      uint32_t val3 = 40000000;
 *
 *      PREPROCESSOR_CTX_CAPTURE({&dta, &val1, &val2, &val3})       // expands to --> (void* []){&dta, &val1, &val2, &val3}
 */
#define PREPROCESSOR_CTX_CAPTURE(...) (void* []) __VA_ARGS__


/*********************************************************************************************************************
 *  PREPROCESSOR CONTEXT GET
 *
 *          PREPROCESSOR_CTX_CAPTURE_GET(ctx,
 *                               uint8_t  *val1,
 *                               uint16_t *val2,
 *                               uint32_t *val3)
 *
 *                               // expands to-->
 *                              uint8_t *val1 = ctx[0];
 *                              uint16_t *val2 = ctx[1];
 *                              uint32_t *val3 = ctx[2];
 *
 */


#define PREPROCESSOR_CTX_GET(name, ...) PREPROCESSOR_MAP_CTX(name, PREPROCESSOR_EMPTY, __VA_ARGS__)



#define PREPROCESSOR_MAP_CTX(name, sep, ...) \
  PREPROCESSOR_IF(PREPROCESSOR_ARGS_NOT_EMPTY(__VA_ARGS__))(PREPROCESSOR_EVAL(PREPROCESSOR_MAP_CTX_INNER(name, sep, 0, __VA_ARGS__)))

#define PREPROCESSOR_MAP_CTX_INNER(name, sep, counter, cur_val, ...)                                                \
  cur_val = name[counter];                                                                                          \
  PREPROCESSOR_IF(PREPROCESSOR_ARGS_NOT_EMPTY(__VA_ARGS__)) (                                                       \
    PREPROCESSOR_DEFER2(_PREPROCESSOR_MAP_CTX_INNER)()(name, sep, PREPROCESSOR_INC(counter), __VA_ARGS__)           \
  )
#define _PREPROCESSOR_MAP_CTX_INNER() PREPROCESSOR_MAP_CTX_INNER


/*********************************************************************************************************************
    EXAMPLE:

#include <stdint.h>

void foo(PREPROCESSOR_CTX_TYPE(ctx))
{
    PREPROCESSOR_CTX_GET(ctx,
                                   uint8_t  *val1,
                                   uint16_t *val2,
                                   uint32_t *val3)
}

int main()
{
    uint8_t val1 = 254;
    uint16_t val2 = 12896;
    uint32_t val3 = 40000000;

    foo(PREPROCESSOR_CTX_CAPTURE({
                                    &val1,
                                    &val2,
                                    &val3
                                 }));
}

*/


#endif /* __PREPROCESSOR_CTX_H__ */
