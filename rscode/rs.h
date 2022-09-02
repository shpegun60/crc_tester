#ifndef _RS_H
#define _RS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rs_ecc.h"

/* Reed Solomon encode/decode routines */
void rs_initialize_ecc (rscode_driver * driver);
int rs_check_syndrome (rscode_driver * driver);
void rs_zero_fill_from (unsigned char * buf, int from, int to);



void rs_decode_data (rscode_driver * driver, unsigned char *data, int nbytes);
void rs_encode_data (rscode_driver * driver, unsigned char *msg, int nbytes, unsigned char *dst);
void rs_encode_data_onlyParity(rscode_driver * driver, unsigned char *msg, int nbytes, int* pos);



void rs_encode_data_continious_start(rscode_driver * driver);
void rs_encode_data_continious_proceed(rscode_driver * driver, unsigned char byte);
void rs_encode_data_continious_end(rscode_driver * driver, unsigned char *dst, int *pos);


#ifdef RSCODE_DEBUG

/* debuging routines */
void rs_print_parity (rscode_driver * driver);
void rs_print_syndrome (rscode_driver * driver);

#endif /* RSCODE_DEBUG */

#ifdef __cplusplus
}
#endif

#endif /* _RS_H */
