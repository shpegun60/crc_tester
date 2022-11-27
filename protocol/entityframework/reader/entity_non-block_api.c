#include "entity_non-block_api.h"


#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#ifdef USE_ENTITY_READ_SERVICE
#include "entity_packet.h"
#include "smart_assert.h"


void entityNonBlockReadLoop(u8* const outBuffer[ENTITY_READ_SYSTEM_BOARD_COUNT], reg* const size[ENTITY_READ_SYSTEM_BOARD_COUNT], const reg outBufferSize, void* const user_ctx)
{
#define NON_BLOCK_READ_TRANSACTION 0
#define NON_BLOCK_WRITE_TRANSACTION 1

    static u8 nonBlockLoopState = NON_BLOCK_WRITE_TRANSACTION;

    M_Assert_Break((outBuffer == NULL || outBufferSize == 0), M_EMPTY, return, "entityNonBlockReadLoop: No valid input!!!");

    switch(nonBlockLoopState) {

    case(NON_BLOCK_READ_TRANSACTION): {

        break;}

    case(NON_BLOCK_WRITE_TRANSACTION): {
        reg packCnt[ENTITY_READ_SYSTEM_BOARD_COUNT]         = {0};
        reg Wpos[ENTITY_READ_SYSTEM_BOARD_COUNT]            = {1};
        EntityWritePoolContainer_t* const writePool = &ersys.writePool; // move to cash pointer

        if(writePool->rdEmpty) {
            nonBlockLoopState = NON_BLOCK_READ_TRANSACTION;
            UNUSED(packCnt);
            UNUSED(Wpos);
            break;
        }

        while(!writePool->rdEmpty) {
            // move to cash values
            EntityReadParent_t* const parent = entityWritePoolContainer_readParent(writePool); // get pointer to parrent;
            const u16 boardNumber = parent->boardNumber;

            // check if board number is valid
            if(boardNumber < ENTITY_READ_SYSTEM_BOARD_COUNT) {
                u8* const data = outBuffer[boardNumber];

                // write data to out buffer
                ENTITY_DBG_ASSERT_BUF(((Wpos[boardNumber] + parent->size + (ENTITIES_SIZEOF + ENTITY_FIELD_SIZEOF)) > outBufferSize), M_EMPTY, return, "entityNonBlockReadLoop: field write size more than outBuffer");

                writeEntityFieldNumbersToBuf(parent->entityNumber, parent->fieldNumber, data, &Wpos[boardNumber]);
                ENTITY_BYTE_CPY(parent->size, parent->data, &data[Wpos[boardNumber]]);
                Wpos[boardNumber] += parent->size;

                ++(packCnt[boardNumber]);
            }
            parent->onValueNotUpdated = 1;

            if(packCnt[boardNumber] == ENTITY_READ_NONBLOCK_API_MTU) {
                break;
            }
        }

        // write sizes & head to buffers
        for(reg i = ENTITY_READ_SYSTEM_BOARD_COUNT; i--;) {
            if(packCnt[i] != 0) {
                outBuffer[i][0] = WRITE_SEVERAL_VALUES;
                *(size[i]) = Wpos[i];
            } else {
                *(size[i]) = 0;
            }
        }

        // move to read transactions
        nonBlockLoopState = NON_BLOCK_READ_TRANSACTION;
        break;}

    }

    UNUSED(outBufferSize);
#undef NON_BLOCK_READ_TRANSACTION
#undef NON_BLOCK_WRITE_TRANSACTION
}

void entityNonBlockReceivePacket(u8* const inBuffer, const reg inBufferSize)
{
    M_Assert_Break((inBuffer == NULL || inBufferSize == 0), M_EMPTY, return, "entityNonBlockReceivePacket: No valid input!!!");

    const u8 cmd = inBuffer[0];

    switch(cmd) {

    case ENTITY_MAIL_SERVICE_PACK: {



        break;}

    default:
        break;
    }
}


#endif /* USE_ENTITY_READ_SERVICE */

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */
