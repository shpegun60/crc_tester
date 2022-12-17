#ifndef __INC_CALLBACK_MANAGER_H__
#define __INC_CALLBACK_MANAGER_H__

#include "my_ctypes.h"
#include "preprocessor_ctx.h"

#ifndef CALL_B_MAN_TEST_DISABLE
//#   define CALL_B_MAN_TEST_DISABLE 1
#endif /* CALL_B_MAN_TEST_DISABLE */

#ifndef CALL_B_MAN_MAX_COMMAND_FUNCTIONS
#   define CALL_B_MAN_MAX_COMMAND_FUNCTIONS 256
#endif /* CALL_B_MAN_MAX_COMMAND_FUNCTIONS */

#if ((CALL_B_MAN_MAX_COMMAND_FUNCTIONS - 1)     < 256)
    typedef u8 CallBManIdType;
#elif ((CALL_B_MAN_MAX_COMMAND_FUNCTIONS - 1)   < 65536)
    typedef u16 CallBManIdType;
#else
    typedef u32 CallBManIdType;
#endif /* CALL_B_MAN_MAX_COMMAND_FUNCTIONS type choose*/


typedef void (*CallbackWorker)(u8* const data, reg* const size, u32 time, PREPROCESSOR_CTX_TYPE(ctx));

typedef struct {
    CallbackWorker workers[CALL_B_MAN_MAX_COMMAND_FUNCTIONS];
} CallbackManager_t;


CallbackManager_t * CallbackManager_new(void);
void CallbackManager_init(CallbackManager_t * const self);
void CallbackManager_addWorker(CallbackManager_t * const self, const CallBManIdType id, const CallbackWorker worker);
int CallbackManager_delete(CallbackManager_t ** self);

//**********************************************************************************************************************************************************************
// callback manager call function with warning!!!
int CallbackManager_proceed(const CallbackManager_t* const self, const CallBManIdType id, u8* const data, reg* const size, u32 time, PREPROCESSOR_CTX_TYPE(ctx));


#endif /* __INC_CALLBACK_MANAGER_H__ */
