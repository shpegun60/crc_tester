/*
 * entity_manager.h
 *
 *  Created on: Oct 01, 2022
 *      Author: Shpegun60
 */
#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "entity_port.h"

#ifdef C_ENTITY_FRAMEWORK_LIB_ENA
#include "my_ctype_cast.h"

/**************************************************************************************************************************************************
 * main struct types definition before (for type saving in entityCallback functions)
 */
typedef struct EntityField  EntityField;
typedef struct Entity       Entity;


/**************************************************************************************************************************************************
 * callback type definition
 */

/**
  * @brief  entityCallback(entity, field, args). Pointer to function array callbacks
  * @param  entity      - pointer to Entity
  * @param  field       - pointer to EntityField
  * @param  value       - pointer to value data (pointer + field shift)
  * @param  context     - pointer to context for callback (programmer must cast to other last defined value).
  * @retval None
  */
typedef struct {
    void (*entityCallback)(Entity* entity, EntityField* field, void* value, void* context); // callback function
    void* context;                                                                          // callback context
} entityCallbackContainer;


/**************************************************************************************************************************************************
 * bit flags mask for value name "bitFlags" in EntityField
 */
#define ENTITY_EMPTY_FLAG    	0x00000000U // nothing (ordinary field)
#define ENTITY_READ_ONLY_MSK 	0x00000001U // its read only field (write protected field)
#define ENTITY_PARAM_MSK 		0x00000002U // its parameter field (writed to flash once and read before start, must uncommitted USE_ENTITY_FLASH)
#define ENTITY_LOG_MSK 			0x00000004U // its log (writed to flash continious, must uncommitted USE_ENTITY_FLASH)

#ifdef USE_ENTITY_POINTER
#define ENTITY_POINTER_MSK 		0x00000008U // its field which contains pointer
#endif /* USE_ENTITY_POINTER */

#ifdef USE_ENTITY_REGISTER
#define ENTITY_REGISTER_MSK 	0x00000010U // its register, bus type atomic write and read operaions with field
#endif /* USE_ENTITY_REGISTER */

#ifdef USE_ENTITY_ATOMIC
#define ENTITY_ATOMIC_MSK 		0x00000020U // its atomic field must specify entity_atomic.h file (before write and read disabling interrupts and after restore it
#endif /* USE_ENTITY_ATOMIC */

#define ENTITY_LOW_UPDATE_MSK 	0x00000040U // its low update field (no impact on the program)
#define ENTITY_READ_ONCE_MSK 	0x00000080U // its read once field  (no impact on the program)
#define ENTITY_ARRAY_MSK        0x00000100U // its array - field    (no impact on the program)
#define ENTITY_ALL_BIT_MSK 		0xFFFFFFFFU // its mask for counting all fields

/**************************************************************************************************************************************************
 * main structs definition
 */
struct EntityField {

#ifdef USE_ENTITY_CALLBACKS

#ifdef USE_ENTITY_READ_CALLBACK
    entityCallbackContainer rdCallback; // read callback
#endif /* USE_ENTITY_READ_CALLBACK */

#ifdef USE_ENTITY_WRITE_CALLBACK
    entityCallbackContainer wrCallback; // write callback
#endif /* USE_ENTITY_WRITE_CALLBACK */

#endif /* USE_ENTITY_CALLBACKS */

    u32     bitFlags; //bit[0] - read only, bit[1] - isParam, bit[2] - is log, bit[3] - is pointer ... see bit flags mask
    u32     shift;
    u8      type;
    char    descr[ENTITY_DESCRIPTION_SIZE];
};

struct Entity {
    void*           pointer;

#ifdef USE_ENTITY_USER_SPACE
    u8              isHeap;
#endif /* USE_ENTITY_USER_SPACE */

    char            descr[ENTITY_DESCRIPTION_SIZE];
    u16             fields_count;
    EntityField*    fields;
};

/**********************************************************************************************************************************************
 *  Entity info structure
 */

typedef struct {
#ifdef USE_ENTITY_PING
    b           userInitReady;
#endif /* USE_ENTITY_PING */

    reg         allocated_entity_pointers;
    reg         entities_count;
    Entity**    entities;
} EntityInfo;

extern EntityInfo entityInfo;  // global variable entities for user projects (adds sizeof(EntityInfo) to .data section)


/*
 * **********************************************************************************************************************************
 *  help functions
 * **********************************************************************************************************************************
 */

// getter entity pointer by entity number
STATIC_FORCEINLINE Entity * getEntityPointer(TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber)
{
    if(entityNumber < entityInfo.entities_count) {
        return entityInfo.entities[entityNumber];
    }
    return NULLPTR(Entity*);
}

// getter field value pointer by entity number and field number
STATIC_FORCEINLINE EntityField * getFieldPointer(TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, TYPEOF_STRUCT(Entity, fields_count) fieldNumber)
{
    if((entityNumber < entityInfo.entities_count) && (fieldNumber < entityInfo.entities[entityNumber]->fields_count)) {
        return &entityInfo.entities[entityNumber]->fields[fieldNumber];
    }
    return NULLPTR(EntityField*);
}

// getter data pointer by entity number
STATIC_FORCEINLINE void * getVoidPointer(TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber)
{
    if(entityNumber < entityInfo.entities_count) {
        return entityInfo.entities[entityNumber]->pointer;
    }
    return NULL;
}

// getter field value pointer by entity number and field number
STATIC_FORCEINLINE void * getFieldValuePointer(TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, TYPEOF_STRUCT(Entity, fields_count) fieldNumber)
{
    if((entityNumber < entityInfo.entities_count) && (fieldNumber < entityInfo.entities[entityNumber]->fields_count)) {
        return (UINT8_TYPE_DC(entityInfo.entities[entityNumber]->pointer) + entityInfo.entities[entityNumber]->fields[fieldNumber].shift);
    }
    return NULL;
}

// check if entity is exists
STATIC_FORCEINLINE int entityPositionIsExists(TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, TYPEOF_STRUCT(Entity, fields_count) fieldNumber)
{
    if((entityNumber < entityInfo.entities_count) && (fieldNumber < entityInfo.entities[entityNumber]->fields_count)) {
        return ENTITY_OK;
    }
    return ENTITY_ERROR;
}


/*
 * **********************************************************************************************************************************
 * entity new/delete functions
 * **********************************************************************************************************************************
 */

/// delete some entity for external using
void deleteEntitityFieldsExternal(reg entityNumber);

/// delete all entities and deallocation all memory
void deleteEntities(void);

/// allocation new entities pointers
int newEntities(reg numberOfEntities);

/// allocation entitites pointer & fields
int initEntity(reg* entityNumber, reg NumberOfFields, reg pointerSize, char descr[ENTITY_DESCRIPTION_SIZE], b isCustomSpace, b isHeap, void* arg);

/*
 * **********************************************************************************************************************************
 *  field functions
 * **********************************************************************************************************************************
 */

/// init field by Entity pointer and field-number
int initField(Entity * entityInst, reg * fieldNumber, TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, TYPEOF_STRUCT(EntityField, shift) shift, TYPEOF_STRUCT(EntityField, type) type, char descr[ENTITY_DESCRIPTION_SIZE], void * field_ptr);

/// init field by Entity number and field-number
STATIC_FORCEINLINE int initFieldPos(reg entityNumber, reg * fieldNumber, TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, TYPEOF_STRUCT(EntityField, shift) shift, TYPEOF_STRUCT(EntityField, type) type, char descr[ENTITY_DESCRIPTION_SIZE], void * field_ptr)
{
    return initField(getEntityPointer(entityNumber), fieldNumber, bitFlags, shift, type, descr, field_ptr);
}

/// init field-array
int initFieldArray(Entity * entityInst, reg * fieldNumber, TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, TYPEOF_STRUCT(EntityField, shift) shift, TYPEOF_STRUCT(EntityField, type) type, int arrayLen, char descr[ENTITY_DESCRIPTION_SIZE], void * field_ptr, int startNum);

///init existing field by pointer
int initFieldFromPtr(EntityField * fieldInst, TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, TYPEOF_STRUCT(EntityField, shift) shift, TYPEOF_STRUCT(EntityField, type) type, char descr[ENTITY_DESCRIPTION_SIZE]);

/// rename field by field number
int fieldRename(Entity * entityInst, reg fieldNumber, char descr[ENTITY_DESCRIPTION_SIZE]);

/*
 * ****************************************************************************************************
 * callBack functions
 * ****************************************************************************************************
 */

#ifdef USE_ENTITY_CALLBACKS

/// init field with callbacks by field-number
int initFieldCallback(Entity * entityInst, reg * fieldNumber, TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, TYPEOF_STRUCT(EntityField, shift) shift, TYPEOF_STRUCT(EntityField, type) type, char descr[ENTITY_DESCRIPTION_SIZE], void * field_ptr,
                      TYPEOF_STRUCT(entityCallbackContainer, entityCallback) readCallback, TYPEOF_STRUCT(entityCallbackContainer, context) readContext, TYPEOF_STRUCT(entityCallbackContainer, entityCallback) writeCallback, TYPEOF_STRUCT(entityCallbackContainer, context) writeContext);

/// init callback function by fieldNumber
int entityInitCallback(Entity * entityInst, reg filedNumber,
                       TYPEOF_STRUCT(entityCallbackContainer, entityCallback) readCallback, TYPEOF_STRUCT(entityCallbackContainer, context) readContext, TYPEOF_STRUCT(entityCallbackContainer, entityCallback) writeCallback, TYPEOF_STRUCT(entityCallbackContainer, context) writeContext);

/// init callback function by description
int entityInitCallback_txt(Entity * entityInst, char descr[ENTITY_DESCRIPTION_SIZE],
                     TYPEOF_STRUCT(entityCallbackContainer, entityCallback) readCallback, TYPEOF_STRUCT(entityCallbackContainer, context) readContext, TYPEOF_STRUCT(entityCallbackContainer, entityCallback) writeCallback, TYPEOF_STRUCT(entityCallbackContainer, context) writeContext);

#endif /* USE_ENTITY_CALLBACKS */



/*
 * **********************************************************************************************************************************
 *  foreach entities
 * **********************************************************************************************************************************
 */

int foreachEntities(int (*predicate)(reg entityNumber, Entity* entity, reg fieldNumber, EntityField* field, void* val, void* context), void* context);

/// string compleate for entities---------------------------------------------------------------------------------------------------
STATIC_FORCEINLINE int entityDescrNotCompleate(const c8* str1, const c8* str2)
{
#if ENTITY_DESCRIPTION_SIZE == 0x01U
    return ((*str1) == (*str2)) ? 0 : 1;
#elif ENTITY_DESCRIPTION_SIZE == 0x02U
    return (( *UINT16_TYPE_DC(str1) ) == ( *UINT16_TYPE_DC(str2) )) ? 0 : 1;
#elif ENTITY_DESCRIPTION_SIZE == 0x04U
    return (( *UINT32_TYPE_DC(str1) ) == ( *UINT32_TYPE_DC(str2) )) ? 0 : 1;
#elif ENTITY_DESCRIPTION_SIZE == 0x08U
    return (( *UINT64_TYPE_DC(str1) ) == ( *UINT64_TYPE_DC(str2) )) ? 0 : 1;
#else

    int n = ENTITY_DESCRIPTION_SIZE;
    while(n--) {
        if(*str1++ != *str2++) {
            return 1;
        }
    }
    return 0;

#endif /* description complementation function selector */
}



#ifdef USE_ENTITY_PING
/*
 * **********************************************************************************************************************************
 *  user Ready functions
 * **********************************************************************************************************************************
 */
void setEntityReadyState(b state);

#endif /* USE_ENTITY_PING */


#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */

#endif /* ENTITY_MANAGER_H */
