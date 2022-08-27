#include "protocol_test.h"
#include "protocol.h"

#include <stdio.h>


int protocolAllTest(int randomSeed, int testCnt, unsigned int testFlags)
{
    printf("\n***************************************************************************************\n");
    printf("----------->>PROTOCOL TEST STARTED<<--------------------\n");
    printf("***************************************************************************************\n");



    printf("\n***************************************************************************************\n");
    printf("----------->>CRC TEST<<--------------------\n");
    printf("***************************************************************************************\n");
    int CRCtest = 0;
#ifndef _MY_CRC_TEST_DISABLE

    if(testFlags & TESTER_CRC) {
        CRCtest = crc_test(randomSeed, testCnt);
    } else {
        printf("...FLAGS: CRC TEST DISABLED\n");
    }

#else
    printf("...DEFINE: CRC TEST DISABLED\n");
#endif /* _MY_CRC_TEST_DISABLE */




    printf("\n***************************************************************************************\n");
    printf("----------->>ENDIAN LOGIC TEST<<--------------------\n");
    printf("***************************************************************************************\n");
    int Endianstest = 0;
#ifndef BYTE_ORDER_TEST_DISABLE

    if(testFlags & TESTER_ENDIAN) {
        Endianstest = endiansTest();
    } else {
        printf("...FLAGS: ENDIAN TEST DISABLED\n");
    }

#else
    printf("...DEFINE: ENDIAN TEST DISABLED\n");
#endif /* BYTE_ORDER_TEST_DISABLE */





    printf("\n***************************************************************************************\n");
    printf("----------->>CONVERT TEST<<--------------------\n");
    printf("***************************************************************************************\n");

    int ConvertTest = 0;
#ifndef CONVERT_TEST_DISABLE
    if(testFlags & TESTER_CONVERT) {
        ConvertTest = convertTest(testCnt, randomSeed);
    } else {
        printf("...FLAGS: CONVERT TEST DISABLED\n");
    }

#else
    printf("...DEFINE: CONVERT TEST DISABLED\n");
#endif /* CONVERT_TEST_DISABLE */


    printf("\n***************************************************************************************\n");
    printf("----------->>RAWPARSER DMA TEST<<--------------------\n");
    printf("***************************************************************************************\n");
    int RawPTest = 0;
#ifndef D_RAW_P_TEST_DISABLE
    if(testFlags & TESTER_RAW_P_DMA) {
        rawParserMacroTest();
        RawPTest = rawParserDmaTest(randomSeed, testCnt, 1);
    } else {
        printf("...FLAGS: RAWPARSER TEST DISABLED\n");
    }

#else
    printf("...DEFINE: RAWPARSER TEST DISABLED\n");
#endif /* D_RAW_P_TEST_DISABLE */



    printf("\n***************************************************************************************\n");
    printf("----------->>REED-SOLOMON ECC TEST<<--------------------\n");
    printf("ERASURES ERRORS: %d, UNDEFINED POSITON ERRORS: %d \n", RSCODE_NPAR, RSCODE_NPAR/2);
    printf("***************************************************************************************\n");



    int Reed_Solomon_Test = 0;
#ifndef RSCODE_TEST_DISABLE
    if(testFlags & TESTER_REED_SOLOMON_ECC) {
        Reed_Solomon_Test = rs_code_test(randomSeed, testFlags);
    } else {
        printf("...FLAGS: REED-SOLOMON TEST DISABLED\n");
    }

#else
    printf("...DEFINE: REED-SOLOMON TEST DISABLED\n");
#endif /* RSCODE_TEST_DISABLE */





    printf("\n***************************************************************************************\n");
    printf("----------->>RESULTS<<--------------------\n");
    printf("***************************************************************************************\n");

    printf("CRC TEST EXIT WITH ERROR: %d\n", CRCtest);
    printf("ENDIAN TEST EXIT WITH ERROR: %d\n", Endianstest);
    printf("CONVERT TEST EXIT WITH ERROR: %d\n", ConvertTest);
    printf("RAWPARSER DMA TEST EXIT WITH ERROR: %d\n", RawPTest);
    printf("REED-SOLOMON TEST EXIT WITH ERROR: %d\n", Reed_Solomon_Test);




    fflush(stdout);
    fflush(stderr);
    return 0;
}

