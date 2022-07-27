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
T TEMPLATE(convertReadLSB, T) (u8 *data, u16 *pos);
void TEMPLATE(convertWriteLSB, T) (u8 *data, u16 *pos, T value);

// position not pointer
T TEMPLATE(convertReadLSB_cpos, T) (u8 *data, u16 pos);
void TEMPLATE(convertWriteLSB_cpos, T) (u8 *data, u16 pos, T value);

/*
 * ******************************************
 * MSB - first
 * ******************************************
 */
T TEMPLATE(convertReadMSB, T) (u8 *data, u16 *pos);
void TEMPLATE(convertWriteMSB, T) (u8 *data, u16 *pos, T value);

// position not pointer
T TEMPLATE(convertReadMSB_cpos, T) (u8 *data, u16 pos);
void TEMPLATE(convertWriteMSB_cpos, T) (u8 *data, u16 pos, T value);

#undef T
