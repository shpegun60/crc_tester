#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "my_ctypes.h"
#include "entity_port.h"

/**************************************************************************************************************************************************
 * main struct types definition
 */
typedef struct EntityField EntityField;
typedef struct Entity Entity;


/**************************************************************************************************************************************************
 * callback type definition
 */

/**
  * @brief  entityCallback[x](entity, field, args). Pointer to function array callbacks. [0] -> write callback; [1] -> read callback
  * @param  entity - pointer to Entity
  * @param  Field_ptr  - pointer to EntityField
  * @param  args   - pointer to data for callaback. [0] - write args; [1] - read args (programmer must cast to other last defined value)
  * @retval None
  */
typedef void (*entityCallback)(Entity* entity, EntityField* field, void* args);


/**************************************************************************************************************************************************
 * bit flags mask for value name "u8 bitFlags" in EntityField
 */
#define ENTITY_EMPTY_FLAG    	0x00U // nothing (ordinary field)
#define ENTITY_READ_ONLY_MSK 	0x01U // its read only field (write protected field)
#define ENTITY_PARAM_MSK 		0x02U // its parameter field (writed to flash once and read before start, must uncommitted USE_ENTITY_FLASH)
#define ENTITY_LOG_MSK 			0x04U // its log (writed to flash continious, must uncommitted USE_ENTITY_FLASH)
#define ENTITY_POINTER_MSK 		0x08U // its pointer, must specify reg in entity_types.h (reading and writing from addres which store in this field)
#define ENTITY_REGISTER_MSK 	0x10U // its register, must specify reg and ENTITY_PERIF_BUS_TYPE_IND in entity_types.h (bus aligned read and write, it happens for one insrtuction, must uncommitted USE_ENTITY_REGISTER)
#define ENTITY_ATOMIC_MSK 		0x20U // its atomic field must specify entity_atomic.h file (before write and read disabling interrupts and after restore it, must uncommitted USE_ENTITY_ATOMIC)
#define ENTITY_ALL_BIT_MSK 		0xFFU // its mask for counting all fields

/**************************************************************************************************************************************************
 * main structs definition
 */
struct EntityField {

#ifdef USE_ENTITY_CALLBACKS

#ifdef USE_ENTITY_READ_CALLBACK
    entityCallback rdCallback;      // read callback
    void *rdArgs;                   // read callback arguments pointer
#endif /* USE_ENTITY_READ_CALLBACK */

#ifdef USE_ENTITY_WRITE_CALLBACK
    entityCallback wrCallback;      // write callback
    void *wrArgs;                   // write callback arguments pointer
#endif /* USE_ENTITY_WRITE_CALLBACK */

#endif /* USE_ENTITY_CALLBACKS */

    u8  bitFlags; //bit[0] - read only, bit[1] - isParam, bit[2] - is log, bit[3] - is pointer ... see bit flags mask
    u16 shift;
    u8  type;
    char descr[ENTITY_DECRIPTION_SIZE];
};

struct Entity {
    void *pointer;
    char descr[ENTITY_DECRIPTION_SIZE];
    u8 fields_count;
    EntityField* fields;
};

#endif // ENTITY_MANAGER_H
