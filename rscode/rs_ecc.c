#include "rs_ecc.h"
#include "rs.h"
#include "berlekamp.h"

void rscode_init(rscode_driver * driver)
{
    zero_fill_from ((unsigned char *)driver, 0, sizeof(rscode_driver));
    initialize_ecc (driver);
}

void rscode_encode(rscode_driver * driver, unsigned char *msg, int nbytes, unsigned char *dst)
{
    encode_data(driver, msg, nbytes,dst);
}

int rscode_decode(rscode_driver * driver, unsigned char *data, int nbytes)
{
    int ret;

    decode_data(driver, data, nbytes);
    ret = check_syndrome (driver);
    if(ret != 0) {
        #ifndef RSCODE_DISABLE_ERASURES_FUNCTIONS
            correct_errors_erasures(driver, data, nbytes, 0, 0);
        #else
            correct_errors_erasures (driver, data, nbytes);
        #endif /* RSCODE_DISABLE_ERASURES_FUNCTIONS */
    }
    return ret;
}

#ifndef RSCODE_DISABLE_ERASURES_FUNCTIONS

int rscode_decode_with_erasures(rscode_driver * driver, unsigned char *data, int nbytes, int nerasures, int * erasures)
{
    int ret;
    decode_data(driver, data, nbytes);
    ret = check_syndrome (driver);
    if(ret!=0) {
        int newret = correct_errors_erasures (driver, data,
                                              nbytes,
                                              nerasures,
                                              erasures);

        if(newret == 0) {
            ret = newret;
        }
    }
    return ret;
}

#endif /* RSCODE_DISABLE_ERASURES_FUNCTIONS */
