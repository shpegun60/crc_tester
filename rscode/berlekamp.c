/***********************************************************************
 * Copyright Henry Minsky (hqm@alum.mit.edu) 1991-2009
 *
 * This software library is licensed under terms of the GNU GENERAL
 * PUBLIC LICENSE
 *
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
 * Commercial licensing is available under a separate license, please
 * contact author for details.
 *
 * Source code is available at http://rscode.sourceforge.net
 * Berlekamp-Peterson and Berlekamp-Massey Algorithms for error-location
 *
 * From Cain, Clark, "Error-Correction Coding For Digital Communications", pp. 205.
 *
 * This finds the coefficients of the error locator polynomial.
 *
 * The roots are then found by looking for the values of a^n
 * where evaluating the polynomial yields zero.
 *
 * Error correction is done using the error-evaluator equation  on pp 207.
 *
 */


#include "rs_ecc.h"
#include "my_ctypes.h"

#ifdef RSCODE_DEBUG
#include <stdio.h>
#endif /* RSCODE_DEBUG */

#include "galois.h"

/* local ANSI declarations */
static int compute_discrepancy (rscode_driver* const driver, const int lambda[], const int S[], const int L, const int n);
static void init_gamma (rscode_driver* const driver, int gamma[]);
static void compute_modified_omega (rscode_driver* const driver);

/********** polynomial arithmetic *******************/

void add_polys (rscode_driver* const driver, int dst[], const int src[])
{
    unsigned i;
    for (i = 0; i < MAXDEG; ++i) {
        dst[i] ^= src[i];
    }
    UNUSED(driver);
}

void copy_poly (rscode_driver* const driver, int dst[], const int src[])
{
    unsigned i;
    for (i = 0; i < MAXDEG; ++i) {
        dst[i] = src[i];
    }
    UNUSED(driver);
}

void scale_poly (rscode_driver* const driver, const int k, int poly[])
{ 
    unsigned i;
    for (i = 0; i < MAXDEG; ++i) {
        poly[i] = gmult(driver, k, poly[i]);
    }
}


void zero_poly (rscode_driver* const driver, int poly[])
{
    unsigned i;
    for (i = 0; i < MAXDEG; ++i) {
        poly[i] = 0;
    }
    UNUSED(driver);
}


/* multiply by z, i.e., shift right by 1 */
static void mul_z_poly (rscode_driver* const driver, int src[])
{
    unsigned i;
    for (i = (MAXDEG - 1); i > 0; --i) {
        src[i] = src[i-1];
    }
    src[0] = 0;
    UNUSED(driver);
}

/* polynomial multiplication */
void mult_polys (rscode_driver* const driver, int dst[], const int p1[], const int p2[])
{
    int i, j;
    int tmp1[MAXDEG*2];

    for (i = 0; i < (MAXDEG*2); ++i) {
        dst[i] = 0;
    }

    for (i = 0; i < MAXDEG; ++i) {
        for(j = MAXDEG; j < (MAXDEG*2); ++j) {
            tmp1[j] = 0;
        }

        /* scale tmp1 by p1[i] */
        for(j = 0; j < MAXDEG; ++j) {
            tmp1[j] = gmult(driver, p2[j], p1[i]);
        }

        /* and mult (shift) tmp1 right by i */
        for (j = (MAXDEG*2)-1; j >= i; --j) {
            tmp1[j] = tmp1[j - i];
        }

        for (j = 0; j < i; ++j) {
            tmp1[j] = 0;
        }

        /* add into partial product */
        for(j = 0; j < (MAXDEG*2); ++j) {
            dst[j] ^= tmp1[j];
        }
    }
}

/* From  Cain, Clark, "Error-Correction Coding For Digital Communications", pp. 216. */
void Modified_Berlekamp_Massey (rscode_driver* const driver)
{	
    int n, L, L2, k, d, i;
    int psi[MAXDEG], psi2[MAXDEG], D[MAXDEG];
    int gamma[MAXDEG];

    /* initialize Gamma, the erasure locator polynomial */
    init_gamma(driver, gamma);

    /* initialize to z */
    copy_poly(driver, D, gamma);
    mul_z_poly(driver, D);

    copy_poly(driver, psi, gamma);
    k = -1;

#ifndef RSCODE_DISABLE_ERASURES_FUNCTIONS
    L = driver->NErasures;
#else
    L = 0;
#endif /* RSCODE_DISABLE_ERASURES_FUNCTIONS */

    TYPEOF_DATA(driver->synBytes[0])* const synBytes = driver->synBytes;
    for (n = L/*driver->NErasures*/; n < RSCODE_NPAR; ++n) {

        d = compute_discrepancy(driver, psi, synBytes, L, n);

        if (d != 0) {

            /* psi2 = psi - d*D */
            for (i = 0; i < MAXDEG; ++i) {
                psi2[i] = psi[i] ^ gmult(driver, d, D[i]);
            }


            if (L < (n-k)) {
                L2 = n-k;
                k = n-L;
                /* D = scale_poly(ginv(d), psi); */
                for (i = 0; i < MAXDEG; ++i) {
                    D[i] = gmult(driver, psi[i], ginv(driver, d));
                }
                L = L2;
            }

            /* psi = psi2 */
            for (i = 0; i < MAXDEG; ++i) {
                psi[i] = psi2[i];
            }
        }

        mul_z_poly(driver, D);
    }

    TYPEOF_DATA(driver->Lambda[0])* const Lambda = driver->Lambda;
    for(i = 0; i < MAXDEG; ++i) {
        Lambda[i] = psi[i];
    }
    compute_modified_omega(driver);
}

/* given Psi (called driver->Lambda in Modified_Berlekamp_Massey) and driver->synBytes,
   compute the combined erasure/error evaluator polynomial as
   Psi*S mod z^4
  */
static void compute_modified_omega (rscode_driver* const driver)
{
    unsigned i;
    int product[MAXDEG*2];
    TYPEOF_DATA(driver->Omega[0])* const Omega = driver->Omega;

    mult_polys(driver, product, driver->Lambda, driver->synBytes);
    zero_poly(driver, Omega);
    for(i = 0; i < RSCODE_NPAR; ++i) {
        Omega[i] = product[i];
    }
}

/* gamma = product (1-z*a^Ij) for erasure locs Ij */
static void init_gamma (rscode_driver* const driver, int gamma[])
{
#ifndef RSCODE_DISABLE_ERASURES_FUNCTIONS
    int e;
#endif /* RSCODE_DISABLE_ERASURES_FUNCTIONS */
    int tmp[MAXDEG];

    zero_poly(driver, gamma);
    zero_poly(driver, tmp);
    gamma[0] = 1;

#ifndef RSCODE_DISABLE_ERASURES_FUNCTIONS
    const TYPEOF_DATA(driver->NErasures) NErasures = driver->NErasures;
    TYPEOF_DATA(driver->ErasureLocs[0])* const ErasureLocs = driver->ErasureLocs;

    for (e = 0; e < NErasures; ++e) {
        copy_poly(driver, tmp, gamma);
        scale_poly(driver, gexp(driver, ErasureLocs[e]), tmp);
        mul_z_poly(driver, tmp);
        add_polys(driver, gamma, tmp);
    }
#endif /* RSCODE_DISABLE_ERASURES_FUNCTIONS */
}


static int compute_discrepancy (rscode_driver* const driver, const int lambda[], const int S[], const int L, const int n)
{
    int i, sum=0;

    for (i = 0; i <= L; ++i) {
        sum ^= gmult(driver, lambda[i], S[n-i]);
    }
    return (sum);
}

/* Finds all the roots of an error-locator polynomial with coefficients
 * driver->Lambda[j] by evaluating driver->Lambda at successive values of alpha.
 *
 * This can be tested with the decoder's equations case.
 */


void Find_Roots (rscode_driver* const driver)
{
    int sum, r, k;
    TYPEOF_DATA(driver->NErrors) NErrors = 0;
    TYPEOF_DATA(driver->Lambda[0])* const Lambda = driver->Lambda;
    TYPEOF_DATA(driver->ErrorLocs[0])* const ErrorLocs = driver->ErrorLocs;

    for (r = 1; r < 256; ++r) {
        sum = 0;
        /* evaluate lambda at r */
        for (k = 0; k < (RSCODE_NPAR + 1); ++k) {
            sum ^= gmult(driver, gexp(driver,(k*r)%255), Lambda[k]);
        }

        if (sum == 0) {
            ErrorLocs[NErrors] = (255-r);
            ++NErrors;
#ifdef RSCODE_DEBUG
            fprintf(stderr, "Root found at r = %d, (255-r) = %d\n", r, (255-r));
#endif /* RSCODE_DEBUG */
        }
    }

    driver->NErrors = NErrors;
}

/* Combined Erasure And Error Magnitude Computation 
 *
 * Pass in the codeword, its size in bytes, as well as
 * an array of any known erasure locations, along the number
 * of these erasures.
 *
 * Evaluate driver->Omega(actually Psi)/driver->Lambda' at the roots
 * alpha^(-i) for error locs i.
 *
 * Returns 1 if everything ok, or 0 if an out-of-bounds error is found
 *
 */

#ifndef RSCODE_DISABLE_ERASURES_FUNCTIONS
int correct_errors_erasures (rscode_driver* const driver, unsigned char* const codeword, const int csize, const int nerasures, const int* const erasures) {
#else
int correct_errors_erasures (rscode_driver* const driver, unsigned char* const codeword, const int csize) {
#endif /* RSCODE_DISABLE_ERASURES_FUNCTIONS */

    int r, i, j, err;

    /* If you want to take advantage of erasure correction, be sure to
     set driver->NErasures and driver->ErasureLocs[] with the locations of erasures.
     */
#ifndef RSCODE_DISABLE_ERASURES_FUNCTIONS
    TYPEOF_DATA(driver->ErasureLocs[0])* const ErasureLocs = driver->ErasureLocs;

    driver->NErasures = nerasures;
    for (i = 0; i < nerasures; ++i) {
        ErasureLocs[i] = erasures[i];
    }
#endif /* RSCODE_DISABLE_ERASURES_FUNCTIONS */

    Modified_Berlekamp_Massey(driver);
    Find_Roots(driver);


    const TYPEOF_DATA(driver->NErrors) NErrors = driver->NErrors;

#ifndef RSCODE_DISABLE_ERASURES_FUNCTIONS
    if (((NErrors <= RSCODE_NPAR) && (NErrors > 0)) && (driver->NErasures <= (2*RSCODE_NPAR))) {
#else
    if ((NErrors <= RSCODE_NPAR) && (NErrors > 0)) {
#endif /* RSCODE_DISABLE_ERASURES_FUNCTIONS */
        /* first check for illegal error locs */
        TYPEOF_DATA(driver->ErrorLocs[0])* const ErrorLocs = driver->ErrorLocs;
        TYPEOF_DATA(driver->Omega[0])* const Omega = driver->Omega;
        TYPEOF_DATA(driver->Lambda[0])* const Lambda = driver->Lambda;

        for (r = 0; r < NErrors; ++r) {
            if (ErrorLocs[r] >= csize) {
#ifdef RSCODE_DEBUG
                fprintf(stderr, "Error loc i=%d outside of codeword length %d\n", i, csize);
#endif /* RSCODE_DEBUG */
                return (0);
            }
        }

        for (r = 0; r < NErrors; ++r) {
            int num, denom;
            i = ErrorLocs[r];
            /* evaluate driver->Omega at alpha^(-i) */

            num = 0;
            for (j = 0; j < MAXDEG; ++j) {
                num ^= gmult(driver, Omega[j], gexp(driver, ((255-i)*j) % 255));
            }

            /* evaluate driver->Lambda' (derivative) at alpha^(-i) ; all odd powers disappear */
            denom = 0;
            for (j = 1; j < MAXDEG; j += 2) {
                denom ^= gmult(driver, Lambda[j], gexp(driver,((255-i)*(j-1)) % 255));
            }

            err = gmult(driver, num, ginv(driver, denom));
#ifdef RSCODE_DEBUG
            fprintf(stderr, "Error magnitude %#x at loc %d\n", err, csize-i);
#endif /* RSCODE_DEBUG */

            codeword[csize-i-1] ^= err;
        }
        return(1);
    } else {
#ifdef RSCODE_DEBUG
        if (NErrors) {
            fprintf(stderr, "Uncorrectable codeword\n");
        }
#endif /* RSCODE_DEBUG */
        return(0);
    }
}

