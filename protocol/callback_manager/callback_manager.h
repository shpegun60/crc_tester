#ifndef __INC_CALLBACK_MANAGER_H__
#define __INC_CALLBACK_MANAGER_H__

#include "my_ctypes.h"

#ifndef CALL_B_MAN_TEST_DISABLE
    //#define CALL_B_MAN_TEST_DISABLE 1
#endif /* CALL_B_MAN_TEST_DISABLE */

#ifndef CALL_B_MAN_MAX_COMMAND_FUNCTIONS
    #define CALL_B_MAN_MAX_COMMAND_FUNCTIONS 256
#endif /* CALL_B_MAN_MAX_COMMAND_FUNCTIONS */

#if ((CALL_B_MAN_MAX_COMMAND_FUNCTIONS - 1)     < 256)
    typedef u8 CallBManIdType;
#elif ((CALL_B_MAN_MAX_COMMAND_FUNCTIONS - 1)   < 65536)
    typedef u16 CallBManIdType;
#else
    typedef u32 CallBManIdType;
#endif /* CALL_B_MAN_MAX_COMMAND_FUNCTIONS type choose*/


#ifndef CALL_B_MAN_ENABLE_DIFFERENCE_CONTEXT
    //#define CALL_B_MAN_ENABLE_DIFFERENCE_CONTEXT 1
#endif /* CALL_B_MAN_ENABLE_DIFFERENCE_CONTEXT */

typedef void (*CallbackWorker)(void * parser, void * context, u32 time);

typedef struct {
    CallbackWorker workers[CALL_B_MAN_MAX_COMMAND_FUNCTIONS];

#ifdef CALL_B_MAN_ENABLE_DIFFERENCE_CONTEXT
    void * context[CALL_B_MAN_MAX_COMMAND_FUNCTIONS];
#else
    void * context;
#endif /* CALL_B_MAN_ENABLE_DIFFERENCE_CONTEXT */

    void * parser;
} CallbackManager_t;


CallbackManager_t * CallbackManager_new(void * const parser);
void CallbackManager_init(CallbackManager_t * const self, void * const parser);
void CallbackManager_addWorker(CallbackManager_t * const self, CallBManIdType const id, CallbackWorker const worker, void * const context);
int CallbackManager_delete(CallbackManager_t ** self);

//**********************************************************************************************************************************************************************
void CallbackManager_proceed(CallbackManager_t * const self, CallBManIdType const id, u32 time);

#endif /* __INC_CALLBACK_MANAGER_H__ */
