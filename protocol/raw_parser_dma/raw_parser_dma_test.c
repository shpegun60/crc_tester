#include "rawparser_dma.h"
#include <stdio.h>
#include <stdlib.h>


#define BUFF_SIZE D_RAW_P_TX_BUF_SIZE



static int receiveTransmittSimpleTest(RawParser_dma_t* desc, u8 * data, rawP_size_t size)
{
    int byteReceiveCompl = 0;
    int arrReceiveCompl = 0;

    RawParser_Frame_t* Txframe = RawParser_dma_shieldFrame(desc, data, size);

    printf("\nRawParser_dma send msg:----------------------------\n");
    for(int i = 0; i < Txframe->size; ++i) {
        printf("%02x.", Txframe->data[i]);

        RawParser_dma_receiveByte(desc, Txframe->data[i]);
    }
    printf("\n");


    // ------------- one byte pushing test---------------------------------------
    RawParser_Frame_t* Rxframe = RawParser_dma_proceed(desc);

    if(Rxframe->size != 0) {
        printf("\nRawParser_dma receive BYTE msg:----------------------------\n");
        for(int i = 0; i < Rxframe->size; ++i) {
            printf("%02x.", Rxframe->data[i]);
        }
        printf("\n");
    }

    byteReceiveCompl += cTypeStrnCmp(size, (c8*)data, (c8*)Rxframe->data);
    printf("\nreceiveTransmittCopyTest BYTE error: %d\n", byteReceiveCompl);



    // ------------- all byte pushing test---------------------------------------
    Txframe = RawParser_dma_shieldFrame(desc, data, size);

    RawParser_dma_receiveArray(desc, Txframe->data, Txframe->size);

    Rxframe = RawParser_dma_proceed(desc);
    if(Rxframe->size != 0) {
        printf("\nRawParser_dma receive ARRAY msg:----------------------------\n");
        for(int i = 0; i < Rxframe->size; ++i) {
            printf("%02x.", Rxframe->data[i]);
        }
        printf("\n");
    }

    arrReceiveCompl += cTypeStrnCmp(size, (c8*)data, (c8*)Rxframe->data);
    printf("\nreceiveTransmittCopyTest ARRAY error: %d\n", arrReceiveCompl);


    int last = 0;
    while (last < size) {
        RawParser_dma_proceed(desc);
        ++last;
    }

    return byteReceiveCompl + arrReceiveCompl;
}

//static int receiveTransmittOptimTest(RawParser_dma_t* desc, u8 * data, rawP_size_t size)
//{

//}

int rawParserDmaTest()
{
    u8 * data = (u8*)calloc(BUFF_SIZE, sizeof(u8));

    for(u32 i = 0; i < BUFF_SIZE; ++i) {
        data[i] = i;
    }

    RawParser_dma_t * prot = rawParser_dma_new(0x1A);


    receiveTransmittSimpleTest(prot, data, 30);



    free(data);
    printf("RawParser_dma deleted succesful: %d\n", rawParser_dma_delete(&prot));
    fflush(stdout);

    return 0;

}






#undef BUFF_SIZE
