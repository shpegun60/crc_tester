#include "pull_container.h"
#include "smart_assert.h"
#include <stdlib.h>


pull_container_t *pullContainer_new(const u16 rows, const u16 columns)
{
    M_Assert_BreakSaveCheck((columns == 0 || rows == 0), M_EMPTY, return NULL, "pullContainer_new: Give me positive values for dimensions genius");
    M_Assert_BreakSaveCheck((columns & (columns - 1)), M_EMPTY, return NULL, "pullContainer_new: columns is not power of 2, val: %d", columns);
    M_Assert_BreakSaveCheck((rows & (rows - 1)), M_EMPTY, return NULL, "pullContainer_new: rows is not power of 2, val: %d", rows);
    M_Assert_BreakSaveCheck((rows  > 16384 || columns > 16384), M_EMPTY, return NULL, "pullContainer_new: rows or calls is too long, RAW: %d, COL: %d", rows, columns);

    pull_container_t* self = (pull_container_t *)calloc(1, sizeof(pull_container_t));
    M_Assert_BreakSaveCheck(self == (pull_container_t *)NULL, M_EMPTY, return self, "pullContainer_new: No memory for allocation ");

    pullContainer_init(self, rows, columns);
    return self;
}

void pullContainer_init(pull_container_t * const self, const u16 rows, const u16 columns)
{
    M_Assert_BreakSaveCheck(self == (pull_container_t *)NULL, M_EMPTY, return, "pullContainer_init: No memory for allocation ");
    M_Assert_BreakSaveCheck((columns == 0 || rows == 0), M_EMPTY, return, "pullContainer_init: Give me positive values for dimensions genius");
    M_Assert_BreakSaveCheck((columns & (columns - 1)), M_EMPTY, return, "pullContainer_init: columns is not power of 2, val: %d", columns);
    M_Assert_BreakSaveCheck((rows & (rows - 1)), M_EMPTY, return, "pullContainer_init: rows is not power of 2, val: %d", rows);
    M_Assert_BreakSaveCheck((rows  > 16384 || columns > 16384), M_EMPTY, return, "pullContainer_init: rows or calls is too long, RAW: %d, COL: %d", rows, columns);

    self->pull = (u8**)malloc(rows * sizeof(u8*));
    M_Assert_BreakSaveCheck((self->pull == NULL), M_EMPTY, return, "pullContainer_init: no memories for allocation data");

    for (int i = 0; i < rows; ++i) {
        self->pull[i] = (u8*)malloc(columns * sizeof(u8));
        M_Assert_Break((self->pull[i] == NULL), M_EMPTY, return, "pullContainer_init: no memories for allocation");
    }

    self->size = (u16*)calloc(rows, sizeof(u16));
    M_Assert_BreakSaveCheck((self->pull == NULL), M_EMPTY, return, "pullContainer_init: no memories for allocation data");


    self->columns = columns;
    self->rows = rows;
    pullContainer_clear(self);
}

void pullContainer_clear(pull_container_t * const self)
{
    M_Assert_BreakSaveCheck(self == (pull_container_t *)NULL, M_EMPTY, return, "pullContainer_clear: No memory allocated");
    self->rd_raw = 0;
    self->wr_raw = 0;
    self->wrFull = 0;
    self->rdEmpty = 1;
}

STATIC_INLINE void pullContainer_proceedSignalls(pull_container_t * const self)
{
    self->rdEmpty = (self->wr_raw == self->rd_raw);
    self->wrFull  = ((self->wr_raw & (self->rows - 1)) == (self->rd_raw & (self->rows - 1))) && (!self->rdEmpty);
}


/* free memory associated with the pull_container_t  */
int pullContainer_delete(pull_container_t **self, u8 isHeap)
{
    M_Assert_BreakSaveCheck(((*self) == NULL || self == NULL), M_EMPTY, return 1, "pullContainer_delete: incorrect input values");

    /* Code for further processing and free the
       dynamically allocated memory */

    for(int i = 0; i < (*self)->rows; ++i) {
        free((*self)->pull[i]);
    }

    free((*self)->pull);
    (*self)->pull = NULL;
    free((*self)->size);
    (*self)->size = NULL;

    (*self)->columns = 0;
    (*self)->rows = 0;
    pullContainer_clear(*self);

    if(isHeap) {
        free(*self);
        (*self) = NULL;
    }
    return 0;
}

//------------------------------------ WRITE FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
int pullContainer_writeArr(pull_container_t * const self, u8 * const data, const u16 len)
{
    M_Assert_Break((self == NULL || data == NULL), M_EMPTY, return 0, "pullContainer_writeArr: incorrect input values");
    M_Assert_Break((self->pull == NULL || self->size == NULL), M_EMPTY, return 0, "pullContainer_writeArr: no allocated memory");
    M_Assert_BreakSaveCheck((len > self->columns), M_EMPTY, return 0, "pullContainer_writeArr: len more than buffer");
    M_Assert_WarningSaveCheck(PULL_CONTAINER_IS_FULL(self), M_EMPTY, return 0, "pullContainer_writeArr: buffer is full!!!");

    reg wr_pos = self->wr_raw & (self->rows - 1);

    for(int i = 0; i < len; ++i) {
        self->pull[wr_pos][i] = data[i];
    }
    self->size[wr_pos] = len;

    ++self->wr_raw;
    M_Assert_Break((self->pull[self->wr_raw & (self->rows - 1)] == NULL), M_EMPTY, return 0, "pullContainer_writeArr: no allocated memory, pull index");
    pullContainer_proceedSignalls(self);
    return len;
}


void pullContainer_getWriteMeta(pull_container_t * const self, u8 ** const data, u16 ** const size)
{
    M_Assert_Break((self == NULL || data == NULL), M_EMPTY, return, "pullContainer_writeArr: incorrect input values");
    M_Assert_Break((self->pull == NULL || self->size == NULL || self->pull[self->wr_raw & (self->rows - 1)] == NULL), M_EMPTY, return, "pullContainer_writeArr: no allocated memory");

    reg wr_pos = self->wr_raw & (self->rows - 1);
    (*data) = &self->pull[wr_pos][0];
    (*size) = &self->size[wr_pos];
}

void pullContainer_nextWritePos(pull_container_t * const self)
{
    M_Assert_Break((self == NULL), M_EMPTY, return, "pullContainer_nextReadPos: incorrect input values");
    M_Assert_Break((self->pull == NULL || self->size == NULL), M_EMPTY, return, "pullContainer_nextReadPos: no allocated memory");
    M_Assert_WarningSaveCheck(PULL_CONTAINER_IS_FULL(self), M_EMPTY, return, "pullContainer_nextWritePos: buffer is full!!!");

    ++self->wr_raw;
    M_Assert_Break((self->pull[self->wr_raw & (self->rows - 1)] == NULL), M_EMPTY, return, "pullContainer_nextWritePos: no allocated memory, pull index");
    pullContainer_proceedSignalls(self);
}

//------------------------------------ READ FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
u16 pullContainer_readArr(pull_container_t * const self, u8 ** const data)
{
    M_Assert_Break((self == NULL || data == NULL), M_EMPTY, return 0, "pullContainer_readArr: incorrect input values");
    M_Assert_Break((self->pull == NULL || self->size == NULL || self->pull[self->rd_raw & (self->rows - 1)] == NULL), M_EMPTY, return 0, "pullContainer_readArr: no allocated memory");

    reg rd_pos = self->rd_raw & (self->rows - 1);
    (*data) = &self->pull[rd_pos][0];
    return self->size[rd_pos];
}

void pullContainer_nextReadPos(pull_container_t * const self)
{
    M_Assert_Break((self == NULL), M_EMPTY, return, "pullContainer_nextReadPos: incorrect input values");
    M_Assert_Break((self->pull == NULL || self->size == NULL), M_EMPTY, return, "pullContainer_nextReadPos: no allocated memory");
    M_Assert_WarningSaveCheck(PULL_CONTAINER_IS_EMPTY(self), M_EMPTY, return, "pullContainer_nextReadPos: buffer is empty!!!");

    ++self->rd_raw;
    M_Assert_Break((self->pull[self->rd_raw & (self->rows - 1)] == NULL), M_EMPTY, return, "pullContainer_nextReadPos: no allocated memory, pull index");
    pullContainer_proceedSignalls(self);
}







