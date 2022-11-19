#include "entity_read_parent_pool.h"

#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#ifdef USE_ENTITY_READ_SERVICE
#include "smart_assert.h"
#include <stdlib.h>



// new / init entity read pool container
EntityReadPoolContainer_t* entityReadPoolContainer_newBoards(TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) boards_count)
{
    M_Assert_BreakSaveCheck((boards_count == 0), M_EMPTY, return NULL, "entityreadPool_new: No valid input");
    EntityReadPoolContainer_t * m = calloc(1, sizeof(EntityReadPoolContainer_t));
    M_Assert_BreakSaveCheck((m == NULL), M_EMPTY, return NULL, "entityReadPoolContainer_newBoards: No memory for allocation");

    entityReadPoolContainer_initBoards(m, boards_count);

    return m;
}

int entityReadPoolContainer_initBoards(EntityReadPoolContainer_t* self,
                                       TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) boards_count)
{
    M_Assert_BreakSaveCheck((self == NULL || boards_count == 0), M_EMPTY, return ENTITY_ERROR, "entityReadPoolContainer_initBoards: No valid input");

    // create boards
    self->boards = calloc(boards_count, sizeof(EN_BoardReadNode_t));
    M_Assert_BreakSaveCheck((self->boards == NULL), M_EMPTY, return ENTITY_ERROR, "entityReadPoolContainer_initBoards: No memory for allocation");
    self->boards_count = boards_count;

    return ENTITY_OK;
}

int entityReadPoolContainer_initBoard(EntityReadPoolContainer_t* self,
                                      TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count)* boardNumber, TYPEOF_STRUCT(EntityInfo, entities_count) entities_count, TYPEOF_STRUCT(Entity, fields_count)* fieldCountList, reg fieldCountListSize)
{
    M_Assert_BreakSaveCheck((self == NULL || boardNumber == NULL || entities_count == 0 || entities_count > MAX_NUBER_OF_ENTITIES
                             || fieldCountList == NULL || fieldCountListSize < entities_count), M_EMPTY, {
                                entityReadPoolContainer_delete(self);
                                return ENTITY_ERROR;
                            }, "entityReadPoolContainer_initBoard: No valid input");

    M_Assert_BreakSaveCheck((self->boards_count == 0 || self->boards == NULL), M_EMPTY, {
                                entityReadPoolContainer_delete(self);
                                return ENTITY_ERROR;
                            }, "entityReadPoolContainer_initBoard: invoke /entityReadPoolContainer_initBoards/ function before");

    M_Assert_BreakElseSaveCheck((self->boards_count > (*boardNumber)), {

                                    self->boards[(*boardNumber)].entities = calloc(entities_count, sizeof(EN_EntityReadNode_t));
                                    M_Assert_BreakSaveCheck((self->boards[(*boardNumber)].entities == NULL), M_EMPTY, {
                                        entityReadPoolContainer_delete(self);
                                        return ENTITY_ERROR;
                                    }, "entityReadPoolContainer_initBoard: No memory for allocation");

                                    self->boards[(*boardNumber)].entities_count = entities_count;

                                    for(TYPEOF_STRUCT(EntityInfo, entities_count) i = 0; i < entities_count; ++i) {
                                        M_Assert_BreakSaveCheck((fieldCountList[i] > MAX_NUBER_OF_FIELDS), M_EMPTY, {
                                            entityReadPoolContainer_delete(self);
                                            return ENTITY_ERROR;
                                        }, "entityReadPoolContainer_initBoard: field count more than maximum fields number");

                                        self->boards[(*boardNumber)].entities[i].fields = calloc(fieldCountList[i], sizeof(EntityReadParent_t*));

                                        M_Assert_BreakSaveCheck((self->boards[(*boardNumber)].entities[i].fields == NULL), M_EMPTY, {
                                            entityReadPoolContainer_delete(self);
                                            return ENTITY_ERROR;
                                        }, "entityReadPoolContainer_initBoard: No memory for allocation");
                                        self->boards[(*boardNumber)].entities[i].fields_count = fieldCountList[i];
                                    }

                                    ++(*boardNumber);

                                    return ENTITY_OK;

                                }, M_EMPTY, M_EMPTY, "entityReadPoolContainer_initBoard: too long board number: %d, allocated: %d", (*boardNumber), self->boards_count);

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

int entityReadPoolContainer_push(EntityReadPoolContainer_t* self, EntityReadParent_t* parent)
{
    M_Assert_BreakSaveCheck((self == NULL || parent == NULL), M_EMPTY, return ENTITY_ERROR, "entityReadPoolContainer_push: no valid input object");

    M_Assert_BreakElseSaveCheck((parent->boardNumber < self->boards_count), {

                                    M_Assert_BreakElseSaveCheck((parent->entityNumber < self->boards[parent->boardNumber].entities_count), {

                                        M_Assert_BreakElseSaveCheck((parent->fieldNumber < self->boards[parent->boardNumber].entities[parent->entityNumber].fields_count), {

                                            self->boards[parent->boardNumber].entities[parent->entityNumber].fields[parent->fieldNumber] = parent;
                                            return ENTITY_OK;

                                        }, M_EMPTY, M_EMPTY, "entityReadPoolContainer_push: no valid input fieldNumber: %d", parent->fieldNumber);

                                    }, M_EMPTY, M_EMPTY, "entityReadPoolContainer_push: no valid input entityNumber: %d", parent->entityNumber);

                                }, M_EMPTY, M_EMPTY, "entityReadPoolContainer_push: no valid input boardNumber: %d", parent->boardNumber);

    return ENTITY_ERROR;
}

EntityReadParent_t* entityReadPoolContainer_getParent(EntityReadPoolContainer_t* self,
                                                      TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) boardNumber, TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, TYPEOF_STRUCT(Entity, fields_count) fieldNumber)
{
    M_Assert_Break((self == NULL), M_EMPTY, return NULL, "entityReadPoolContainer_getParent: no valid input object");

    if(boardNumber < self->boards_count) {
        if(entityNumber < self->boards[boardNumber].entities_count) {
            if(fieldNumber < self->boards[boardNumber].entities[entityNumber].fields_count) {
                return self->boards[boardNumber].entities[entityNumber].fields[fieldNumber];
            }
        }
    }

    return NULL;
}

void entityReadPool_foreach(EntityReadPoolContainer_t* self, int (* predicate)(EntityReadParent_t* self, void* ctx), void* ctx)
{
    M_Assert_Break((self == NULL), M_EMPTY, return, "entityReadPool_foreach: no valid input object");
    M_Assert_BreakSaveCheck((predicate == NULL), M_EMPTY, return, "entityReadPool_foreach: no valid input predicate");

    for(TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) board = 0; board < (self)->boards_count; ++board) {                                 // board`s counting
        for(TYPEOF_STRUCT(EntityInfo, entities_count) entity = 0; entity < (self)->boards[board].entities_count; ++entity) {                       // entities counting
            for(TYPEOF_STRUCT(Entity, fields_count) field = 0; field < (self)->boards[board].entities[entity].fields_count; ++field) {             // field`s counting

                if((self)->boards[board].entities[entity].fields[field] != NULL) {
                    predicate((self)->boards[board].entities[entity].fields[field], ctx);
                }

            }
        }
    }
}

#endif /* USE_ENTITY_READ_SERVICE */

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */
