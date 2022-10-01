#include "entity_manager.h"

#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#include "smart_assert.h"
#include <stdlib.h>
#include <stdio.h>

EntityInfo entityInfo = {0, 0, NULLPTR(TYPEOF_STRUCT(EntityInfo, entities))};  // global variable entities for user projects (adds sizeof(EntityInfo) to .data section)

/*
 * ****************************************************************************************************
 * entity new/delete functions
 * ****************************************************************************************************
 */

// malloc new entities pointers
int newEntities(int nomberOfEntities)
{
    clearEntities(); // clear all entities

    M_Assert_BreakSaveCheck((nomberOfEntities > MAX_NUBER_OF_ENTITIES), M_EMPTY, return ENTITY_ERROR, "newEntities: No valid input");

    entityInfo.entities = (Entity**) calloc(nomberOfEntities, sizeof(Entity*)); // allocation & initialization all entity
    M_Assert_BreakSaveCheck((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities))), M_EMPTY, return ENTITY_ERROR, "newEntities: No memory for allocation Entity pointers");

    entityInfo.allocated_entity_pointers = nomberOfEntities;
    return ENTITY_OK;
}

void clearEntities(void)
{
    for(int i = 0; i < entityInfo.entities_count; ++i) {
        if(entityInfo.entities[i]->isHeap) {
            free(entityInfo.entities[i]->pointer);
        }
        free(entityInfo.entities[i]->fields);
        free(entityInfo.entities[i]);
        entityInfo.entities[i] = NULLPTR(TYPEOF_STRUCT(EntityInfo, entities[i]));
    }

    free(entityInfo.entities);
    entityInfo.entities_count = 0;
    entityInfo.allocated_entity_pointers = 0;
    entityInfo.entities = NULLPTR(TYPEOF_STRUCT(EntityInfo, entities));
}

void clearEntitityFields(int entityNumber)
{
    M_Assert_BreakSaveCheck((entityInfo.entities_count > entityInfo.allocated_entity_pointers), M_EMPTY, return, "initEntity: No free entity for initialization!!!");
    M_Assert_BreakSaveCheck((entityInfo.entities[entityNumber] == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities[entityNumber]))), M_EMPTY, return, "initEntity: entity number: %d is null", entityNumber);

    if(entityInfo.entities[entityNumber]->isHeap) {
        free(entityInfo.entities[entityNumber]->pointer);
    }

    free(entityInfo.entities[entityNumber]->fields);
    free(entityInfo.entities[entityNumber]);
    entityInfo.entities[entityNumber] = NULLPTR(TYPEOF_STRUCT(EntityInfo, entities[entityNumber]));
}

int initEntity(int NumberOfFields, reg pointerSize, char * descr, b isCustomSpace, b isHeap, void* arg)
{
    M_Assert_BreakSaveCheck((entityInfo.entities_count == entityInfo.allocated_entity_pointers), M_EMPTY, return ENTITY_ERROR, "initEntity: There is no free entity for initialization!!!");

    entityInfo.entities[entityInfo.entities_count] = (Entity*) calloc(1, sizeof(Entity));
    M_Assert_BreakSaveCheck(entityInfo.entities[entityInfo.entities_count] == NULLPTR(Entity*), M_EMPTY, return ENTITY_ERROR, "newEntities: No memory for allocation Entitity");

    // copy entity description-------------------------------------------------------------------------------------------------------------------------------------------------------------------
    MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DECRIPTION_SIZE, (u8 *)descr, (u8 *)entityInfo.entities[entityInfo.entities_count]->descr);


    // allocation pointer to data----------------------------------------------------------------------------------------------------------------------------------------------------------------
    if(isCustomSpace) {
        entityInfo.entities[entityInfo.entities_count]->isHeap  = isHeap;
        entityInfo.entities[entityInfo.entities_count]->pointer = arg;
    } else {
        entityInfo.entities[entityInfo.entities_count]->isHeap  = 1;
        entityInfo.entities[entityInfo.entities_count]->pointer = calloc(1, pointerSize);
    }

    M_Assert_BreakSaveCheck((entityInfo.entities[entityInfo.entities_count]->pointer == NULL), {
        clearEntitityFields(entityInfo.entities_count);
    }, return ENTITY_ERROR, "newEntities: No memory for allocation Entitity or invalid user input space");


    entityInfo.entities[entityInfo.entities_count]->fields = (EntityField*) calloc(NumberOfFields, sizeof(EntityField));
    if(entityInfo.entities[entityInfo.entities_count]->fields == NULLPTR(TYPEOF_STRUCT(Entity, fields))) {
        clearEntitityFields(entityInfo.entities_count);
        return ENTITY_ERROR;
    }
    entityInfo.entities[entityInfo.entities_count]->fields_count = NumberOfFields;

    char str[(ENTITY_DECRIPTION_SIZE << 1) + 1] = {};
    for(int i = 0; i < NumberOfFields; ++i) {


#ifdef USE_ENTITY_CALLBACKS

#   ifdef USE_ENTITY_READ_CALLBACK
        entityInfo.entities[entityInfo.entities_count]->fields[i].rdCallback.entityCallback = NULLPTR(TYPEOF_STRUCT(entityCallbackContainer, entityCallback));
        entityInfo.entities[entityInfo.entities_count]->fields[i].rdCallback.context        = NULLPTR(TYPEOF_STRUCT(entityCallbackContainer, context));
#   endif /* USE_ENTITY_READ_CALLBACK */

#   ifdef USE_ENTITY_WRITE_CALLBACK
        entityInfo.entities[entityInfo.entities_count]->fields[i].wrCallback.entityCallback = NULLPTR(TYPEOF_STRUCT(entityCallbackContainer, entityCallback));
        entityInfo.entities[entityInfo.entities_count]->fields[i].wrCallback.context        = NULLPTR(TYPEOF_STRUCT(entityCallbackContainer, context));
#   endif /* USE_ENTITY_WRITE_CALLBACK */

#endif /* USE_ENTITY_CALLBACKS */

        sprintf(str, "%d", i);
        entityInfo.entities[entityInfo.entities_count]->fields[i].bitFlags = 0;
        entityInfo.entities[entityInfo.entities_count]->fields[i].shift = 0;
        entityInfo.entities[entityInfo.entities_count]->fields[i].type = VOID_TYPE;
        MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DECRIPTION_SIZE, (u8 *)str, (u8 *)entityInfo.entities[entityInfo.entities_count]->fields[i].descr);
    }

    ++entityInfo.entities_count;
    return (entityInfo.entities_count - 1U);
}


/*
 * ****************************************************************************************************
 * help functions
 * ***************************************************************************************************
 */



#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */
