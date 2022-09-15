#ifndef PULL_CONTAINER_H
#define PULL_CONTAINER_H

#include "my_ctypes.h"


#define PULL_CONTAINER_IS_EMPTY(handle)\
    (handle)->rdEmpty
#define PULL_CONTAINER_IS_FULL(handle)\
    (handle)->wrFull

typedef struct {
    u16 * size;
    u8 ** pull;
    u16 columns;
    u16 rows;

    reg rd_raw;
    reg wr_raw;
    u8 wrFull;
    u8 rdEmpty;
} pull_container_t;

pull_container_t *pullContainer_new(const u16 rows, const u16 columns);
void pullContainer_init(pull_container_t * const self, const u16 rows, const u16 columns);
void pullContainer_clear(pull_container_t * const self);

/* free memory associated with the pull_container_t  */
int pullContainer_delete(pull_container_t **self, u8 isHeap);

//------------------------------------ WRITE FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
int pullContainer_writeArr(pull_container_t * const self, u8 * const data, const u16 len);
void pullContainer_getWriteMeta(pull_container_t * const self, u8 ** const data, u16 ** const size);
void pullContainer_nextWritePos(pull_container_t * const self);
//------------------------------------ READ FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
u16 pullContainer_readArr(pull_container_t * const self, u8 ** const data);
void pullContainer_nextReadPos(pull_container_t * const self);

#endif /* PULL_CONTAINER_H */
