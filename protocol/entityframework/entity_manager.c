#include "entity_manager.h"


#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#include "smart_assert.h"
#include <stdlib.h>

EntityInfo entityInfo = {0, 0, NULL};  // global variable entities for user projects (adds sizeof(EntityInfo) to .data section)

/*
 * ****************************************************************************************************
 * entity new/delete functions
 * ***************************************************************************************************
 */

// malloc new entities pointers
int newEntities(int nomberOfEntities)
{
    clearEntities(); // clear all entities

    if(nomberOfEntities == ENTITY_ERROR) {
        return ENTITY_ERROR;
    }

    entityInfo.entities = (Entity**) calloc(nomberOfEntities, sizeof(Entity*)); // allocation & initialization all entity
    if(entityInfo.entities == NULL) {
        return ENTITY_ERROR;
    }

    entityInfo.allocated_entity_pointers = nomberOfEntities;
    return ENTITY_OK;
}

//***********************************************************************************************************

/*
 * ****************************************************************************************************
 * help functions
 * ***************************************************************************************************
 */

//typedef struct {
//    u16 allocated_entity_pointers;
//    u16 entities_count;
//    Entity** entities;
//} EntityInfo;

//struct EntityField {

//#ifdef USE_ENTITY_CALLBACKS

//#ifdef USE_ENTITY_READ_CALLBACK
//    entityCallbackContainer rdCallback; // read callback
//#endif /* USE_ENTITY_READ_CALLBACK */

//#ifdef USE_ENTITY_WRITE_CALLBACK
//    entityCallbackContainer wrCallback; // write callback
//#endif /* USE_ENTITY_WRITE_CALLBACK */

//#endif /* USE_ENTITY_CALLBACKS */

//    u16     bitFlags; //bit[0] - read only, bit[1] - isParam, bit[2] - is log, bit[3] - is pointer ... see bit flags mask
//    u16     shift;
//    u8      type;
//    char    descr[ENTITY_DECRIPTION_SIZE];
//};

//struct Entity {
//    void*           pointer;
//    char            descr[ENTITY_DECRIPTION_SIZE];
//    u16             fields_count;
//    EntityField*    fields;
//};

void clearEntities(void)
{
    for(int i = 0; i < entityInfo.entities_count; ++i) {
        free(entityInfo.entities[i]->pointer);
        free(entityInfo.entities[i]->fields);
        free(entityInfo.entities[i]);
    }

    free(entityInfo.entities);
    entityInfo.entities_count = 0;
    entityInfo.allocated_entity_pointers = 0;
    entityInfo.entities = NULL;
}

void clearEntitityFields(int entityNumber)
{
    M_Assert_WarningElseSaveCheck((entityNumber < entityInfo.entities_count), {
                                      free(entityInfo.entities[entityNumber]->pointer);
                                      free(entityInfo.entities[entityNumber]->fields);
                                      free(entityInfo.entities[entityNumber]);
                                  }, M_EMPTY, M_EMPTY, "clearEntitityFields: number is not exists, entity number input: %d, numbers possible: %d", entityNumber, entityInfo.entities_count);
}



#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */
