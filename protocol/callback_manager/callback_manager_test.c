#include "callback_manager_test.h"

#ifndef CALL_B_MAN_TEST_DISABLE


u8* ptr_parser = (u8*)123456;

u8* ptr_context1 = (u8*)987654;
u8* ptr_context2 = (u8*)214365;
u32 time1 = 3;
u32 time2 = 4;


static int testNotPassed = 2;


void testWorker1 (void * parser, void * context, u32 time);
void testWorker2 (void * parser, void * context, u32 time);

int callbackManagerTest(void)
{
    //-----------------------------------------------------------------------------
    testNotPassed = 2;
    CallbackManager_t * manager =  CallbackManager_new(ptr_parser);
    CallbackManager_addWorker(manager, 0, testWorker1, ptr_context1);
    CallbackManager_addWorker(manager, 1, testWorker2, ptr_context2);

    if(!CallbackManager_proceed(manager, 0, time1)) {
        ++testNotPassed;
    }

    if(!CallbackManager_proceed(manager, 1, time2)) {
        ++testNotPassed;
    }


    testNotPassed += CallbackManager_delete(&manager);
    if(manager != (CallbackManager_t*)NULL) {
        ++testNotPassed;
    }
    //-----------------------------------------------------------------------------
    return testNotPassed;
}


void testWorker1 (void * parser, void * context, u32 time)
{
    u8* parser_internal = parser;
    u8* context_internal = context;

#ifdef CALL_B_MAN_ENABLE_DIFFERENCE_CONTEXT
    if((parser_internal == ptr_parser) && (context_internal == ptr_context1) && (time1 == time)) {
        --testNotPassed;
    }
#else
    if((parser_internal == ptr_parser) && (context_internal == ptr_context2) && (time1 == time)) {
        --testNotPassed;
    }
#endif /* CALL_B_MAN_ENABLE_DIFFERENCE_CONTEXT */

}

void testWorker2 (void * parser, void * context, u32 time)
{
    u8* parser_internal = parser;
    u8* context_internal = context;

    if((parser_internal == ptr_parser) && (context_internal == ptr_context2) && (time2 == time)) {
        --testNotPassed;
    }
}

#endif /* CALL_B_MAN_TEST_DISABLE */
