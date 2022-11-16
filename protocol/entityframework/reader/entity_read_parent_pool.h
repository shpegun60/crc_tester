#ifndef ENTITY_READ_PARENT_POOL_H
#define ENTITY_READ_PARENT_POOL_H

#include "entity_port.h"

#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#ifdef USE_ENTITY_READ_SERVICE

#include "entity_read_base.h"

typedef struct {
    TYPEOF_STRUCT(Entity, fields_count) fields_count;
    EntityReadParent_t ** pool;
} FieldReadPool_t;

typedef struct {
    TYPEOF_STRUCT(EntityInfo, entities_count) entities_count;
    FieldReadPool_t * fields;
} EntityReadPool_t;

EntityReadPool_t* entityReadPool_new(TYPEOF_STRUCT(EntityInfo, entities_count) entities_count, TYPEOF_STRUCT(Entity, fields_count) *fieldCountList, reg fieldListSize);
int entityReadPool_init(EntityReadPool_t* self, TYPEOF_STRUCT(EntityInfo, entities_count) entities_count, TYPEOF_STRUCT(Entity, fields_count) *fieldCountList, reg fieldListSize);
int entityReadPool_delete(EntityReadPool_t** self);


int entityReadPool_push(EntityReadPool_t* self, EntityReadParent_t* parent, TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, TYPEOF_STRUCT(Entity, fields_count) fieldNumber);
EntityReadParent_t* entityReadPool_getParent(EntityReadPool_t* self, TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, TYPEOF_STRUCT(Entity, fields_count) fieldNumber);

#endif /* USE_ENTITY_READ_SERVICE */

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */

#endif // ENTITY_READ_PARENT_POOL_H
