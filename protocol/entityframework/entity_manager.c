#include "entity_manager.h"


#ifdef C_ENTITY_FRAMEWORK_LIB_ENA
#include "smart_assert.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef USE_ENTITY_PING
EntityInfo entityInfo = {0, 0, 0, NULLPTR(TYPEOF_STRUCT(EntityInfo, entities))};    // global variable entities for user projects (adds sizeof(EntityInfo) to .data section, and adds to .text sizeof(EntityInfo) initialization data)
#else
EntityInfo entityInfo = {   0, 0, NULLPTR(TYPEOF_STRUCT(EntityInfo, entities))};    // global variable entities for user projects (adds sizeof(EntityInfo) to .data section, and adds to .text sizeof(EntityInfo) initialization data)
#endif /* USE_ENTITY_PING */

/*
 * **********************************************************************************************************************************
 * entity new/delete functions
 * **********************************************************************************************************************************
 */

/// delete some entity for internal using
static void deleteEntitityFieldsInternal(reg entityNumber)
{
    M_Assert_BreakSaveCheck((entityNumber > entityInfo.entities_count), M_EMPTY, return, "deleteEntitityFieldsInternal: No entity for delete!!!");
    M_Assert_BreakSaveCheck((entityInfo.entities[entityNumber] == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities[entityNumber]))), M_EMPTY, return, "initEntity: entity number: %d is null", entityNumber);

#ifdef USE_ENTITY_USER_SPACE
    if(entityInfo.entities[entityNumber]->isHeap) {
        free(entityInfo.entities[entityNumber]->pointer);
        entityInfo.entities[entityNumber]->pointer = NULL;
    }
#else
    free(entityInfo.entities[entityNumber]->pointer);
    entityInfo.entities[entityNumber]->pointer = NULL;
#endif /* USE_ENTITY_USER_SPACE */

    free(entityInfo.entities[entityNumber]->fields);
    entityInfo.entities[entityNumber]->fields = NULLPTR(TYPEOF_STRUCT(EntityInfo, entities[entityNumber]->fields));

    free(entityInfo.entities[entityNumber]);
    entityInfo.entities[entityNumber] = NULLPTR(TYPEOF_STRUCT(EntityInfo, entities[entityNumber]));
}

/// delete some entity for external using
void deleteEntitityFieldsExternal(reg entityNumber)
{
    M_Assert_BreakSaveCheck((entityNumber >= entityInfo.entities_count), M_EMPTY, return, "deleteEntitityFieldsInternal: No entity for delete!!!");
    M_Assert_BreakSaveCheck((entityInfo.entities[entityNumber] == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities[entityNumber]))), M_EMPTY, return, "initEntity: entity number: %d is null", entityNumber);

#ifdef USE_ENTITY_USER_SPACE
    if(entityInfo.entities[entityNumber]->isHeap) {
        free(entityInfo.entities[entityNumber]->pointer);
        entityInfo.entities[entityNumber]->pointer = NULL;
    }
#else
    free(entityInfo.entities[entityNumber]->pointer);
    entityInfo.entities[entityNumber]->pointer = NULL;
#endif /* USE_ENTITY_USER_SPACE */

    free(entityInfo.entities[entityNumber]->fields);
    entityInfo.entities[entityNumber]->fields = NULLPTR(TYPEOF_STRUCT(EntityInfo, entities[entityNumber]->fields));

    free(entityInfo.entities[entityNumber]);
    entityInfo.entities[entityNumber] = NULLPTR(TYPEOF_STRUCT(EntityInfo, entities[entityNumber]));
}


/// delete all entities and deallocation all memory
void deleteEntities(void)
{
    for(reg i = 0; i < entityInfo.entities_count; ++i) {
        deleteEntitityFieldsInternal(i);
    }

    free(entityInfo.entities);
    pointerInit(sizeof(entityInfo), (u8*)&entityInfo);
}



/// allocation new entities pointers
int newEntities(reg numberOfEntities)
{
    M_Assert_BreakSaveCheck((numberOfEntities > MAX_NUBER_OF_ENTITIES), M_EMPTY, return ENTITY_ERROR, "newEntities: No valid input number of entities, value: %d, max: %d", numberOfEntities, MAX_NUBER_OF_ENTITIES);

    deleteEntities(); // delete all entities

    entityInfo.entities = (Entity**) calloc(numberOfEntities, sizeof(Entity*)); // allocation & initialization all entity pointers
    M_Assert_BreakSaveCheck((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities))), M_EMPTY, return ENTITY_ERROR, "newEntities: No memory for allocation Entity pointers");

    entityInfo.allocated_entity_pointers = numberOfEntities;
    return ENTITY_OK;
}


/// allocation entitites pointer & fields
int initEntity(reg* entityNumber, reg NumberOfFields, reg pointerSize, char descr[ENTITY_DESCRIPTION_SIZE], b isCustomSpace, b isHeap, void* arg)
{
    M_Assert_BreakSaveCheck((NumberOfFields > MAX_NUBER_OF_FIELDS), M_EMPTY, return ENTITY_ERROR, "initEntity: No valid input number of fields, value: %d, max: %d", NumberOfFields, MAX_NUBER_OF_FIELDS);
    M_Assert_BreakSaveCheck((entityInfo.entities_count == entityInfo.allocated_entity_pointers), M_EMPTY, return ENTITY_ERROR, "initEntity: There is no free entity for initialization!!!, use /newEntities/ function before");

    char str[(ENTITY_DESCRIPTION_SIZE + 8) + 1] = {};

    // allocation pointer to Entity--------------------------------------------------------------------------------------------------------------------------------------------------------------
    entityInfo.entities[entityInfo.entities_count] = (Entity*) calloc(1, sizeof(Entity));
    M_Assert_BreakSaveCheck(entityInfo.entities[entityInfo.entities_count] == NULLPTR(Entity*), M_EMPTY, return ENTITY_ERROR, "newEntities: No memory for allocation Entitity");

    // copy entity description-------------------------------------------------------------------------------------------------------------------------------------------------------------------
    if(descr) {
        MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DESCRIPTION_SIZE, (u8 *)descr, (u8 *)entityInfo.entities[entityInfo.entities_count]->descr);
    } else {
        sprintf(str, "E%d", entityInfo.entities_count);
        MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DESCRIPTION_SIZE, (u8 *)str, (u8 *)entityInfo.entities[entityInfo.entities_count]->descr);
    }


    // allocation or initialization pointer to data----------------------------------------------------------------------------------------------------------------------------------------------
#ifdef USE_ENTITY_USER_SPACE

    if(isCustomSpace) {
        entityInfo.entities[entityInfo.entities_count]->isHeap  = isHeap;
        entityInfo.entities[entityInfo.entities_count]->pointer = arg;
    } else {
        entityInfo.entities[entityInfo.entities_count]->isHeap  = 1;
        entityInfo.entities[entityInfo.entities_count]->pointer = calloc(1, pointerSize);
    }

    M_Assert_BreakSaveCheck((entityInfo.entities[entityInfo.entities_count]->pointer == NULL), {
        deleteEntitityFieldsInternal(entityInfo.entities_count);
    }, return ENTITY_ERROR, (isCustomSpace ? "newEntities: Invalid user input space" : "newEntities: No memory for allocation Entitity"));

#else

    entityInfo.entities[entityInfo.entities_count]->pointer = calloc(1, pointerSize);
    UNUSED(isCustomSpace);
    UNUSED(isHeap);
    UNUSED(arg);

    M_Assert_BreakSaveCheck((entityInfo.entities[entityInfo.entities_count]->pointer == NULL), {
        deleteEntitityFieldsInternal(entityInfo.entities_count);
    }, return ENTITY_ERROR, "newEntities: No memory for allocation Entitity");

#endif /* USE_ENTITY_USER_SPACE */


    // allocation pointer to Fields------------------------------------------------------------------------------------------------------------------------------------------------------------
    entityInfo.entities[entityInfo.entities_count]->fields = (EntityField*) calloc(NumberOfFields, sizeof(EntityField));
    M_Assert_BreakSaveCheck((entityInfo.entities[entityInfo.entities_count]->fields == NULLPTR(TYPEOF_STRUCT(Entity, fields))), {
        deleteEntitityFieldsInternal(entityInfo.entities_count);
    }, return ENTITY_ERROR, "newEntities: No memory for allocation EntityField");

    entityInfo.entities[entityInfo.entities_count]->fields_count = NumberOfFields;

    // initialization Fields-------------------------------------------------------------------------------------------------------------------------------------------------------------------
    for(reg i = 0; i < NumberOfFields; ++i) {

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

        sprintf(str, "F%d", i);
        MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DESCRIPTION_SIZE, (u8 *)str, (u8 *)entityInfo.entities[entityInfo.entities_count]->fields[i].descr);
    }

    (*entityNumber) = entityInfo.entities_count;
    ++entityInfo.entities_count;

    return ENTITY_OK;
}

/*
 * **********************************************************************************************************************************
 *  field functions
 * **********************************************************************************************************************************
 */

/// init field by Entity pointer and field-number
int initField(Entity * entityInst, reg * fieldNumber, TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, TYPEOF_STRUCT(EntityField, shift) shift, TYPEOF_STRUCT(EntityField, type) type, char descr[ENTITY_DESCRIPTION_SIZE], void * field_ptr)
{
    M_Assert_BreakSaveCheck((entityInst == NULLPTR(Entity *) || fieldNumber == NULL), M_EMPTY, return ENTITY_ERROR, "initField: No valid input");

#ifdef USE_ENTITY_REGISTER
    M_Assert_BreakSaveCheck((bitFlags & ENTITY_REGISTER_MSK) && (type != REG_TYPE || type != SREG_TYPE), M_EMPTY, return ENTITY_ERROR, "initField: with ENTITY_REGISTER_MSK flag must be REG_TYPE or SREG_TYPE types only!!!");
#endif /* USE_ENTITY_REGISTER */

    M_Assert_BreakElseSaveCheck((entityInst->fields_count > (*fieldNumber)), {

                                    entityInst->fields[(*fieldNumber)].bitFlags     = bitFlags;
                                    entityInst->fields[(*fieldNumber)].shift        = shift;
                                    entityInst->fields[(*fieldNumber)].type         = type;

                                    if(descr) {
                                        MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DESCRIPTION_SIZE, (u8 *)descr, (u8 *)entityInst->fields[(*fieldNumber)].descr);
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
int initFieldArray(Entity * entityInst, reg * fieldNumber, TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, TYPEOF_STRUCT(EntityField, shift) shift, TYPEOF_STRUCT(EntityField, type) type, int arrayLen, char descr[ENTITY_DESCRIPTION_SIZE], void * field_ptr, int startNum)
{
    M_Assert_BreakSaveCheck((entityInst == NULLPTR(Entity *) || fieldNumber == NULL) || (arrayLen == 0), M_EMPTY, return ENTITY_ERROR, "initFieldArray: No valid input");

#ifdef USE_ENTITY_REGISTER
    M_Assert_BreakSaveCheck((bitFlags & ENTITY_REGISTER_MSK) && (type != REG_TYPE || type != SREG_TYPE), M_EMPTY, return ENTITY_ERROR, "initFieldArray: with ENTITY_REGISTER_MSK flag must be REG_TYPE or SREG_TYPE types only!!!");
#endif /* USE_ENTITY_REGISTER */

    M_Assert_BreakElseSaveCheck((entityInst->fields_count > ((*fieldNumber) + arrayLen)), {

                                    char str[(ENTITY_DESCRIPTION_SIZE + 8) + 1] = {};
                                    bitFlags |= ENTITY_ARRAY_MSK;
                                    for(int i = 0; i < arrayLen; ++i) {

                                        entityInst->fields[(*fieldNumber)].bitFlags   = bitFlags;
                                        entityInst->fields[(*fieldNumber)].shift      = shift;
                                        entityInst->fields[(*fieldNumber)].type       = type;

                                        if(descr) {
                                            sprintf(str, "%s%d", descr, (i + startNum));
                                            MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DESCRIPTION_SIZE, (u8 *)str, (u8 *)entityInst->fields[(*fieldNumber)].descr);
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
int initFieldFromPtr(EntityField * fieldInst, TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, TYPEOF_STRUCT(EntityField, shift) shift, TYPEOF_STRUCT(EntityField, type) type, char descr[ENTITY_DESCRIPTION_SIZE])
{
    M_Assert_BreakSaveCheck((fieldInst == NULLPTR(EntityField *)), M_EMPTY, return ENTITY_ERROR, "initFieldPtr: No valid input");

#ifdef USE_ENTITY_REGISTER
    M_Assert_BreakSaveCheck((bitFlags & ENTITY_REGISTER_MSK) && (type != REG_TYPE || type != SREG_TYPE), M_EMPTY, return ENTITY_ERROR, "initFieldFromPtr: with ENTITY_REGISTER_MSK flag must be REG_TYPE or SREG_TYPE types only!!!");
#endif /* USE_ENTITY_REGISTER */

    fieldInst->bitFlags     = bitFlags;
    fieldInst->shift        = shift;
    fieldInst->type         = type;

    if(descr) {
        MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DESCRIPTION_SIZE, (u8 *)descr, (u8 *)fieldInst->descr);
    }
    return ENTITY_OK;
}

/// rename field by field number
int fieldRename(Entity * entityInst, reg fieldNumber, char descr[ENTITY_DESCRIPTION_SIZE])
{
    M_Assert_BreakSaveCheck((entityInst == NULLPTR(Entity *) || descr == NULLPTR(char *)), M_EMPTY, return ENTITY_ERROR, "fieldRename: No valid input");
    M_Assert_BreakElseSaveCheck((entityInst->fields_count > fieldNumber), {

                                    MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DESCRIPTION_SIZE, (u8 *)descr, (u8 *)entityInst->fields[fieldNumber].descr);
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
int initFieldCallback(Entity * entityInst, reg * fieldNumber, TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, TYPEOF_STRUCT(EntityField, shift) shift, TYPEOF_STRUCT(EntityField, type) type, char descr[ENTITY_DESCRIPTION_SIZE], void * field_ptr,
                      TYPEOF_STRUCT(entityCallbackContainer, entityCallback) readCallback, TYPEOF_STRUCT(entityCallbackContainer, context) readContext, TYPEOF_STRUCT(entityCallbackContainer, entityCallback) writeCallback, TYPEOF_STRUCT(entityCallbackContainer, context) writeContext)
{
    M_Assert_BreakSaveCheck((entityInst == NULLPTR(Entity *) || fieldNumber == NULLPTR(reg *)), M_EMPTY, return ENTITY_ERROR, "initFieldCallback: No valid input");

#ifdef USE_ENTITY_REGISTER
    M_Assert_BreakSaveCheck((bitFlags & ENTITY_REGISTER_MSK) && (type != REG_TYPE || type != SREG_TYPE), M_EMPTY, return ENTITY_ERROR, "initFieldCallback: with ENTITY_REGISTER_MSK flag must be REG_TYPE or SREG_TYPE types only!!!");
#endif /* USE_ENTITY_REGISTER */

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
                                        MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DESCRIPTION_SIZE, (u8 *)descr, (u8 *)entityInst->fields[(*fieldNumber)].descr);
                                    }

                                    if(field_ptr) {
                                        cTypePointerInit(type, (u8 *)field_ptr);
                                    }

                                    ++(*fieldNumber);
                                    return ENTITY_OK;
                                }, M_EMPTY, M_EMPTY, "initFieldCallback: fieldNumber: &d,  is too long than allocated fields_count: %d", (*fieldNumber), entityInst->fields_count);

    return ENTITY_ERROR;
}

/// init callback function by fieldNumber
int entityInitCallback(Entity * entityInst, reg filedNumber,
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
int entityInitCallback_txt(Entity * entityInst, char descr[ENTITY_DESCRIPTION_SIZE],
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

int foreachEntities(int (*predicate)(reg entityNumber, Entity* entity, reg fieldNumber, EntityField* field, void* val, void* context), void* context)
{
    M_Assert_BreakSaveCheck((predicate == NULLPTR(TYPEOF_DATA(predicate))), M_EMPTY, return ENTITY_ERROR, "foreachEntities: no valid function");

    void* ptr;
    for(TYPEOF_STRUCT(EntityInfo, entities_count) i = 0; i < entityInfo.entities_count; ++i) {
        for(TYPEOF_STRUCT(Entity, fields_count) j = 0; j < entityInfo.entities[i]->fields_count; ++j) {
            ptr = (entityInfo.entities[i]->pointer + entityInfo.entities[i]->fields[j].shift);
            if(predicate(i, entityInfo.entities[i], j, &entityInfo.entities[i]->fields[j], ptr, context)) {
                return ENTITY_OK;
            }
        }
    }
    return ENTITY_OK;
}

#ifdef USE_ENTITY_PING
/*
 * **********************************************************************************************************************************
 *  user Ready functions
 * **********************************************************************************************************************************
 */
void setEntityReadyState(b state)
{
    entityInfo.userInitReady = state;
}

#endif /* USE_ENTITY_PING */




#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */
