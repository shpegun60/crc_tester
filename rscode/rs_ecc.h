/* Reed Solomon Coding for glyphs
 * Copyright Henry Minsky (hqm@alum.mit.edu) 1991-2009
 *
 * This software library is licensed under terms of the GNU GENERAL
 * PUBLIC LICENSE
 *
 * RSCODE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RSCODE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Rscode.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Source code is available at http://rscode.sourceforge.net
 *
 * Commercial licensing is available under a separate license, please
 * contact author for details.
 *
 */

/****************************************************************
  
  Below is NPAR, the only compile-time parameter you should have to
  modify.
  
  It is the number of parity bytes which will be appended to
  your data to create a codeword.

  Note that the maximum codeword size is 255, so the
  sum of your message length plus parity should be less than
  or equal to this maximum limit.

  In practice, you will get slooow error correction and decoding
  if you use more than a reasonably small number of parity bytes.
  (say, 10 or 20)

  ****************************************************************/

#ifndef _ECC_H
#define _ECC_H

#ifdef __cplusplus
extern "C" {
#endif


#include "rscode-config.h"
#include <inttypes.h>


/* **************************************************************** */

/* Maximum degree of various polynomials. */
#define MAXDEG (RSCODE_NPAR*2)

/*************************************/

typedef struct {
  /* generator polynomial */
  int genPoly[MAXDEG*2];
  /* Encoder parity bytes */
  int pBytes[MAXDEG];
  /* Decoder syndrome bytes */
  int synBytes[MAXDEG];
  /* The Error Locator Polynomial, also known as Lambda or Sigma. Lambda[0] == 1 */
  int Lambda[MAXDEG];
  /* The Error Evaluator Polynomial */
  int Omega[MAXDEG];
  /* error locations found using Chien's search*/
  int ErrorLocs[256];
  int NErrors;
#ifndef RSCODE_DISABLE_ERASURES_FUNCTIONS
  /* erasure flags */
  int ErasureLocs[256];
  int NErasures;
#endif /* RSCODE_DISABLE_ERASURES_FUNCTIONS */
} rscode_driver;

void rscode_init(rscode_driver* const driver);
void rscode_encode(rscode_driver * const driver, unsigned char * const msg, const int nbytes, unsigned char * const dst);
int rscode_decode(rscode_driver * const driver, unsigned char * const data, const int nbytes);

#ifndef RSCODE_DISABLE_ERASURES_FUNCTIONS
int rscode_decode_with_erasures(rscode_driver * const driver, unsigned char * const data, const int nbytes, const int nerasures, int * const erasures);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _ECC_H */
