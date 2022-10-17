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
#define ENTITY_POINTER_MSK 		0x00000008U // its pointer, must specify reg in entity_types.h (reading and writing from addres which store in this field)
#define ENTITY_REGISTER_MSK 	0x00000010U // its register, must specify reg and ENTITY_PERIF_BUS_TYPE_IND in entity_types.h (bus aligned read and write, it happens for one insrtuction, must uncommitted USE_ENTITY_REGISTER)
#define ENTITY_ATOMIC_MSK 		0x00000020U // its atomic field must specify entity_atomic.h file (before write and read disabling interrupts and after restore it, must uncommitted USE_ENTITY_ATOMIC)
#define ENTITY_LOW_UPDATE_MSK 	0x00000040U // its atomic field must specify entity_atomic.h file (before write and read disabling interrupts and after restore it, must uncommitted USE_ENTITY_ATOMIC)
#define ENTITY_READ_ONCE_MSK 	0x00000080U // its atomic field must specify entity_atomic.h file (before write and read disabling interrupts and after restore it, must uncommitted USE_ENTITY_ATOMIC)
#define ENTITY_ARRAY_MSK        0x00000100U // its atomic field must specify entity_atomic.h file (before write and read disabling interrupts and after restore it, must uncommitted USE_ENTITY_ATOMIC)
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

    u32         allocated_entity_pointers;
    u32         entities_count;
    Entity**    entities;
} EntityInfo;

extern EntityInfo entityInfo;  // global variable entities for user projects (adds sizeof(EntityInfo) to .data section)


/*
 * **********************************************************************************************************************************
 * entity new/delete functions
 * **********************************************************************************************************************************
 */

/// delete some entity for external using
void deleteEntitityFieldsExternal(u32 entityNumber);

/// delete all entities and deallocation all memory
void deleteEntities(void);

/// allocation new entities pointers
int newEntities(u32 numberOfEntities);

/// allocation entitites pointer & fields
int initEntity(u32 NumberOfFields, reg pointerSize, char descr[ENTITY_DESCRIPTION_SIZE], b isCustomSpace, b isHeap, void* arg);

/*
 * **********************************************************************************************************************************
 *  field functions
 * **********************************************************************************************************************************
 */

/// init field by field-number
int initField(Entity * entityInst, int * fieldNumber, TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, TYPEOF_STRUCT(EntityField, shift) shift, TYPEOF_STRUCT(EntityField, type) type, char descr[ENTITY_DESCRIPTION_SIZE], void * field_ptr);

/// init field-array
int initFieldArray(Entity * entityInst, int * fieldNumber, TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, TYPEOF_STRUCT(EntityField, shift) shift, TYPEOF_STRUCT(EntityField, type) type, int arrayLen, char descr[ENTITY_DESCRIPTION_SIZE], void * field_ptr, int startNum);

///init existing field by pointer
int initFieldFromPtr(EntityField * fieldInst, TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, TYPEOF_STRUCT(EntityField, shift) shift, TYPEOF_STRUCT(EntityField, type) type, char descr[ENTITY_DESCRIPTION_SIZE]);

/// rename field by field number
int fieldRename(Entity * entityInst, int fieldNumber, char descr[ENTITY_DESCRIPTION_SIZE]);

/*
 * ****************************************************************************************************
 * callBack functions
 * ****************************************************************************************************
 */

#ifdef USE_ENTITY_CALLBACKS

/// init field with callbacks by field-number
int initFieldCallback(Entity * entityInst, int * fieldNumber, TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, TYPEOF_STRUCT(EntityField, shift) shift, TYPEOF_STRUCT(EntityField, type) type, char descr[ENTITY_DESCRIPTION_SIZE], void * field_ptr,
                      TYPEOF_STRUCT(entityCallbackContainer, entityCallback) readCallback, TYPEOF_STRUCT(entityCallbackContainer, context) readContext, TYPEOF_STRUCT(entityCallbackContainer, entityCallback) writeCallback, TYPEOF_STRUCT(entityCallbackContainer, context) writeContext);

/// init callback function by fieldNumber
int entityInitCallback(Entity * entityInst, int filedNumber,
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

int foreachEntities(int (*predicate)(int entityNumber, Entity* entity, int fieldNumber, EntityField* field, void* context), void* context);

/// string compleate for entities---------------------------------------------------------------------------------------------------
int entityDescrNotCompleate(const c8* str1, const c8* str2);




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
