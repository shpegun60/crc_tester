#ifndef _RS_H
#define _RS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rs_ecc.h"

/* Reed Solomon encode/decode routines */
void initialize_ecc (rscode_driver * driver);
int check_syndrome (rscode_driver * driver);
void decode_data (rscode_driver * driver, unsigned char *data, int nbytes);
void encode_data (rscode_driver * driver, unsigned char *msg, int nbytes, unsigned char *dst);
void zero_fill_from (unsigned char * buf, int from, int to);


#ifdef RSCODE_DEBUG

/* debuging routines */
void print_parity (rscode_driver * driver);
void print_syndrome (rscode_driver * driver);

#endif /* RSCODE_DEBUG */

#ifdef __cplusplus
}
#endif

#endif /* _RS_H */
