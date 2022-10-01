#include "entity_manager.h"

#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#include "smart_assert.h"
#include <stdlib.h>
#include <stdio.h>

EntityInfo entityInfo = {0, 0, NULLPTR(TYPEOF_STRUCT(EntityInfo, entities))};  // global variable entities for user projects (adds sizeof(EntityInfo) to .data section)

/*
 * **********************************************************************************************************************************
 * entity new/delete functions
 * **********************************************************************************************************************************
 */

// delete all entities and deallocation all memory
void deleteEntities(void)
{
    for(int i = 0; i < entityInfo.entities_count; ++i) {
        deleteEntitityFields(i);
    }

    free(entityInfo.entities);
    entityInfo.entities_count = 0;
    entityInfo.allocated_entity_pointers = 0;
    entityInfo.entities = NULLPTR(TYPEOF_STRUCT(EntityInfo, entities));
}

// delete some entity
void deleteEntitityFields(int entityNumber)
{
    M_Assert_BreakSaveCheck((entityNumber > entityInfo.entities_count), M_EMPTY, return, "deleteEntitityFields: No entity for delete!!!");
    M_Assert_BreakSaveCheck((entityInfo.entities[entityNumber] == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities[entityNumber]))), M_EMPTY, return, "initEntity: entity number: %d is null", entityNumber);

    if(entityInfo.entities[entityNumber]->isHeap) {
        free(entityInfo.entities[entityNumber]->pointer);
        entityInfo.entities[entityNumber]->pointer = NULL;
    }

    free(entityInfo.entities[entityNumber]->fields);
    entityInfo.entities[entityNumber]->fields = NULLPTR(TYPEOF_STRUCT(EntityInfo, entities[entityNumber]->fields));

    free(entityInfo.entities[entityNumber]);
    entityInfo.entities[entityNumber] = NULLPTR(TYPEOF_STRUCT(EntityInfo, entities[entityNumber]));
}

// allocation new entities pointers
int newEntities(int nomberOfEntities)
{
    deleteEntities(); // delete all entities

    M_Assert_BreakSaveCheck((nomberOfEntities > MAX_NUBER_OF_ENTITIES), M_EMPTY, return ENTITY_ERROR, "newEntities: No valid input");

    entityInfo.entities = (Entity**) calloc(nomberOfEntities, sizeof(Entity*)); // allocation & initialization all entity pointers
    M_Assert_BreakSaveCheck((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities))), M_EMPTY, return ENTITY_ERROR, "newEntities: No memory for allocation Entity pointers");

    entityInfo.allocated_entity_pointers = nomberOfEntities;
    return ENTITY_OK;
}


// allocation entitites pointer & fields
int initEntity(int NumberOfFields, reg pointerSize, char * descr, b isCustomSpace, b isHeap, void* arg)
{
    M_Assert_BreakSaveCheck((entityInfo.entities_count == entityInfo.allocated_entity_pointers), M_EMPTY, return ENTITY_ERROR, "initEntity: There is no free entity for initialization!!!, use /newEntities/ function before");

    // allocation pointer to Entity-------------------------------------------------------------------------------------------------------------------------------------------------------------
    entityInfo.entities[entityInfo.entities_count] = (Entity*) calloc(1, sizeof(Entity));
    M_Assert_BreakSaveCheck(entityInfo.entities[entityInfo.entities_count] == NULLPTR(Entity*), M_EMPTY, return ENTITY_ERROR, "newEntities: No memory for allocation Entitity");

    // copy entity description-------------------------------------------------------------------------------------------------------------------------------------------------------------------
    MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DECRIPTION_SIZE, (u8 *)descr, (u8 *)entityInfo.entities[entityInfo.entities_count]->descr);


    // allocation or initialization pointer to data----------------------------------------------------------------------------------------------------------------------------------------------
    if(isCustomSpace) {
        entityInfo.entities[entityInfo.entities_count]->isHeap  = isHeap;
        entityInfo.entities[entityInfo.entities_count]->pointer = arg;
    } else {
        entityInfo.entities[entityInfo.entities_count]->isHeap  = 1;
        entityInfo.entities[entityInfo.entities_count]->pointer = calloc(1, pointerSize);
    }

    M_Assert_BreakSaveCheck((entityInfo.entities[entityInfo.entities_count]->pointer == NULL), {
        deleteEntitityFields(entityInfo.entities_count);
    }, return ENTITY_ERROR, (isCustomSpace ? "newEntities: Invalid user input space" : "newEntities: No memory for allocation Entitity"));

    // allocation pointer to Fields------------------------------------------------------------------------------------------------------------------------------------------------------------
    entityInfo.entities[entityInfo.entities_count]->fields = (EntityField*) calloc(NumberOfFields, sizeof(EntityField));
    M_Assert_BreakSaveCheck((entityInfo.entities[entityInfo.entities_count]->fields == NULLPTR(TYPEOF_STRUCT(Entity, fields))), {
        deleteEntitityFields(entityInfo.entities_count);
    }, return ENTITY_ERROR, "newEntities: No memory for allocation EntityField");

    entityInfo.entities[entityInfo.entities_count]->fields_count = NumberOfFields;

    // initialization Fields-------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

        entityInfo.entities[entityInfo.entities_count]->fields[i].bitFlags  = 0;
        entityInfo.entities[entityInfo.entities_count]->fields[i].shift     = 0;
        entityInfo.entities[entityInfo.entities_count]->fields[i].type      = VOID_TYPE;

        sprintf(str, "%d", i);
        MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DECRIPTION_SIZE, (u8 *)str, (u8 *)entityInfo.entities[entityInfo.entities_count]->fields[i].descr);
    }

    ++entityInfo.entities_count;
    return (entityInfo.entities_count - 1U);
}

/*
 * **********************************************************************************************************************************
 *  field functions
 * **********************************************************************************************************************************
 */

//int initField(Entity * entityInst, u8 * filedNo, u8 bitFlags, u16 shift, u8 type, char * descr, void * field_ptr)
//{
//    if(entityInst && (entityInst->fields_count > (*filedNo))) {
//        u16 pos = 0;

//        entityInst->fields[*filedNo].bitFlags = bitFlags;
//        entityInst->fields[*filedNo].shift = shift;
//        entityInst->fields[*filedNo].type = type;

//        to_pos_copy(ENTITY_DECRIPTION_LEN, (u8 *)descr, (u8 *)entityInst->fields[*filedNo].descr, &pos, ENTITY_DIR_COPY);
//        (*filedNo)++;

//        entityPointerInit_t(type, (u8 *)field_ptr);

//        return ENTITY_OK;
//    }
//    return ENTITY_ERROR;
//}


/*
 * **********************************************************************************************************************************
 *  foreach entities
 * **********************************************************************************************************************************
 */

int foreachEntities(int (*predicate)(int entityNumber, Entity* entity, int fieldNumber, EntityField* field, void* context), void* context)
{
    M_Assert_BreakSaveCheck((predicate == NULLPTR(TYPEOF_DATA(predicate))), M_EMPTY, return ENTITY_ERROR, "foreachEntities: no valid function");

    for(int i = 0; i < entityInfo.entities_count; ++i) {
        for(int j = 0; j < entityInfo.entities[i]->fields_count; ++j) {
            if(predicate(i, entityInfo.entities[i], j, &entityInfo.entities[i]->fields[j], context)) {
                return ENTITY_OK;
            }
        }
    }
    return ENTITY_OK;
}


#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */
