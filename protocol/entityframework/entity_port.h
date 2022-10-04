#ifndef ENTITY_PORT_H
#define ENTITY_PORT_H

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
#ifndef ENTITY_DECRIPTION_SIZE
#   define ENTITY_DECRIPTION_SIZE 	0x04U
#endif /* ENTITY_DECRIPTION_SIZE */

/* **********************************************************************************************************************************
 * Maximum number of entity
 */
#ifndef MAX_NUBER_OF_ENTITIES
#   define MAX_NUBER_OF_ENTITIES 255
#endif /* MAX_NUBER_OF_ENTITIES */

/* **********************************************************************************************************************************
 * USE USER custom space (not allocation for ENTITY POINTER)
 */


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
 *  ERROR / OK macro
 */

#define ENTITY_ERROR 0x00
#define ENTITY_OK 	 0x01
//------------------------------------



/* **********************************************************************************************************************************
 *  USER INPUT CHECK MACRO (do not edit this section!!!!)
 */

#if ENTITY_DECRIPTION_SIZE > 0xFFU
    #error ENTITY_DECRIPTION_SIZE is too long
#endif /* ENTITY_DECRIPTION_SIZE check*/


#endif /* ENTITY_PORT_H */
