#include "templates.h"
#include "my_ctypes.h"

#ifndef T
    #define T int
#endif /* T */

/*
 * ******************************************
 * LSB - first
 * ******************************************
 */
T TEMPLATE(convertRead_LSB, T) (u8* const data, u16 * const pos);
void TEMPLATE(convertWrite_LSB, T) (u8* const data, u16* const pos, const T value);

//with check buffer-----------------------------------------------------------------------------
T TEMPLATE(convertReadCheck_LSB, T) (u8* const data, u16* const pos, const unsigned int buffSize, b* const ok);
void TEMPLATE(convertWriteCheck_LSB, T) (u8* const data, u16* const pos, const T value, const unsigned int buffSize, b* const ok);

// position not a pointer-----------------------------------------------------------------------
T TEMPLATE(convertRead_cpos_LSB, T) (u8* const data, const u16 pos);
void TEMPLATE(convertWrite_cpos_LSB, T) (u8* const data, const u16 pos, const T value);

//with check buffer-----------------------------------------------------------------------------
T TEMPLATE(convertReadCheck_cpos_LSB, T) (u8* const data, const u16 pos, const unsigned int buffSize, b* const ok);
void TEMPLATE(convertWriteCheck_cpos_LSB, T) (u8* const data, const u16 pos, const T value, const unsigned int buffSize, b* const ok);

/*
 * ******************************************
 * MSB - first
 * ******************************************
 */

T TEMPLATE(convertRead_MSB, T) (u8* const data, u16* const pos);
void TEMPLATE(convertWrite_MSB, T) (u8* const data, u16* const pos, const T value);

//with check buffer-----------------------------------------------------------------------------
T TEMPLATE(convertReadCheck_MSB, T) (u8* const data, u16* const pos, const unsigned int buffSize, b* const ok);
void TEMPLATE(convertWriteCheck_MSB, T) (u8* const data, u16* const pos, const T value, const unsigned int buffSize, b* const ok);

// position not a pointer
T TEMPLATE(convertRead_cpos_MSB, T) (u8* const data, const u16 pos);
void TEMPLATE(convertWrite_cpos_MSB, T) (u8* const data, const u16 pos, const T value);

//with check buffer-----------------------------------------------------------------------------
T TEMPLATE(convertReadCheck_cpos_MSB, T) (u8* const data, const u16 pos, const unsigned int buffSize, b* const ok);
void TEMPLATE(convertWriteCheck_cpos_MSB, T) (u8* const data, const u16 pos, const T value, const unsigned int buffSize, b* const ok);


#ifndef CONVERT_TEST_DISABLE
/*
 * ******************************************
 * test
 * ******************************************
 */
int TEMPLATE(convertTest, T) (int testN);

#endif /* CONVERT_TEST_DISABLE */

#undef T
