#include "static_pull_container.h"
#include "smart_assert.h"
#include "inline.h"
#include <stdlib.h>


static_pull_container_t* staticPullContainer_new(void)
{
    static_pull_container_t* self = (static_pull_container_t *)calloc(1, sizeof(static_pull_container_t));
    M_Assert_BreakSaveCheck(self == (static_pull_container_t *)NULL, M_EMPTY, return self, "staticPullContainer_new: No memory for allocation ");

    staticPullContainer_init(self);
    return self;
}

void staticPullContainer_init(static_pull_container_t * const self)
{
    M_Assert_BreakSaveCheck(self == (static_pull_container_t *)NULL, M_EMPTY, return, "staticPullContainer_init: No valid input");
    self->rd_raw = 0;
    self->wr_raw = 0;
    self->wrFull = 0;
    self->rdEmpty = 1;
}


/* free memory associated with the static_pull_container_t  */
int staticPullContainer_delete(static_pull_container_t **self)
{
    M_Assert_BreakSaveCheck(((*self) == NULL || self == NULL), M_EMPTY, return 1, "staticPullContainer_delete: incorrect input values");

    /* Code for further processing and free the
       dynamically allocated memory */

    free(*self);
    (*self) = NULL;
    return 0;
}

STATIC_INLINE void staticPullContainer_proceedSignalls(static_pull_container_t * const self)
{
    self->rdEmpty = (self->wr_raw == self->rd_raw);
    self->wrFull  = ((self->wr_raw & (STATIC_PULL_CONTAINER_RAWS - 1)) == (self->rd_raw & (STATIC_PULL_CONTAINER_RAWS - 1))) && (!self->rdEmpty);
}


//------------------------------------ WRITE FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
int staticPullContainer_writeArr(static_pull_container_t * const self, u8 * const data, const u16 len)
{
    M_Assert_Break((self == NULL || data == NULL), M_EMPTY, return 0, "staticPullContainer_writeArr: incorrect input values");
    M_Assert_BreakSaveCheck((len > STATIC_PULL_CONTAINER_COLUMNS), M_EMPTY, return 0, "staticPullContainer_writeArr: len more than buffer");
    M_Assert_WarningSaveCheck(STATIC_PULL_CONTAINER_IS_FULL(self), M_EMPTY, return 0, "staticPullContainer_writeArr: buffer is full!!!");

    reg wr_pos = self->wr_raw & (STATIC_PULL_CONTAINER_RAWS - 1);

    for(int i = 0; i < len; ++i) {
        self->pull[wr_pos][i] = data[i];
    }
    self->size[wr_pos] = len;

    ++self->wr_raw;
    staticPullContainer_proceedSignalls(self);
    return len;
}


void staticPullContainer_getWriteMeta(static_pull_container_t * const self, u8 ** const data, u16 ** const size)
{
    M_Assert_Break((self == NULL || data == NULL || size == NULL), M_EMPTY, return, "staticPullContainer_getWriteMeta: incorrect input values");
    reg wr_pos = self->wr_raw & (STATIC_PULL_CONTAINER_RAWS - 1);
    (*data) = &self->pull[wr_pos][0];
    (*size) = &self->size[wr_pos];
}

void staticPullContainer_nextWritePos(static_pull_container_t * const self)
{
    M_Assert_Break((self == NULL), M_EMPTY, return, "staticPullContainer_nextWritePos: incorrect input values");
    M_Assert_WarningSaveCheck(STATIC_PULL_CONTAINER_IS_FULL(self), M_EMPTY, return, "staticPullContainer_nextWritePos: buffer is full!!!");
    ++self->wr_raw;
    staticPullContainer_proceedSignalls(self);
}

//------------------------------------ READ FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
u16 staticPullContainer_readArr(static_pull_container_t * const self, u8 ** const data)
{
    M_Assert_Break((self == NULL || data == NULL), M_EMPTY, return 0, "staticPullContainer_readArr: incorrect input values");
    reg rd_pos = self->rd_raw & (STATIC_PULL_CONTAINER_RAWS - 1);
    (*data) = &self->pull[rd_pos][0];
    return self->size[rd_pos];
}

void staticPullContainer_nextReadPos(static_pull_container_t * const self)
{
    M_Assert_Break((self == NULL), M_EMPTY, return, "staticPullContainer_nextReadPos: incorrect input values");
    M_Assert_WarningSaveCheck(STATIC_PULL_CONTAINER_IS_EMPTY(self), M_EMPTY, return, "staticPullContainer_nextReadPos: buffer is empty!!!");
    ++self->rd_raw;
    staticPullContainer_proceedSignalls(self);
}

