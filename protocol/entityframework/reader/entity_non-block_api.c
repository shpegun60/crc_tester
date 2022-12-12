#include "entity_non-block_api.h"


#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#ifdef USE_ENTITY_READ_SERVICE

#include "IntrusiveLinkedList.h"
#include "entity_packet.h"
#include "smart_assert.h"


typedef struct {
    u32 lastTime                                    [ENTITY_READ_SYSTEM_BOARD_COUNT][ENTITY_READ_NONBLOCK_READ_PACKETS_QUEUE];
    u8  packCounter                                 [ENTITY_READ_SYSTEM_BOARD_COUNT];
    u8  len                                         [ENTITY_READ_SYSTEM_BOARD_COUNT][ENTITY_READ_NONBLOCK_READ_PACKETS_QUEUE];
    EntityReadParent_t* entityTable                 [ENTITY_READ_SYSTEM_BOARD_COUNT][ENTITY_READ_NONBLOCK_READ_PACKETS_QUEUE][ENTITY_READ_API_MTU];
    IntrusiveSListNode* lastNodes                   [ENTITY_READ_SYSTEM_BOARD_COUNT];
} EntityNonBlockPacketTable_t;

EntityNonBlockPacketTable_t requestNonBlockPackTable; // in .bss

void entityNonBlock_init(void)
{
    pointerInit(sizeof(requestNonBlockPackTable), (u8*)&requestNonBlockPackTable);

    for(reg i = 0; i < ENTITY_READ_SYSTEM_BOARD_COUNT; ++i) {
        requestNonBlockPackTable.lastNodes[i] = &ersys.readPool.boards[i].needReadList;
    }
}


void entityNonBlockReadLoop(EntityNonBlockReadLoopVariables_t* const var)
{
    reg writeRequestCnt[ENTITY_READ_SYSTEM_BOARD_COUNT]         = {0};
    reg readRequestCnt[ENTITY_READ_SYSTEM_BOARD_COUNT]          = {0};
    reg Wpos[ENTITY_READ_SYSTEM_BOARD_COUNT]                    = {2};
    reg writeReqHead[ENTITY_READ_SYSTEM_BOARD_COUNT]            = {0};

    M_Assert_Break((var == NULL || var->outBuffer == NULL || var->size == NULL), M_EMPTY, return, "entityNonBlockReadLoop: No valid input!!!");

    for(reg i = ENTITY_READ_SYSTEM_BOARD_COUNT; i--;) {

        u8* const data = var->outBuffer[i];

        // write request fields ---------------------------------------------------------------------------------------------------------------------------------------------------------------
        EntityWritePoolContainer_t* const writePool = &ersys.writePool[i]; // move to cash pointer

        while((!writePool->rdEmpty) && (writeRequestCnt[i] != ENTITY_WRITE_API_MTU)) {
            // move to cash values
            EntityReadParent_t* const parent = entityWritePoolContainer_readParent(writePool); // get pointer to parent;
            const reg parentSize = parent->size;
            M_Assert_Break((parent == NULL), M_EMPTY, return, "entityNonBlockReadLoop: No valid parent!!!");


            // write data to out buffer
            ENTITY_DBG_ASSERT_BUF(((Wpos[i] + parentSize + (ENTITIES_SIZEOF + ENTITY_FIELD_SIZEOF)) > var->outBufferSize[i]),
                                  M_EMPTY, return, "entityNonBlockReadLoop: field write size more than outBuffer");

            writeEntityFieldNumbersToBuf(parent->entityNumber, parent->fieldNumber, data, &Wpos[i]);
            ENTITY_BYTE_CPY(parentSize, parent->data, &data[Wpos[i]]);
            Wpos[i] += parentSize;

            parent->onValueNotUpdated = 1;
            ++(writeRequestCnt[i]);
        }

        // write sizes & head to buffers
        if(writeRequestCnt[i] != 0) {
            var->outBuffer[i][0] = WRITE_SEVERAL_VALUES_GLUED;
            var->outBuffer[i][1] = writeRequestCnt[i];
            writeReqHead[i] = Wpos[i];
            Wpos[i] += 3;
        } else {
            Wpos[i] = 3;
        }

        // read request fields ---------------------------------------------------------------------------------------------------------------------------------------------------------------
        IntrusiveSList* const needReadList = &ersys.readPool.boards[i].needReadList;
        TYPEOF_STRUCT(EntityNonBlockPacketTable_t, packCounter[0]) requestTablePos = requestNonBlockPackTable.packCounter[i];

        if(!islist_is_empty(needReadList)) {
            const TYPEOF_STRUCT(EntityNonBlockPacketTable_t, packCounter[0]) requestTablePos_saved = requestTablePos;

            do {

                if((requestNonBlockPackTable.len[i][requestTablePos] == 0) || ((var->time - requestNonBlockPackTable.lastTime[i][requestTablePos]) > ENTITY_READ_NONBLOCK_MAX_TIME_REQUEST)) {
                    requestNonBlockPackTable.packCounter[i] = (requestTablePos + 1) & ENTITY_READ_NONBLOCK_MSK;
                    requestNonBlockPackTable.len[i][requestTablePos] = 0;

                    IntrusiveSListNode* readPos = requestNonBlockPackTable.lastNodes[i];

                    while(readRequestCnt[i] != ENTITY_READ_API_MTU) {

                        const NeedReadListNode_t* const readNode = islist_entry_of_position(readPos, NeedReadListNode_t, link);
                        EntityReadParent_t* const parent = readNode->field;
                        M_Assert_Break((parent == NULL), M_EMPTY, return, "entityNonBlockReadLoop: No valid parent!!!");

                        ENTITY_DBG_ASSERT_BUF(((Wpos[i] + (ENTITIES_SIZEOF + ENTITY_FIELD_SIZEOF)) > var->outBufferSize[i]), M_EMPTY, return, "entityNonBlockReadLoop: field request size more than outBuffer");
                        writeEntityFieldNumbersToBuf(parent->entityNumber, parent->fieldNumber, data, &Wpos[i]);
                        requestNonBlockPackTable.entityTable[i][requestTablePos][readRequestCnt[i]] = parent;

                        ++readRequestCnt[i];

                        readPos = readPos->next;
                        if(readPos->next == NULL) {
                            readPos = needReadList;
                        }
                        requestNonBlockPackTable.lastNodes[i] = readPos;
                    }
                    break;
                }
                requestTablePos = (requestTablePos + 1) & ENTITY_READ_NONBLOCK_MSK;
            } while( requestTablePos != requestTablePos_saved );

        }

        // write sizes & head to buffers
        if(readRequestCnt[i] != 0) {

            var->outBuffer[i][writeReqHead[i]] = READ_SEVERAL_VALUES_GLUED;
            var->outBuffer[i][writeReqHead[i] + 1] = readRequestCnt[i];
            var->outBuffer[i][writeReqHead[i] + 2] = requestTablePos;
            *(var->size[i]) = Wpos[i];

            requestNonBlockPackTable.len[i][requestTablePos] = readRequestCnt[i];
            requestNonBlockPackTable.lastTime[i][requestTablePos] = var->time;
        } else {
            *(var->size[i]) = writeReqHead[i];
        }
    }
}

void entityNonBlockReceivePacket(const u16 boardNumber, u8* const inBuffer, const reg inBufferSize)
{
    M_Assert_Break((inBuffer == NULL || inBufferSize < 2 || boardNumber < ENTITY_READ_SYSTEM_BOARD_COUNT), M_EMPTY, return, "entityNonBlockReceivePacket: No valid input!!!");

    const u8 cmd = inBuffer[0];

    switch(cmd) {

    case ENTITY_MAIL_SERVICE_PACK: {

        break;}

    case READ_SEVERAL_VALUES_GLUED: {
        // move to cash
        reg      Rpos               = 3;
        const u8 requestLenRx       =  inBuffer[1];
        const u8 requestTablePosRx  = (inBuffer[2] & ENTITY_READ_NONBLOCK_MSK);
        const TYPEOF_STRUCT(EntityNonBlockPacketTable_t, len[0][0]) requestLenTable = requestNonBlockPackTable.len[boardNumber][requestTablePosRx];

        // do logic
        if((requestLenRx == 0) || (requestLenTable == 0) || (requestLenRx != requestLenTable)) {
            return;
        }

        u8 requestCnt = 0;
        while(requestCnt != requestLenRx) {
            EntityReadParent_t* const parent = requestNonBlockPackTable.entityTable[boardNumber][requestTablePosRx][requestCnt];
            M_Assert_Break((parent == NULL), M_EMPTY, return, "entityNonBlockReceivePacket: No valid parent!!!");
            const reg parentSize = parent->size;

            ENTITY_DBG_ASSERT_BUF(((Rpos + parentSize) > inBufferSize), M_EMPTY, return, "entityNonBlockReceivePacket: parent size more than input buffer");
            ENTITY_BYTE_CPY(parentSize, &inBuffer[Rpos], parent->data);
            Rpos += parentSize;

            ++requestCnt;
        }

        requestNonBlockPackTable.len[boardNumber][requestTablePosRx] = 0;
        break;}

    default:
        break;
    }

    UNUSED(inBufferSize);
}


#endif /* USE_ENTITY_READ_SERVICE */

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */
