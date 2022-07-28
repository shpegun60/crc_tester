#ifndef __CONVERT_H__
#define __CONVERT_H__

#ifdef T
    #undef T
#endif

#include "my_ctypes.h"

#ifndef CONVERT_TEST_DISABLE
    //#define CONVERT_TEST_DISABLE
#endif /* CONVERT_TEST_DISABLE */


#define T u8
#include "convert_template.h"
#define T u16
#include "convert_template.h"
#define T u24
#include "convert_template.h"
#define T u32
#include "convert_template.h"
#define T u64
#include "convert_template.h"

#define T c8
#include "convert_template.h"
#define T i8
#include "convert_template.h"
#define T i16
#include "convert_template.h"
#define T i24
#include "convert_template.h"
#define T i32
#include "convert_template.h"
#define T i64
#include "convert_template.h"

#define T f32
#include "convert_template.h"
#define T f64
#include "convert_template.h"
#define T f128
#include "convert_template.h"


#define T b
#include "convert_template.h"

#define T reg
#include "convert_template.h"
#define T sreg
#include "convert_template.h"


#ifndef CONVERT_TEST_DISABLE
int convertTest();
#endif /* CONVERT_TEST_DISABLE */


#endif /* __CONVERT_H__ */
