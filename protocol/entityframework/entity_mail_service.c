#include "entity_mail_service.h"


#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#ifdef USE_ENTITY_MAIL_SERVICE

#include "entity_packet.h"
#include "smart_assert.h"


typedef struct {
    TYPEOF_STRUCT(EntityInfo, entities_count)   entityNumber;
    TYPEOF_STRUCT(Entity, fields_count)         fieldNumber;

    u32 lastUpdateTimeMs;
    u32 updateTimeMs;

    IntrusiveSListNode link;
} EntityMailNode_t;


EntityMailService_t* entityMailService_new(void)
{
    EntityMailService_t* m = (EntityMailService_t*) calloc(1, sizeof(EntityMailService_t)); // allocation & initialization all entity mail service
    M_Assert_BreakSaveCheck(m == NULLPTR(EntityMailService_t*), M_EMPTY, return NULL, "entityMailService_new: No memory for allocation Entity mail service");

    entityMailService_init(m);

    return m;
}

int entityMailService_init(EntityMailService_t* const self)
{
    M_Assert_BreakSaveCheck(self == NULLPTR(EntityMailService_t*), M_EMPTY, return ENTITY_ERROR, "entityMailService_init: no valid input object");

    islist_init(&self->head);
    self->mailDis = 1;
    self->lastStream = &self->head;

    return ENTITY_OK;
}

static IntrusiveSListNode *find_TheSameLetter(IntrusiveSList* const head,
                                              const TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber,
                                              const TYPEOF_STRUCT(Entity, fields_count) fieldNumber)
{
    islist_for_each(position, head) {
        EntityMailNode_t *letter = islist_entry_of_position(position, EntityMailNode_t, link);
        if ((letter->entityNumber == entityNumber) && (letter->fieldNumber == fieldNumber)) {
            return position;
        }
    }
    return NULLPTR(IntrusiveSListNode*);
}

int EntityMailService_subscribe(EntityMailService_t* const self,
                                const TYPEOF_STRUCT(EntityInfo, entities_count)   entityNumber,
                                const TYPEOF_STRUCT(Entity, fields_count)         fieldNumber, u32 updateTimeMs, u32 time)
{
    M_Assert_Break(self == NULLPTR(EntityMailService_t*), M_EMPTY, return ENTITY_ERROR, "EntityMailService_subscribe: no valid input object");
    M_Assert_WarningSaveCheck(entityPositionIsExists(entityNumber, fieldNumber) != ENTITY_OK, M_EMPTY, return ENTITY_ERROR, "EntityMailService_subscribe: subscribing entity is not exists, entity: %d, field: %d", entityNumber, fieldNumber);

    IntrusiveSListNode * same_position = find_TheSameLetter(&self->head, entityNumber, fieldNumber);

    if(same_position == NULLPTR(IntrusiveSListNode*)) {

        EntityMailNode_t* letter = (EntityMailNode_t *) malloc(sizeof(EntityMailNode_t));
        M_Assert_BreakSaveCheck(letter == NULLPTR(EntityMailNode_t*), M_EMPTY, return ENTITY_ERROR, "EntityMailService_subscribe: No memory for allocation letter");

        // check the minimum timings
        if(updateTimeMs < ENTITY_MAIL_MINIMUM_TIME_SENDING_MS) {
            updateTimeMs = ENTITY_MAIL_MINIMUM_TIME_SENDING_MS;
        }

        letter->entityNumber        = entityNumber;
        letter->fieldNumber         = fieldNumber;
        letter->updateTimeMs        = updateTimeMs;
        letter->lastUpdateTimeMs    = time;

        islist_init(&(letter->link));
        islist_insert_back(&self->head, &(letter->link));
    } else {
        EntityMailNode_t *letter        = islist_entry_of_position(same_position, EntityMailNode_t, link);
        letter->updateTimeMs            = updateTimeMs;
        letter->lastUpdateTimeMs        = time;
    }
    return ENTITY_OK;
}

int EntityMailService_unsubscribe(EntityMailService_t* const self,
                                  const TYPEOF_STRUCT(EntityInfo, entities_count)   entityNumber,
                                  const TYPEOF_STRUCT(Entity, fields_count)         fieldNumber)
{
    M_Assert_Break(self == NULLPTR(EntityMailService_t*), M_EMPTY, return ENTITY_ERROR, "EntityMailService_unsubscribe: no valid input object");

    IntrusiveSListNode * position = find_TheSameLetter(&self->head, entityNumber, fieldNumber);

    if(position != NULLPTR(IntrusiveSListNode*)) {
        EntityMailNode_t *letter   = islist_entry_of_position(position, EntityMailNode_t, link);
        islist_remove_back(position);
        free(letter);
        return ENTITY_OK;
    } else {
        return ENTITY_ERROR;
    }
}


void EntityMailService_getStream(EntityMailService_t* const self, u32 time,
                                 u8* const outputBuffer, reg* const size, const reg maxOutBufferSize)
{
    M_Assert_Break(self == NULLPTR(EntityMailService_t*) || (maxOutBufferSize < (2 + sizeof(time))) , M_EMPTY, return, "EntityMailService_getStream: no valid input object");

    if(self->mailDis || islist_is_empty(&self->head)) {
        (*size) = 0;
        return;
    }

    u8 packetCnt = 0;
    reg Wpos = (2 + sizeof(time));
    IntrusiveSListNode *position = self->lastStream;

    while((packetCnt != ENTITY_MAIL_MTU) && (position->next != NULL)) {

        EntityMailNode_t* const letter = islist_entry_of_position(position, EntityMailNode_t, link);

        if((time - letter->lastUpdateTimeMs) > letter->updateTimeMs) {
            // read field to out buffer
            const TYPEOF_STRUCT(EntityInfo, entities_count)   entityNumber = letter->entityNumber;                      // move to cash (read-only parameter)
            const TYPEOF_STRUCT(Entity, fields_count)         fieldNumber = letter->fieldNumber;                        // move to cash (read-only parameter)

            const Entity*       const entity    = entityInfo.entities[entityNumber];                                    // move to cash (read-only parameter)
            const EntityField*  const field     = &entity->fields[fieldNumber];                                         // move to cash (read-only parameter)

            const reg typeLen = getMYCTypeLen(field->type);
            void* const data_ptr = (entity->pointer + field->shift);

            ENTITY_DBG_ASSERT_BUF(((Wpos + typeLen + (ENTITIES_SIZEOF + ENTITY_FIELD_SIZEOF)) > maxOutBufferSize), M_EMPTY, return, "EntityMailService_getStream: field read size more than buffer");

            writeEntityFieldNumbersToBuf(entityNumber, fieldNumber, outputBuffer, &Wpos);

            proceedReadEntity(field->bitFlags, data_ptr, &outputBuffer[Wpos], typeLen);
            Wpos += typeLen;

            letter->lastUpdateTimeMs = time;
            ++packetCnt;
        }

        position = position->next;
        self->lastStream = position;
    }

    // move to head if end of container
    if(position->next == NULL) {
        self->lastStream = &self->head;
    }

    if(packetCnt != 0) {
        // write entity cnt
        outputBuffer[0] = ENTITY_MAIL_SERVICE_PACK;
        outputBuffer[1] = packetCnt;
        // write time
        ENTITY_BYTE_CPY(sizeof(time), (u8*)&time, &outputBuffer[2]);
        (*size) = Wpos;
    } else {
        (*size) = 0;
    }
}

#endif /* USE_ENTITY_MAIL_SERVICE */

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */

