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

#include <ctype.h>
#include "galois.h"
#include "berlekamp.h"

#include "rs.h"

static void rs_compute_genpoly (rscode_driver * driver, int nbytes, int * genpoly);

/* Initialize lookup tables, polynomials, etc. */
void rs_initialize_ecc (rscode_driver * driver)
{
    /* Compute the encoder generator polynomial */
    rs_compute_genpoly(driver, RSCODE_NPAR, driver->genPoly);
}

void rs_zero_fill_from (unsigned char * buf, int from, int to)
{
    int i;
    for (i = from; i < to; ++i) {
        buf[i] = 0;
    }
}

#ifdef RSCODE_DEBUG

/* debuging routines */
void rs_print_parity (rscode_driver * driver)
{
    int i;
    printf("Parity Bytes: ");
    for (i = 0; i < RSCODE_NPAR; ++i) {
        printf("[%d]:%x, ", i, driver->pBytes[i]);
    }
    printf("\n");
}


void rs_print_syndrome (rscode_driver * driver)
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
void rs_build_codeword (rscode_driver * driver, unsigned char * msg, int nbytes, unsigned char * dst)
{
    int i;
    for (i = 0; i < nbytes; ++i) {
        dst[i] = msg[i];
    }

    for (i = 0; i < RSCODE_NPAR; ++i) {
        dst[i+nbytes] = driver->pBytes[RSCODE_NPAR-1-i];
    }
}

/**********************************************************
 * Reed Solomon Decoder
 *
 * Computes the syndrome of a codeword. Puts the results
 * into the synBytes[] array.
 */

void rs_decode_data(rscode_driver * driver, unsigned char * data, int nbytes)
{
    int i, j, sum;
    for (j = 0; j < RSCODE_NPAR; ++j) {
        sum	= 0;
        for (i = 0; i < nbytes; ++i) {
            sum = data[i] ^ gmult(driver, gexp(driver, j+1), sum);
        }
        driver->synBytes[j]  = sum;
    }
}


/* Check if the syndrome is zero */
int rs_check_syndrome (rscode_driver * driver)
{
    int i, nz = 0;
    for (i = 0 ; i < RSCODE_NPAR; ++i) {
        if (driver->synBytes[i] != 0) {
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

static void rs_compute_genpoly (rscode_driver * driver, int nbytes, int * genpoly)
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
void rs_encode_data_continious_start(rscode_driver * driver)
{
    int i;
    for(i = 0; i < RSCODE_NPAR+1; ++i) {
        driver->pBytes[i] = 0;
    }
}

void rs_encode_data_continious_proceed(rscode_driver * driver, unsigned char byte)
{
    int dbyte, j;

    dbyte = byte ^ driver->pBytes[RSCODE_NPAR-1];

    for (j = RSCODE_NPAR-1; j > 0; --j) {
        driver->pBytes[j] = driver->pBytes[j-1] ^ gmult(driver, driver->genPoly[j], dbyte);
    }
    driver->pBytes[0] = gmult(driver, driver->genPoly[0], dbyte);
}

void rs_encode_data_continious_end(rscode_driver * driver, unsigned char *dst, int *pos)
{
    int i;
    for (i = 0; i < RSCODE_NPAR; ++i) {
        dst[i+(*pos)] = driver->pBytes[RSCODE_NPAR-1-i];
    }
    (*pos) += RSCODE_NPAR;
}

/* RS code encode all to one funtions---------------------------------------------------------------
 *
 */
void rs_encode_data(rscode_driver * driver, unsigned char *msg, int nbytes, unsigned char *dst)
{
    int i,dbyte, j;

    for(i = 0; i < RSCODE_NPAR+1; ++i) {
        driver->pBytes[i] = 0;
    }

    for (i = 0; i < nbytes; ++i) {
        dbyte = msg[i] ^ driver->pBytes[RSCODE_NPAR-1];

        for (j = RSCODE_NPAR-1; j > 0; --j) {
            driver->pBytes[j] = driver->pBytes[j-1] ^ gmult(driver, driver->genPoly[j], dbyte);
        }
        driver->pBytes[0] = gmult(driver, driver->genPoly[0], dbyte);
    }

    rs_build_codeword(driver, msg, nbytes, dst);
}

void rs_encode_data_onlyParity(rscode_driver * driver, unsigned char *msg, int nbytes, int* pos)
{
    int i,dbyte, j;

    for(i = 0; i < RSCODE_NPAR+1; ++i) {
        driver->pBytes[i] = 0;
    }

    for (i = 0; i < nbytes; ++i) {
        dbyte = msg[i] ^ driver->pBytes[RSCODE_NPAR-1];

        for (j = RSCODE_NPAR-1; j > 0; --j) {
            driver->pBytes[j] = driver->pBytes[j-1] ^ gmult(driver, driver->genPoly[j], dbyte);
        }
        driver->pBytes[0] = gmult(driver, driver->genPoly[0], dbyte);
    }

    rs_encode_data_continious_end(driver, msg, pos);
}





