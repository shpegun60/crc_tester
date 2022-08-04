#include "rawparser_dma.h"
#include <stdio.h>
#include <stdlib.h>


#define BUFF_SIZE D_RAW_P_TX_BUF_SIZE
#define RANDOM_TEST_CNT 500000


static int receiveTransmittSimpleTest(RawParser_dma_t* desc, u8 * data, rawP_size_t size)
{
    int byteReceiveCompl = 0;
    int arrReceiveCompl = 0;

    RawParser_Frame_t* Txframe = RawParser_dma_shieldFrame(desc, data, size);

    if(Txframe == NULL) {
        return 1;
    }

    for(int i = 0; i < Txframe->size; ++i) {
        RawParser_dma_receiveByte(desc, Txframe->data[i]);
    }


    // ------------- one byte pushing test---------------------------------------
    RawParser_Frame_t* Rxframe = RawParser_dma_proceed(desc);

    if(Rxframe && Rxframe->size != 0) {
        byteReceiveCompl += cTypeStrnCmp(size, (c8*)data, (c8*)Rxframe->data);
    } else {
        byteReceiveCompl++;
    }

    Txframe->size = 0;



//    // ------------- all byte pushing test---------------------------------------
//    Txframe = RawParser_dma_shieldFrame(desc, data, size);

//    if(Txframe == NULL) {
//        return byteReceiveCompl + 1;
//    }

//    RawParser_dma_receiveArray(desc, Txframe->data, Txframe->size);

//    Rxframe = RawParser_dma_proceed(desc);
//    if(Rxframe && Rxframe->size != 0) {
//        arrReceiveCompl += cTypeStrnCmp(size, (c8*)data, (c8*)Rxframe->data);
//    } else {
//        arrReceiveCompl++;
//    }

    int last = 0;
    while (last < size) {
        RawParser_dma_proceed(desc);
        ++last;
    }

    return byteReceiveCompl + arrReceiveCompl;
}

static int receiveTransmittCollisionsTest(RawParser_dma_t* desc, u8 * data, rawP_size_t size)
{
    int byteCollisisons = 0;
    int arrCollisisons = 0;

    RawParser_Frame_t* Txframe = RawParser_dma_shieldFrame(desc, data, size);

    if(Txframe == NULL) {
        return 1;
    }

    reg lenBreak = rand() % size;

    // replace data (emulate real glitches)
    c8 b_rand = rand() % 256;
    while(Txframe->data[lenBreak] == b_rand) {
        b_rand = rand() % 256;
    }
    Txframe->data[lenBreak] = b_rand;


    for(int i = 0; i < Txframe->size; ++i) {
        RawParser_dma_receiveByte(desc, Txframe->data[i]);
    }


    // ------------- one byte pushing test---------------------------------------
    RawParser_Frame_t* Rxframe = RawParser_dma_proceed(desc);

    if(Rxframe && Rxframe->size != 0) {
        byteCollisisons++;
    }

    Txframe->size = 0;



    // ------------- all byte pushing test---------------------------------------
    Txframe = RawParser_dma_shieldFrame(desc, data, size);

    if(Txframe == NULL) {
        return 1;
    }

    lenBreak = rand() % size;

    b_rand = rand() % 256;
    while(Txframe->data[lenBreak] == b_rand) {
        b_rand = rand() % 256;
    }
    Txframe->data[lenBreak] = b_rand;


    RawParser_dma_receiveArray(desc, Txframe->data, Txframe->size);

    Rxframe = RawParser_dma_proceed(desc);
    if(Rxframe && Rxframe->size != 0) {
        arrCollisisons++;
    }


    int last = 0;
    while (last < size) {
        RawParser_dma_proceed(desc);
        ++last;
    }

    return byteCollisisons + arrCollisisons;




    return 0;
}

int rawParserDmaTest(unsigned int random_seed)
{
    int errorCounter = 0;
    int collisionCounter = 0;
    u8 * data = (u8*)calloc(BUFF_SIZE, sizeof(u8));

    for(u32 i = 0; i < BUFF_SIZE; ++i) {
        data[i] = i;
    }

    RawParser_dma_t * prot = rawParser_dma_new(0x1A);


    errorCounter += receiveTransmittSimpleTest(prot, data, 30);

    srand(random_seed); // use current time as seed for random generator

    // random rx/tx test----------------------------------------------------------
    for(int i = 0; i < RANDOM_TEST_CNT; ++i) {
        reg len = 0;

        while(len == 0) {
#ifdef D_RAW_P_TWO_BYTES_LEN_SUPPORT
            len = rand() % (D_RAW_P_TX_BUF_SIZE >> 1);
#else
            len = rand() % (D_RAW_P_MAX_ERROR_LEN - 5);
#endif /* D_RAW_P_TWO_BYTES_LEN_SUPPORT */
        }


        for(reg j = 0; j < len; ++j) {
            data[j] = rand() % 256;
        }
        errorCounter += receiveTransmittSimpleTest(prot, data, len);
        //collisionCounter+= receiveTransmittCollisionsTest(prot, data, len);
    }


    printf("\n----------RAW PARSER DMA FINISHED!!!-------------------------\nRAW_PARSER_DMA EXIT WITH ERROR: %d\n", errorCounter);
    printf("PACKET COUNTER: %d\n", RANDOM_TEST_CNT * 2);
    printf("WITH COLLISIONS: %d\n", collisionCounter);
    printf("PACKED CRC LOOSE percents: %f\n", (f32)((f32)collisionCounter / (f32)RANDOM_TEST_CNT) * 100.0f * 0.5f);

    free(data);
    printf("RawParser_dma deleted error: %d\n", rawParser_dma_delete(&prot));
    fflush(stdout);

    return errorCounter;
}





#undef BUFF_SIZE
#undef RANDOM_TEST_CNT
