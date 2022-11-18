#ifndef __MY_CTYPE_ID_H__
#define __MY_CTYPE_ID_H__ 1

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
#define TYPE_ARRAY_LENGTH   (SREG_TYPE + 1)


/* ***************************************************************************
 * PREPROCESSOR MACRO FOR GETTING ID WHEN IS KNOWN TYPE
 *
 *      MY_CTYPE_GET_TYPE_ID(reg) // expands to 16
 *
 */

#define MY_CTYPE_GET_TYPE_ID(type) MY_CTYPE_GET_TYPE_ID_IMPL(type)
#define MY_CTYPE_GET_TYPE_ID_IMPL(type) MY_CTYPE_GET_TYPE_ID_ ## type

#define MY_CTYPE_GET_TYPE_ID_uni    VOID_TYPE               // uni
// unsigned type
#define MY_CTYPE_GET_TYPE_ID_u8     UINT8_TYPE              // u8
#define MY_CTYPE_GET_TYPE_ID_u16    UINT16_TYPE             // u16
#define MY_CTYPE_GET_TYPE_ID_u32    UINT32_TYPE             // u32
#define MY_CTYPE_GET_TYPE_ID_u64    UINT64_TYPE             // u64
//signed type
#define MY_CTYPE_GET_TYPE_ID_c8     CHAR_TYPE               // c8
#define MY_CTYPE_GET_TYPE_ID_i8     INT8_TYPE               // i8
#define MY_CTYPE_GET_TYPE_ID_i16    INT16_TYPE              // i16
#define MY_CTYPE_GET_TYPE_ID_i32    INT32_TYPE              // i32
#define MY_CTYPE_GET_TYPE_ID_i64    INT64_TYPE              // i64
// floating point type
#define MY_CTYPE_GET_TYPE_ID_f32    FLOAT_TYPE              // f32
#define MY_CTYPE_GET_TYPE_ID_f64    DOUBLE_TYPE             // f64
#define MY_CTYPE_GET_TYPE_ID_f128   LONG_DOUBLE_TYPE        // f128
// other types
#define MY_CTYPE_GET_TYPE_ID_b      BOOL_TYPE               //  b
#define MY_CTYPE_GET_TYPE_ID_u24    UINT24_TYPE             // u24
#define MY_CTYPE_GET_TYPE_ID_i24    INT24_TYPE              // i24
#define MY_CTYPE_GET_TYPE_ID_reg    REG_TYPE                // reg
#define MY_CTYPE_GET_TYPE_ID_sreg   SREG_TYPE               // sreg


/* ***************************************************************************
 * PREPROCESSOR MACRO FOR GETTING TYPE WHEN IS KNOWN ID
 *
 *      MY_CTYPE_GET_TYPE(UINT16_TYPE) // expands to u16
 *
 */

#define MY_CTYPE_GET_TYPE(typeId) MY_CTYPE_GET_TYPE_IMPL(typeId)
#define MY_CTYPE_GET_TYPE_IMPL(typeId) MY_CTYPE_GET_TYPE_ ## typeId

#define MY_CTYPE_GET_TYPE_0         uni                     // VOID_TYPE
// unsigned type
#define MY_CTYPE_GET_TYPE_1          u8                     // UINT8_TYPE
#define MY_CTYPE_GET_TYPE_2         u16                     // UINT16_TYPE
#define MY_CTYPE_GET_TYPE_3         u32                     // UINT32_TYPE
#define MY_CTYPE_GET_TYPE_4         u64                     // UINT64_TYPE
//signed type
#define MY_CTYPE_GET_TYPE_5          c8                     // CHAR_TYPE
#define MY_CTYPE_GET_TYPE_6          i8                     // INT8_TYPE
#define MY_CTYPE_GET_TYPE_7         i16                     // INT16_TYPE
#define MY_CTYPE_GET_TYPE_8         i32                     // INT32_TYPE
#define MY_CTYPE_GET_TYPE_9         i64                     // INT64_TYPE
// floating point type
#define MY_CTYPE_GET_TYPE_10         f32                    // FLOAT_TYPE
#define MY_CTYPE_GET_TYPE_11         f64                    // DOUBLE_TYPE
#define MY_CTYPE_GET_TYPE_12        f128                    // LONG_DOUBLE_TYPE
// other types
#define MY_CTYPE_GET_TYPE_13          b                     // BOOL_TYPE
#define MY_CTYPE_GET_TYPE_14         u24                    // UINT24_TYPE
#define MY_CTYPE_GET_TYPE_15         i24                    // INT24_TYPE
#define MY_CTYPE_GET_TYPE_16         reg                    // REG_TYPE
#define MY_CTYPE_GET_TYPE_17        sreg                    // SREG_TYPE

#endif // __MY_CTYPE_ID_H__
