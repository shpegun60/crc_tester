#ifndef ENTITY_PORT_H
#define ENTITY_PORT_H

#include "my_ctypes.h"
#include "byte_order.h"

/* **********************************************************************************************************************************
 * Entity libraty enable & version info
 *
 * Version: VVRRPP --> VV = Version, RR = Revision, PP = Patch
 * Example: 030200 = 3.2.0
 */
#ifndef C_ENTITY_FRAMEWORK_LIB_ENA
#   define C_ENTITY_FRAMEWORK_LIB_ENA
#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */

#ifdef C_ENTITY_FRAMEWORK_LIB_ENA
#   define C_ENTITY_FRAMEWORK_LIB_MAJOR            2
#   define C_ENTITY_FRAMEWORK_LIB_MINOR            0
#   define C_ENTITY_FRAMEWORK_LIB_PATCHLEVEL       1
#   define C_ENTITY_FRAMEWORK_LIB_VERSION   ((C_ENTITY_FRAMEWORK_LIB_MAJOR * 10000) +   \
                                            (C_ENTITY_FRAMEWORK_LIB_MINOR * 100) +      \
                                            (C_ENTITY_FRAMEWORK_LIB_PATCHLEVEL))
#   define C_ENTITY_FRAMEWORK_LIB_VERSION_INFO "Version: VVRRPP --> VV = Version, RR = Revision, PP = Patch"
#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */


/* **********************************************************************************************************************************
 * Entity description size in bytes
 */
#ifndef ENTITY_DESCRIPTION_SIZE
#   define ENTITY_DESCRIPTION_SIZE 	0x04U
#endif /* ENTITY_DESCRIPTION_SIZE */

/* **********************************************************************************************************************************
 * Maximum number of entities & fields
 * WARNING!!! it changes the entity protocol for read & write, see entity_packet.c
 */
#ifndef MAX_NUBER_OF_ENTITIES
#   define MAX_NUBER_OF_ENTITIES 300
#endif /* MAX_NUBER_OF_ENTITIES */

#ifndef MAX_NUBER_OF_FIELDS
#   define MAX_NUBER_OF_FIELDS 255
#endif /* MAX_NUBER_OF_FIELDS */



#if (MAX_NUBER_OF_ENTITIES < 256U)
#   define ENTITIES_SIZEOF 1U
#else
#   define ENTITIES_SIZEOF 2U
#endif /* (MAX_NUBER_OF_ENTITIES < 256U) */


#if (MAX_NUBER_OF_FIELDS < 256U)
#   define ENTITY_FIELD_SIZEOF 1U
#else
#   define ENTITY_FIELD_SIZEOF 2U
#endif /* (MAX_NUBER_OF_FIELDS < 256U) */


/* **********************************************************************************************************************************
 * USE USER custom space (not allocation for ENTITY POINTER)
 * WARNING!!!! add`s some bytes to Entity struct
 */

#ifndef USE_ENTITY_USER_SPACE
//#   define USE_ENTITY_USER_SPACE
#endif /* USE_ENTITY_USER_SPACE */


/* **********************************************************************************************************************************
 *  ENTITY use callbacks
 *
 *  if you want use read or write callbacks uncommit this defines
 *  WARNING!!!! This feature adds some bytes to each EntityField ==>
 *  =>  sizeof(entityCallbackContainer)  if defined USE_ENTITY_READ_CALLBACK +
 *      sizeof(entityCallbackContainer)  if defined USE_ENTITY_WRITE_CALLBACK
 * **********************************************************************************************************************************
 */

#ifndef USE_ENTITY_CALLBACKS
#   define USE_ENTITY_CALLBACKS
#endif /* USE_ENTITY_CALLBACKS */

#ifdef USE_ENTITY_CALLBACKS

#   ifndef USE_ENTITY_READ_CALLBACK
#       define USE_ENTITY_READ_CALLBACK
#   endif /* USE_ENTITY_READ_CALLBACK */

#   ifndef USE_ENTITY_WRITE_CALLBACK
#       define USE_ENTITY_WRITE_CALLBACK
#   endif /* USE_ENTITY_WRITE_CALLBACK */

#endif /* USE_ENTITY_CALLBACKS */

/* **********************************************************************************************************************************
 *  ENTITY use ping
 *  if you want to use ping function & ready flag define this
 *  WARNING!!! adds some bytes to EntityInfo struct
 * **********************************************************************************************************************************
 */

#ifndef USE_ENTITY_PING
//#   define USE_ENTITY_PING
#endif /* USE_ENTITY_PING */



/* **********************************************************************************************************************************
 *  ENTITY use atomic operations
 *  some fileds may be atomic for read & write
 *  WARNING!!! add's check atomic flag and call some functions before and after each write & read
 *
 *  -->User must be define some atomic functions in entity_atomic.h!!!
 * **********************************************************************************************************************************
 */

#ifndef USE_ENTITY_ATOMIC
//#   define USE_ENTITY_ATOMIC
#endif /* USE_ENTITY_ATOMIC */

#ifdef USE_ENTITY_ATOMIC // atomic functions choose
#   define ENTITY_ATOMIC_STM32
//#   define ENTITY_ATOMIC_ESP32
#endif /* USE_ENTITY_ATOMIC */


/* **********************************************************************************************************************************
 *  ENTITY use pointer (field which contains pointer to other value)
 *  if you want to use entity - pointer define this
 *  WARNING!!! adds some check to entity read & write functions
 * **********************************************************************************************************************************
 */

#ifndef USE_ENTITY_POINTER
#   define USE_ENTITY_POINTER
#endif /* USE_ENTITY_POINTER */

/* **********************************************************************************************************************************
 *  ENTITY use register (bus type atomic read & write)
 *  if you want to use entity - register define this
 *  WARNING!!! adds some check to entity read & write functions
 * **********************************************************************************************************************************
 */

#ifndef USE_ENTITY_REGISTER
#   define USE_ENTITY_REGISTER
#endif /* USE_ENTITY_REGISTER */

/* **********************************************************************************************************************************
 *  Optimization for check buffers after RELEASE see entity_packet.c and smart_asert.h
 * **********************************************************************************************************************************
 */

#ifndef OPTIMIZE_ENTITY_BUFFER_CHECK_AFTER_RELEASE
//#   define OPTIMIZE_ENTITY_BUFFER_CHECK_AFTER_RELEASE
#endif /* OPTIMIZE_ENTITY_BUFFER_CHECK_AFTER_RELEASE */

/* **********************************************************************************************************************************
 *  Macro for Defining Entity Copy function (Platform depent)
 * **********************************************************************************************************************************
 */
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
#   define ENTITY_BYTE_CPY(n, from, to) MY_CTYPE_USER_DATA_MEMCPY((n), (from), (to))
#   define ENTITY_TYPE_CPY(type, from, to) cTypeMemcpy((type), (from), (to))
#   define ENTITY_REG_CPY(from, to) MY_CTYPE_COPY_REGISTERS((from), (to))
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
#   define ENTITY_BYTE_CPY(n, from, to) MY_CTYPE_USER_DATA_REVCPY((n), (from), (to))
#   define ENTITY_TYPE_CPY(type, from, to) cTypeRevcpy((type), (from), (to))
#   define ENTITY_REG_CPY(from, to) MY_CTYPE_REVCOPY_REGISTERS((from), (to))
#else
#   error unsupported endianness
#endif /* MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN */


/* **********************************************************************************************************************************
 *  ERROR / OK macro
 */

#define ENTITY_ERROR 0x00
#define ENTITY_OK 	 0x01
//------------------------------------



/* **********************************************************************************************************************************
 *  USER INPUT CHECK MACRO (do not edit this section!!!!)
 */

#if ENTITY_DESCRIPTION_SIZE > 0xFFU
#   error ENTITY_DESCRIPTION_SIZE is too long
#endif /* ENTITY_DESCRIPTION_SIZE check*/

#if MAX_NUBER_OF_ENTITIES > 0xFFFFUL
#   error MAX_NUBER_OF_ENTITIES is too long
#endif /* MAX_NUBER_OF_ENTITIES check*/

#if MAX_NUBER_OF_FIELDS > 0xFFFFUL
#   error MAX_NUBER_OF_FIELDS is too long
#endif /* MAX_NUBER_OF_FIELDS check*/


#endif /* ENTITY_PORT_H */
