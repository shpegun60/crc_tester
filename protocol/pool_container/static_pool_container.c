#include "static_pool_container.h"
#include "smart_assert.h"
#include "inline.h"
#include <stdlib.h>


static_pool_container_t* staticPoolContainer_new(void)
{
    static_pool_container_t* self = (static_pool_container_t *)calloc(1, sizeof(static_pool_container_t));
    M_Assert_BreakSaveCheck(self == (static_pool_container_t *)NULL, M_EMPTY, return self, "staticPoolContainer_new: No memory for allocation ");

    staticPoolContainer_init(self);
    return self;
}

void staticPoolContainer_init(static_pool_container_t * const self)
{
    M_Assert_BreakSaveCheck(self == (static_pool_container_t *)NULL, M_EMPTY, return, "staticPoolContainer_init: No valid input");
    self->tail  = 0;
    self->head  = 0;
    self->wrFull  = 0;
    self->rdEmpty = 1;
}


/* free memory associated with the static_pool_container_t  */
int staticPoolContainer_delete(static_pool_container_t **self)
{
    M_Assert_BreakSaveCheck(((*self) == NULL || self == NULL), M_EMPTY, return 1, "staticPoolContainer_delete: incorrect input values");

    /* Code for further processing and free the
       dynamically allocated memory */

    free(*self);
    (*self) = NULL;
    return 0;
}

STATIC_FORCEINLINE void staticPoolContainer_proceedSignalls(static_pool_container_t * const self)
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

    const u8 rdEmpty = self->rdEmpty = (head == tail);
    self->wrFull  = ((head & (STATIC_POOL_CONTAINER_RAWS - 1U)) == (tail & (STATIC_POOL_CONTAINER_RAWS - 1U))) && (!rdEmpty);
}


//------------------------------------ WRITE FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
int staticPoolContainer_writeArr(static_pool_container_t * const self, u8 * const data, const u16 len)
{
    M_Assert_Break((self == NULL || data == NULL), M_EMPTY, return 0, "staticPoolContainer_writeArr: incorrect input values");
    M_Assert_BreakSaveCheck((len > STATIC_POOL_CONTAINER_COLUMNS), M_EMPTY, return 0, "staticPoolContainer_writeArr: len more than buffer");
    M_Assert_WarningSaveCheck(STATIC_POOL_CONTAINER_IS_FULL(self), M_EMPTY, return 0, "staticPoolContainer_writeArr: buffer is full!!!");

    const reg wr_pos = self->head & (STATIC_POOL_CONTAINER_RAWS - 1U);
    u8* const buffer_raw = self->pool[wr_pos];

    for(reg i = 0; i != len; ++i) {
        buffer_raw[i] = data[i];
    }
    self->size[wr_pos] = len;

    ++self->head;
    staticPoolContainer_proceedSignalls(self);
    return len;
}


void staticPoolContainer_getWriteMeta(static_pool_container_t * const self, u8 ** const data, u16 ** const size)
{
    M_Assert_Break((self == NULL || data == NULL || size == NULL), M_EMPTY, return, "staticPoolContainer_getWriteMeta: incorrect input values");
    const reg wr_pos = self->head & (STATIC_POOL_CONTAINER_RAWS - 1U);
    (*data) = &self->pool[wr_pos][0];
    (*size) = &self->size[wr_pos];
}

void staticPoolContainer_nextWritePos(static_pool_container_t * const self)
{
    M_Assert_Break((self == NULL), M_EMPTY, return, "staticPoolContainer_nextWritePos: incorrect input values");
    M_Assert_WarningSaveCheck(STATIC_POOL_CONTAINER_IS_FULL(self), M_EMPTY, return, "staticPoolContainer_nextWritePos: buffer is full!!!");
    ++self->head;
    staticPoolContainer_proceedSignalls(self);
}

//------------------------------------ READ FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
u16 staticPoolContainer_readArr(static_pool_container_t * const self, u8 ** const data)
{
    M_Assert_Break((self == NULL || data == NULL), M_EMPTY, return 0, "staticPoolContainer_readArr: incorrect input values");
    const reg rd_pos = self->tail & (STATIC_POOL_CONTAINER_RAWS - 1U);
    (*data) = &self->pool[rd_pos][0];
    return self->size[rd_pos];
}

void staticPoolContainer_nextReadPos(static_pool_container_t * const self)
{
    M_Assert_Break((self == NULL), M_EMPTY, return, "staticPoolContainer_nextReadPos: incorrect input values");
    M_Assert_WarningSaveCheck(STATIC_POOL_CONTAINER_IS_EMPTY(self), M_EMPTY, return, "staticPoolContainer_nextReadPos: buffer is empty!!!");
    ++self->tail;
    staticPoolContainer_proceedSignalls(self);
}

