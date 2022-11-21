#include "preprocessor_template.h"
#include "my_ctypes.h"

#ifndef T
#   define T int
#endif /* T */

T TEMPLATE(readRuntimeCast, T)(const void* const data, const reg type, reg* const ok);
reg TEMPLATE(writeRuntimeCast, T)(void* const data, const reg type, const T value);


#undef T
