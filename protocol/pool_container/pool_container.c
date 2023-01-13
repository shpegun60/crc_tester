#include "pool_container.h"
#include "smart_assert.h"
#include <stdlib.h>


pool_container_t *poolContainer_new(const u16 rows, const u16 columns)
{
    M_Assert_BreakSaveCheck((columns == 0 || rows == 0), M_EMPTY, return NULL, "poolContainer_new: Give me positive values for dimensions genius");
    M_Assert_BreakSaveCheck((columns & (columns - 1)), M_EMPTY, return NULL, "poolContainer_new: columns is not power of 2, val: %d", columns);
    M_Assert_BreakSaveCheck((rows & (rows - 1)), M_EMPTY, return NULL, "poolContainer_new: rows is not power of 2, val: %d", rows);
    M_Assert_BreakSaveCheck((rows  > 16384 || columns > 16384), M_EMPTY, return NULL, "poolContainer_new: rows or colls is too long, RAW: %d, COL: %d", rows, columns);

    pool_container_t* self = (pool_container_t *)calloc(1, sizeof(pool_container_t));
    M_Assert_BreakSaveCheck(self == (pool_container_t *)NULL, M_EMPTY, return self, "poolContainer_new: No memory for allocation ");

    poolContainer_init(self, rows, columns);
    return self;
}

void poolContainer_init(pool_container_t * const self, const u16 rows, const u16 columns)
{
    M_Assert_BreakSaveCheck(self == (pool_container_t *)NULL, M_EMPTY, return, "poolContainer_init: No memory for allocation ");
    M_Assert_BreakSaveCheck((columns == 0 || rows == 0), M_EMPTY, return, "poolContainer_init: Give me positive values for dimensions genius");
    M_Assert_BreakSaveCheck((columns & (columns - 1)), M_EMPTY, return, "poolContainer_init: columns is not power of 2, val: %d", columns);
    M_Assert_BreakSaveCheck((rows & (rows - 1)), M_EMPTY, return, "poolContainer_init: rows is not power of 2, val: %d", rows);
    M_Assert_BreakSaveCheck((rows  > 16384 || columns > 16384), M_EMPTY, return, "poolContainer_init: rows or colls is too long, RAW: %d, COL: %d", rows, columns);

    self->pool = (u8**)malloc(rows * sizeof(u8*));
    M_Assert_BreakSaveCheck((self->pool == NULL), M_EMPTY, return, "poolContainer_init: no memories for allocation data");

    for (reg i = 0; i < rows; ++i) {
        self->pool[i] = (u8*)malloc(columns * sizeof(u8));
        M_Assert_Break((self->pool[i] == NULL), M_EMPTY, return, "poolContainer_init: no memories for allocation");
    }

    self->size = (u16*)calloc(rows, sizeof(u16));
    M_Assert_BreakSaveCheck((self->pool == NULL), M_EMPTY, return, "poolContainer_init: no memories for allocation data");


    self->columns = columns;
    self->rows = rows;
    self->msk = (reg)(rows - 1U);
    poolContainer_clear(self);
}

void poolContainer_clear(pool_container_t * const self)
{
    M_Assert_BreakSaveCheck(self == (pool_container_t *)NULL, M_EMPTY, return, "poolContainer_clear: No memory allocated");
    self->tail = 0;
    self->head = 0;
    self->wrFull = 0;
    self->rdEmpty = 1;
}

STATIC_FORCEINLINE void poolContainer_proceedSignalls(pool_container_t * const self)
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


/* free memory associated with the pool_container_t  */
int poolContainer_delete(pool_container_t **self, u8 isHeap)
{
    M_Assert_BreakSaveCheck(((*self) == NULL || self == NULL), M_EMPTY, return 1, "poolContainer_delete: incorrect input values");

    /* Code for further processing and free the
       dynamically allocated memory */

    for(reg i = 0; i < (*self)->rows; ++i) {
        free((*self)->pool[i]);
    }

    free((*self)->pool);
    (*self)->pool = NULL;
    free((*self)->size);
    (*self)->size = NULL;

    (*self)->columns = 0;
    (*self)->rows = 0;
    poolContainer_clear(*self);

    if(isHeap) {
        free(*self);
        (*self) = NULL;
    }
    return 0;
}

//------------------------------------ WRITE FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
int poolContainer_writeArr(pool_container_t * const self, u8 * const data, const u16 len)
{
    M_Assert_Break((self == NULL || data == NULL), M_EMPTY, return 0, "poolContainer_writeArr: incorrect input values");
    M_Assert_Break((self->pool == NULL || self->size == NULL), M_EMPTY, return 0, "poolContainer_writeArr: no allocated memory");
    M_Assert_BreakSaveCheck((len > self->columns), M_EMPTY, return 0, "poolContainer_writeArr: len more than buffer");
    M_Assert_WarningSaveCheck(POOL_CONTAINER_IS_FULL(self), M_EMPTY, return 0, "poolContainer_writeArr: buffer is full!!!");

    const reg wr_pos = self->head & self->msk;
    u8* const buffer_raw = self->pool[wr_pos];

    for(reg i = 0; i != len; ++i) {
        buffer_raw[i] = data[i];
    }
    self->size[wr_pos] = len;

    ++self->head;
    M_Assert_Break((self->pool[self->head & self->msk] == NULL), M_EMPTY, return 0, "poolContainer_writeArr: no allocated memory, pool index");
    poolContainer_proceedSignalls(self);
    return len;
}


void poolContainer_getWriteMeta(pool_container_t * const self, u8 ** const data, u16 ** const size)
{
    M_Assert_Break((self == NULL || data == NULL), M_EMPTY, return, "poolContainer_writeArr: incorrect input values");
    M_Assert_Break((self->pool == NULL || self->size == NULL || self->pool[self->head & self->msk] == NULL), M_EMPTY, return, "poolContainer_writeArr: no allocated memory");

    const reg wr_pos = self->head & self->msk;
    (*data) = &self->pool[wr_pos][0];
    (*size) = &self->size[wr_pos];
}

void poolContainer_nextWritePos(pool_container_t * const self)
{
    M_Assert_Break((self == NULL), M_EMPTY, return, "poolContainer_nextReadPos: incorrect input values");
    M_Assert_Break((self->pool == NULL || self->size == NULL), M_EMPTY, return, "poolContainer_nextReadPos: no allocated memory");
    M_Assert_WarningSaveCheck(POOL_CONTAINER_IS_FULL(self), M_EMPTY, return, "poolContainer_nextWritePos: buffer is full!!!");

    ++self->head;
    M_Assert_Break((self->pool[self->head & self->msk] == NULL), M_EMPTY, return, "poolContainer_nextWritePos: no allocated memory, pool index");
    poolContainer_proceedSignalls(self);
}

//------------------------------------ READ FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
u16 poolContainer_readArr(pool_container_t * const self, u8 ** const data)
{
    M_Assert_Break((self == NULL || data == NULL), M_EMPTY, return 0, "poolContainer_readArr: incorrect input values");
    M_Assert_Break((self->pool == NULL || self->size == NULL || self->pool[self->tail & self->msk] == NULL), M_EMPTY, return 0, "poolContainer_readArr: no allocated memory");

    const reg rd_pos = self->tail & self->msk;
    (*data) = &self->pool[rd_pos][0];
    return self->size[rd_pos];
}

void poolContainer_nextReadPos(pool_container_t * const self)
{
    M_Assert_Break((self == NULL), M_EMPTY, return, "poolContainer_nextReadPos: incorrect input values");
    M_Assert_Break((self->pool == NULL || self->size == NULL), M_EMPTY, return, "poolContainer_nextReadPos: no allocated memory");
    M_Assert_WarningSaveCheck(POOL_CONTAINER_IS_EMPTY(self), M_EMPTY, return, "poolContainer_nextReadPos: buffer is empty!!!");

    ++self->tail;
    M_Assert_Break((self->pool[self->tail & self->msk] == NULL), M_EMPTY, return, "poolContainer_nextReadPos: no allocated memory, pool index");
    poolContainer_proceedSignalls(self);
}







