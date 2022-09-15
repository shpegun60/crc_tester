#include "pull_container_test.h"

#ifndef PULL_CONTAINER_TEST_DISABLE
#include "pull_container.h"
#include "static_pull_container.h"
#include <stdlib.h>
#include <string.h>

// pull test global data
static u8 data[STATIC_PULL_CONTAINER_RAWS][STATIC_PULL_CONTAINER_COLUMNS];
static u16 size[STATIC_PULL_CONTAINER_RAWS];

static void initArr()
{
    for(reg i = 0; i < STATIC_PULL_CONTAINER_RAWS; ++i) {
        for(reg j = 0; j < STATIC_PULL_CONTAINER_COLUMNS; ++j) {
            data[i][j] = rand() & 0xFFU;
        }
        size[i] = rand() & (STATIC_PULL_CONTAINER_COLUMNS - 1);
    }
}

/* STATIC PULL CONTAINER TESTS -----------------------------------------------------------------------------------
 *
 */
// read from static pull and check
int pullContainerStatic_readCheck(static_pull_container_t * container)
{
    int counterNotValid = 0;
    for(reg i = 0; i < STATIC_PULL_CONTAINER_RAWS; ++i) {
        u8* rddata = NULL;
        u16 rdLen = staticPullContainer_readArr(container, &rddata);    // read metadata for read
        staticPullContainer_nextReadPos(container);                     // next read pos (if buffer not empty)

        if(i < (STATIC_PULL_CONTAINER_RAWS - 1)) {
            if(STATIC_PULL_CONTAINER_IS_EMPTY(container) || STATIC_PULL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        } else if(i == (STATIC_PULL_CONTAINER_RAWS - 1)) {
            if(!STATIC_PULL_CONTAINER_IS_EMPTY(container) || STATIC_PULL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        }

        if(rdLen != size[i] || rddata == NULL) {
            ++counterNotValid;
            continue;
        }

        counterNotValid += cTypeStrnCmp(rdLen, (c8*)&data[i][0], (c8*)rddata); // compleate data
    }
    staticPullContainer_nextReadPos(container); //generate warning
    return counterNotValid;
}

int testPullContainerStatic()
{
    int counterNotValid = 0;
    static_pull_container_t * container = staticPullContainer_new();
    if(container == NULL) {
        return 1;
    }
    initArr();

    /* test array writing function ------------------------------------------------------------------------------------------
     */
    // write to pull
    for(reg i = 0; i < STATIC_PULL_CONTAINER_RAWS; ++i) {
        if(i == 0) {
            if(!STATIC_PULL_CONTAINER_IS_EMPTY(container) || STATIC_PULL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        } else if(i < (STATIC_PULL_CONTAINER_RAWS - 1)) {
            if(STATIC_PULL_CONTAINER_IS_EMPTY(container) || STATIC_PULL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        }

        if(staticPullContainer_writeArr(container, &data[i][0], size[i]) != size[i]) { // write data to pull container
            ++counterNotValid;
        }

        if(i == (STATIC_PULL_CONTAINER_RAWS - 1)) {
            if(!STATIC_PULL_CONTAINER_IS_FULL(container) || STATIC_PULL_CONTAINER_IS_EMPTY(container)) {
                ++counterNotValid;
            }
        }
    }

    // read from pull
    counterNotValid += pullContainerStatic_readCheck(container);


    /* test writing from meta functions ------------------------------------------------------------------------------------------
     */
    initArr(); // refresh arrays

    for(reg i = 0; i < STATIC_PULL_CONTAINER_RAWS; ++i) {
        u8* wrdata = NULL;
        u16* wrsize = NULL;
        staticPullContainer_getWriteMeta(container, &wrdata, &wrsize);
        if(wrdata == NULL || wrsize == NULL) {
            ++counterNotValid;
            continue;
        }


        if(i == 0) {
            if(!STATIC_PULL_CONTAINER_IS_EMPTY(container) || STATIC_PULL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        } else if(i < (STATIC_PULL_CONTAINER_RAWS - 1)) {
            if(STATIC_PULL_CONTAINER_IS_EMPTY(container) || STATIC_PULL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        }


        memcpy(wrdata, &data[i][0], size[i]);   // copy to pull buffer
        *wrsize = size[i];                      // copy len to pull buffer
        staticPullContainer_nextWritePos(container);    // next pos

        if(i == (STATIC_PULL_CONTAINER_RAWS - 1)) {
            if(!STATIC_PULL_CONTAINER_IS_FULL(container) || STATIC_PULL_CONTAINER_IS_EMPTY(container)) {
                ++counterNotValid;
            }
        }
    }
    staticPullContainer_nextWritePos(container);    // generate warning

    // read from pull
    counterNotValid += pullContainerStatic_readCheck(container);



    counterNotValid += staticPullContainer_delete(&container);
    if(container != NULL) {
        ++counterNotValid;
    }
    return counterNotValid;
}

/* DYNAMIC PULL CONTAINER TESTS -----------------------------------------------------------------------------------
 *
 */

// read from static pull and check
int pullContainerDynamic_readCheck(pull_container_t * container)
{
    int counterNotValid = 0;
    for(reg i = 0; i < STATIC_PULL_CONTAINER_RAWS; ++i) {
        u8* rddata = NULL;
        u16 rdLen = pullContainer_readArr(container, &rddata);    // read metadata for read
        pullContainer_nextReadPos(container);                     // next read pos (if buffer not empty)

        if(i < (STATIC_PULL_CONTAINER_RAWS - 1)) {
            if(STATIC_PULL_CONTAINER_IS_EMPTY(container) || STATIC_PULL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        } else if(i == (STATIC_PULL_CONTAINER_RAWS - 1)) {
            if(!STATIC_PULL_CONTAINER_IS_EMPTY(container) || STATIC_PULL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        }

        if(rdLen != size[i] || rddata == NULL) {
            ++counterNotValid;
            continue;
        }

        counterNotValid += cTypeStrnCmp(rdLen, (c8*)&data[i][0], (c8*)rddata); // compleate data
    }
    pullContainer_nextReadPos(container); //generate warning
    return counterNotValid;
}


int testPullContainerDynamic()
{
    int counterNotValid = 0;
    pull_container_t* container = pullContainer_new(STATIC_PULL_CONTAINER_RAWS, STATIC_PULL_CONTAINER_COLUMNS);
    if(container == NULL) {
        return 1;
    }
    initArr();

    /* test array writing function ------------------------------------------------------------------------------------------
     */
    // write to pull
    for(reg i = 0; i < STATIC_PULL_CONTAINER_RAWS; ++i) {
        if(i == 0) {
            if(!STATIC_PULL_CONTAINER_IS_EMPTY(container) || STATIC_PULL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        } else if(i < (STATIC_PULL_CONTAINER_RAWS - 1)) {
            if(STATIC_PULL_CONTAINER_IS_EMPTY(container) || STATIC_PULL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        }

        if(pullContainer_writeArr(container, &data[i][0], size[i]) != size[i]) { // write data to pull container
            ++counterNotValid;
        }

        if(i == (STATIC_PULL_CONTAINER_RAWS - 1)) {
            if(!STATIC_PULL_CONTAINER_IS_FULL(container) || STATIC_PULL_CONTAINER_IS_EMPTY(container)) {
                ++counterNotValid;
            }
        }
    }

    // read from pull
    counterNotValid += pullContainerDynamic_readCheck(container);


    /* test writing from meta functions ------------------------------------------------------------------------------------------
     */
    initArr(); // refresh arrays

    for(reg i = 0; i < STATIC_PULL_CONTAINER_RAWS; ++i) {
        u8* wrdata = NULL;
        u16* wrsize = NULL;
        pullContainer_getWriteMeta(container, &wrdata, &wrsize);
        if(wrdata == NULL || wrsize == NULL) {
            ++counterNotValid;
            continue;
        }


        if(i == 0) {
            if(!STATIC_PULL_CONTAINER_IS_EMPTY(container) || STATIC_PULL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        } else if(i < (STATIC_PULL_CONTAINER_RAWS - 1)) {
            if(STATIC_PULL_CONTAINER_IS_EMPTY(container) || STATIC_PULL_CONTAINER_IS_FULL(container)) {
                ++counterNotValid;
            }
        }


        memcpy(wrdata, &data[i][0], size[i]);   // copy to pull buffer
        *wrsize = size[i];                      // copy len to pull buffer
        pullContainer_nextWritePos(container);  // next pos

        if(i == (STATIC_PULL_CONTAINER_RAWS - 1)) {
            if(!STATIC_PULL_CONTAINER_IS_FULL(container) || STATIC_PULL_CONTAINER_IS_EMPTY(container)) {
                ++counterNotValid;
            }
        }
    }
    pullContainer_nextWritePos(container);    // generate warning

    // read from pull
    counterNotValid += pullContainerDynamic_readCheck(container);




    counterNotValid += pullContainer_delete(&container, 1);
    if(container != NULL) {
        ++counterNotValid;
    }
    return counterNotValid;
}

int pullContainerTest(int randomSeed, int testCnt)
{
    int counterNotValid = 0;
    srand(randomSeed);

    ////////////////////////////////////////////////////////////////////

    while(testCnt--) {
        counterNotValid += testPullContainerStatic();
        counterNotValid += testPullContainerDynamic();
    }

    //////////////////////////////////////////////////////////////////
    return counterNotValid;
}

#endif /* PULL_CONTAINER_TEST_DISABLE */
