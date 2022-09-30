#ifndef ENTITY_PORT_H
#define ENTITY_PORT_H

/***************************************************************
 * Entity description size in bytes
 */
#define ENTITY_DECRIPTION_SIZE 	0x04U


/*
 * **********************************************************************************************************************************
 *  define use callbacks
 *
 *  if you want use read and write callbacks uncommit this define
 *  WARNING!!!! This feature adds 4 * sizeof(void pointer) byte to each EntityField
 * **********************************************************************************************************************************
*/
#ifndef USE_ENTITY_CALLBACKS
    #define USE_ENTITY_CALLBACKS
#endif /* USE_ENTITY_CALLBACKS */

#ifdef USE_ENTITY_CALLBACKS
    #ifndef USE_ENTITY_READ_CALLBACK
        #define USE_ENTITY_READ_CALLBACK
    #endif /* USE_ENTITY_READ_CALLBACK */

    #ifndef USE_ENTITY_WRITE_CALLBACK
        #define USE_ENTITY_WRITE_CALLBACK
    #endif /* USE_ENTITY_WRITE_CALLBACK */
#endif /* USE_ENTITY_CALLBACKS */


#endif // ENTITY_PORT_H
