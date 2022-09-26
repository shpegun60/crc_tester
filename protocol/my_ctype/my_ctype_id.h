#ifndef __MY_CTYPE_ID_H__
#define __MY_CTYPE_ID_H__ 1

#include "preprocessor_if.h"
#include "preprocessor_type_eq.h"
#include "preprocessor_nat_eq.h"

// types mapping ------------------------------------------
#define VOID_TYPE 			0   // uni
// unsigned type
#define UINT8_TYPE  		1   // u8
#define UINT16_TYPE 		2   // u16
#define UINT32_TYPE 		3   // u32
#define UINT64_TYPE 		4   // u64
//signed type
#define CHAR_TYPE 			5   // c8
#define INT8_TYPE 			6   // i8
#define INT16_TYPE 			7   // i16
#define INT32_TYPE 			8   // i32
#define INT64_TYPE 			9   // i64
// floating point type
#define FLOAT_TYPE 			10  // f32
#define DOUBLE_TYPE 		11  // f64
#define LONG_DOUBLE_TYPE 	12  // f128     platform depend type
// other types
#define BOOL_TYPE 			13  // b
#define UINT24_TYPE 		14  // u24
#define INT24_TYPE 			15  // i24
#define REG_TYPE 			16  // reg      platform depend type
#define SREG_TYPE 			17  // sreg     platform depend type
#define TYPE_ARRAY_LENGTH   ((u8)(SREG_TYPE + 1))


// all types. Commit this if types defines upper--------------------------



typedef uint16_t        ;
typedef uint32_t        ;
typedef uint32_t        ;
typedef uint64_t        ;

typedef char            ;
typedef int8_t          ;
typedef int16_t         ;
typedef int32_t         ;
typedef int32_t         ;
typedef int64_t         ;

typedef float           ;
typedef double          ;
typedef long double     ; // platform depend type

// bool type -------------------------
typedef unsigned char   ;

// bus types defining (platform depend) ----------------------------------------------
typedef size_t      ;    // platform depend type
typedef ptrdiff_t   ;   // platform depend type
//------------------------------------------------------------------


/* ***************************************************************************
 * PREPROCESSOR MACRO FOR GETTING ID WHEN IS KNOWN TYPE
 *
 *      MY_CTYPE_GET_TYPE_ID(reg) // expands to 16
 *
 */
#define MY_CTYPE_GET_TYPE_ID(type) MY_CTYPE_GET_TYPE_ID_IMPL(type)
#define MY_CTYPE_GET_TYPE_ID_IMPL(type)\
    PREPROCESSOR_IF_ELSE(PREPROCESSOR_TYPE_EQ(type, uni))( \
        VOID_TYPE, \
        PREPROCESSOR_IF_ELSE(PREPROCESSOR_TYPE_EQ(type, u8))( \
            UINT8_TYPE, \
            PREPROCESSOR_IF_ELSE(PREPROCESSOR_TYPE_EQ(type, u16))( \
                UINT16_TYPE, \
                PREPROCESSOR_IF_ELSE(PREPROCESSOR_TYPE_EQ(type, u24))( \
                    UINT24_TYPE, \
                    PREPROCESSOR_IF_ELSE(PREPROCESSOR_TYPE_EQ(type, u32))( \
                        UINT32_TYPE, \
                        PREPROCESSOR_IF_ELSE(PREPROCESSOR_TYPE_EQ(type, u64))( \
                            UINT64_TYPE, \
                            PREPROCESSOR_IF_ELSE(PREPROCESSOR_TYPE_EQ(type, c8))( \
                                CHAR_TYPE, \
                                PREPROCESSOR_IF_ELSE(PREPROCESSOR_TYPE_EQ(type, i8))( \
                                    INT8_TYPE, \
                                    PREPROCESSOR_IF_ELSE(PREPROCESSOR_TYPE_EQ(type, i16))( \
                                        INT16_TYPE, \
                                        PREPROCESSOR_IF_ELSE(PREPROCESSOR_TYPE_EQ(type, i24))( \
                                            INT24_TYPE, \
                                            PREPROCESSOR_IF_ELSE(PREPROCESSOR_TYPE_EQ(type, i32))( \
                                                INT32_TYPE, \
                                                PREPROCESSOR_IF_ELSE(PREPROCESSOR_TYPE_EQ(type, i64))( \
                                                    INT64_TYPE, \
                                                    PREPROCESSOR_IF_ELSE(PREPROCESSOR_TYPE_EQ(type, f32))( \
                                                        FLOAT_TYPE, \
                                                        PREPROCESSOR_IF_ELSE(PREPROCESSOR_TYPE_EQ(type, f64))( \
                                                            DOUBLE_TYPE, \
                                                            PREPROCESSOR_IF_ELSE(PREPROCESSOR_TYPE_EQ(type, f128))( \
                                                                LONG_DOUBLE_TYPE, \
                                                                PREPROCESSOR_IF_ELSE(PREPROCESSOR_TYPE_EQ(type, b))( \
                                                                    BOOL_TYPE, \
                                                                    PREPROCESSOR_IF_ELSE(PREPROCESSOR_TYPE_EQ(type, reg))( \
                                                                        REG_TYPE, \
                                                                        PREPROCESSOR_IF_ELSE(PREPROCESSOR_TYPE_EQ(type, sreg))( \
                                                                            SREG_TYPE, \
                                                                            VOID_TYPE\
                                                                        )\
                                                                    )\
                                                                )\
                                                            )\
                                                        )\
                                                    )\
                                                )\
                                            )\
                                        )\
                                    )\
                                )\
                            )\
                        )\
                    )\
                )\
            )\
        )\
    )



/* ***************************************************************************
 * PREPROCESSOR MACRO FOR GETTING TYPE WHEN IS KNOWN ID
 *
 *      MY_CTYPE_GET_TYPE(UINT16_TYPE) // expands to u16
 *
 */

#define MY_CTYPE_GET_TYPE(id) MY_CTYPE_GET_TYPE_IMPL(id)
#define MY_CTYPE_GET_TYPE_IMPL(id)\
    PREPROCESSOR_IF_ELSE(PREPROCESSOR_NAT_EQ(id, VOID_TYPE))( \
        uni, \
        PREPROCESSOR_IF_ELSE(PREPROCESSOR_NAT_EQ(id, UINT8_TYPE))( \
            u8, \
            PREPROCESSOR_IF_ELSE(PREPROCESSOR_NAT_EQ(id, UINT16_TYPE))( \
                u16, \
                PREPROCESSOR_IF_ELSE(PREPROCESSOR_NAT_EQ(id, UINT24_TYPE))( \
                    u24, \
                    PREPROCESSOR_IF_ELSE(PREPROCESSOR_NAT_EQ(id, UINT32_TYPE))( \
                        u32, \
                        PREPROCESSOR_IF_ELSE(PREPROCESSOR_NAT_EQ(id, UINT64_TYPE))( \
                            u64, \
                            PREPROCESSOR_IF_ELSE(PREPROCESSOR_NAT_EQ(id, CHAR_TYPE))( \
                                c8, \
                                PREPROCESSOR_IF_ELSE(PREPROCESSOR_NAT_EQ(id, INT8_TYPE))( \
                                    i8, \
                                    PREPROCESSOR_IF_ELSE(PREPROCESSOR_NAT_EQ(id, INT16_TYPE))( \
                                        i16, \
                                        PREPROCESSOR_IF_ELSE(PREPROCESSOR_NAT_EQ(id, INT24_TYPE))( \
                                            i24, \
                                            PREPROCESSOR_IF_ELSE(PREPROCESSOR_NAT_EQ(id, INT32_TYPE))( \
                                                i32, \
                                                PREPROCESSOR_IF_ELSE(PREPROCESSOR_NAT_EQ(id, INT64_TYPE))( \
                                                    i64, \
                                                    PREPROCESSOR_IF_ELSE(PREPROCESSOR_NAT_EQ(id, FLOAT_TYPE))( \
                                                        f32, \
                                                        PREPROCESSOR_IF_ELSE(PREPROCESSOR_NAT_EQ(id, DOUBLE_TYPE))( \
                                                            f64, \
                                                            PREPROCESSOR_IF_ELSE(PREPROCESSOR_NAT_EQ(id, LONG_DOUBLE_TYPE))( \
                                                                f128, \
                                                                PREPROCESSOR_IF_ELSE(PREPROCESSOR_NAT_EQ(id, BOOL_TYPE))( \
                                                                    b, \
                                                                    PREPROCESSOR_IF_ELSE(PREPROCESSOR_NAT_EQ(id, REG_TYPE))( \
                                                                        reg, \
                                                                        PREPROCESSOR_IF_ELSE(PREPROCESSOR_NAT_EQ(id, SREG_TYPE))( \
                                                                            sreg, \
                                                                            uni\
                                                                        )\
                                                                    )\
                                                                )\
                                                            )\
                                                        )\
                                                    )\
                                                )\
                                            )\
                                        )\
                                    )\
                                )\
                            )\
                        )\
                    )\
                )\
            )\
        )\
    )

#endif // __MY_CTYPE_ID_H__
