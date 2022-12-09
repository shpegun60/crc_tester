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

    M_Assert_BreakElseSaveCheck((self->boards_count > (*boardNumber)), {

                                    self->boards[(*boardNumber)].entities = calloc(entities_count, sizeof(EN_EntityReadNode_t));
                                    M_Assert_BreakSaveCheck((self->boards[(*boardNumber)].entities == NULL), M_EMPTY, {
                                        goto error;
                                    }, "entityReadPoolContainer_initBoard: No memory for allocation");

                                    self->boards[(*boardNumber)].entities_count = entities_count;

                                    for(TYPEOF_STRUCT(EntityInfo, entities_count) i = 0; i < entities_count; ++i) {
                                        M_Assert_BreakSaveCheck((fieldCountList[i] > MAX_NUBER_OF_FIELDS), M_EMPTY, {
                                            goto error;
                                        }, "entityReadPoolContainer_initBoard: field count more than maximum fields number");

                                        self->boards[(*boardNumber)].entities[i].fields = calloc(fieldCountList[i], sizeof(EntityReadParent_t*));

                                        M_Assert_BreakSaveCheck((self->boards[(*boardNumber)].entities[i].fields == NULL), M_EMPTY, {
                                            goto error;
                                        }, "entityReadPoolContainer_initBoard: No memory for allocation");
                                        self->boards[(*boardNumber)].entities[i].fields_count = fieldCountList[i];
                                    }

                                    ++(*boardNumber);

                                    return ENTITY_OK;

                                }, M_EMPTY, M_EMPTY, "entityReadPoolContainer_initBoard: too long board number: %d, allocated: %d", (*boardNumber), self->boards_count);

    error:
        entityReadPoolContainer_delete(self);
        return ENTITY_ERROR;
}



int entityReadPoolContainer_delete(EntityReadPoolContainer_t* self)
{
    M_Assert_BreakSaveCheck((self == NULL), M_EMPTY, return ENTITY_ERROR, "entityReadPoolContainer_delete: no valid data for delete");

    for(TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) board = 0; board < (self)->boards_count; ++board) {                                 // board`s counting
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

int entityReadPoolContainer_push(EntityReadPoolContainer_t* const self, EntityReadParent_t* const parent)
{
    M_Assert_BreakSaveCheck((self == NULL || parent == NULL), M_EMPTY, return ENTITY_ERROR, "entityReadPoolContainer_push: no valid input object");

    // move to cash all values --------------------------------------------------------------------------------
    const   EN_BoardReadNode_t*   const     board            = &self->boards[parent->boardNumber];
    const   EN_EntityReadNode_t*  const     entity           = &board->entities[parent->entityNumber];
    EntityReadParent_t** const              field            = &entity->fields[parent->fieldNumber];

    const TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count)    boards_count    = (self)->boards_count;
    const TYPEOF_STRUCT(EntityInfo, entities_count)                 entities_count  = board->entities_count;
    const TYPEOF_STRUCT(Entity, fields_count)                       fields_count    = entity->fields_count;

    // do logic --------------------------------------------------------------------------------
    M_Assert_BreakElseSaveCheck((parent->boardNumber < boards_count), {

                                    M_Assert_BreakElseSaveCheck((parent->entityNumber < entities_count), {

                                        M_Assert_BreakElseSaveCheck((parent->fieldNumber < fields_count), {

                                            *field = parent;
                                            return ENTITY_OK;

                                        }, M_EMPTY, M_EMPTY, "entityReadPoolContainer_push: no valid input fieldNumber: %d", parent->fieldNumber);

                                    }, M_EMPTY, M_EMPTY, "entityReadPoolContainer_push: no valid input entityNumber: %d", parent->entityNumber);

                                }, M_EMPTY, M_EMPTY, "entityReadPoolContainer_push: no valid input boardNumber: %d", parent->boardNumber);

    return ENTITY_ERROR;
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

    return NULL;
}

int entityReadPool_foreach(EntityReadPoolContainer_t* const self, int (* const predicate)(EntityReadParent_t* const field, void* ctx), void* const ctx)
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

int entityReadPool_foreach_startsAfter(EntityReadPoolContainer_t* const self, EntityReadParent_t* const startField, int (* const predicate)(EntityReadParent_t* const field, PREPROCESSOR_CTX_TYPE(ctx)), PREPROCESSOR_CTX_TYPE(ctx))
{
    M_Assert_Break((self == NULL) || (self->boards == NULL), M_EMPTY, return 0, "entityReadPool_foreach: no valid input object");
    M_Assert_BreakSaveCheck((predicate == NULL), M_EMPTY, return 0, "entityReadPool_foreach: no valid input predicate");

    TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count)    board_start       ;
    TYPEOF_STRUCT(EntityInfo, entities_count)                 entitity_start    ;
    TYPEOF_STRUCT(Entity, fields_count)                       field_start       ;

    // check if element is last or NULL, than start from head
    if(startField == NULL) {
        board_start         = 0;
        entitity_start      = 0;
        field_start         = 0;
    } else {
        board_start         = startField->boardNumber;
        entitity_start      = startField->entityNumber;
        field_start         = startField->fieldNumber;
    }


    const TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) boards_count = (self)->boards_count;

    for(TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) board = board_start; board < boards_count; ++board) {                     // board`s counting
        const   EN_BoardReadNode_t*   const   board_ptr                 = &self->boards[board];         // move to cash
        const TYPEOF_STRUCT(EntityInfo, entities_count) entities_count  = board_ptr->entities_count;    // move to cash

        for(TYPEOF_STRUCT(EntityInfo, entities_count) entity = entitity_start; entity < entities_count; ++entity) {                         // entities counting
            const   EN_EntityReadNode_t*  const   entity_ptr            = &board_ptr->entities[entity]; // move to cash
            const TYPEOF_STRUCT(Entity, fields_count) fields_count      = entity_ptr->fields_count;     // move to cash

            for(TYPEOF_STRUCT(Entity, fields_count) field = field_start; field < fields_count; ++field) {                                // field`s counting

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




//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
STATIC_FORCEINLINE int entityReadPool_foreach_board(EntityReadPoolContainer_t* const self, TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) board, EntityReadParent_t* const startField, int (* const predicate)(EntityReadParent_t* const field, PREPROCESSOR_CTX_TYPE(ctx)), PREPROCESSOR_CTX_TYPE(ctx))
{
    TYPEOF_STRUCT(EntityInfo, entities_count)                 entitity_start    ;
    TYPEOF_STRUCT(Entity, fields_count)                       field_start       ;

    // check if element is last or NULL, than start from head
    if(startField == NULL) {
        entitity_start      = 0;
        field_start         = 0;
    } else {
        entitity_start      = startField->entityNumber;
        field_start         = startField->fieldNumber + 1;
    }


    const   EN_BoardReadNode_t*   const   board_ptr                 = &self->boards[board];         // move to cash
    const TYPEOF_STRUCT(EntityInfo, entities_count) entities_count  = board_ptr->entities_count;    // move to cash

    for(TYPEOF_STRUCT(EntityInfo, entities_count) entity = entitity_start; entity < entities_count; ++entity) {                        // entities counting
        const   EN_EntityReadNode_t*  const   entity_ptr            = &board_ptr->entities[entity]; // move to cash
        const TYPEOF_STRUCT(Entity, fields_count) fields_count      = entity_ptr->fields_count;     // move to cash

        for(TYPEOF_STRUCT(Entity, fields_count) field = field_start; field < fields_count; ++field) {                                  // field`s counting

            EntityReadParent_t* const field_ptr = entity_ptr->fields[field];                        // move to cash

            if(field_ptr != NULL) {
                if(predicate(field_ptr, ctx)) {

                    if( (field == (fields_count - 1)) && (entity == (entities_count - 1)) && (board == (ENTITY_READ_SYSTEM_BOARD_COUNT - 1)) ) {
                        return 1;
                    } else {
                        return 0;
                    }

                }
            }

        }
    }

    return 1;
}


void entityReadPool_foreach_boards_separation(EntityReadPoolContainer_t* const self, EntityReadParent_t* startFields [ENTITY_READ_SYSTEM_BOARD_COUNT], int (* const predicate)(EntityReadParent_t* const field, PREPROCESSOR_CTX_TYPE(ctx)), PREPROCESSOR_CTX_TYPE(ctx))
{
    M_Assert_Break((self == NULL) || (self->boards == NULL) || (startFields == NULL), M_EMPTY, return, "entityReadPool_foreach: no valid input object");
    M_Assert_Break((ENTITY_READ_SYSTEM_BOARD_COUNT > self->boards_count), M_EMPTY, return, "entityReadPool_foreach: no valid input boards count");
    M_Assert_BreakSaveCheck((predicate == NULL), M_EMPTY, return, "entityReadPool_foreach: no valid input predicate");



    for(TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) board = 0; board < ENTITY_READ_SYSTEM_BOARD_COUNT; ++board) {     // board`s counting

        if(entityReadPool_foreach_board(self, board, startFields[board], predicate, ctx)) {
            startFields[board] = NULL;
        }

    }
}


#endif /* USE_ENTITY_READ_SERVICE */

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */
