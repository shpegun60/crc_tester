#ifndef _BERLEKAMP_H
#define _BERLEKAMP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rs_ecc.h"


/* polynomial arithmetic */
void copy_poly(rscode_driver * driver, int dst[], int src[]);

void add_polys(rscode_driver * driver, int dst[], int src[]) ;
void scale_poly(rscode_driver * driver, int k, int poly[]);
void mult_polys(rscode_driver * driver, int dst[], int p1[], int p2[]);

void zero_poly(rscode_driver * driver, int poly[]);

/* Error location routines */
#ifndef RSCODE_DISABLE_ERASURES_FUNCTIONS
int correct_errors_erasures (rscode_driver * driver, unsigned char codeword[], int csize, int nerasures, int erasures[]);
#else
int correct_errors_erasures (rscode_driver * driver, unsigned char codeword[], int csize);
#endif /* RSCODE_DISABLE_ERASURES_FUNCTIONS */

#ifdef __cplusplus
}
#endif

#endif /* _BERLEKAMP_H */
