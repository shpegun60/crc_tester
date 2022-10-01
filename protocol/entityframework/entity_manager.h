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
#include "my_ctypes.h"

/**************************************************************************************************************************************************
 * main struct types definition before (for type saving in entityCallback function)
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
  * @param  context     - pointer to context for callback (programmer must cast to other last defined value).
  * @retval None
  */

typedef struct {
    void (*entityCallback)(Entity* entity, EntityField* field, void* context); // callback function
    void* context;                                                             // callback context
} entityCallbackContainer;


/**************************************************************************************************************************************************
 * bit flags mask for value name "u16 bitFlags" in EntityField
 */
#define ENTITY_EMPTY_FLAG    	0x0000U // nothing (ordinary field)
#define ENTITY_READ_ONLY_MSK 	0x0001U // its read only field (write protected field)
#define ENTITY_PARAM_MSK 		0x0002U // its parameter field (writed to flash once and read before start, must uncommitted USE_ENTITY_FLASH)
#define ENTITY_LOG_MSK 			0x0004U // its log (writed to flash continious, must uncommitted USE_ENTITY_FLASH)
#define ENTITY_POINTER_MSK 		0x0008U // its pointer, must specify reg in entity_types.h (reading and writing from addres which store in this field)
#define ENTITY_REGISTER_MSK 	0x0010U // its register, must specify reg and ENTITY_PERIF_BUS_TYPE_IND in entity_types.h (bus aligned read and write, it happens for one insrtuction, must uncommitted USE_ENTITY_REGISTER)
#define ENTITY_ATOMIC_MSK 		0x0020U // its atomic field must specify entity_atomic.h file (before write and read disabling interrupts and after restore it, must uncommitted USE_ENTITY_ATOMIC)
#define ENTITY_LOW_UPDATE_MSK 	0x0040U // its atomic field must specify entity_atomic.h file (before write and read disabling interrupts and after restore it, must uncommitted USE_ENTITY_ATOMIC)
#define ENTITY_READ_ONCE_MSK 	0x0080U // its atomic field must specify entity_atomic.h file (before write and read disabling interrupts and after restore it, must uncommitted USE_ENTITY_ATOMIC)
#define ENTITY_ALL_BIT_MSK 		0xFFFFU // its mask for counting all fields

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

    u16     bitFlags; //bit[0] - read only, bit[1] - isParam, bit[2] - is log, bit[3] - is pointer ... see bit flags mask
    u16     shift;
    u8      type;
    char    descr[ENTITY_DECRIPTION_SIZE];
};

struct Entity {
    void*           pointer;
    char            descr[ENTITY_DECRIPTION_SIZE];
    u16             fields_count;
    EntityField*    fields;
};

/**********************************************************************************************************************************************
 *  Entity info structure
 */

typedef struct {
    u16 allocated_entity_pointers;
    u16 entities_count;
    Entity** entities;
} EntityInfo;

extern EntityInfo entityInfo; // global variable entities for user projects (adds sizeof(EntityInfo) to .data section)



void clearEntities(void);

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */

#endif /* ENTITY_MANAGER_H */
