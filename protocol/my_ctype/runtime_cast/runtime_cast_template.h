#include "preprocessor_template.h"
#include "my_ctypes.h"

#ifndef T
#   define T int
#endif /* T */

T TEMPLATE(reedRuntimeCast, T)(void* data, u8 type);
int TEMPLATE(writeRuntimeCast, T)(void* data, u8 type, T value);


#undef T
