#include "templates.h"
#include "my_ctypes.h"

#ifndef T
    #define T u32
#endif /* T */

/*
 * ******************************************
 * LSB - first
 * ******************************************
 */
T TEMPLATE(convertRead_LSB, T) (u8 *data, u16 *pos);
void TEMPLATE(convertWrite_LSB, T) (u8 *data, u16 *pos, T value);

// position not a pointer
T TEMPLATE(convertRead_cpos_LSB, T) (u8 *data, u16 pos);
void TEMPLATE(convertWrite_cpos_LSB, T) (u8 *data, u16 pos, T value);

/*
 * ******************************************
 * MSB - first
 * ******************************************
 */
T TEMPLATE(convertRead_MSB, T) (u8 *data, u16 *pos);
void TEMPLATE(convertWrite_MSB, T) (u8 *data, u16 *pos, T value);

// position not a pointer
T TEMPLATE(convertRead_cpos_MSB, T) (u8 *data, u16 pos);
void TEMPLATE(convertWrite_cpos_MSB, T) (u8 *data, u16 pos, T value);


#ifndef CONVERT_TEST_DISABLE
/*
 * ******************************************
 * test
 * ******************************************
 */
int TEMPLATE(convertTest, T) (int testN);

#endif /* CONVERT_TEST_DISABLE */

#undef T
