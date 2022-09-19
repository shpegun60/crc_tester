#include "pool_container_test.h"

#ifndef POOL_CONTAINER_TEST_DISABLE
#include "pool_container.h"
#include "static_pool_container.h"
#include <stdlib.h>
#include <string.h>

// pool test global data
static u8 data[STATIC_POOL_CONTAINER_RAWS][STATIC_POOL_CONTAINER_COLUMNS];
static u16 size[STATIC_POOL_CONTAINER_RAWS];

static void initArr()
{
    for(reg i = 0; i < STATIC_POOL_CONTAINER_RAWS; ++i) {
        for(reg j = 0; j < STATIC_POOL_CONTAINER_COLUMNS; ++j) {
            data[i][j] = rand() & 0xFFU;
        }
        size[i] = rand() & (STATIC_POOL_CONTAINER_COLUMNS - 1);
    }
}

/* STATIC pool CONTAINER TESTS -----------------------------------------------------------------------------------
 *
 */
// read from static pool and check
int poolContainerStatic_readCheck(static_pool_container_t * container)
{
    int counterNotValid = 0;
    for(reg i = 0; i < STATIC_POOL_CONTAINER_RAWS; ++i) {
        u8* rddata = NULL;
        u16 rdLen = staticPoolContainer_readArr(container, &rddata);    // read metadata for read
        staticPoolContainer_nextReadPos(container);                     // next read pos (if buffer not empty)

        if(i < (STATIC_POOL_CONTAINER_RAWS - 1)) {
            if(STATIC_POOL_CONTAINER_IS_EMPTY(container) || STATIC_POOL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        } else if(i == (STATIC_POOL_CONTAINER_RAWS - 1)) {
            if(!STATIC_POOL_CONTAINER_IS_EMPTY(container) || STATIC_POOL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        }

        if(rdLen != size[i] || rddata == NULL) {
            ++counterNotValid;
            continue;
        }

        counterNotValid += cTypeStrnCmp(rdLen, (c8*)&data[i][0], (c8*)rddata); // compleate data
    }
    staticPoolContainer_nextReadPos(container); //generate warning
    return counterNotValid;
}

int testpoolContainerStatic()
{
    int counterNotValid = 0;
    static_pool_container_t * container = staticPoolContainer_new();
    if(container == NULL) {
        return 1;
    }
    initArr();

    /* test array writing function ------------------------------------------------------------------------------------------
     */
    // write to pool
    for(reg i = 0; i < STATIC_POOL_CONTAINER_RAWS; ++i) {
        if(i == 0) {
            if(!STATIC_POOL_CONTAINER_IS_EMPTY(container) || STATIC_POOL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        } else if(i < (STATIC_POOL_CONTAINER_RAWS - 1)) {
            if(STATIC_POOL_CONTAINER_IS_EMPTY(container) || STATIC_POOL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        }

        if(staticPoolContainer_writeArr(container, &data[i][0], size[i]) != size[i]) { // write data to pool container
            ++counterNotValid;
        }

        if(i == (STATIC_POOL_CONTAINER_RAWS - 1)) {
            if(!STATIC_POOL_CONTAINER_IS_FULL(container) || STATIC_POOL_CONTAINER_IS_EMPTY(container)) {
                ++counterNotValid;
            }
        }
    }

    // read from pool
    counterNotValid += poolContainerStatic_readCheck(container);


    /* test writing from meta functions ------------------------------------------------------------------------------------------
     */
    initArr(); // refresh arrays

    for(reg i = 0; i < STATIC_POOL_CONTAINER_RAWS; ++i) {
        u8* wrdata = NULL;
        u16* wrsize = NULL;
        staticPoolContainer_getWriteMeta(container, &wrdata, &wrsize);
        if(wrdata == NULL || wrsize == NULL) {
            ++counterNotValid;
            continue;
        }


        if(i == 0) {
            if(!STATIC_POOL_CONTAINER_IS_EMPTY(container) || STATIC_POOL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        } else if(i < (STATIC_POOL_CONTAINER_RAWS - 1)) {
            if(STATIC_POOL_CONTAINER_IS_EMPTY(container) || STATIC_POOL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        }


        memcpy(wrdata, &data[i][0], size[i]);   // copy to pool buffer
        *wrsize = size[i];                      // copy len to pool buffer
        staticPoolContainer_nextWritePos(container);    // next pos

        if(i == (STATIC_POOL_CONTAINER_RAWS - 1)) {
            if(!STATIC_POOL_CONTAINER_IS_FULL(container) || STATIC_POOL_CONTAINER_IS_EMPTY(container)) {
                ++counterNotValid;
            }
        }
    }
    staticPoolContainer_nextWritePos(container);    // generate warning

    // read from pool
    counterNotValid += poolContainerStatic_readCheck(container);



    counterNotValid += staticPoolContainer_delete(&container);
    if(container != NULL) {
        ++counterNotValid;
    }
    return counterNotValid;
}

/* DYNAMIC pool CONTAINER TESTS -----------------------------------------------------------------------------------
 *
 */

// read from static pool and check
int poolContainerDynamic_readCheck(pool_container_t * container)
{
    int counterNotValid = 0;
    for(reg i = 0; i < STATIC_POOL_CONTAINER_RAWS; ++i) {
        u8* rddata = NULL;
        u16 rdLen = poolContainer_readArr(container, &rddata);    // read metadata for read
        poolContainer_nextReadPos(container);                     // next read pos (if buffer not empty)

        if(i < (STATIC_POOL_CONTAINER_RAWS - 1)) {
            if(POOL_CONTAINER_IS_EMPTY(container) || POOL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        } else if(i == (STATIC_POOL_CONTAINER_RAWS - 1)) {
            if(!POOL_CONTAINER_IS_EMPTY(container) || POOL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        }

        if(rdLen != size[i] || rddata == NULL) {
            ++counterNotValid;
            continue;
        }

        counterNotValid += cTypeStrnCmp(rdLen, (c8*)&data[i][0], (c8*)rddata); // compleate data
    }
    poolContainer_nextReadPos(container); //generate warning
    return counterNotValid;
}


int testpoolContainerDynamic()
{
    int counterNotValid = 0;
    pool_container_t* container = poolContainer_new(STATIC_POOL_CONTAINER_RAWS, STATIC_POOL_CONTAINER_COLUMNS);
    if(container == NULL) {
        return 1;
    }
    initArr();

    /* test array writing function ------------------------------------------------------------------------------------------
     */
    // write to pool
    for(reg i = 0; i < STATIC_POOL_CONTAINER_RAWS; ++i) {
        if(i == 0) {
            if(!POOL_CONTAINER_IS_EMPTY(container) || POOL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        } else if(i < (STATIC_POOL_CONTAINER_RAWS - 1)) {
            if(POOL_CONTAINER_IS_EMPTY(container) || POOL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        }

        if(poolContainer_writeArr(container, &data[i][0], size[i]) != size[i]) { // write data to pool container
            ++counterNotValid;
        }

        if(i == (STATIC_POOL_CONTAINER_RAWS - 1)) {
            if(!POOL_CONTAINER_IS_FULL(container) || POOL_CONTAINER_IS_EMPTY(container)) {
                ++counterNotValid;
            }
        }
    }

    // read from pool
    counterNotValid += poolContainerDynamic_readCheck(container);


    /* test writing from meta functions ------------------------------------------------------------------------------------------
     */
    initArr(); // refresh arrays

    for(reg i = 0; i < STATIC_POOL_CONTAINER_RAWS; ++i) {
        u8* wrdata = NULL;
        u16* wrsize = NULL;
        poolContainer_getWriteMeta(container, &wrdata, &wrsize);
        if(wrdata == NULL || wrsize == NULL) {
            ++counterNotValid;
            continue;
        }


        if(i == 0) {
            if(!POOL_CONTAINER_IS_EMPTY(container) || POOL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        } else if(i < (STATIC_POOL_CONTAINER_RAWS - 1)) {
            if(POOL_CONTAINER_IS_EMPTY(container) || POOL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        }


        memcpy(wrdata, &data[i][0], size[i]);   // copy to pool buffer
        *wrsize = size[i];                      // copy len to pool buffer
        poolContainer_nextWritePos(container);  // next pos

        if(i == (STATIC_POOL_CONTAINER_RAWS - 1)) {
            if(!POOL_CONTAINER_IS_FULL(container) || POOL_CONTAINER_IS_EMPTY(container)) {
                ++counterNotValid;
            }
        }
    }
    poolContainer_nextWritePos(container);    // generate warning

    // read from pool
    counterNotValid += poolContainerDynamic_readCheck(container);




    counterNotValid += poolContainer_delete(&container, 1);
    if(container != NULL) {
        ++counterNotValid;
    }
    return counterNotValid;
}

int poolContainerTest(int randomSeed, int testCnt)
{
    int counterNotValid = 0;
    srand(randomSeed);

    ////////////////////////////////////////////////////////////////////

    while(testCnt--) {
        counterNotValid += testpoolContainerStatic();
        counterNotValid += testpoolContainerDynamic();
    }

    //////////////////////////////////////////////////////////////////
    return counterNotValid;
}

#endif /* POOL_CONTAINER_TEST_DISABLE */
