/*--------------------------------------- FILE MUST BE GENERATED (DO NOT EDIT!!!) --------------------------------------------*/
#ifndef ENTITY_READ_SYSTEM_H
#define ENTITY_READ_SYSTEM_H

#include "entity_port.h"

#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#ifdef USE_ENTITY_READ_SERVICE

#include "entity_read_parent_pool.h"

typedef struct {

    /* BEGIN GENERATION CODE */
    // creation fields as a structure fields
    ENTITY_READ_CHIELD_DECL(u8, name);
    ENTITY_READ_CHIELD_DECL(u16, name1);

    /* END GENERATION CODE */

    EntityWritePoolContainer_t writePool;   // for transmitting only changed fields
    EntityReadPoolContainer_t  readPool;    // for fast searching fields by boardNumber, entityNumber and fieldNumber if data is received
} EntityReadSystem_t;

extern EntityReadSystem_t ersys;


int initEntityReadSystem(void);


#endif /* USE_ENTITY_READ_SERVICE */

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */


#endif /* ENTITY_READ_SYSTEM_H */
/*--------------------------------------- FILE MUST BE GENERATED (DO NOT EDIT!!!) --------------------------------------------*/
