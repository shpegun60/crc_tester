#ifndef STATIC_PULL_CONTAINER_H
#define STATIC_PULL_CONTAINER_H


#include "my_ctypes.h"


#ifndef STATIC_PULL_CONTAINER_COLUMNS
    #define STATIC_PULL_CONTAINER_COLUMNS 256
#endif /* STATIC_PULL_CONTAINER_COLUMNS */

#ifndef STATIC_PULL_CONTAINER_RAWS
    #define STATIC_PULL_CONTAINER_RAWS 256
#endif /* STATIC_PULL_CONTAINER_RAWS */



#define STATIC_PULL_CONTAINER_IS_EMPTY(handle)\
    (handle)->rdEmpty
#define STATIC_PULL_CONTAINER_IS_FULL(handle)\
    (handle)->wrFull

typedef struct {
    u16 size[STATIC_PULL_CONTAINER_RAWS];
    u8 pull[STATIC_PULL_CONTAINER_RAWS][STATIC_PULL_CONTAINER_COLUMNS];

    reg rd_raw;
    reg wr_raw;
    u8 wrFull;
    u8 rdEmpty;
} static_pull_container_t;


static_pull_container_t* staticPullContainer_new(void);
void staticPullContainer_init(static_pull_container_t * const self);

/* free memory associated with the static_pull_container_t  */
int staticPullContainer_delete(static_pull_container_t **self);


//------------------------------------ WRITE FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
int staticPullContainer_writeArr(static_pull_container_t * const self, u8 * const data, const u16 len);
void staticPullContainer_getWriteMeta(static_pull_container_t * const self, u8 ** const data, u16 ** const size);
void staticPullContainer_nextWritePos(static_pull_container_t * const self);

//------------------------------------ READ FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
u16 staticPullContainer_readArr(static_pull_container_t * const self, u8 ** const data);
void staticPullContainer_nextReadPos(static_pull_container_t * const self);

/* USER DEFINES CHECK ----------------------------------------------------------------------------------------------------------------------------------------
 */
#if ((STATIC_PULL_CONTAINER_COLUMNS - 1) & STATIC_PULL_CONTAINER_COLUMNS) || (STATIC_PULL_CONTAINER_COLUMNS == 0) || (STATIC_PULL_CONTAINER_COLUMNS > 16384) // columns buffer is power of 2 size check
    #error STATIC_PULL_CONTAINER_COLUMNS must be is power of 2
#endif //check if power of 2 STATIC_PULL_CONTAINER_COLUMNS

#if ((STATIC_PULL_CONTAINER_RAWS - 1) & STATIC_PULL_CONTAINER_RAWS) || (STATIC_PULL_CONTAINER_RAWS == 0) || (STATIC_PULL_CONTAINER_RAWS > 16384) // raws buffer is power of 2 size check
    #error STATIC_PULL_CONTAINER_RAWS must be is power of 2
#endif //check if power of 2 STATIC_PULL_CONTAINER_RAWS




#if (STATIC_PULL_CONTAINER_COLUMNS > 16384) // columns buffer if too long check
    #error STATIC_PULL_CONTAINER_COLUMNS is too long
#endif //check STATIC_PULL_CONTAINER_COLUMNS

#if (STATIC_PULL_CONTAINER_RAWS > 16384) // raws buffer if too long check
    #error STATIC_PULL_CONTAINER_RAWS is too long
#endif //check STATIC_PULL_CONTAINER_RAWS


#endif /* STATIC_PULL_CONTAINER_H */
