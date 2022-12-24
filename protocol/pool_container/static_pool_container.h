#ifndef STATIC_POOL_CONTAINER_H
#define STATIC_POOL_CONTAINER_H


#include "my_ctypes.h"


#ifndef STATIC_POOL_CONTAINER_COLUMNS
    #define STATIC_POOL_CONTAINER_COLUMNS 256 // maximum number of bytes per one array
#endif /* STATIC_POOL_CONTAINER_COLUMNS */

#ifndef STATIC_POOL_CONTAINER_RAWS
    #define STATIC_POOL_CONTAINER_RAWS 128 // maximum number of arrays
#endif /* STATIC_POOL_CONTAINER_RAWS */



#define STATIC_POOL_CONTAINER_IS_EMPTY(handle)\
    (handle)->rdEmpty
#define STATIC_POOL_CONTAINER_IS_FULL(handle)\
    (handle)->wrFull

typedef struct {
    u16 size[STATIC_POOL_CONTAINER_RAWS];
    u8 pool[STATIC_POOL_CONTAINER_RAWS][STATIC_POOL_CONTAINER_COLUMNS];

    reg tail;
    reg head;
    u8 wrFull;
    u8 rdEmpty;
} static_pool_container_t;


static_pool_container_t* staticPoolContainer_new(void);
void staticPoolContainer_init(static_pool_container_t * const self);
#define staticPontainer_clear(self) staticPoolContainer_init((self))

/* free memory associated with the static_Pool_container_t  */
int staticPoolContainer_delete(static_pool_container_t **self);


//------------------------------------ WRITE FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
int staticPoolContainer_writeArr(static_pool_container_t * const self, u8 * const data, const u16 len);
void staticPoolContainer_getWriteMeta(static_pool_container_t * const self, u8 ** const data, u16 ** const size);
void staticPoolContainer_nextWritePos(static_pool_container_t * const self);

//------------------------------------ READ FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
u16 staticPoolContainer_readArr(static_pool_container_t * const self, u8 ** const data);
void staticPoolContainer_nextReadPos(static_pool_container_t * const self);

/* USER DEFINES CHECK ----------------------------------------------------------------------------------------------------------------------------------------
 */
#if ((STATIC_POOL_CONTAINER_COLUMNS - 1) & STATIC_POOL_CONTAINER_COLUMNS) || (STATIC_POOL_CONTAINER_COLUMNS == 0) || (STATIC_POOL_CONTAINER_COLUMNS > 16384) // columns buffer is power of 2 size check
    #error STATIC_Pool_CONTAINER_COLUMNS must be is power of 2
#endif //check if power of 2 STATIC_POOL_CONTAINER_COLUMNS

#if ((STATIC_POOL_CONTAINER_RAWS - 1) & STATIC_POOL_CONTAINER_RAWS) || (STATIC_POOL_CONTAINER_RAWS == 0) || (STATIC_POOL_CONTAINER_RAWS > 16384) // raws buffer is power of 2 size check
    #error STATIC_Pool_CONTAINER_RAWS must be is power of 2
#endif //check if power of 2 STATIC_POOL_CONTAINER_RAWS




#if (STATIC_POOL_CONTAINER_COLUMNS > 16384) // columns buffer if too long check
    #error STATIC_POOL_CONTAINER_COLUMNS is too long
#endif //check STATIC_POOL_CONTAINER_COLUMNS

#if (STATIC_POOL_CONTAINER_RAWS > 16384) // raws buffer if too long check
    #error STATIC_POOL_CONTAINER_RAWS is too long
#endif //check STATIC_POOL_CONTAINER_RAWS


#endif /* STATIC_POOL_CONTAINER_H */
