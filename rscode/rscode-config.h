#ifndef _RSCODE_CONFIG_H
#define _RSCODE_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef RSCODE_TEST_DISABLE
    //#define RSCODE_TEST_DISABLE 1
#endif /* RSCODE_TEST_DISABLE */




/* Print debugging info */
#if !defined(RSCODE_DEBUG) && !defined(RSCODE_TEST_DISABLE)
    #define RSCODE_DEBUG 1
#endif /* RSCODE_DEBUG */




/* Disable erasures functions */
#ifndef RSCODE_DISABLE_ERASURES_FUNCTIONS
    //#define RSCODE_DISABLE_ERASURES_FUNCTIONS 1
#endif /* RSCODE_DISABLE_ERASURES_FUNCTIONS */



/* Number of parity bytes
 *
 *  ERASURES ERRORS             == RSCODE_NPAR
 *  UNDEFINED POSITON ERRORS    == (RSCODE_NPAR / 2)
 */
#ifndef RSCODE_NPAR
    #define RSCODE_NPAR 16
#endif /* RSCODE_NPAR */



#ifdef __cplusplus
}
#endif

#endif /* _RSCODE_CONFIG_H */
