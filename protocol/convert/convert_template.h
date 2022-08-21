#include "preprocessor_template.h"
#include "my_ctypes.h"

#ifndef T
    #define T int
#endif /* T */

/*
 * ******************************************
 * LSB - first
 * ******************************************
 */
T TEMPLATE(convertRead_LSB, T) (u8* data, reg* pos);
void TEMPLATE(convertWrite_LSB, T) (u8* data, reg* pos, T value);

//with check buffer-----------------------------------------------------------------------------
T TEMPLATE(convertReadCheck_LSB, T) (u8* data, reg* pos, reg buffSize, b* ok);
void TEMPLATE(convertWriteCheck_LSB, T) (u8* data, reg* pos, T value, reg buffSize, b* ok);

// position not a pointer-----------------------------------------------------------------------
T TEMPLATE(convertRead_cpos_LSB, T) (u8* data, reg pos);
void TEMPLATE(convertWrite_cpos_LSB, T) (u8* data, reg pos, T value);

//with check buffer-----------------------------------------------------------------------------
T TEMPLATE(convertReadCheck_cpos_LSB, T) (u8* data, reg pos, reg buffSize, b* ok);
void TEMPLATE(convertWriteCheck_cpos_LSB, T) (u8* data, reg pos, T value, reg buffSize, b* ok);

/*
 * ******************************************
 * MSB - first
 * ******************************************
 */

T TEMPLATE(convertRead_MSB, T) (u8* data, reg* pos);
void TEMPLATE(convertWrite_MSB, T) (u8* data, reg* pos, T value);

//with check buffer-----------------------------------------------------------------------------
T TEMPLATE(convertReadCheck_MSB, T) (u8* data, reg* pos, reg buffSize, b* ok);
void TEMPLATE(convertWriteCheck_MSB, T) (u8* data, reg* pos, T value, reg buffSize, b* ok);

// position not a pointer
T TEMPLATE(convertRead_cpos_MSB, T) (u8* data, reg pos);
void TEMPLATE(convertWrite_cpos_MSB, T) (u8* data, reg pos, T value);

//with check buffer-----------------------------------------------------------------------------
T TEMPLATE(convertReadCheck_cpos_MSB, T) (u8* data, reg pos, reg buffSize, b* ok);
void TEMPLATE(convertWriteCheck_cpos_MSB, T) (u8* data, reg pos, T value, reg buffSize, b* ok);


#ifndef CONVERT_TEST_DISABLE
/*
 * ******************************************
 * test
 * ******************************************
 */

int TEMPLATE(convertTest, T) (int testN, int randomSeed);

#endif /* CONVERT_TEST_DISABLE */

#undef T
