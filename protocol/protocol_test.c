#include "protocol_test.h"
#include "entity_tests.h"
#include "protocol.h"
#include "raw_parser_it_test.h"
#include "raw_parser_general_test.h"

#include <slip_test.h>
#include <stdio.h>

#include "entity_port.h"
#include "entity_manager.h"

#include "callback_container.h"
#include "static_callback_container.h"

#include "reader/entity_read_system.h"

void callback(void ** context)
{
    (void)(context);
    return;
}

void static_callback(PREPROCESSOR_CTX_TYPE(context))
{
    PREPROCESSOR_CTX_GET(context,
                         const int* const a,
                         void* const alloc);

    (void)a;
    free(alloc);
    return;
}

void receive_callback(const TEMPLATE(StaticCallbackContainer, void) rcv)
{
    SCC_SAFE_CALL(&rcv);
    return;
}

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
    int RawP_DMA_Test = 0;
#ifndef D_RAW_P_TEST_DISABLE
    if(testFlags & TESTER_RAW_P_DMA) {
        rawParserMacroTest();
        RawP_DMA_Test = rawParserDmaTest(randomSeed, testCnt, 1);
    } else {
        printf("...FLAGS: RAWPARSER TEST DISABLED\n");
    }

#else
    printf("...DEFINE: RAWPARSER TEST DISABLED\n");
#endif /* D_RAW_P_TEST_DISABLE */



    printf("\n***************************************************************************************\n");
    printf("----------->>REED-SOLOMON ECC TEST<<--------------------\n");
    printf("ERASURES ERRORS DETECTION (+ bytes to packet): %d, UNDEFINED POSITON ERRORS DETECTION: %d \n", RSCODE_NPAR, RSCODE_NPAR/2);
    printf("***************************************************************************************\n");



    int Reed_Solomon_Test = 0;
#ifndef RSCODE_TEST_DISABLE
    if(testFlags & TESTER_REED_SOLOMON_ECC) {
        Reed_Solomon_Test = rs_code_test(randomSeed, testCnt);
    } else {
        printf("...FLAGS: REED-SOLOMON TEST DISABLED\n");
    }

#else
    printf("...DEFINE: REED-SOLOMON TEST DISABLED\n");
#endif /* RSCODE_TEST_DISABLE */



    printf("\n***************************************************************************************\n");
    printf("----------->>CALLBACK MANAGER TEST<<--------------------\n");
    printf("***************************************************************************************\n");

    int CallbackManager_Test = 0;
#ifndef CALL_B_MAN_TEST_DISABLE
    if(testFlags & TESTER_CALLBACK_MANAGER) {
        CallbackManager_Test = callbackManagerTest();
    } else {
        printf("...FLAGS: CALLBACK MANAGER TEST DISABLED\n");
    }
#else
    printf("...DEFINE: CALLBACK MANAGER TEST DISABLED\n");
#endif /* CALL_B_MAN_TEST_DISABLE */

    printf("\n***************************************************************************************\n");
    printf("----------->>POOL CONTAINER TEST<<--------------------\n");
    printf("***************************************************************************************\n");

    int Pool_Container_Test = 0;

#ifndef POOL_CONTAINER_TEST_DISABLE
    if(testFlags & TESTER_PULL_CONTAINER) {
        Pool_Container_Test = poolContainerTest(randomSeed, testCnt);
    } else {
        printf("...FLAGS: POOL CONTAINER TEST DISABLED\n");
    }
#else
    printf("...DEFINE: POOL CONTAINER TEST DISABLED\n");
#endif /* CALL_B_MAN_TEST_DISABLE */




    printf("\n***************************************************************************************\n");
    printf("----------->>RAWPARSER IT TEST<<--------------------\n");
    printf("***************************************************************************************\n");

    int RawParserItTest = 0;

#ifndef D_RAW_P_TEST_DISABLE
    if(testFlags & TESTER_RAW_P_IT) {
        RawParserItTest = rawParserItTest(randomSeed, testCnt, 1);
    } else {
        printf("...FLAGS: RAWPARSER IT TEST DISABLED\n");
    }
#else
    printf("...DEFINE: RAWPARSER IT TEST TEST DISABLED\n");
#endif /* CALL_B_MAN_TEST_DISABLE */



    printf("\n***************************************************************************************\n");
    printf("----------->>RAWPARSER GENERAL TEST<<--------------------\n");
    printf("***************************************************************************************\n");

    int RawParserGeneralTest = 0;

#ifndef D_RAW_P_TEST_DISABLE
    if(testFlags & TESTER_RAW_P_GEN) {
        RawParserGeneralTest = rawParserGenTest(randomSeed, testCnt);
    } else {
        printf("...FLAGS: RAWPARSER GENERAL TEST DISABLED\n");
    }
#else
    printf("...DEFINE: RAWPARSER GENERAL TEST DISABLED\n");
#endif /* CALL_B_MAN_TEST_DISABLE */





    printf("\n***************************************************************************************\n");
    printf("----------->>RESULTS<<--------------------\n");
    printf("***************************************************************************************\n");

    printf("CRC TEST EXIT WITH ERROR: %d\n", CRCtest);
    printf("ENDIAN TEST EXIT WITH ERROR: %d\n", Endianstest);
    printf("CONVERT TEST EXIT WITH ERROR: %d\n", ConvertTest);
    printf("RAWPARSER DMA TEST EXIT WITH ERROR: %d\n", RawP_DMA_Test);
    printf("REED-SOLOMON TEST EXIT WITH ERROR: %d\n", Reed_Solomon_Test);
    printf("CALLBACK MANAGER TEST EXIT WITH ERROR: %d\n", CallbackManager_Test);
    printf("POOL CONTAINER TEST EXIT WITH ERROR: %d\n", Pool_Container_Test);
    printf("RAWPARSER IT TEST EXIT WITH ERROR: %d\n", RawParserItTest);
    printf("RAWPARSER GENERAL TEST EXIT WITH ERROR: %d\n", RawParserGeneralTest);
    printf("COMPILER VERSION: %s %ld\n", PREDEF_COMPILER_NAME, __STDC_VERSION__);
    printf("ENTITY VERSION: %d, %s\n", C_ENTITY_FRAMEWORK_LIB_VERSION, C_ENTITY_FRAMEWORK_LIB_VERSION_INFO);

//    u32 data;
//    if (__builtin_types_compatible_p(int, MY_CTYPE_GET_TYPE(INT32_TYPE))) {
//        printf("TYPE YES---------------------------------id: %d\n", MY_CTYPE_GET_TYPE_ID(int));
//    } else {
//        printf("TYPE NOT---------------------------------\n");
//    }


    //fieldRename(NULL, 0, "add");
    //b ok = 0;
    TEMPLATE(CallbackContainer, void, p2) callback_ibj;
    TEMPLATE(initCallbackContainerUser, void, p2)(&callback_ibj, callback, NULL);
    CC_CALL(&callback_ibj);


    int a = -1234345;
    receive_callback((const TEMPLATE(StaticCallbackContainer, void)){static_callback, PREPROCESSOR_CTX_CAPTURE({&a, malloc(10)})});

    //entityTest(randomSeed, testCnt);



    slip_test(randomSeed, testCnt);




    fflush(stdout);
    fflush(stderr);
    return 0;



}






