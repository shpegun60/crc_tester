#include "entity_read_parent_pool.h"

#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#ifdef USE_ENTITY_READ_SERVICE
#include "smart_assert.h"
#include <stdlib.h>

EntityReadPool_t* entityReadPool_new(TYPEOF_STRUCT(EntityInfo, entities_count) entities_count, TYPEOF_STRUCT(Entity, fields_count) *fieldCountList, reg fieldListSize)
{
    M_Assert_BreakSaveCheck((entities_count == 0 || fieldCountList == NULL), M_EMPTY, return NULL, "entityreadPool_new: No valid input");
    EntityReadPool_t * m = (EntityReadPool_t*)calloc(1, sizeof(EntityReadPool_t));
    M_Assert_BreakSaveCheck((m == NULL), M_EMPTY, return NULL, "entityreadPool_new: No memory for allocation");

    entityReadPool_init(m, entities_count, fieldCountList, fieldListSize);
    return m;
}

int entityReadPool_init(EntityReadPool_t* self, TYPEOF_STRUCT(EntityInfo, entities_count) entities_count, TYPEOF_STRUCT(Entity, fields_count) *fieldCountList, reg fieldListSize)
{
    M_Assert_BreakSaveCheck((entities_count == 0 || fieldCountList == NULL || self == NULL || (fieldListSize < entities_count)), M_EMPTY, return ENTITY_ERROR, "entityreadPool_init: No valid input");
    self->fields = (FieldReadPool_t *) calloc (entities_count, sizeof(FieldReadPool_t));

    M_Assert_BreakSaveCheck((self->fields == NULL), M_EMPTY, return ENTITY_ERROR, "entityreadPool_init: No memory for allocation");
    self->entities_count = entities_count;

    for(TYPEOF_STRUCT(EntityInfo, entities_count) i = 0; i < entities_count; ++i) {
        self->fields[i].pool = (EntityReadParent_t**)calloc (fieldCountList[i], sizeof(EntityReadParent_t*));
        M_Assert_BreakSaveCheck((self->fields == NULL), M_EMPTY, return ENTITY_ERROR, "entityreadPool_init: No memory for allocation");

        self->fields[i].fields_count = fieldCountList[i];
    }

    return ENTITY_OK;
}

int entityReadPool_delete(EntityReadPool_t** self)
{
    M_Assert_BreakSaveCheck((self == NULL), M_EMPTY, return ENTITY_ERROR, "entityreadPool_delete: no valid data for delete");

    for(TYPEOF_STRUCT(EntityInfo, entities_count) i = 0; i < (*self)->entities_count; ++i) {
        free((*self)->fields[i].pool);
        (*self)->fields[i].pool = NULL;
    }

    free((*self)->fields);
    (*self)->fields = NULL;

    free((*self));
    (*self) = NULL;
    return ENTITY_OK;
}

int entityReadPool_push(EntityReadPool_t* self, EntityReadParent_t* parent, TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, TYPEOF_STRUCT(Entity, fields_count) fieldNumber)
{
    M_Assert_BreakSaveCheck((self == NULL || parent == NULL), M_EMPTY, return ENTITY_ERROR, "entityReadPool_push: no valid input object");

    M_Assert_BreakElseSaveCheck((entityNumber < self->entities_count), {
                                    M_Assert_BreakElseSaveCheck((fieldNumber < self->fields[entityNumber].fields_count), {
                                        self->fields[entityNumber].pool[fieldNumber] = parent;
                                        return ENTITY_OK;
                                    }, M_EMPTY, M_EMPTY, "entityReadPool_push: no valid input fieldNumber: %d", fieldNumber);
                                }, M_EMPTY, M_EMPTY, "entityReadPool_push: no valid input entityNumber: %d", entityNumber);
    return ENTITY_ERROR;
}

EntityReadParent_t* entityReadPool_getParent(EntityReadPool_t* self, TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, TYPEOF_STRUCT(Entity, fields_count) fieldNumber)
{
    M_Assert_Break((self == NULL), M_EMPTY, return NULL, "entityReadPool_getParent: no valid input object");

    M_Assert_WarningElseSaveCheck((entityNumber < self->entities_count), {
                                    M_Assert_WarningElseSaveCheck((fieldNumber < self->fields[entityNumber].fields_count), {

                                        return self->fields[entityNumber].pool[fieldNumber];

                                    }, M_EMPTY, M_EMPTY, "entityReadPool_getParent: no valid input fieldNumber: %d", fieldNumber);
                                }, M_EMPTY, M_EMPTY, "entityReadPool_getParent: no valid input entityNumber: %d", entityNumber);
    return NULL;
}


#endif /* USE_ENTITY_READ_SERVICE */

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */
