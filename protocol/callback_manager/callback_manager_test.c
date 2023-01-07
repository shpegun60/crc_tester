#include "callback_manager_test.h"

#ifndef CALL_B_MAN_TEST_DISABLE


static u8* ptr_context1 = (u8*)1;
static u8* inputData1 = (u8*)2;
static u8* outputData1 = (u8*)3;
static reg* const size1 = (reg*)4;
static reg const maxOutBufferSize1 = 5;

static u8* ptr_context2 = (u8*)6;
static u8* inputData2 = (u8*)7;
static u8* outputData2 = (u8*)8;
static reg* const size2 = (reg*)9;
static reg const maxOutBufferSize2 = 10;


void testWorker1 (u8* const inputData, u8* const outputData, reg* const size, const reg maxOutBufferSize, PREPROCESSOR_CTX_TYPE(ctx));
void testWorker2 (u8* const inputData, u8* const outputData, reg* const size, const reg maxOutBufferSize, PREPROCESSOR_CTX_TYPE(ctx));

int callbackManagerTest(void)
{
    int testNotPassed = 2;
    int ctx_val1 = 123;
    int ctx_val2 = -456;

    //-----------------------------------------------------------------------------
    CallbackManager_t * manager =  CallbackManager_new();

    CallbackManager_addWorker(manager, 0, testWorker1, PREPROCESSOR_CTX_CAPTURE({
                                                                                    ptr_context1,
                                                                                    &ctx_val1,
                                                                                    &testNotPassed}));
    CallbackManager_addWorker(manager, 1, testWorker2, PREPROCESSOR_CTX_CAPTURE({
                                                                                    ptr_context2,
                                                                                    &ctx_val2,
                                                                                    &testNotPassed}));


//    int CallbackManager_proceed(const CallbackManager_t* const self, const CallBManIdType id, u8* const inputData, u8* const outputData, reg* const size, const reg maxOutBufferSize);

    if(!CallbackManager_proceed(manager, 0, inputData1, outputData1, size1, maxOutBufferSize1)) {
        ++testNotPassed;
    }

    if(!CallbackManager_proceed(manager, 1, inputData2, outputData2, size2, maxOutBufferSize2)) {
        ++testNotPassed;
    }


    testNotPassed += CallbackManager_delete(&manager);
    if(manager != (CallbackManager_t*)NULL) {
        ++testNotPassed;
    }
    //-----------------------------------------------------------------------------
    return testNotPassed;
}


void testWorker1 (u8* const inputData, u8* const outputData, reg* const size, const reg maxOutBufferSize, PREPROCESSOR_CTX_TYPE(ctx))
{
    PREPROCESSOR_CTX_GET(ctx,
                         u8* const ptr_context,
                         const int* const val,
                         int* const testNotPassed);

#ifdef CALL_B_MAN_ENABLE_DIFFERENCE_CONTEXT
    if((inputData == inputData1) && (outputData == outputData1) && (size == size1) && (maxOutBufferSize == maxOutBufferSize1) && (ptr_context == ptr_context1) && (*val == 123) ) {
        --(*testNotPassed);
    }
#else
    if((inputData == inputData1) && (outputData == outputData1) && (size == size1) && (maxOutBufferSize == maxOutBufferSize1) && (ptr_context == ptr_context2) && (*val == -456) ) {
        --(*testNotPassed);
    }
#endif /* CALL_B_MAN_ENABLE_DIFFERENCE_CONTEXT */
}

void testWorker2 (u8* const inputData, u8* const outputData, reg* const size, const reg maxOutBufferSize, PREPROCESSOR_CTX_TYPE(ctx))
{
    PREPROCESSOR_CTX_GET(ctx,
                         u8* const ptr_context,
                         const int* const val,
                         int* const testNotPassed);

    if((inputData == inputData2) && (outputData == outputData2) && (size == size2) && (maxOutBufferSize == maxOutBufferSize2) && (ptr_context == ptr_context2) && (*val == -456) ) {
        --(*testNotPassed);
    }
}

#endif /* CALL_B_MAN_TEST_DISABLE */
