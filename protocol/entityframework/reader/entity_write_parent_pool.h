#ifndef ENTITY_WRITE_PARENT_POOL_H
#define ENTITY_WRITE_PARENT_POOL_H

#include "entity_port.h"


#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#ifdef USE_ENTITY_READ_SERVICE

typedef struct EntityReadParent EntityReadParent_t;

// container for writed fiedld`s
typedef struct {
    reg head;
    reg tail;
    b rdEmpty;
    b wrFull;

    reg msk;
    EntityReadParent_t** writePool;
} EntityWritePoolContainer_t;

EntityWritePoolContainer_t* entityWritePoolContainer_new(const reg pointerCount);
int entityWritePoolContainer_init(EntityWritePoolContainer_t* const self, const reg pointerCount);
int entityWritePoolContainer_delete(EntityWritePoolContainer_t** const self, const b isHeap);

STATIC_FORCEINLINE void entityWritePoolContainer_proceedSignalls(EntityWritePoolContainer_t * const self)
{
    /*
     *  Borrowed from Synchronous FIFO systemVerilog code (AW --> is fifo size, power of 2):
     *
     *  assign ptr_match   = (wr_addr[AW-1:0] == rd_addr[AW-1:0]);
     *  assign wr_full     = ptr_match & (wr_addr[AW]==!rd_addr[AW]);
     *  assign fifo_empty  = ptr_match & (wr_addr[AW]==rd_addr[AW]);
     *
     */

    const reg head = self->head;
    const reg tail = self->tail;
    const reg msk  = self->msk;

    const u8 rdEmpty = self->rdEmpty = (head == tail);
    self->wrFull  = ((head & msk) == (tail & msk)) && (!rdEmpty);
}

STATIC_FORCEINLINE void entityWritePoolContainer_pushParent(EntityWritePoolContainer_t * const self, EntityReadParent_t* const parent)
{
    const reg wr_pos = (self->head & self->msk);
    self->writePool[wr_pos] = parent;

    ++self->head;
    entityWritePoolContainer_proceedSignalls(self);
}

STATIC_FORCEINLINE EntityReadParent_t* entityWritePoolContainer_readParent(EntityWritePoolContainer_t * const self)
{
    const reg rd_pos = (self->tail & self->msk);
    EntityReadParent_t* const parent = self->writePool[rd_pos];

    ++self->tail;
    entityWritePoolContainer_proceedSignalls(self);
    return parent;
}

#endif /* USE_ENTITY_READ_SERVICE */

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */

#endif /* ENTITY_WRITE_PARENT_POOL_H */
