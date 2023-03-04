#ifndef __CONVERT_H__
#define __CONVERT_H__ 1

#ifdef T
    #undef T
#endif

#include "my_ctypes.h"

#ifndef CONVERT_TEST_DISABLE
    //#define CONVERT_TEST_DISABLE
#endif /* CONVERT_TEST_DISABLE */

// MAIN TEMPLATE FUNCTIOINS--------------------------------------------
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

/*
 * ******************************************
 * LSB - first universal
 * ******************************************
 */

void TEMPLATE(convertRead_LSB, uni)(reg n, u8* data, reg* pos, void* value);
void TEMPLATE(convertWrite_LSB, uni)(reg n, u8* data, reg* pos, void* value);


//with check buffer-----------------------------------------------------------------------------
void TEMPLATE(convertReadCheck_LSB, uni) (reg n, u8* data, reg* pos, void* value, reg buffSize, b* ok);
void TEMPLATE(convertWriteCheck_LSB, uni) (reg n, u8* data, reg* pos, void* value, reg buffSize, b* ok);

// position not a pointer
void TEMPLATE(convertWrite_cpos_LSB, uni)(reg n, u8* data, reg pos, void* value);
void TEMPLATE(convertRead_cpos_LSB, uni)(reg n, u8* data, reg pos, void* value);

//with check buffer-----------------------------------------------------------------------------
void TEMPLATE(convertReadCheck_cpos_LSB, uni) (reg n, u8* data, reg pos, void* value, reg buffSize, b* ok);
void TEMPLATE(convertWriteCheck_cpos_LSB, uni) (reg n, u8* data, reg pos, void* value, reg buffSize, b* ok);
/*
 * ******************************************
 * MSB - first universal
 * ******************************************
 */

void TEMPLATE(convertRead_MSB, uni)(reg n, u8* data, reg* pos, void* value);
void TEMPLATE(convertWrite_MSB, uni)(reg n, u8* data, reg* pos, void* value);

//with check buffer-----------------------------------------------------------------------------
void TEMPLATE(convertReadCheck_MSB, uni) (reg n, u8* data, reg* pos, void* value, reg buffSize, b* ok);
void TEMPLATE(convertWriteCheck_MSB, uni) (reg n, u8* data, reg* pos, void* value, reg buffSize, b* ok);

// position not a pointer
void TEMPLATE(convertRead_cpos_MSB, uni)(reg n, u8* data, reg pos, void* value);
void TEMPLATE(convertWrite_cpos_MSB, uni)(reg n, u8* data, reg pos, void* value);

//with check buffer-----------------------------------------------------------------------------
void TEMPLATE(convertReadCheck_cpos_MSB, uni) (reg n, u8* data, reg pos, void* value, reg buffSize, b* ok);
void TEMPLATE(convertWriteCheck_cpos_MSB, uni) (reg n, u8* data, reg pos, void* value, reg buffSize, b* ok);



// TEST--------------------------------------------


#ifndef CONVERT_TEST_DISABLE
/*
 * ****************************************
 * Random Write/Read test for convert
 *  * return 0 if test compleated, else - not compleated
 * ****************************************
 */

int convertTest(int testCount, int randomSeed);
#endif /* CONVERT_TEST_DISABLE */


#endif /* __CONVERT_H__ */
