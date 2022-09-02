#include "rscode_test.h"

#ifndef RSCODE_TEST_DISABLE

#include "rs_ecc.h"
#include "rs.h"
#include "berlekamp.h"

#include <stdio.h>
#include <stdlib.h>

unsigned char msg[] = "Nervously I loaded the twin ducks aboard the revolving platform.";
unsigned char codeword[256];

/* Some debugging routines to introduce errors or erasures
   into a codeword.
   */

/* Introduce a byte error at LOC */
void byte_err (int err, int loc, unsigned char *dst)
{
    printf("Adding Error at loc %d, data %#x\n", loc, dst[loc-1]);
    dst[loc-1] ^= err;
}

/* Pass in location of error (first byte position is
   labeled starting at 1, not 0), and the codeword.
*/
void byte_erasure (int loc, unsigned char dst[], int cwsize, int erasures[])
{
    printf("Erasure at loc %d, data %#x\n", loc, dst[loc-1]);
    dst[loc-1] = 0;

    (void)cwsize;
    (void)erasures;
}


// string compleate------------------------------------------
unsigned char rscodeDataCmp(int n, const unsigned char* str1, const unsigned char* str2)
{
    while(n--) {
        if(*str1++ != *str2++) {
            return ((unsigned char)1);
        }
    }
    return ((unsigned char)0);
}

void rscodePrintArray(const unsigned char* descr, const unsigned char* data, int len)
{
    if(descr == NULL || data == NULL) {
        printf("\rscodePrintArray --> NULL!!!!!\n");
        return;
    }

    printf("%s, len %d, data is: 0x%x", descr, len, data[0]);
    for(int i = 1; i < len; ++i) {
        printf("%x", data[i]);
    }
    printf("\n");
    fflush(stdout);
    fflush(stderr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int rs_code_copy_func_test(rscode_driver * drv, int testCount)
{
    unsigned char rnd_msg[256] = {};
    int len = 0;
    int len_last = 0;
    int errCount = 0;

    printf("\n\n-----------------------------------------------\n");
    printf("rscode_encode slow test\n");
    printf("-----------------------------------------------\n");

    while(testCount--) {

        printf("\n\n ----------------- ITERATION: %d ----------------------- \n", testCount);

        len = rand() % (256 - RSCODE_NPAR - 1);
        while(len == 0 || len_last == len) {
            len = rand() % (256 - RSCODE_NPAR - 1);
        }
        len_last = len;



        while(len--) {
            rnd_msg[len] = rand() % 256;
        }

        /* Encode data into codeword, adding NPAR parity bytes */
        rscode_encode(drv, rnd_msg, len_last, codeword);
        rscodePrintArray((unsigned char *)"Encoded -->", codeword, len_last/* + RSCODE_NPAR*/);

        int nErrors = rand() % (RSCODE_NPAR / 2);
        while(nErrors == 0) {
            nErrors = rand() % (RSCODE_NPAR / 2);
        }

        printf("Make Error: %d\n", nErrors);
        while(nErrors--) {
            int pos = rand() % len_last;
            codeword[pos] = rand() % 256;
        }

        rscodePrintArray((unsigned char *)"WITH ERRORS -->", codeword, len_last/* + RSCODE_NPAR*/);

        /* Now decode -- encoded codeword size must be passed */
        rscode_decode(drv, codeword, len_last + RSCODE_NPAR);

        rscodePrintArray((unsigned char *)"Decoded -->", codeword, len_last);

        errCount += rscodeDataCmp(len_last, rnd_msg, codeword);
        printf("COMPLEATE ERROR: %d\n", errCount);
    }

    return errCount;
}


static int rs_code_only_par_func_test(rscode_driver * drv, int testCount)
{
    unsigned char rnd_msg[256] = {};
    unsigned char rnd_msg_CHK[256] = {};
    int pos = 0;

    int len = 0;
    int len_last = 0;
    int errCount = 0;

    printf("\n\n-----------------------------------------------\n");
    printf("rs_encode_data_onlyParity test\n");
    printf("-----------------------------------------------\n");


    while(testCount--) {
        pos = 0;

        printf("\n\n ----------------- ITERATION: %d ----------------------- \n", testCount);

        len = rand() % (256 - RSCODE_NPAR - 1);
        while(len == 0 || len_last == len) {
            len = rand() % (256 - RSCODE_NPAR - 1);
        }
        len_last = len;



        while(len--) {
            rnd_msg[len] = rand() % 256;
            ++pos;
        }

        for(int i = 0; i < len_last; ++i) {
            rnd_msg_CHK[i] = rnd_msg[i];
        }

        /* Encode data into codeword, adding NPAR parity bytes */
        rs_encode_data_onlyParity(drv, rnd_msg, len_last);
        rs_encode_data_continious_end(drv, rnd_msg, &pos);
        rscodePrintArray((unsigned char *)"Encoded -->", rnd_msg, len_last/* + RSCODE_NPAR*/);

        int nErrors = rand() % (RSCODE_NPAR / 2);
        while(nErrors == 0) {
            nErrors = rand() % (RSCODE_NPAR / 2);
        }

        printf("Make Error: %d\n", nErrors);
        while(nErrors--) {
            int pos_in = rand() % len_last;
            rnd_msg[pos_in] = rand() % 256;
        }

        rscodePrintArray((unsigned char *)"WITH ERRORS -->", rnd_msg, len_last/* + RSCODE_NPAR*/);

        /* Now decode -- encoded codeword size must be passed */
        rscode_decode(drv, rnd_msg, len_last + RSCODE_NPAR);

        rscodePrintArray((unsigned char *)"Decoded -->", rnd_msg, len_last);

        errCount += rscodeDataCmp(len_last, rnd_msg, rnd_msg_CHK);
        printf("COMPLEATE ERROR: %d\n", errCount);
    }

    return errCount;
}


static int rs_code_only_continious_func_test(rscode_driver * drv, int testCount)
{
    unsigned char rnd_msg[256] = {};
    unsigned char rnd_msg_CHK[256] = {};
    int pos = 0;

    int len = 0;
    int len_last = 0;
    int errCount = 0;

    printf("\n\n-----------------------------------------------\n");
    printf("rs_encode continious test\n");
    printf("-----------------------------------------------\n");


    while(testCount--) {
        pos = 0;

        printf("\n\n ----------------- ITERATION: %d ----------------------- \n", testCount);

        len = rand() % (256 - RSCODE_NPAR - 1);
        while(len == 0 || len_last == len) {
            len = rand() % (256 - RSCODE_NPAR - 1);
        }
        len_last = len;



        while(len--) {
            rnd_msg[len] = rand() % 256;
            ++pos;
        }

        for(int i = 0; i < len_last; ++i) {
            rnd_msg_CHK[i] = rnd_msg[i];
        }

        /* Encode data into codeword, adding NPAR parity bytes */
        rs_encode_data_continious_start(drv);
        for(int i = 0; i < len_last; ++i) {
            rs_encode_data_continious_proceed(drv, rnd_msg[i]);
        }
        rs_encode_data_continious_end(drv, rnd_msg, &pos);

        rscodePrintArray((unsigned char *)"Encoded -->", rnd_msg, len_last/* + RSCODE_NPAR*/);

        int nErrors = rand() % (RSCODE_NPAR / 2);
        while(nErrors == 0) {
            nErrors = rand() % (RSCODE_NPAR / 2);
        }

        printf("Make Error: %d\n", nErrors);
        while(nErrors--) {
            int pos_in = rand() % len_last;
            rnd_msg[pos_in] = rand() % 256;
        }

        rscodePrintArray((unsigned char *)"WITH ERRORS -->", rnd_msg, len_last/* + RSCODE_NPAR*/);

        /* Now decode -- encoded codeword size must be passed */
        rscode_decode(drv, rnd_msg, len_last + RSCODE_NPAR);

        rscodePrintArray((unsigned char *)"Decoded -->", rnd_msg, len_last);

        errCount += rscodeDataCmp(len_last, rnd_msg, rnd_msg_CHK);
        printf("COMPLEATE ERROR: %d\n", errCount);
    }

    return errCount;
}






int rs_code_test(int randomSeed, int testCount)
{
    int erasures[256];
    int nerasures = 0;
    int errCount = 0;

    /* Initialization the RS ECC library */
    rscode_driver drv;
    rs_initialize_ecc (&drv);


    //--------------------------SIMPLE MSG TEST---------------------------------------------------------------------
    /* ************** */

    /* Encode data into codeword, adding NPAR parity bytes */
    rscode_encode(&drv, msg, sizeof(msg), codeword);

    printf("Encoded data is: \"%s\"\n", codeword);

#define ML (sizeof(msg) + RSCODE_NPAR)


    /* Add one error and two erasures */
    byte_err(0x35, 3, codeword);

    byte_err(0x23, 17, codeword);
    byte_err(0x34, 19, codeword);
    byte_err(0x34, 20, codeword);
    byte_err((int)"", 21, codeword);


    printf("with some errors: \"%s\"\n", codeword);

    /* We need to indicate the position of the erasures.  Eraseure
       positions are indexed (1 based) from the end of the message... */

#ifndef RSCODE_DISABLE_ERASURES_FUNCTIONS
    erasures[nerasures++] = ML-17;
    erasures[nerasures++] = ML-19;

    /* Now decode -- encoded codeword size must be passed */
    rscode_decode_with_erasures(&drv, codeword, ML, nerasures, erasures);
#else
    rscode_decode(&drv, codeword, ML);
    (void)nerasures;
    (void)erasures;
#endif /* RSCODE_DISABLE_ERASURES_FUNCTIONS */

    printf("Corrected codeword: \"%s\"\n", codeword);

    errCount += rscodeDataCmp(sizeof(msg), msg, codeword);
    printf("TEXT COMPLEATE ERROR: %d\n", errCount);

    if(errCount != 0) {
        return errCount;
    }
#undef ML
    //-----------------------------------------------------------------------------------------------

    fflush(stdout);
    fflush(stderr);
    printf("\n -----------------RS CODE RAND TEST ----------------------- \n");
    srand(randomSeed); // use current time as seed for random generator

    errCount += rs_code_copy_func_test(&drv, testCount);
    errCount += rs_code_only_par_func_test(&drv, testCount);
    errCount += rs_code_only_continious_func_test(&drv, testCount);


    printf("\n\n-----------------------------------------------\n");
    printf("RS CODECOMPLEATE ERROR: %d\n", errCount);
    printf("-----------------------------------------------\n");

    fflush(stdout);
    fflush(stderr);

    return errCount;
}





#endif /* RSCODE_TEST_DISABLE */
