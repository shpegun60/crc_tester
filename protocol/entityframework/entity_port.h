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
 */
#ifndef MAX_NUBER_OF_ENTITIES
#   define MAX_NUBER_OF_ENTITIES 255
#endif /* MAX_NUBER_OF_ENTITIES */

#ifndef MAX_NUBER_OF_FIELDS
#   define MAX_NUBER_OF_FIELDS 255
#endif /* MAX_NUBER_OF_FIELDS */



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
//#   define ENTITY_ATOMIC_STM32
//#   define ENTITY_ATOMIC_ESP32
#endif /* USE_ENTITY_ATOMIC */


/* **********************************************************************************************************************************
 *  Macro for Defining Entity Copy function
 * **********************************************************************************************************************************
 */
#if MY_ENDIAN_ORDER == MY_LITTLE_ENDIAN
#   define ENTITY_BYTE_CPY(n, from, to) MY_CTYPE_USER_DATA_MEMCPY((n), (from), (to))
#   define ENTITY_TYPE_CPY(type, from, to) cTypeMemcpy((type), (from), (to))
#elif MY_ENDIAN_ORDER == MY_BIG_ENDIAN
#   define ENTITY_BYTE_CPY(n, from, to) MY_CTYPE_USER_DATA_REVCPY((n), (from), (to))
#   define ENTITY_TYPE_CPY(type, from, to) cTypeRevcpy((type), (from), (to))
#else
#    error unsupported endianness
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
    #error ENTITY_DESCRIPTION_SIZE is too long
#endif /* ENTITY_DESCRIPTION_SIZE check*/

#if MAX_NUBER_OF_ENTITIES > 0xFFFFUL
    #error MAX_NUBER_OF_ENTITIES is too long
#endif /* MAX_NUBER_OF_ENTITIES check*/

#if MAX_NUBER_OF_FIELDS > 0xFFFFUL
    #error MAX_NUBER_OF_FIELDS is too long
#endif /* MAX_NUBER_OF_FIELDS check*/


#endif /* ENTITY_PORT_H */
