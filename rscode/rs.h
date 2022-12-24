#ifndef _RS_H
#define _RS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rs_ecc.h"

/* Reed Solomon encode/decode routines */
void rs_initialize_ecc (rscode_driver * const driver);
int rs_check_syndrome (rscode_driver* const driver);
void rs_zero_fill_from (unsigned char * const buf, const int from, const int to);



void rs_decode_data (rscode_driver * const driver, unsigned char * const data, const int nbytes);
void rs_encode_data (rscode_driver * const driver, unsigned char * const msg, const int nbytes, unsigned char * const dst);
void rs_encode_data_onlyParity(rscode_driver * const driver, const unsigned char * const msg, const int nbytes);



void rs_encode_data_continious_start(rscode_driver * const driver);
void rs_encode_data_continious_proceed(rscode_driver * const driver, const unsigned char byte);
void rs_encode_data_continious_end(rscode_driver* const driver, unsigned char* const dst, int* const pos);


#ifdef RSCODE_DEBUG

/* debuging routines */
void rs_print_parity (rscode_driver * const driver);
void rs_print_syndrome (rscode_driver* const driver);

#endif /* RSCODE_DEBUG */

#ifdef __cplusplus
}
#endif

#endif /* _RS_H */
