#include "entity_read_parent_pool.h"


#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#ifdef USE_ENTITY_READ_SERVICE
#include "entity_read_system.h"
#include "smart_assert.h"
#include <stdlib.h>



// new / init entity read pool container
EntityReadPoolContainer_t* entityReadPoolContainer_newBoards(const TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) boards_count)
{
    M_Assert_BreakSaveCheck((boards_count == 0), M_EMPTY, return NULL, "entityreadPool_new: No valid input");
    EntityReadPoolContainer_t * m = calloc(1, sizeof(EntityReadPoolContainer_t));
    M_Assert_BreakSaveCheck((m == NULL), M_EMPTY, return NULL, "entityReadPoolContainer_newBoards: No memory for allocation");

    entityReadPoolContainer_initBoards(m, boards_count);

    return m;
}

int entityReadPoolContainer_initBoards(EntityReadPoolContainer_t* const self,
                                       const TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) boards_count)
{
    M_Assert_BreakSaveCheck((self == NULL || boards_count == 0), M_EMPTY, return ENTITY_ERROR, "entityReadPoolContainer_initBoards: No valid input");

    // create boards
    self->boards = calloc(boards_count, sizeof(EN_BoardReadNode_t));
    M_Assert_BreakSaveCheck((self->boards == NULL), M_EMPTY, return ENTITY_ERROR, "entityReadPoolContainer_initBoards: No memory for allocation");
    self->boards_count = boards_count;

    // init only read list
    for(TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) i = 0; i < boards_count; ++i) {
        islist_init(&self->boards[i].needReadList);
    }

    return ENTITY_OK;
}

int entityReadPoolContainer_initBoard(EntityReadPoolContainer_t* const self,
                                      TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count)* const boardNumber, const TYPEOF_STRUCT(EntityInfo, entities_count) entities_count, TYPEOF_STRUCT(Entity, fields_count)* const fieldCountList, const reg fieldCountListSize)
{
    M_Assert_BreakSaveCheck((self == NULL || boardNumber == NULL || entities_count == 0 || entities_count > MAX_NUBER_OF_ENTITIES
                             || fieldCountList == NULL || fieldCountListSize < entities_count), M_EMPTY, {
                                goto error;
                            }, "entityReadPoolContainer_initBoard: No valid input");

    M_Assert_BreakSaveCheck((self->boards_count == 0 || self->boards == NULL), M_EMPTY, {
                                goto error;
                            }, "entityReadPoolContainer_initBoard: invoke /entityReadPoolContainer_initBoards/ function before");

    // move to cash constants pointers and values
    TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) boardNumberInternal = (*boardNumber);
    EN_BoardReadNode_t*   const             board            = &self->boards[boardNumberInternal];

    M_Assert_BreakElseSaveCheck((self->boards_count > boardNumberInternal), {

                                    board->entities = calloc(entities_count, sizeof(EN_EntityReadNode_t));
                                    M_Assert_BreakSaveCheck((board->entities == NULL), M_EMPTY, {
                                        goto error;
                                    }, "entityReadPoolContainer_initBoard: No memory for allocation");

                                    board->entities_count = entities_count;

                                    for(TYPEOF_STRUCT(EntityInfo, entities_count) i = 0; i < entities_count; ++i) {
                                        M_Assert_BreakSaveCheck((fieldCountList[i] > MAX_NUBER_OF_FIELDS), M_EMPTY, {
                                            goto error;
                                        }, "entityReadPoolContainer_initBoard: field count more than maximum fields number");

                                        EN_EntityReadNode_t*  const     entity           = &board->entities[i];

                                        entity->fields = calloc(fieldCountList[i], sizeof(EntityReadParent_t*));

                                        M_Assert_BreakSaveCheck((entity->fields == NULL), M_EMPTY, {
                                            goto error;
                                        }, "entityReadPoolContainer_initBoard: No memory for allocation");
                                        entity->fields_count = fieldCountList[i];
                                    }

                                    ++boardNumberInternal;
                                    (*boardNumber) = boardNumberInternal;

                                    return ENTITY_OK;

                                }, M_EMPTY, M_EMPTY, "entityReadPoolContainer_initBoard: too long board number: %d, allocated: %d", (*boardNumber), self->boards_count);

    return ENTITY_ERROR;

    error:
        entityReadPoolContainer_delete(self);
        return ENTITY_ERROR;
}



int entityReadPoolContainer_delete(EntityReadPoolContainer_t* self)
{
    M_Assert_BreakSaveCheck((self == NULL), M_EMPTY, return ENTITY_ERROR, "entityReadPoolContainer_delete: no valid data for delete");

    for(TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) board = 0; board < (self)->boards_count; ++board) {                                 // board`s counting

       islist_destroy(&self->boards[board].needReadList, NeedReadListNode_t, link, free);

        for(TYPEOF_STRUCT(EntityInfo, entities_count) entity = 0; entity < (self)->boards[board].entities_count; ++entity) {                       // entities counting
            free((self)->boards[board].entities[entity].fields); // clear all fields
            (self)->boards[board].entities[entity].fields = NULL;
        }
        free((self)->boards[board].entities); // clear all entities
        (self)->boards[board].entities = NULL;
    }

    free((self)->boards); // clear all boards
    (self)->boards = NULL;
    (self)->boards_count = 0;

    return ENTITY_OK;
}

static IntrusiveSListNode *find_TheSameParent(IntrusiveSList* const head, EntityReadParent_t* const parent)
{
    islist_for_each(position, head) {
        const NeedReadListNode_t* const readNode = islist_entry_of_position(position, NeedReadListNode_t, link);
        const EntityReadParent_t* const finded_parrent = readNode->field;

        if (finded_parrent && (finded_parrent->boardNumber == parent->boardNumber)
                && (finded_parrent->entityNumber == parent->entityNumber)
                && (finded_parrent->fieldNumber == parent->fieldNumber)) {
            return position;
        }
    }
    return NULLPTR(IntrusiveSListNode*);
}

int entityReadPoolContainer_push(EntityReadPoolContainer_t* const self, EntityReadParent_t* const parent)
{
    M_Assert_BreakSaveCheck((self == NULL || parent == NULL), M_EMPTY, return ENTITY_ERROR, "entityReadPoolContainer_push: no valid input object");

    // move to cash all values --------------------------------------------------------------------------------
    EN_BoardReadNode_t*   const             board            = &self->boards[parent->boardNumber];
    const   EN_EntityReadNode_t*  const     entity           = &board->entities[parent->entityNumber];
    EntityReadParent_t** const              field            = &entity->fields[parent->fieldNumber];

    const TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count)    boards_count    = (self)->boards_count;
    const TYPEOF_STRUCT(EntityInfo, entities_count)                 entities_count  = board->entities_count;
    const TYPEOF_STRUCT(Entity, fields_count)                       fields_count    = entity->fields_count;

    // do logic --------------------------------------------------------------------------------
    M_Assert_BreakElseSaveCheck((parent->boardNumber < boards_count), {

                                    M_Assert_BreakElseSaveCheck((parent->entityNumber < entities_count), {

                                        M_Assert_BreakElseSaveCheck((parent->fieldNumber < fields_count), {

                                            (*field) = parent;

                                            if(parent->readEnable) {
                                                const IntrusiveSListNode * const same_position = find_TheSameParent(&board->needReadList, parent);

                                                if(same_position == NULLPTR(IntrusiveSListNode*)) {
                                                    NeedReadListNode_t* needReadNode = (NeedReadListNode_t *) malloc(sizeof(NeedReadListNode_t));
                                                    M_Assert_BreakSaveCheck(needReadNode == NULLPTR(NeedReadListNode_t*), M_EMPTY, return ENTITY_ERROR, "entityReadPoolContainer_push: No memory for allocation letter");

                                                    needReadNode->field = parent;
                                                    islist_init(&(needReadNode->link));
                                                    islist_insert_back(&board->needReadList, &(needReadNode->link));
                                                }
                                            }

                                            return ENTITY_OK;

                                        }, M_EMPTY, M_EMPTY, "entityReadPoolContainer_push: no valid input fieldNumber: %d", parent->fieldNumber);

                                    }, M_EMPTY, M_EMPTY, "entityReadPoolContainer_push: no valid input entityNumber: %d", parent->entityNumber);

                                }, M_EMPTY, M_EMPTY, "entityReadPoolContainer_push: no valid input boardNumber: %d", parent->boardNumber);

    return ENTITY_ERROR;
}

int entityReadPoolContainer_setReadEnable(EntityReadPoolContainer_t* const self, EntityReadParent_t* const parent)
{
    M_Assert_BreakSaveCheck((self == NULL || parent == NULL), M_EMPTY, return ENTITY_ERROR, "entityReadPoolContainer_setReadEnable: no valid input object");

    parent->readEnable = 1;
    return entityReadPoolContainer_push(self, parent);
}

int entityReadPoolContainer_setReadDisable(EntityReadPoolContainer_t* const self, EntityReadParent_t* const parent)
{
    M_Assert_BreakSaveCheck((self == NULL || parent == NULL), M_EMPTY, return ENTITY_ERROR, "entityReadPoolContainer_setReadDisable: no valid input object");

    // move to cash all values --------------------------------------------------------------------------------
    EN_BoardReadNode_t*   const             board            = &self->boards[parent->boardNumber];

    // do logic --------------------------------------------------------------------------------

    parent->readEnable = 0;
    IntrusiveSListNode * const same_position = find_TheSameParent(&board->needReadList, parent);

    if(same_position != NULLPTR(IntrusiveSListNode*)) {
        NeedReadListNode_t *readNode   = islist_entry_of_position(same_position, NeedReadListNode_t, link);
        islist_remove_back(same_position);
        free(readNode);
    }

    return ENTITY_OK;
}



EntityReadParent_t* entityReadPoolContainer_getParent(EntityReadPoolContainer_t* const self,
                                                      const TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) boardNumber, const TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, const TYPEOF_STRUCT(Entity, fields_count) fieldNumber)
{
    M_Assert_Break((self == NULL), M_EMPTY, return NULL, "entityReadPoolContainer_getParent: no valid input object");

    // move to cash all values --------------------------------------------------------------------------------
    const   EN_BoardReadNode_t*   const     board             = &self->boards[boardNumber];
    const   EN_EntityReadNode_t*  const     entity            = &board->entities[entityNumber];
    EntityReadParent_t*   const             field             = entity->fields[fieldNumber];

    const TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count)    boards_count    = (self)->boards_count;
    const TYPEOF_STRUCT(EntityInfo, entities_count)                 entities_count  = board->entities_count;
    const TYPEOF_STRUCT(Entity, fields_count)                       fields_count    = entity->fields_count;

    // do logic -----------------------------------------------------------------------------------------------
    if((boardNumber < boards_count) && (entityNumber < entities_count) && (fieldNumber < fields_count)) {
        return field;
    }

    return NULLPTR(EntityReadParent_t*);
}

int entityReadPool_foreach(EntityReadPoolContainer_t* const self,
                           int (* const predicate)(EntityReadParent_t* const field, PREPROCESSOR_CTX_TYPE(ctx)), PREPROCESSOR_CTX_TYPE(ctx))
{
    M_Assert_Break((self == NULL) || (self->boards == NULL), M_EMPTY, return 0, "entityReadPool_foreach: no valid input object");
    M_Assert_BreakSaveCheck((predicate == NULL), M_EMPTY, return 0, "entityReadPool_foreach: no valid input predicate");

    const TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) boards_count = (self)->boards_count;

    for(TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) board = 0; board != boards_count; ++board) {                     // board`s counting
        const   EN_BoardReadNode_t*   const   board_ptr                 = &self->boards[board];         // move to cash
        const TYPEOF_STRUCT(EntityInfo, entities_count) entities_count  = board_ptr->entities_count;    // move to cash

        for(TYPEOF_STRUCT(EntityInfo, entities_count) entity = 0; entity != entities_count; ++entity) {                         // entities counting
            const   EN_EntityReadNode_t*  const   entity_ptr            = &board_ptr->entities[entity]; // move to cash
            const TYPEOF_STRUCT(Entity, fields_count) fields_count      = entity_ptr->fields_count;     // move to cash

            for(TYPEOF_STRUCT(Entity, fields_count) field = 0; field != fields_count; ++field) {                                // field`s counting

                EntityReadParent_t* const field_ptr = entity_ptr->fields[field];                        // move to cash

                if(field_ptr != NULL) {
                    if(predicate(field_ptr, ctx)) {

                        if( (field == (fields_count - 1)) && (entity == (entities_count - 1)) && (board == (boards_count - 1)) ) {
                            return 1;
                        } else {
                            return 0;
                        }

                    }
                }

            }
        }
    }

    return 1;
}

#endif /* USE_ENTITY_READ_SERVICE */

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */
