#ifndef ENTITY_READ_PARENT_POOL_H
#define ENTITY_READ_PARENT_POOL_H

#include "entity_port.h"

#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#ifdef USE_ENTITY_READ_SERVICE

#include "entity_read_base.h"

/// declarating container nodes
typedef struct {
    TYPEOF_STRUCT(Entity, fields_count) fields_count;
    EntityReadParent_t** fields;
} EN_EntityReadNode_t;

typedef struct {
    TYPEOF_STRUCT(EntityInfo, entities_count) entities_count;
    EN_EntityReadNode_t* entities;
} EN_BoardReadNode_t;

/// declarating root of container
typedef struct {
    u16 boards_count;
    EN_BoardReadNode_t* boards;
} EntityReadPoolContainer_t;


// new / init entity read pool container
EntityReadPoolContainer_t* entityReadPoolContainer_newBoards(const TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) boards_count);

int entityReadPoolContainer_initBoards(EntityReadPoolContainer_t* const self,
                                       const TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) boards_count);

int entityReadPoolContainer_initBoard(EntityReadPoolContainer_t* const self,
                                      TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count)* const boardNumber, const TYPEOF_STRUCT(EntityInfo, entities_count) entities_count, TYPEOF_STRUCT(Entity, fields_count)* const fieldCountList, const reg fieldCountListSize);

int entityReadPoolContainer_delete(EntityReadPoolContainer_t* self);


// main logics
int entityReadPoolContainer_push(EntityReadPoolContainer_t* const self, EntityReadParent_t* const parent);

EntityReadParent_t* entityReadPoolContainer_getParent(EntityReadPoolContainer_t* const self,
                                                      const TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) boardNumber, const TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, const TYPEOF_STRUCT(Entity, fields_count) fieldNumber);

void entityReadPool_foreach(EntityReadPoolContainer_t* const self, int (* const predicate)(EntityReadParent_t* field, void* ctx), void* const ctx);



#endif /* USE_ENTITY_READ_SERVICE */

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */

#endif /* ENTITY_READ_PARENT_POOL_H */
