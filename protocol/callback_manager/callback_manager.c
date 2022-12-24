#include "callback_manager.h"
#include "smart_assert.h"

#include <stdlib.h>

#define LIB "callback manager"
#define ENA 1

CallbackManager_t * CallbackManager_new(void)
{
    CallbackManager_t * m_manager = malloc(sizeof(CallbackManager_t));
    M_Assert_BreakSaveCheck(m_manager == (CallbackManager_t *)NULL, M_EMPTY, return m_manager, M_LIB_DATA_DEF "CallbackManager_new: No memory for allocation ", ENA, LIB);

    CallbackManager_init(m_manager);
    return m_manager;
}

void CallbackManager_init(CallbackManager_t * const self)
{
    M_Assert_BreakSaveCheck(self == (CallbackManager_t *)NULL, M_EMPTY, return, M_LIB_DATA_DEF "CallbackManager_init: no valid input data", ENA, LIB);
    for (unsigned i = 0; i < CALL_B_MAN_MAX_COMMAND_FUNCTIONS; ++i) {
        self->workers[i] = (CallbackWorker)NULL;
    }
}

int CallbackManager_delete(CallbackManager_t ** self)
{
    M_Assert_BreakSaveCheck((self == NULL) || (*self == NULL), M_EMPTY, return 1, M_LIB_DATA_DEF "CallbackManager_delete: No allocated memory", ENA, LIB);
    free(*self);
    *self = NULL;

    return 0;
}

void CallbackManager_addWorker(CallbackManager_t * const self, const CallBManIdType id, const CallbackWorker worker)
{
    M_Assert_Break(self == (CallbackManager_t *)NULL, M_EMPTY, return, M_LIB_DATA_DEF "CallbackManager_addWorker: no valid input data", ENA, LIB);

#if !((CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 256U) || (CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 65536UL) || (CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 4294967296UL))
    M_Assert_WarningSaveCheck(id > (CALL_B_MAN_MAX_COMMAND_FUNCTIONS - 1), M_EMPTY, return, M_LIB_DATA_DEF "CallbackManager_addWorker: no valid input id", ENA, LIB);
#endif /* !((CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 256U) || (CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 65536UL) || (CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 4294967296UL)) */

    self->workers[id] = worker;
}


//**********************************************************************************************************************************************************************
int CallbackManager_proceed(const CallbackManager_t* const self, const CallBManIdType id, u8* const data, reg* const size, u32 time, PREPROCESSOR_CTX_TYPE(ctx))
{
    M_Assert_Break(self == (CallbackManager_t *)NULL, M_EMPTY, return 0, M_LIB_DATA_DEF "CallbackManager_proceed: no valid input data", ENA, LIB);

#if !((CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 256U) || (CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 65536UL) || (CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 4294967296UL))
    M_Assert_WarningSaveCheck(id > (CALL_B_MAN_MAX_COMMAND_FUNCTIONS - 1), M_EMPTY, return 0, M_LIB_DATA_DEF "CallbackManager_proceed: no valid input id", ENA, LIB);
#endif /* !((CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 256U) || (CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 65536UL) || (CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 4294967296UL)) */

    // move to cash
    const CallbackWorker worker = self->workers[id];

    // do logic
    M_Assert_SafeFunctionCall((worker != (CallbackWorker)NULL),  {
                                  worker(data, size, time, ctx);
                                  return 1;
                              });
    return 0;
}

#undef LIB
#undef ENA
