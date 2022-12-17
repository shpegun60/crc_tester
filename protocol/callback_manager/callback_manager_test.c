#include "callback_manager_test.h"

#ifndef CALL_B_MAN_TEST_DISABLE


static u8* ptr_context1 = (u8*)1;
static u32 time1 = 2;
static u8* data1 = (u8*)3;
static reg* const size1 = (reg*)4;

static u8* ptr_context2 = (u8*)5;
static u32 time2 = 6;
static u8* data2 = (u8*)7;
static reg* const size2 = (reg*)8;


void testWorker1 (u8* const data, reg* const size, u32 time, PREPROCESSOR_CTX_TYPE(ctx));
void testWorker2 (u8* const data, reg* const size, u32 time, PREPROCESSOR_CTX_TYPE(ctx));

int callbackManagerTest(void)
{
    //(const CallbackManager_t* const self, const CallBManIdType id, u8* const data, reg* const size, u32 time, PREPROCESSOR_CTX_TYPE(ctx))
    int testNotPassed = 2;
    //-----------------------------------------------------------------------------
    CallbackManager_t * manager =  CallbackManager_new();
    CallbackManager_addWorker(manager, 0, testWorker1);
    CallbackManager_addWorker(manager, 1, testWorker2);

    int ctx_val1 = 123;
    int ctx_val2 = -456;

    if(!CallbackManager_proceed(manager, 0, data1, size1, time1, PREPROCESSOR_CTX_CAPTURE({
                                                                                          ptr_context1,
                                                                                          &ctx_val1,
                                                                                          &testNotPassed}) )) {
        ++testNotPassed;
    }

    if(!CallbackManager_proceed(manager, 1, data2, size2, time2, PREPROCESSOR_CTX_CAPTURE({
                                                                                          ptr_context2,
                                                                                          &ctx_val2,
                                                                                          &testNotPassed}) )) {
        ++testNotPassed;
    }


    testNotPassed += CallbackManager_delete(&manager);
    if(manager != (CallbackManager_t*)NULL) {
        ++testNotPassed;
    }
    //-----------------------------------------------------------------------------
    return testNotPassed;
}


void testWorker1 (u8* const data, reg* const size, u32 time, PREPROCESSOR_CTX_TYPE(ctx))
{
    PREPROCESSOR_CTX_GET(ctx,
                         u8* const ptr_context,
                         const int* const val,
                         int* const testNotPassed);

    if((data == data1) && (size == size1) && (time1 == time) && (ptr_context == ptr_context1) && (*val == 123) ) {
        --(*testNotPassed);
    }
}

void testWorker2 (u8* const data, reg* const size, u32 time, PREPROCESSOR_CTX_TYPE(ctx))
{
    PREPROCESSOR_CTX_GET(ctx,
                         u8* const ptr_context,
                         const int* const val,
                         int* const testNotPassed);

    if((data == data2) && (size == size2) && (time2 == time) && (ptr_context == ptr_context2) && (*val == -456)) {
        --(*testNotPassed);
    }
}

#endif /* CALL_B_MAN_TEST_DISABLE */
