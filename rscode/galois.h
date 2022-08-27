#ifndef _GALOIS_H
#define _GALOIS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rs_ecc.h"

int gexp(rscode_driver * driver, int z);
int gmult(rscode_driver * driver, int x, int y);
int ginv(rscode_driver * driver, int elt);


#ifdef __cplusplus
}
#endif

#endif /* _GALOIS_H */
