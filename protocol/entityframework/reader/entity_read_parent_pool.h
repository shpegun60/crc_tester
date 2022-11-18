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


/// new / init entity read pool container
EntityReadPoolContainer_t* entityReadPoolContainer_newBoards(TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) boards_count);

int entityReadPoolContainer_initBoards(EntityReadPoolContainer_t* self,
                                       TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) boards_count);

/// init board by boardNumber, entities_count, fieldCountList
int entityReadPoolContainer_initBoard(EntityReadPoolContainer_t* self,
                                      TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count)* boardNumber, TYPEOF_STRUCT(EntityInfo, entities_count) entities_count, TYPEOF_STRUCT(Entity, fields_count)* fieldCountList, reg fieldCountListSize);

int entityReadPoolContainer_delete(EntityReadPoolContainer_t* self);


/// push / counting functions
int entityReadPoolContainer_push(EntityReadPoolContainer_t* self, EntityReadParent_t* parent);

EntityReadParent_t* entityReadPoolContainer_getParent(EntityReadPoolContainer_t* self,
                                                      TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) boardNumber, TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, TYPEOF_STRUCT(Entity, fields_count) fieldNumber);

void entityReadPool_foreach(EntityReadPoolContainer_t* self, int (* predicate)(EntityReadParent_t* self, void* ctx), void* ctx);

#endif /* USE_ENTITY_READ_SERVICE */

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */

#endif // ENTITY_READ_PARENT_POOL_H
