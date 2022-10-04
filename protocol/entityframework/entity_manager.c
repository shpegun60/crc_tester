#include "entity_manager.h"


#ifdef C_ENTITY_FRAMEWORK_LIB_ENA
#include "smart_assert.h"
#include "my_ctype_cast.h"

#include <stdlib.h>
#include <stdio.h>

EntityInfo entityInfo = {0, 0, NULLPTR(TYPEOF_STRUCT(EntityInfo, entities))};  // global variable entities for user projects (adds sizeof(EntityInfo) to .data section)

/*
 * **********************************************************************************************************************************
 * entity new/delete functions
 * **********************************************************************************************************************************
 */

/// delete all entities and deallocation all memory
void deleteEntities(void)
{
    for(u32 i = 0; i < entityInfo.entities_count; ++i) {
        deleteEntitityFields(i);
    }

    free(entityInfo.entities);
    entityInfo.entities_count               = 0;
    entityInfo.allocated_entity_pointers    = 0;
    entityInfo.entities                     = NULLPTR(TYPEOF_STRUCT(EntityInfo, entities));
}

/// delete some entity
void deleteEntitityFields(u32 entityNumber)
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

/// allocation new entities pointers
int newEntities(u32 nomberOfEntities)
{
    M_Assert_BreakSaveCheck((nomberOfEntities > MAX_NUBER_OF_ENTITIES), M_EMPTY, return ENTITY_ERROR, "newEntities: No valid input");

    deleteEntities(); // delete all entities

    entityInfo.entities = (Entity**) calloc(nomberOfEntities, sizeof(Entity*)); // allocation & initialization all entity pointers
    M_Assert_BreakSaveCheck((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities))), M_EMPTY, return ENTITY_ERROR, "newEntities: No memory for allocation Entity pointers");

    entityInfo.allocated_entity_pointers = nomberOfEntities;
    return ENTITY_OK;
}


/// allocation entitites pointer & fields
int initEntity(int NumberOfFields, reg pointerSize, char descr[ENTITY_DECRIPTION_SIZE], b isCustomSpace, b isHeap, void* arg)
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

/// init field by field-number
int initField(Entity * entityInst, int * fieldNumber, TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, TYPEOF_STRUCT(EntityField, shift) shift, TYPEOF_STRUCT(EntityField, type) type, char descr[ENTITY_DECRIPTION_SIZE], void * field_ptr)
{
    M_Assert_BreakSaveCheck((entityInst == NULLPTR(Entity *) || fieldNumber == NULL), M_EMPTY, return ENTITY_ERROR, "initField: No valid input");
    M_Assert_BreakElseSaveCheck((entityInst->fields_count > (*fieldNumber)), {

                                    entityInst->fields[(*fieldNumber)].bitFlags     = bitFlags;
                                    entityInst->fields[(*fieldNumber)].shift        = shift;
                                    entityInst->fields[(*fieldNumber)].type         = type;

                                    if(descr) {
                                        MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DECRIPTION_SIZE, (u8 *)descr, (u8 *)entityInst->fields[(*fieldNumber)].descr);
                                    }

                                    if(field_ptr) {
                                        cTypePointerInit(type, (u8 *)field_ptr);
                                    }

                                    ++(*fieldNumber);
                                    return ENTITY_OK;
                                }, M_EMPTY, M_EMPTY, "initField: fieldNumber: &d,  is too long than allocated fields_count: %d", (*fieldNumber), entityInst->fields_count);

    return ENTITY_ERROR;
}

/// init field-array
int initFieldArray(Entity * entityInst, int * fieldNumber, TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, TYPEOF_STRUCT(EntityField, shift) shift, TYPEOF_STRUCT(EntityField, type) type, int arrayLen, char descr[ENTITY_DECRIPTION_SIZE], void * field_ptr, int startNum)
{
    M_Assert_BreakSaveCheck((entityInst == NULLPTR(Entity *) || fieldNumber == NULL), M_EMPTY, return ENTITY_ERROR, "initField: No valid input");
    M_Assert_BreakElseSaveCheck((entityInst->fields_count > ((*fieldNumber) + arrayLen)), {

                                    char str[(ENTITY_DECRIPTION_SIZE << 1) + 1] = {};
                                    for(int i = 0; i < arrayLen; ++i) {

                                        entityInst->fields[(*fieldNumber)].bitFlags   = bitFlags;
                                        entityInst->fields[(*fieldNumber)].shift      = shift;
                                        entityInst->fields[(*fieldNumber)].type       = type;

                                        if(descr) {
                                            sprintf(str, "%s%d", descr, (i + startNum));
                                            MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DECRIPTION_SIZE, (u8 *)str, (u8 *)entityInst->fields[(*fieldNumber)].descr);
                                        }

                                        shift += getMYCTypeLen(type);
                                        ++(*fieldNumber);
                                    }

                                    if(field_ptr) {
                                        pointerInit((arrayLen * getMYCTypeLen(type)), field_ptr);
                                    }
                                    return ENTITY_OK;

                                }, M_EMPTY, M_EMPTY, "initFieldArray: fieldNumber: &d,  is too long than allocated fields_count: %d", (*fieldNumber) + arrayLen, entityInst->fields_count);

    return ENTITY_ERROR;
}


///init existing field by pointer
int initFieldFromPtr(EntityField * fieldInst, TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, TYPEOF_STRUCT(EntityField, shift) shift, TYPEOF_STRUCT(EntityField, type) type, char descr[ENTITY_DECRIPTION_SIZE])
{
    M_Assert_BreakSaveCheck((fieldInst == NULLPTR(EntityField *)), M_EMPTY, return ENTITY_ERROR, "initFieldPtr: No valid input");

    fieldInst->bitFlags     = bitFlags;
    fieldInst->shift        = shift;
    fieldInst->type         = type;

    if(descr) {
        MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DECRIPTION_SIZE, (u8 *)descr, (u8 *)fieldInst->descr);
    }
    return ENTITY_OK;
}

/// rename field by field number
int fieldRename(Entity * entityInst, int fieldNumber, char descr[ENTITY_DECRIPTION_SIZE])
{
    M_Assert_BreakSaveCheck((entityInst == NULLPTR(Entity *) || descr == NULLPTR(char *)), M_EMPTY, return ENTITY_ERROR, "fieldRename: No valid input");
    M_Assert_BreakElseSaveCheck((entityInst->fields_count > fieldNumber), {

                                    MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DECRIPTION_SIZE, (u8 *)descr, (u8 *)entityInst->fields[fieldNumber].descr);
                                    return ENTITY_OK;

                                }, M_EMPTY, M_EMPTY, "fieldRename: fieldNumber: &d,  is too long than allocated fields_count: %d", fieldNumber, entityInst->fields_count);
    return ENTITY_ERROR;
}



/*
 * ****************************************************************************************************
 * callBack functions
 * ***************************************************************************************************
 */

#ifdef USE_ENTITY_CALLBACKS

/// init field with callbacks by field-number
int initFieldCallback(Entity * entityInst, int * fieldNumber, TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, TYPEOF_STRUCT(EntityField, shift) shift, TYPEOF_STRUCT(EntityField, type) type, char descr[ENTITY_DECRIPTION_SIZE], void * field_ptr,
                      TYPEOF_STRUCT(entityCallbackContainer, entityCallback) readCallback, TYPEOF_STRUCT(entityCallbackContainer, context) readContext, TYPEOF_STRUCT(entityCallbackContainer, entityCallback) writeCallback, TYPEOF_STRUCT(entityCallbackContainer, context) writeContext)
{
    M_Assert_BreakSaveCheck((entityInst == NULLPTR(Entity *) || fieldNumber == NULLPTR(int *)), M_EMPTY, return ENTITY_ERROR, "initField: No valid input");
    M_Assert_BreakElseSaveCheck((entityInst->fields_count > (*fieldNumber)), {

                                    entityInst->fields[(*fieldNumber)].bitFlags     = bitFlags;
                                    entityInst->fields[(*fieldNumber)].shift        = shift;
                                    entityInst->fields[(*fieldNumber)].type         = type;

                                #ifdef USE_ENTITY_READ_CALLBACK
                                    entityInst->fields[(*fieldNumber)].rdCallback.entityCallback = readCallback;
                                    entityInst->fields[(*fieldNumber)].rdCallback.context        = readContext;
                                #else
                                    UNUSED(readCallback);
                                    UNUSED(readContext);
                                #endif /* USE_ENTITY_READ_CALLBACK */

                                #ifdef USE_ENTITY_WRITE_CALLBACK
                                    entityInst->fields[(*fieldNumber)].wrCallback.entityCallback = writeCallback;
                                    entityInst->fields[(*fieldNumber)].wrCallback.context        = writeContext;
                                #else
                                    UNUSED(writeCallback);
                                    UNUSED(writeContext);
                                #endif /* USE_ENTITY_WRITE_CALLBACK */

                                    if(descr) {
                                        MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DECRIPTION_SIZE, (u8 *)descr, (u8 *)entityInst->fields[(*fieldNumber)].descr);
                                    }

                                    if(field_ptr) {
                                        cTypePointerInit(type, (u8 *)field_ptr);
                                    }

                                    ++(*fieldNumber);
                                    return ENTITY_OK;
                                }, M_EMPTY, M_EMPTY, "initField: fieldNumber: &d,  is too long than allocated fields_count: %d", (*fieldNumber), entityInst->fields_count);

    return ENTITY_ERROR;
}

/// init callback function by fieldNumber
int entityInitCallback(Entity * entityInst, int filedNumber,
                       TYPEOF_STRUCT(entityCallbackContainer, entityCallback) readCallback, TYPEOF_STRUCT(entityCallbackContainer, context) readContext, TYPEOF_STRUCT(entityCallbackContainer, entityCallback) writeCallback, TYPEOF_STRUCT(entityCallbackContainer, context) writeContext)
{
    M_Assert_BreakSaveCheck((entityInst == NULLPTR(Entity *)), M_EMPTY, return ENTITY_ERROR, "initCallback: No valid input");
    M_Assert_BreakElseSaveCheck((entityInst->fields_count > filedNumber), {

                                #ifdef USE_ENTITY_READ_CALLBACK
                                    entityInst->fields[filedNumber].rdCallback.entityCallback = readCallback;
                                    entityInst->fields[filedNumber].rdCallback.context        = readContext;
                                #else
                                    UNUSED(readCallback);
                                    UNUSED(readContext);
                                #endif /* USE_ENTITY_READ_CALLBACK */

                                #ifdef USE_ENTITY_WRITE_CALLBACK
                                    entityInst->fields[filedNumber].wrCallback.entityCallback = writeCallback;
                                    entityInst->fields[filedNumber].wrCallback.context        = writeContext;
                                #else
                                    UNUSED(writeCallback);
                                    UNUSED(writeContext);
                                #endif /* USE_ENTITY_WRITE_CALLBACK */

                                    return ENTITY_OK;

                                }, M_EMPTY, M_EMPTY, "initCallback: fieldNumber: &d,  is too long than allocated fields_count: %d", filedNumber, entityInst->fields_count);
    return ENTITY_ERROR;
}

/// init callback function by description
int entityInitCallback_txt(Entity * entityInst, char descr[ENTITY_DECRIPTION_SIZE],
                     TYPEOF_STRUCT(entityCallbackContainer, entityCallback) readCallback, TYPEOF_STRUCT(entityCallbackContainer, context) readContext, TYPEOF_STRUCT(entityCallbackContainer, entityCallback) writeCallback, TYPEOF_STRUCT(entityCallbackContainer, context) writeContext)
{
    M_Assert_BreakSaveCheck((entityInst == NULLPTR(Entity *)), M_EMPTY, return ENTITY_ERROR, "initCallback_txt: No valid input");

    for(int it = 0; it < entityInst->fields_count; ++it) {

        if(entityDescrNotCompleate(descr, entityInst->fields[it].descr) == 0) {

#ifdef USE_ENTITY_READ_CALLBACK
            entityInst->fields[it].rdCallback.entityCallback = readCallback;
            entityInst->fields[it].rdCallback.context        = readContext;
#else
            UNUSED(readCallback);
            UNUSED(readContext);
#endif /* USE_ENTITY_READ_CALLBACK */

#ifdef USE_ENTITY_WRITE_CALLBACK
            entityInst->fields[it].wrCallback.entityCallback = writeCallback;
            entityInst->fields[it].wrCallback.context        = writeContext;
#else
            UNUSED(writeCallback);
            UNUSED(writeContext);
#endif /* USE_ENTITY_WRITE_CALLBACK */

            return ENTITY_OK;
        }
    }
    return ENTITY_ERROR;
}

#endif /* USE_ENTITY_CALLBACKS */



/*
 * **********************************************************************************************************************************
 *  foreach entities
 * **********************************************************************************************************************************
 */

int foreachEntities(int (*predicate)(int entityNumber, Entity* entity, int fieldNumber, EntityField* field, void* context), void* context)
{
    M_Assert_BreakSaveCheck((predicate == NULLPTR(TYPEOF_DATA(predicate))), M_EMPTY, return ENTITY_ERROR, "foreachEntities: no valid function");

    for(u32 i = 0; i < entityInfo.entities_count; ++i) {
        for(u32 j = 0; j < entityInfo.entities[i]->fields_count; ++j) {
            if(predicate(i, entityInfo.entities[i], j, &entityInfo.entities[i]->fields[j], context)) {
                return ENTITY_OK;
            }
        }
    }
    return ENTITY_OK;
}
/// string compleate for entities---------------------------------------------------------------------------------------------------
int entityDescrNotCompleate(const c8* str1, const c8* str2)
{
#if ENTITY_DECRIPTION_SIZE == 0x01U
    return ((*str1) == (*str2)) ? 0 : 1;
#elif ENTITY_DECRIPTION_SIZE == 0x02U
    return (( *UINT16_TYPE_DC(str1) ) == ( *UINT16_TYPE_DC(str2) )) ? 0 : 1;
#elif ENTITY_DECRIPTION_SIZE == 0x04U
    return (( *UINT32_TYPE_DC(str1) ) == ( *UINT32_TYPE_DC(str2) )) ? 0 : 1;
#elif ENTITY_DECRIPTION_SIZE == 0x08U
    return (( *UINT64_TYPE_DC(str1) ) == ( *UINT64_TYPE_DC(str2) )) ? 0 : 1;
#else

    int n = ENTITY_DECRIPTION_SIZE;
    while(n--) {
        if(*str1++ != *str2++) {
            return 1;
        }
    }
    return 0;

#endif /* description complemntation function selector */
}




#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */
