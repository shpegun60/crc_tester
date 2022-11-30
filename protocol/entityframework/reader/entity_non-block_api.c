#include "entity_non-block_api.h"


#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#ifdef USE_ENTITY_READ_SERVICE

#include "entity_packet.h"
#include "smart_assert.h"

typedef struct {
    u32 time            [ENTITY_READ_SYSTEM_BOARD_COUNT][ENTITY_READ_NONBLOCK_READ_PACKETS_QUEUE];
    u8  packCounter     [ENTITY_READ_SYSTEM_BOARD_COUNT];
    u8  len             [ENTITY_READ_SYSTEM_BOARD_COUNT][ENTITY_READ_NONBLOCK_READ_PACKETS_QUEUE];
    u32 entityTable     [ENTITY_READ_SYSTEM_BOARD_COUNT][ENTITY_READ_NONBLOCK_READ_PACKETS_QUEUE][ENTITY_READ_NONBLOCK_API_MTU];
    EntityReadParent_t* lastField;
} EntityNonBlockPacketTable_t;

EntityNonBlockPacketTable_t readNonBlockPackTable = {};


static int countReadFields(EntityReadParent_t* const field, PREPROCESSOR_CTX_TYPE(ctx))
{
    // gettting context
    EntityNonBlockReadLoopVariables_t *const var = ctx[0];
    reg *const packCnt = ctx[1];
    reg *const Wpos = ctx[2];

    // move to cash some values
    const u16 boardNumber = field->boardNumber;
    u8* const data = var->outBuffer[field->boardNumber];

    // check if board number is valid and read enable
    if((boardNumber < ENTITY_READ_SYSTEM_BOARD_COUNT) && field->readEnable) {




        ENTITY_DBG_ASSERT_BUF(((Wpos[boardNumber] + (ENTITIES_SIZEOF + ENTITY_FIELD_SIZEOF)) > var->outBufferSize), M_EMPTY, return 1, "countReadFields: field request size more than outBuffer");
        writeEntityFieldNumbersToBuf(field->entityNumber, field->fieldNumber, data, &Wpos[boardNumber]);
        ++(packCnt[boardNumber]);
    }

    readNonBlockPackTable.lastField = field;

    if(packCnt[boardNumber] == ENTITY_READ_NONBLOCK_API_MTU) {
        return 1;
    }
    return 0;
}


void entityNonBlockReadLoop(EntityNonBlockReadLoopVariables_t* const var)
{
#define NON_BLOCK_READ_TRANSACTION  0
#define NON_BLOCK_WRITE_TRANSACTION 1

    static u8 nonBlockLoopState = NON_BLOCK_WRITE_TRANSACTION;
    reg packCnt[ENTITY_READ_SYSTEM_BOARD_COUNT]         = {0};
    reg Wpos[ENTITY_READ_SYSTEM_BOARD_COUNT]            = {1};
    //int ssize = sizeof(EntityNonBlockPacketTable_t);

    M_Assert_Break((var == NULL || var->outBuffer == NULL || var->outBufferSize == 0 || var->size == NULL), M_EMPTY, return, "entityNonBlockReadLoop: No valid input!!!");

    switch(nonBlockLoopState) {

    case(NON_BLOCK_READ_TRANSACTION): {
        EntityReadPoolContainer_t* const readPool = &ersys.readPool;



        if(entityReadPool_foreach_startsAfter(readPool, readNonBlockPackTable.lastField, countReadFields, PREPROCESSOR_CTX_CAPTURE({var, &packCnt[0], &Wpos[0]}))) {
            readNonBlockPackTable.lastField = NULL;
        }

        // write sizes & head to buffers
        for(reg i = ENTITY_READ_SYSTEM_BOARD_COUNT; i--;) {
            if(packCnt[0] != 0) {
                var->outBuffer[i][0] = READ_SEVERAL_VALUES;
                *(var->size[i]) = Wpos[i];
            } else {
                *(var->size[i]) = 0;
            }
        }

        break;}

    case(NON_BLOCK_WRITE_TRANSACTION): {
        EntityWritePoolContainer_t* const writePool = &ersys.writePool; // move to cash pointer

        if(writePool->rdEmpty) {
            nonBlockLoopState = NON_BLOCK_READ_TRANSACTION;
            UNUSED(packCnt);
            UNUSED(Wpos);
            break;
        }

        while(!writePool->rdEmpty) {
            // move to cash values
            EntityReadParent_t* const parent = entityWritePoolContainer_readParent(writePool); // get pointer to parent;
            M_Assert_Break((parent == NULL), M_EMPTY, return, "entityNonBlockReadLoop: No valid parent!!!");
            const u16 boardNumber = parent->boardNumber;

            // check if board number is valid
            if(boardNumber < ENTITY_READ_SYSTEM_BOARD_COUNT) {
                u8* const data = var->outBuffer[boardNumber];

                // write data to out buffer
                ENTITY_DBG_ASSERT_BUF(((Wpos[boardNumber] + parent->size + (ENTITIES_SIZEOF + ENTITY_FIELD_SIZEOF)) > var->outBufferSize), M_EMPTY, return, "entityNonBlockReadLoop: field write size more than outBuffer");

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
            if(packCnt[0] != 0) {
                var->outBuffer[i][0] = WRITE_SEVERAL_VALUES;
                *(var->size[i]) = Wpos[i];
            } else {
                *(var->size[i]) = 0;
            }
        }

        // move to read transactions
        nonBlockLoopState = NON_BLOCK_READ_TRANSACTION;
        break;}

    }

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
