#include "convert.h"

#ifdef T
    #undef T
#endif

#define T u8
#include "convert_template.c"
#define T u16
#include "convert_template.c"
#define T u24
#include "convert_template.c"
#define T u32
#include "convert_template.c"
#define T u64
#include "convert_template.c"

#define T c8
#include "convert_template.c"
#define T i8
#include "convert_template.c"
#define T i16
#include "convert_template.c"
#define T i24
#include "convert_template.c"
#define T i32
#include "convert_template.c"
#define T i64
#include "convert_template.c"

#define T f32
#include "convert_template.c"
#define T f64
#include "convert_template.c"
#define T f128
#include "convert_template.c"


#define T b
#include "convert_template.c"

#define T reg
#include "convert_template.c"
#define T sreg
#include "convert_template.c"

