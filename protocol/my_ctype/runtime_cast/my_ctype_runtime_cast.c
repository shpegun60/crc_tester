#include "my_ctype_runtime_cast.h"

#ifdef T
#   undef T
#endif /* T */

#define T u8
#include "runtime_cast_template.c"
#define T u16
#include "runtime_cast_template.c"
#define T u24
#include "runtime_cast_template.c"
#define T u32
#include "runtime_cast_template.c"
#define T u64
#include "runtime_cast_template.c"

#define T c8
#include "runtime_cast_template.c"
#define T i8
#include "runtime_cast_template.c"
#define T i16
#include "runtime_cast_template.c"
#define T i24
#include "runtime_cast_template.c"
#define T i32
#include "runtime_cast_template.c"
#define T i64
#include "runtime_cast_template.c"

#define T f32
#include "runtime_cast_template.c"
#define T f64
#include "runtime_cast_template.c"
#define T f128
#include "runtime_cast_template.c"


#define T b
#include "runtime_cast_template.c"

#define T reg
#include "runtime_cast_template.c"
#define T sreg
#include "runtime_cast_template.c"
