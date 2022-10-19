#include "callback_manager.h"
#include "smart_assert.h"

#include <stdlib.h>

CallbackManager_t * CallbackManager_new(void * const parser)
{
    CallbackManager_t * m_manager = malloc(sizeof(CallbackManager_t));
    M_Assert_BreakSaveCheck(m_manager == (CallbackManager_t *)NULL, M_EMPTY, return m_manager, "CallbackManager_new: No memory for allocation ");
    M_Assert_BreakSaveCheck(parser == NULL, M_EMPTY, return m_manager, "CallbackManager_init: parser must not be NULL");

    CallbackManager_init(m_manager, parser);
    return m_manager;
}

void CallbackManager_init(CallbackManager_t * const self, void * const parser)
{
    M_Assert_BreakSaveCheck(self == (CallbackManager_t *)NULL, M_EMPTY, return, "CallbackManager_init: no valid input data");
    M_Assert_BreakSaveCheck(parser == NULL, M_EMPTY, return, "CallbackManager_init: parser must not be NULL");
    for (int i = 0; i < CALL_B_MAN_MAX_COMMAND_FUNCTIONS; ++i) {
        self->workers[i] = (CallbackWorker)NULL;

#ifdef CALL_B_MAN_ENABLE_DIFFERENCE_CONTEXT
        self->context[i] = NULL;
#endif /* CALL_B_MAN_ENABLE_DIFFERENCE_CONTEXT */
    }

#ifndef CALL_B_MAN_ENABLE_DIFFERENCE_CONTEXT
    self->context = NULL;
#endif /* CALL_B_MAN_ENABLE_DIFFERENCE_CONTEXT */

     self->parser = parser;
}

int CallbackManager_delete(CallbackManager_t ** self)
{
    M_Assert_BreakSaveCheck((self == NULL) || (*self == NULL), M_EMPTY, return 1, "CallbackManager_delete: No allocated memory");
    free(*self);
    *self = NULL;

    return 0;
}

void CallbackManager_addWorker(CallbackManager_t * const self, const CallBManIdType id, const CallbackWorker worker, void * const context)
{
    M_Assert_Break(self == (CallbackManager_t *)NULL, M_EMPTY, return, "CallbackManager_addWorker: no valid input data");

#if !((CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 256U) || (CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 65536UL) || (CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 4294967296UL))
    M_Assert_WarningSaveCheck(id > (CALL_B_MAN_MAX_COMMAND_FUNCTIONS - 1), M_EMPTY, return, "CallbackManager_addWorker: no valid input id");
#endif /* !((CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 256U) || (CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 65536UL) || (CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 4294967296UL)) */

    self->workers[id] = worker;

#ifdef CALL_B_MAN_ENABLE_DIFFERENCE_CONTEXT
    self->context[id] = context;
#else
    self->context = context;
#endif /* CALL_B_MAN_ENABLE_DIFFERENCE_CONTEXT */
}


//**********************************************************************************************************************************************************************
int CallbackManager_proceed(const CallbackManager_t* const self, const CallBManIdType id, u32 time)
{
    M_Assert_Break(self == (CallbackManager_t *)NULL, M_EMPTY, return 0, "CallbackManager_proceed: no valid input data");
    M_Assert_Break(self->parser == NULL, M_EMPTY, return 0, "CallbackManager_proceed: parser must not be NULL, call CallbackManager_init function before");

#if !((CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 256U) || (CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 65536UL) || (CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 4294967296UL))
    M_Assert_WarningSaveCheck(id > (CALL_B_MAN_MAX_COMMAND_FUNCTIONS - 1), M_EMPTY, return 0, "CallbackManager_proceed: no valid input id");
#endif /* !((CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 256U) || (CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 65536UL) || (CALL_B_MAN_MAX_COMMAND_FUNCTIONS == 4294967296UL)) */


#ifdef CALL_B_MAN_ENABLE_DIFFERENCE_CONTEXT
    M_Assert_SafeFunctionCall((self->workers[id] != (CallbackWorker)NULL),  {
                                  self->workers[id](self->parser, self->context[id], time);
                                  return 1;
                              });
#else
    M_Assert_SafeFunctionCall((self->workers[id] != (CallbackWorker)NULL),  {
                                  self->workers[id](self->parser, self->context, time);
                                  return 1;
                              });
#endif /* CALL_B_MAN_ENABLE_DIFFERENCE_CONTEXT */
    return 0;
}




