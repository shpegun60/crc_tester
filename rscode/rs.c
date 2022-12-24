/* 
 * Reed Solomon Encoder/Decoder
 *
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

 * Commercial licensing is available under a separate license, please
 * contact author for details.
 *
 * Source code is available at http://rscode.sourceforge.net
 */

#include "rs_ecc.h"
#ifdef RSCODE_DEBUG
#include <stdio.h>
#endif /* RSCODE_DEBUG */

#include "my_ctypes.h"
#include <ctype.h>
#include "galois.h"
#include "berlekamp.h"

#include "rs.h"

static void rs_build_codeword (rscode_driver* const driver, unsigned char* const msg, const int nbytes, unsigned char* const dst);
static void rs_compute_genpoly (rscode_driver * const driver, const int nbytes, int * const genpoly);

/* Initialize lookup tables, polynomials, etc. */
void rs_initialize_ecc (rscode_driver* const driver)
{
    /* Compute the encoder generator polynomial */
    rs_compute_genpoly(driver, RSCODE_NPAR, driver->genPoly);
}

void rs_zero_fill_from (unsigned char* const buf, const int from, const int to)
{
    int i;
    for (i = from; i < to; ++i) {
        buf[i] = 0;
    }
}

#ifdef RSCODE_DEBUG

/* debuging routines */
void rs_print_parity (rscode_driver* const driver)
{
    int i;
    printf("Parity Bytes: ");
    for (i = 0; i < RSCODE_NPAR; ++i) {
        printf("[%d]:%x, ", i, driver->pBytes[i]);
    }
    printf("\n");
}


void rs_print_syndrome (rscode_driver* const driver)
{
    int i;
    printf("Syndrome Bytes: ");
    for (i = 0; i < RSCODE_NPAR; ++i) {
        printf("[%d]:%x, ", i, driver->synBytes[i]);
    }
    printf("\n");
}

#endif /* RSCODE_DEBUG */

/* Append the parity bytes onto the end of the message */
static void rs_build_codeword (rscode_driver* const driver, unsigned char* const msg, const int nbytes, unsigned char* const dst)
{
    int i;
    for (i = 0; i < nbytes; ++i) {
        dst[i] = msg[i];
    }

    const TYPEOF_DATA(driver->pBytes[0])* const pBytes = driver->pBytes;

    for (i = 0; i < RSCODE_NPAR; ++i) {
        dst[i+nbytes] = pBytes[RSCODE_NPAR-1-i];
    }
}

/**********************************************************
 * Reed Solomon Decoder
 *
 * Computes the syndrome of a codeword. Puts the results
 * into the synBytes[] array.
 */

void rs_decode_data(rscode_driver* const driver, unsigned char* const data, const int nbytes)
{
    int i, j, sum;
    TYPEOF_DATA(driver->synBytes[0])* const synBytes = driver->synBytes;

    for (j = 0; j < RSCODE_NPAR; ++j) {
        sum	= 0;
        for (i = 0; i < nbytes; ++i) {
            sum = data[i] ^ gmult(driver, gexp(driver, j+1), sum);
        }
        synBytes[j]  = sum;
    }
}


/* Check if the syndrome is zero */
int rs_check_syndrome (rscode_driver* const driver)
{
    int i, nz = 0;
    const TYPEOF_DATA(driver->synBytes[0])* const synBytes = driver->synBytes;

    for (i = 0 ; i < RSCODE_NPAR; ++i) {
        if (synBytes[i] != 0) {
            nz = 1;
            break;
        }
    }
    return nz;
}

/* Create a generator polynomial for an n byte RS code. 
 * The coefficients are returned in the driver->genPoly arg.
 * Make sure that the driver->genPoly array which is passed in is
 * at least n+1 bytes long.
 */

static void rs_compute_genpoly (rscode_driver* const driver, const int nbytes, int* const genpoly)
{
    int i, tp[256], tp1[256];

    /* multiply (x + a^n) for n = 1 to nbytes */

    zero_poly(driver, tp1);
    tp1[0] = 1;

    for (i = 1; i <= nbytes; ++i) {
        zero_poly(driver, tp);
        tp[0] = gexp(driver,i);		/* set up x+a^n */
        tp[1] = 1;

        mult_polys(driver, genpoly, tp, tp1);
        copy_poly(driver, tp1, genpoly);
    }
}

/* Simulate a LFSR with generator polynomial for n byte RS code. 
 * Pass in a pointer to the data array, and amount of data.
 *
 * The parity bytes are deposited into driver->pBytes[], and the whole message
 * and parity are copied to dest to make a codeword.
 *
 */

/* RS code encode continious---------------------------------------------------------------
 *
 */
void rs_encode_data_continious_start(rscode_driver* const driver)
{
    int i;
    TYPEOF_DATA(driver->pBytes[0])* const pBytes = driver->pBytes;

    for(i = 0; i < RSCODE_NPAR+1; ++i) {
        pBytes[i] = 0;
    }
}

void rs_encode_data_continious_proceed(rscode_driver* const driver, const unsigned char byte)
{
    int j;
    TYPEOF_DATA(driver->pBytes[0])* const pBytes = driver->pBytes;
    TYPEOF_DATA(driver->genPoly[0])* const genPoly = driver->genPoly;

    const int dbyte = byte ^ pBytes[RSCODE_NPAR-1];

    for (j = RSCODE_NPAR-1; j > 0; --j) {
        pBytes[j] = pBytes[j-1] ^ gmult(driver, genPoly[j], dbyte);
    }
    pBytes[0] = gmult(driver, genPoly[0], dbyte);
}

void rs_encode_data_continious_end(rscode_driver* const driver, unsigned char* const dst, int* const pos)
{
    int i;
    TYPEOF_DATA(driver->pBytes[0])* const pBytes = driver->pBytes;

    for (i = 0; i < RSCODE_NPAR; ++i) {
        dst[i+(*pos)] =pBytes[RSCODE_NPAR-1-i];
    }
    (*pos) += RSCODE_NPAR;
}

/* RS code encode all to one funtions---------------------------------------------------------------
 *
 */
void rs_encode_data(rscode_driver* const driver, unsigned char* const msg, const int nbytes, unsigned char* const dst)
{
    int i,dbyte, j;
    TYPEOF_DATA(driver->pBytes[0])* const pBytes = driver->pBytes;
    const TYPEOF_DATA(driver->genPoly[0])* const genPoly = driver->genPoly;

    for(i = 0; i < RSCODE_NPAR+1; ++i) {
        pBytes[i] = 0;
    }

    for (i = 0; i < nbytes; ++i) {
        dbyte = msg[i] ^ pBytes[RSCODE_NPAR-1];

        for (j = RSCODE_NPAR-1; j > 0; --j) {
            pBytes[j] = pBytes[j-1] ^ gmult(driver, genPoly[j], dbyte);
        }
        pBytes[0] = gmult(driver, genPoly[0], dbyte);
    }

    rs_build_codeword(driver, msg, nbytes, dst);
}

void rs_encode_data_onlyParity(rscode_driver* const driver, const unsigned char* const msg, const int nbytes)
{
    int i, dbyte, j;
    TYPEOF_DATA(driver->pBytes[0])* const pBytes = driver->pBytes;
    const TYPEOF_DATA(driver->genPoly[0])* const genPoly = driver->genPoly;

    for(i = 0; i < RSCODE_NPAR+1; ++i) {
        pBytes[i] = 0;
    }

    for (i = 0; i < nbytes; ++i) {
        dbyte = msg[i] ^ pBytes[RSCODE_NPAR-1];

        for (j = RSCODE_NPAR-1; j > 0; --j) {
            pBytes[j] = pBytes[j-1] ^ gmult(driver, genPoly[j], dbyte);
        }
        pBytes[0] = gmult(driver, genPoly[0], dbyte);
    }
}





