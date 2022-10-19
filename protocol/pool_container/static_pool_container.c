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
    self->rd_raw  = 0;
    self->wr_raw  = 0;
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
    self->rdEmpty = (self->wr_raw == self->rd_raw);
    self->wrFull  = ((self->wr_raw & (STATIC_POOL_CONTAINER_RAWS - 1)) == (self->rd_raw & (STATIC_POOL_CONTAINER_RAWS - 1))) && (!self->rdEmpty);
}


//------------------------------------ WRITE FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
int staticPoolContainer_writeArr(static_pool_container_t * const self, u8 * const data, const u16 len)
{
    M_Assert_Break((self == NULL || data == NULL), M_EMPTY, return 0, "staticPoolContainer_writeArr: incorrect input values");
    M_Assert_BreakSaveCheck((len > STATIC_POOL_CONTAINER_COLUMNS), M_EMPTY, return 0, "staticPoolContainer_writeArr: len more than buffer");
    M_Assert_WarningSaveCheck(STATIC_POOL_CONTAINER_IS_FULL(self), M_EMPTY, return 0, "staticPoolContainer_writeArr: buffer is full!!!");

    reg wr_pos = self->wr_raw & (STATIC_POOL_CONTAINER_RAWS - 1);

    for(int i = 0; i < len; ++i) {
        self->pool[wr_pos][i] = data[i];
    }
    self->size[wr_pos] = len;

    ++self->wr_raw;
    staticPoolContainer_proceedSignalls(self);
    return len;
}


void staticPoolContainer_getWriteMeta(static_pool_container_t * const self, u8 ** const data, u16 ** const size)
{
    M_Assert_Break((self == NULL || data == NULL || size == NULL), M_EMPTY, return, "staticPoolContainer_getWriteMeta: incorrect input values");
    reg wr_pos = self->wr_raw & (STATIC_POOL_CONTAINER_RAWS - 1);
    (*data) = &self->pool[wr_pos][0];
    (*size) = &self->size[wr_pos];
}

void staticPoolContainer_nextWritePos(static_pool_container_t * const self)
{
    M_Assert_Break((self == NULL), M_EMPTY, return, "staticPoolContainer_nextWritePos: incorrect input values");
    M_Assert_WarningSaveCheck(STATIC_POOL_CONTAINER_IS_FULL(self), M_EMPTY, return, "staticPoolContainer_nextWritePos: buffer is full!!!");
    ++self->wr_raw;
    staticPoolContainer_proceedSignalls(self);
}

//------------------------------------ READ FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
u16 staticPoolContainer_readArr(static_pool_container_t * const self, u8 ** const data)
{
    M_Assert_Break((self == NULL || data == NULL), M_EMPTY, return 0, "staticPoolContainer_readArr: incorrect input values");
    reg rd_pos = self->rd_raw & (STATIC_POOL_CONTAINER_RAWS - 1);
    (*data) = &self->pool[rd_pos][0];
    return self->size[rd_pos];
}

void staticPoolContainer_nextReadPos(static_pool_container_t * const self)
{
    M_Assert_Break((self == NULL), M_EMPTY, return, "staticPoolContainer_nextReadPos: incorrect input values");
    M_Assert_WarningSaveCheck(STATIC_POOL_CONTAINER_IS_EMPTY(self), M_EMPTY, return, "staticPoolContainer_nextReadPos: buffer is empty!!!");
    ++self->rd_raw;
    staticPoolContainer_proceedSignalls(self);
}

