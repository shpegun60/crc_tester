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
 * Entity test enable/disable
 */
#ifndef ENTITY_DESCRIPTION_SIZE
//#   define ENTITY_TEST_DISABLE 	1
#endif /* ENTITY_DESCRIPTION_SIZE */


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
#   define OPTIMIZE_ENTITY_BUFFER_CHECK_AFTER_RELEASE
#endif /* OPTIMIZE_ENTITY_BUFFER_CHECK_AFTER_RELEASE */

#ifdef OPTIMIZE_ENTITY_BUFFER_CHECK_AFTER_RELEASE
#   define ENTITY_DBG_ASSERT_BUF M_Assert_Break
//#   define ENTITY_DBG_ASSERT_BUF(...)
#else
#   define ENTITY_DBG_ASSERT_BUF M_Assert_BreakSaveCheck
//#   define ENTITY_DBG_ASSERT_BUF M_Assert_Break
#endif /* OPTIMIZE_ENTITY_BUFFER_CHECK_AFTER_RELEASE */

/* **********************************************************************************************************************************
 *
 * **********************************************************************************************************************************
 */

#ifndef USE_ENTITY_MAIL_SERVICE
#   define USE_ENTITY_MAIL_SERVICE
#endif /* USE_ENTITY_MAIL_SERVICE */


#ifndef USE_ENTITY_READ_SERVICE
#   define USE_ENTITY_READ_SERVICE
#endif /* USE_ENTITY_READ_SERVICE */

/* **********************************************************************************************************************************
 *  Macro for Defining Entity Copy function (Platform depent)
 * **********************************************************************************************************************************
 */
#if defined(__LITTLE_ENDIAN__)
#   define ENTITY_BYTE_CPY(n, from, to) MY_CTYPE_USER_DATA_MEMCPY((n), (from), (to))
#   define ENTITY_TYPE_CPY(type, from, to) cTypeMemcpy((type), (from), (to))
#   define ENTITY_REG_CPY(from, to) MY_CTYPE_COPY_REGISTERS((from), (to))
#else /* defined(__BIG_ENDIAN__) */
#   define ENTITY_BYTE_CPY(n, from, to) MY_CTYPE_USER_DATA_REVCPY((n), (from), (to))
#   define ENTITY_TYPE_CPY(type, from, to) cTypeRevcpy((type), (from), (to))
#   define ENTITY_REG_CPY(from, to) MY_CTYPE_REVCOPY_REGISTERS((from), (to))
#endif /* ORDER SELECTION */


/* **********************************************************************************************************************************
 *  ERROR / OK macro
 */

#define ENTITY_ERROR            0x00
#define ENTITY_OK               0x01
#define ENTITY_NUMBER_ERROR     0x00010000UL
//------------------------------------

/* **********************************************************************************************************************************
 *  COMMANDS macro
 */
// general commands
#define READ_ENTITY_DESCRIPTIONS 	0x00U
#define READ_ENTITY_FIELDS 			0x01U
#define READ_FIELD_VALUE 			0x02U
#define WRITE_FIELD_VALUE 			0x03U
#define READ_SEVERAL_VALUES 		0x04U
#define WRITE_SEVERAL_VALUES 		0x05U
// glued command (for read/write in per one packet)
#define READ_SEVERAL_VALUES_GLUED 	0x06U
#define WRITE_SEVERAL_VALUES_GLUED 	0x07U
#define SEVERAL_VALUES_GLUED_END 	0x09U
// flash operations
#define ENTITY_FLASH_OPERATION 		0x08U
// flash operations

// mail service for stream from device
#define ENTITY_MAIL_SERVICE_PACK    0x33U

// entity read type dimensions
#define ENTITY_READ_DIMENSIONS      0x0AU



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
