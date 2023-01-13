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

static EntityNonBlockPacketTable_t requestNonBlockPackTable; // in .bss

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

    for(reg board = ENTITY_READ_SYSTEM_BOARD_COUNT; board--;) {

        u8* const data = var->outBuffer[board];
        const reg outBufferSize = var->outBufferSize[board];

        // write request fields ---------------------------------------------------------------------------------------------------------------------------------------------------------------
        EntityWritePoolContainer_t* const writePool = &ersys.writePool[board]; // move to cash pointer

        while((!writePool->rdEmpty) && (writeRequestCnt[board] != ENTITY_WRITE_API_MTU)) {
            // move to cash values
            EntityReadParent_t* const parent = entityWritePoolContainer_readParent(writePool); // get pointer to parent;
            const reg parentSize = parent->size;
            M_Assert_Break((parent == NULL), M_EMPTY, return, "entityNonBlockReadLoop: No valid parent!!!");


            // write data to out buffer
            ENTITY_DBG_ASSERT_BUF(((Wpos[board] + parentSize + (ENTITIES_SIZEOF + ENTITY_FIELD_SIZEOF)) > outBufferSize),
                                  M_EMPTY, return, "entityNonBlockReadLoop: field write size more than outBuffer");

            writeEntityFieldNumbersToBuf(parent->entityNumber, parent->fieldNumber, data, &Wpos[board]);
            ENTITY_BYTE_CPY(parentSize, parent->data, &data[Wpos[board]]);
            Wpos[board] += parentSize;

            parent->onValueNotUpdated = 1;
            ++(writeRequestCnt[board]);
        }

        // write sizes & head to buffers
        if(writeRequestCnt[board] != 0) {
            data[0] = WRITE_SEVERAL_VALUES_GLUED;
            data[1] = writeRequestCnt[board];
            writeReqHead[board] = Wpos[board];
            Wpos[board] += 3;
        } else {
            Wpos[board] = 3;
        }

        // read request fields ---------------------------------------------------------------------------------------------------------------------------------------------------------------
        IntrusiveSList* const needReadList = &ersys.readPool.boards[board].needReadList;
        TYPEOF_STRUCT(EntityNonBlockPacketTable_t, packCounter[0]) requestTablePos = requestNonBlockPackTable.packCounter[board];

        if(!islist_is_empty(needReadList)) {
            const TYPEOF_STRUCT(EntityNonBlockPacketTable_t, packCounter[0]) requestTablePos_saved = requestTablePos;

            do {

                if((requestNonBlockPackTable.len[board][requestTablePos] == 0) || ((var->time - requestNonBlockPackTable.lastTime[board][requestTablePos]) > ENTITY_READ_NONBLOCK_MAX_TIME_REQUEST)) {
                    requestNonBlockPackTable.packCounter[board] = (requestTablePos + 1) & ENTITY_READ_NONBLOCK_MSK;
                    requestNonBlockPackTable.len[board][requestTablePos] = 0;

                    IntrusiveSListNode* readPos = requestNonBlockPackTable.lastNodes[board];

                    while(readRequestCnt[board] != ENTITY_READ_API_MTU) {

                        const NeedReadListNode_t* const readNode = islist_entry_of_position(readPos, NeedReadListNode_t, link);
                        EntityReadParent_t* const parent = readNode->field;
                        M_Assert_Break((parent == NULL), M_EMPTY, return, "entityNonBlockReadLoop: No valid parent!!!");

                        if(parent->readEnable) {
                            ENTITY_DBG_ASSERT_BUF(((Wpos[board] + (ENTITIES_SIZEOF + ENTITY_FIELD_SIZEOF)) > outBufferSize), M_EMPTY, return, "entityNonBlockReadLoop: field request size more than outBuffer");
                            writeEntityFieldNumbersToBuf(parent->entityNumber, parent->fieldNumber, data, &Wpos[board]);
                            requestNonBlockPackTable.entityTable[board][requestTablePos][readRequestCnt[board]] = parent;

                            ++readRequestCnt[board];
                        }

                        readPos = readPos->next;
                        if(readPos->next == NULL) {
                            readPos = needReadList;
                        }
                        requestNonBlockPackTable.lastNodes[board] = readPos;
                    }
                    break;
                }
                requestTablePos = (requestTablePos + 1) & ENTITY_READ_NONBLOCK_MSK;
            } while( requestTablePos != requestTablePos_saved );

        }

        // write sizes & head to buffers
        if(readRequestCnt[board] != 0) {

            data[writeReqHead[board]] = READ_SEVERAL_VALUES_GLUED;
            data[writeReqHead[board] + 1] = readRequestCnt[board];
            data[writeReqHead[board] + 2] = requestTablePos;
            *(var->size[board]) = Wpos[board];

            requestNonBlockPackTable.len[board][requestTablePos] = readRequestCnt[board];
            requestNonBlockPackTable.lastTime[board][requestTablePos] = var->time;
        } else {
            data[writeReqHead[board]++] = SEVERAL_VALUES_GLUED_END;
            *(var->size[board]) = writeReqHead[board];
        }
    }
}

void entityNonBlockReceivePacket(const u16 boardNumber, u8* const inBuffer, const reg inBufferSize)
{
    M_Assert_Break((inBuffer == NULL || inBufferSize < 2 || boardNumber > (ENTITY_READ_SYSTEM_BOARD_COUNT - 1)), M_EMPTY, return, "entityNonBlockReceivePacket: No valid input!!!");

    const u8 cmd = inBuffer[0];

    switch(cmd) {

    case ENTITY_MAIL_SERVICE_PACK: {
        TYPEOF_STRUCT(EntityInfo, entities_count)   entityNumber;
        TYPEOF_STRUCT(Entity, fields_count)         fieldNumber;
        EntityReadPoolContainer_t* const            readPool = &ersys.readPool;

        reg      Rpos = (2 + sizeof(u32));

        const u8 packetCnt = inBuffer[1];

        for(reg i = 0; i < packetCnt; ++i) {

            ENTITY_DBG_ASSERT_BUF(((Rpos + (ENTITIES_SIZEOF + ENTITY_FIELD_SIZEOF)) > inBufferSize), M_EMPTY, return, "entityNonBlockReceivePacket:read size more than input buffer");
            readEntityFieldNumbersfromBuf(&entityNumber, &fieldNumber, inBuffer, &Rpos);

            EntityReadParent_t* parent = entityReadPoolContainer_getParent(readPool, boardNumber, entityNumber, fieldNumber);
            M_Assert_Warning((parent == NULL), M_EMPTY, return, "entityNonBlockReceivePacket: No valid parent!!!");

            const reg parentSize = parent->size;
            ENTITY_DBG_ASSERT_BUF(((Rpos + parentSize) > inBufferSize), M_EMPTY, return, "entityNonBlockReceivePacket:read size more than input buffer");
            ENTITY_BYTE_CPY(parentSize, &inBuffer[Rpos], parent->data);
            Rpos += parentSize;
        }

        break;}

    case READ_SEVERAL_VALUES_GLUED :
    case WRITE_SEVERAL_VALUES_GLUED: {
        // move to cash
        reg      Rpos               = 3;
        const u8 requestLenRx       =  inBuffer[1];
        const u8 requestTablePosRx  = (inBuffer[2] & ENTITY_READ_NONBLOCK_MSK);
        const TYPEOF_STRUCT(EntityNonBlockPacketTable_t, len[0][0]) requestLenTable = requestNonBlockPackTable.len[boardNumber][requestTablePosRx];

        // do logic
        if( (requestLenRx != requestLenTable) || (requestLenRx == 0) || (requestLenTable == 0) ) {
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
