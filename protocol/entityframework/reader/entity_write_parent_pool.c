#include "entity_write_parent_pool.h"

#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#ifdef USE_ENTITY_READ_SERVICE
#include "smart_assert.h"
#include <stdlib.h>


// write pool container for entities parent ---------------------------------------------------------
EntityWritePoolContainer_t* entityWritePoolContainer_new(const reg pointerCount)
{
    M_Assert_BreakSaveCheck(NOT_IS_POWER_OF_2(pointerCount), M_EMPTY, return NULL, "entityWritePoolContainer_new: No valid input");

    EntityWritePoolContainer_t* m = calloc(1, sizeof(EntityWritePoolContainer_t));
    M_Assert_BreakSaveCheck((m == NULL), M_EMPTY, return NULL, "entityWritePoolContainer_new: No memory for allocation");

    entityWritePoolContainer_init(m, pointerCount);
    return m;
}

int entityWritePoolContainer_init(EntityWritePoolContainer_t* const self, const reg pointerCount)
{
    M_Assert_BreakSaveCheck(((self == NULL) || NOT_IS_POWER_OF_2(pointerCount)), M_EMPTY, return ENTITY_ERROR, "entityWritePoolContainer_init: No valid input");

    self->writePool = calloc(pointerCount, sizeof(EntityReadParent_t*));
    M_Assert_BreakSaveCheck((self->writePool == NULL), M_EMPTY, return ENTITY_ERROR, "entityWritePoolContainer_init: No memory for allocation");

    self->msk = (pointerCount - 1);
    self->head = 0;
    self->tail = 0;
    self->wrFull = 0;
    self->rdEmpty = 1;
    return ENTITY_OK;
}

int entityWritePoolContainer_delete(EntityWritePoolContainer_t** const self, const b isHeap)
{
    if(self == NULL || (*self) == NULL) {
        return ENTITY_ERROR;
    }
    free((*self)->writePool);
    (*self)->writePool = NULL;

    if(isHeap) {
        free((*self));
        (*self) = NULL;
    }
    return ENTITY_OK;
}


#endif /* USE_ENTITY_READ_SERVICE */

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */
