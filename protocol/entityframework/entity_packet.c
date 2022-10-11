#include "entity_packet.h"


#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#include "entity_manager.h"
#include "smart_assert.h"


/*------------------- Packet formaters function`s ----------------------------------
 * basic`s
 */

int read_entities_descriptions(TYPEOF_STRUCT(EntityInfo, entities_count) startEntityNumber, TYPEOF_STRUCT(EntityInfo, entities_count) len, u8 *outputData, reg *size, reg maxOutBufferSize)
{   //send {entities[0].descr, ... entities[entities_count].descr}

    M_Assert_Break((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities)) || maxOutBufferSize == 0), M_EMPTY, return ENTITY_ERROR, "read_entities_descriptions: No allocated entities or buffer size is 0, read false");

    reg pos = 0;

    if(startEntityNumber < entityInfo.entities_count) {

        return ENTITY_OK;
    } else {

    }

    *size = pos;
    return ENTITY_ERROR;
}


#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */
