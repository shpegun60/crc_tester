#ifndef MY_CTYPE_RUNTIME_CAST_H
#define MY_CTYPE_RUNTIME_CAST_H

#ifdef T
#   undef T
#endif /* T */

#include "my_ctypes.h"


// MAIN TEMPLATE FUNCTIOINS--------------------------------------------
#define T u8
#include "runtime_cast_template.h"
#define T u16
#include "runtime_cast_template.h"
#define T u24
#include "runtime_cast_template.h"
#define T u32
#include "runtime_cast_template.h"
#define T u64
#include "runtime_cast_template.h"

#define T c8
#include "runtime_cast_template.h"
#define T i8
#include "runtime_cast_template.h"
#define T i16
#include "runtime_cast_template.h"
#define T i24
#include "runtime_cast_template.h"
#define T i32
#include "runtime_cast_template.h"
#define T i64
#include "runtime_cast_template.h"

#define T f32
#include "runtime_cast_template.h"
#define T f64
#include "runtime_cast_template.h"
#define T f128
#include "runtime_cast_template.h"


#define T b
#include "runtime_cast_template.h"

#define T reg
#include "runtime_cast_template.h"
#define T sreg
#include "runtime_cast_template.h"

#endif /* MY_CTYPE_RUNTIME_CAST_H */
