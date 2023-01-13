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
static void deleteEntitityFieldsInternal(const reg entityNumber)
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
void deleteEntitityFieldsExternal(const reg entityNumber)
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
    TYPEOF_STRUCT(EntityInfo, entities_count) entities_count = entityInfo.entities_count;

    for(TYPEOF_STRUCT(EntityInfo, entities_count) i = 0; i < entities_count; ++i) {
        deleteEntitityFieldsInternal(i);
    }

    free(entityInfo.entities);
    pointerInit(sizeof(entityInfo), (u8*)&entityInfo);
}



/// allocation new entities pointers
int newEntities(const reg numberOfEntities)
{
    M_Assert_BreakSaveCheck((numberOfEntities > MAX_NUBER_OF_ENTITIES), M_EMPTY, return ENTITY_ERROR, "newEntities: No valid input number of entities, value: %d, max: %d", numberOfEntities, MAX_NUBER_OF_ENTITIES);

    deleteEntities(); // delete all entities

    entityInfo.entities = (Entity**) calloc(numberOfEntities, sizeof(Entity*)); // allocation & initialization all entity pointers
    M_Assert_BreakSaveCheck((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities))), M_EMPTY, return ENTITY_ERROR, "newEntities: No memory for allocation Entity pointers");

    entityInfo.allocated_entity_pointers = numberOfEntities;
    return ENTITY_OK;
}


/// allocation entitites pointer & fields
int initEntity(reg* const entityNumber, const reg NumberOfFields, const reg pointerSize, const char descr[ENTITY_DESCRIPTION_SIZE], const b isCustomSpace, const b isHeap, void* arg)
{
    M_Assert_BreakSaveCheck((NumberOfFields > MAX_NUBER_OF_FIELDS), M_EMPTY, return ENTITY_ERROR, "initEntity: No valid input number of fields, value: %d, max: %d", NumberOfFields, MAX_NUBER_OF_FIELDS);

    TYPEOF_STRUCT(EntityInfo, entities_count) entities_count = entityInfo.entities_count;
    const TYPEOF_STRUCT(EntityInfo, allocated_entity_pointers) allocated_entity_pointers = entityInfo.allocated_entity_pointers;

    M_Assert_BreakSaveCheck((entities_count >= allocated_entity_pointers), M_EMPTY, return ENTITY_ERROR, "initEntity: There is no free entity for initialization!!!, use /newEntities/ function before");

    char str[(ENTITY_DESCRIPTION_SIZE + 8) + 1] = {};

    // allocation pointer to Entity--------------------------------------------------------------------------------------------------------------------------------------------------------------
    Entity* const new_entity = entityInfo.entities[entities_count] = (Entity*) calloc(1, sizeof(Entity));
    M_Assert_BreakSaveCheck(new_entity == NULLPTR(Entity*), M_EMPTY, return ENTITY_ERROR, "newEntities: No memory for allocation Entitity");

    // copy entity description-------------------------------------------------------------------------------------------------------------------------------------------------------------------
    if(descr) {
        MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DESCRIPTION_SIZE, (u8 *)descr, (u8 *)new_entity->descr);
    } else {
        sprintf(str, "E%d", entities_count);
        MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DESCRIPTION_SIZE, (u8 *)str, (u8 *)new_entity->descr);
    }


    // allocation or initialization pointer to data----------------------------------------------------------------------------------------------------------------------------------------------
#ifdef USE_ENTITY_USER_SPACE

    if(isCustomSpace) {
        new_entity->isHeap  = isHeap;
        new_entity->pointer = arg;
    } else {
        new_entity->isHeap  = 1;
        new_entity->pointer = calloc(1, pointerSize);
    }

    M_Assert_BreakSaveCheck((new_entity->pointer == NULL), {
        deleteEntitityFieldsInternal(entities_count);
    }, return ENTITY_ERROR, (isCustomSpace ? "newEntities: Invalid user input space" : "newEntities: No memory for allocation Entitity"));

#else

    new_entity->pointer = calloc(1, pointerSize);
    UNUSED(isCustomSpace);
    UNUSED(isHeap);
    UNUSED(arg);

    M_Assert_BreakSaveCheck((new_entity->pointer == NULL), {
        deleteEntitityFieldsInternal(entities_count);
    }, return ENTITY_ERROR, "newEntities: No memory for allocation Entitity");

#endif /* USE_ENTITY_USER_SPACE */


    // allocation pointer to Fields------------------------------------------------------------------------------------------------------------------------------------------------------------
    new_entity->fields = (EntityField*) calloc(NumberOfFields, sizeof(EntityField));
    M_Assert_BreakSaveCheck((new_entity->fields == NULLPTR(TYPEOF_STRUCT(Entity, fields))), {
        deleteEntitityFieldsInternal(entities_count);
    }, return ENTITY_ERROR, "newEntities: No memory for allocation EntityField");

    new_entity->fields_count = NumberOfFields;

    // initialization Fields-------------------------------------------------------------------------------------------------------------------------------------------------------------------
    for(reg i = 0; i < NumberOfFields; ++i) {
        EntityField* const    field = &new_entity->fields[i];

#ifdef USE_ENTITY_CALLBACKS

#   ifdef USE_ENTITY_READ_CALLBACK
        field->rdCallback.entityCallback = NULLPTR(TYPEOF_STRUCT(entityCallbackContainer, entityCallback));
        field->rdCallback.context        = NULLPTR(TYPEOF_STRUCT(entityCallbackContainer, context));
#   endif /* USE_ENTITY_READ_CALLBACK */

#   ifdef USE_ENTITY_WRITE_CALLBACK
        field->wrCallback.entityCallback = NULLPTR(TYPEOF_STRUCT(entityCallbackContainer, entityCallback));
        field->wrCallback.context        = NULLPTR(TYPEOF_STRUCT(entityCallbackContainer, context));
#   endif /* USE_ENTITY_WRITE_CALLBACK */

#endif /* USE_ENTITY_CALLBACKS */

        field->bitFlags  = 0;
        field->shift     = 0;
        field->type      = VOID_TYPE;

        sprintf(str, "F%d", i);
        MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DESCRIPTION_SIZE, (u8 *)str, (u8 *)field->descr);
    }

    (*entityNumber) = entities_count;
    ++entities_count;
    entityInfo.entities_count = entities_count;
    return ENTITY_OK;
}

/*
 * **********************************************************************************************************************************
 *  field functions
 * **********************************************************************************************************************************
 */

/// init field by Entity pointer and field-number
int initField(Entity* const entityInst, reg* const fieldNumber, const TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, const TYPEOF_STRUCT(EntityField, shift) shift, const TYPEOF_STRUCT(EntityField, type) type, const char descr[ENTITY_DESCRIPTION_SIZE], void* const field_ptr)
{
    M_Assert_BreakSaveCheck((entityInst == NULLPTR(Entity *) || fieldNumber == NULL), M_EMPTY, return ENTITY_ERROR, "initField: No valid input");

#ifdef USE_ENTITY_REGISTER
    M_Assert_BreakSaveCheck((bitFlags & ENTITY_REGISTER_MSK) && !(type == REG_TYPE || type == SREG_TYPE), M_EMPTY, return ENTITY_ERROR, "initField: with ENTITY_REGISTER_MSK flag must be REG_TYPE or SREG_TYPE types only!!!");
#endif /* USE_ENTITY_REGISTER */

    reg fieldNumber_readed = (*fieldNumber);
    const TYPEOF_STRUCT(Entity, fields_count) fields_count          = entityInst->fields_count;

    M_Assert_BreakElseSaveCheck((fields_count > fieldNumber_readed), {
                                    EntityField* const    field = &entityInst->fields[fieldNumber_readed];

                                    field->bitFlags     = bitFlags;
                                    field->shift        = shift;
                                    field->type         = type;

                                    if(descr) {
                                        MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DESCRIPTION_SIZE, (u8 *)descr, (u8 *)field->descr);
                                    }

                                    if(field_ptr) {
                                        cTypePointerInit(type, (u8 *)field_ptr);
                                    }

                                    ++fieldNumber_readed;
                                    (*fieldNumber) = fieldNumber_readed;
                                    return ENTITY_OK;
                                }, M_EMPTY, M_EMPTY, "initField: fieldNumber: &d,  is too long than allocated fields_count: %d", fieldNumber_readed, fields_count);

    return ENTITY_ERROR;
}


/// init field-array
int initFieldArray(Entity* const entityInst, reg* const fieldNumber, TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, TYPEOF_STRUCT(EntityField, shift) shift, const TYPEOF_STRUCT(EntityField, type) type, const int arrayLen, const char descr[ENTITY_DESCRIPTION_SIZE], void* const field_ptr, const int startNum)
{
    M_Assert_BreakSaveCheck((entityInst == NULLPTR(Entity *) || fieldNumber == NULL) || (arrayLen == 0), M_EMPTY, return ENTITY_ERROR, "initFieldArray: No valid input");

#ifdef USE_ENTITY_REGISTER
    M_Assert_BreakSaveCheck((bitFlags & ENTITY_REGISTER_MSK) && (type != REG_TYPE || type != SREG_TYPE), M_EMPTY, return ENTITY_ERROR, "initFieldArray: with ENTITY_REGISTER_MSK flag must be REG_TYPE or SREG_TYPE types only!!!");
#endif /* USE_ENTITY_REGISTER */

    reg fieldNumber_readed = (*fieldNumber);
    const TYPEOF_STRUCT(Entity, fields_count) fields_count          = entityInst->fields_count;
    const reg typeLen = getMYCTypeLen(type);

    M_Assert_BreakElseSaveCheck((fields_count > (fieldNumber_readed + arrayLen)), {

                                    char str[(ENTITY_DESCRIPTION_SIZE + 8) + 1] = {};
                                    bitFlags |= ENTITY_ARRAY_MSK;
                                    for(int i = 0; i < arrayLen; ++i) {
                                        EntityField* const    field = &entityInst->fields[fieldNumber_readed];

                                        field->bitFlags   = bitFlags;
                                        field->shift      = shift;
                                        field->type       = type;

                                        if(descr) {
                                            sprintf(str, "%s%d", descr, (i + startNum));
                                        } else {
                                            sprintf(str, ":[%d]", (i + startNum));
                                        }
                                        MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DESCRIPTION_SIZE, (u8 *)str, (u8 *)field->descr);

                                        shift += typeLen;
                                        ++fieldNumber_readed;
                                    }

                                    if(field_ptr) {
                                        pointerInit((arrayLen * typeLen), field_ptr);
                                    }

                                    (*fieldNumber) = fieldNumber_readed;
                                    return ENTITY_OK;

                                }, M_EMPTY, M_EMPTY, "initFieldArray: fieldNumber: &d,  is too long than allocated fields_count: %d", fieldNumber_readed + arrayLen, fields_count);

    return ENTITY_ERROR;
}


///init existing field by pointer
int initFieldFromPtr(EntityField* const fieldInst, const TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, const TYPEOF_STRUCT(EntityField, shift) shift, const TYPEOF_STRUCT(EntityField, type) type, const char descr[ENTITY_DESCRIPTION_SIZE])
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
int fieldRename(Entity* const entityInst, const reg fieldNumber, const char descr[ENTITY_DESCRIPTION_SIZE])
{
    M_Assert_BreakSaveCheck((entityInst == NULLPTR(Entity *) || descr == NULLPTR(char *)), M_EMPTY, return ENTITY_ERROR, "fieldRename: No valid input");

    const TYPEOF_STRUCT(Entity, fields_count) fields_count          = entityInst->fields_count;

    M_Assert_BreakElseSaveCheck((fields_count > fieldNumber), {

                                    MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DESCRIPTION_SIZE, (u8 *)descr, (u8 *)entityInst->fields[fieldNumber].descr);
                                    return ENTITY_OK;

                                }, M_EMPTY, M_EMPTY, "fieldRename: fieldNumber: &d,  is too long than allocated fields_count: %d", fieldNumber, fields_count);
    return ENTITY_ERROR;
}



/*
 * ****************************************************************************************************
 * callBack functions
 * ***************************************************************************************************
 */

#ifdef USE_ENTITY_CALLBACKS

/// init field with callbacks by field-number
int initFieldCallback(Entity* const  entityInst, reg* const fieldNumber, const TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, const TYPEOF_STRUCT(EntityField, shift) shift, const TYPEOF_STRUCT(EntityField, type) type, const char descr[ENTITY_DESCRIPTION_SIZE], void* const field_ptr,
                      const TYPEOF_STRUCT(entityCallbackContainer, entityCallback) readCallback, const TYPEOF_STRUCT(entityCallbackContainer, context) readContext, const TYPEOF_STRUCT(entityCallbackContainer, entityCallback) writeCallback, const TYPEOF_STRUCT(entityCallbackContainer, context) writeContext)
{
    M_Assert_BreakSaveCheck((entityInst == NULLPTR(Entity *) || fieldNumber == NULLPTR(reg *)), M_EMPTY, return ENTITY_ERROR, "initFieldCallback: No valid input");

#ifdef USE_ENTITY_REGISTER
    M_Assert_BreakSaveCheck((bitFlags & ENTITY_REGISTER_MSK) && (type != REG_TYPE || type != SREG_TYPE), M_EMPTY, return ENTITY_ERROR, "initFieldCallback: with ENTITY_REGISTER_MSK flag must be REG_TYPE or SREG_TYPE types only!!!");
#endif /* USE_ENTITY_REGISTER */

    reg fieldNumber_readed = (*fieldNumber);
    const TYPEOF_STRUCT(Entity, fields_count) fields_count          = entityInst->fields_count;

    M_Assert_BreakElseSaveCheck((fields_count > fieldNumber_readed), {

                                    EntityField* const    field = &entityInst->fields[fieldNumber_readed];

                                    field->bitFlags     = bitFlags;
                                    field->shift        = shift;
                                    field->type         = type;

                                #ifdef USE_ENTITY_READ_CALLBACK
                                    field->rdCallback.entityCallback = readCallback;
                                    field->rdCallback.context        = readContext;
                                #else
                                    UNUSED(readCallback);
                                    UNUSED(readContext);
                                #endif /* USE_ENTITY_READ_CALLBACK */

                                #ifdef USE_ENTITY_WRITE_CALLBACK
                                    field->wrCallback.entityCallback = writeCallback;
                                    field->wrCallback.context        = writeContext;
                                #else
                                    UNUSED(writeCallback);
                                    UNUSED(writeContext);
                                #endif /* USE_ENTITY_WRITE_CALLBACK */

                                    if(descr) {
                                        MY_CTYPE_USER_DATA_MEMCPY(ENTITY_DESCRIPTION_SIZE, (u8 *)descr, (u8 *)field->descr);
                                    }

                                    if(field_ptr) {
                                        cTypePointerInit(type, (u8 *)field_ptr);
                                    }

                                    ++fieldNumber_readed;
                                    (*fieldNumber) = fieldNumber_readed;
                                    return ENTITY_OK;
                                }, M_EMPTY, M_EMPTY, "initFieldCallback: fieldNumber: &d,  is too long than allocated fields_count: %d", fieldNumber_readed, fields_count);

    return ENTITY_ERROR;
}

/// init callback function by fieldNumber
int entityInitCallback(Entity* const entityInst, const reg filedNumber,
                       const TYPEOF_STRUCT(entityCallbackContainer, entityCallback) readCallback, const TYPEOF_STRUCT(entityCallbackContainer, context) readContext, const TYPEOF_STRUCT(entityCallbackContainer, entityCallback) writeCallback, const TYPEOF_STRUCT(entityCallbackContainer, context) writeContext)
{
    M_Assert_BreakSaveCheck((entityInst == NULLPTR(Entity *)), M_EMPTY, return ENTITY_ERROR, "initCallback: No valid input");

    const TYPEOF_STRUCT(Entity, fields_count) fields_count          = entityInst->fields_count;

    M_Assert_BreakElseSaveCheck((fields_count > filedNumber), {

                                    EntityField* const    field = &entityInst->fields[filedNumber];

                                #ifdef USE_ENTITY_READ_CALLBACK
                                    field->rdCallback.entityCallback = readCallback;
                                    field->rdCallback.context        = readContext;
                                #else
                                    UNUSED(readCallback);
                                    UNUSED(readContext);
                                #endif /* USE_ENTITY_READ_CALLBACK */

                                #ifdef USE_ENTITY_WRITE_CALLBACK
                                    field->wrCallback.entityCallback = writeCallback;
                                    field->wrCallback.context        = writeContext;
                                #else
                                    UNUSED(writeCallback);
                                    UNUSED(writeContext);
                                #endif /* USE_ENTITY_WRITE_CALLBACK */

                                    return ENTITY_OK;

                                }, M_EMPTY, M_EMPTY, "initCallback: fieldNumber: &d,  is too long than allocated fields_count: %d", filedNumber, fields_count);
    return ENTITY_ERROR;
}

/// init callback function by description
int entityInitCallback_txt(Entity* const entityInst, const char descr[ENTITY_DESCRIPTION_SIZE],
                           const TYPEOF_STRUCT(entityCallbackContainer, entityCallback) readCallback, const TYPEOF_STRUCT(entityCallbackContainer, context) readContext, const TYPEOF_STRUCT(entityCallbackContainer, entityCallback) writeCallback, const TYPEOF_STRUCT(entityCallbackContainer, context) writeContext)
{
    M_Assert_BreakSaveCheck((entityInst == NULLPTR(Entity *)), M_EMPTY, return ENTITY_ERROR, "initCallback_txt: No valid input");

    const TYPEOF_STRUCT(Entity, fields_count) fields_count          = entityInst->fields_count;

    for(TYPEOF_STRUCT(Entity, fields_count) it = 0; it < fields_count; ++it) {

        EntityField* const    field = &entityInst->fields[it];
        if(entityDescrNotCompleate(descr, field->descr) == 0) {



#ifdef USE_ENTITY_READ_CALLBACK
            field->rdCallback.entityCallback = readCallback;
            field->rdCallback.context        = readContext;
#else
            UNUSED(readCallback);
            UNUSED(readContext);
#endif /* USE_ENTITY_READ_CALLBACK */

#ifdef USE_ENTITY_WRITE_CALLBACK
            field->wrCallback.entityCallback = writeCallback;
            field->wrCallback.context        = writeContext;
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

int foreachEntities(int (* const predicate)(reg entityNumber, Entity* entity, reg fieldNumber, EntityField* field, void* val, PREPROCESSOR_CTX_TYPE(ctx)), PREPROCESSOR_CTX_TYPE(ctx))
{
    M_Assert_BreakSaveCheck((predicate == NULLPTR(TYPEOF_DATA(predicate))), M_EMPTY, return ENTITY_ERROR, "foreachEntities: no valid function");

    const TYPEOF_STRUCT(EntityInfo, entities_count) entities_count = entityInfo.entities_count;

    for(TYPEOF_STRUCT(EntityInfo, entities_count) i = 0; i != entities_count; ++i) {
        Entity* const entity = entityInfo.entities[i]; // move to cash
        const TYPEOF_STRUCT(Entity, fields_count) fields_count          = entity->fields_count;

        for(TYPEOF_STRUCT(Entity, fields_count) j = 0; j != fields_count; ++j) {
            EntityField* const    field = &entity->fields[j];   // move to cash
            void* ptr = (entity->pointer + field->shift);       // move to cash

            if(predicate(i, entity, j, field, ptr, ctx)) {
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
void setEntityReadyState(const b state)
{
    entityInfo.userInitReady = state;
}

#endif /* USE_ENTITY_PING */




#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */
