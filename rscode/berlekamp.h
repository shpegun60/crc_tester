#ifndef _BERLEKAMP_H
#define _BERLEKAMP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rs_ecc.h"


/* polynomial arithmetic */
void copy_poly (rscode_driver* const driver, int dst[], const int src[]);

void add_polys (rscode_driver* const driver, int dst[], const int src[]);
void scale_poly (rscode_driver* const driver, const int k, int poly[]);
void mult_polys (rscode_driver* const driver, int dst[], const int p1[], const int p2[]);

void zero_poly (rscode_driver* const driver, int poly[]);

/* Error location routines */
#ifndef RSCODE_DISABLE_ERASURES_FUNCTIONS
int correct_errors_erasures (rscode_driver* const driver, unsigned char* const codeword, const int csize, const int nerasures, const int* const erasures);
#else
int correct_errors_erasures (rscode_driver* const driver, unsigned char* const codeword, const int csize);
#endif /* RSCODE_DISABLE_ERASURES_FUNCTIONS */

#ifdef __cplusplus
}
#endif

#endif /* _BERLEKAMP_H */
