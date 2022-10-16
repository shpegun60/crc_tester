#include "entity_packet.h"


#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#include "entity_manager.h"
#include "smart_assert.h"


/*------------------- Packet formaters function`s ----------------------------------
 * basic`s
 */

#ifdef USE_ENTITY_PING

b entities_ping(void)
{
    return entityInfo.userInitReady;
}

#endif /* USE_ENTITY_PING */

int readEntitiesSizes(u8 *outputData, reg *size, reg maxOutBufferSize)
{
    M_Assert_Break((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities)) || (maxOutBufferSize < (8 + TYPE_ARRAY_LENGTH)) || outputData == NULL || size == NULL), M_EMPTY, return ENTITY_ERROR, "read_entities_size: No allocated entities or invalid input");
    reg pos = 0;

    // main sizes -----------------------------------------------------------------------------------------
    outputData[pos++] = ENTITY_OK;                              // OK byte
    outputData[pos++] = ENTITY_DESCRIPTION_SIZE;                // description sizeof

    // entities sizes -------------------------------------------------------------------------------------
#if (MAX_NUBER_OF_ENTITIES < 256U)
    outputData[pos++] = 1;                                      // entities count sizeof
#else
    outputData[pos++] = 2;                                      // entities count sizeof
#endif /* MAX_NUBER_OF_ENTITIES < 256 */

    // fields sizes --------------------------------------------------------------------------------------
#if (MAX_NUBER_OF_FIELDS < 256U)
    outputData[pos++] = 1;                                      // fields count sizeof
#else
    outputData[pos++] = 2;                                      // fields count sizeof
#endif /* MAX_NUBER_OF_FIELDS < 256 */

    outputData[pos++] = SIZEOF_STRUCT(EntityField, bitFlags);   // fields bitFlags sizeof
    outputData[pos++] = SIZEOF_STRUCT(EntityField, shift);      // fields shift sizeof
    outputData[pos++] = 1;                                      // fields type sizeof

    // system types sizes --------------------------------------------------------------------------------
    outputData[pos++] = TYPE_ARRAY_LENGTH;                      // size array types
    for(int i = 0; i < TYPE_ARRAY_LENGTH; ++i) {
        outputData[pos++] = typeLengthMappingArray[i];          // copy array types
    }

    *size = pos;
    return ENTITY_OK;
}

int readEntitiesDescriptions(TYPEOF_STRUCT(EntityInfo, entities_count) startEntityNumber, TYPEOF_STRUCT(EntityInfo, entities_count) len, u8 *outputData, reg *size, reg maxOutBufferSize)
{   //send {entities[0].descr, ... entities[entities_count].descr}

    M_Assert_Break((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities)) || maxOutBufferSize < 4 || outputData == NULL || size == NULL), M_EMPTY, return ENTITY_ERROR, "read_entities_descriptions: No allocated entities or invalid input");

    reg pos = 0;

    if(startEntityNumber < entityInfo.entities_count) {
        outputData[pos++] = ENTITY_OK;
        outputData[pos++] = ENTITY_DESCRIPTION_SIZE;                // description sizeof

        TYPEOF_STRUCT(EntityInfo, entities_count) tmp = (entityInfo.entities_count - startEntityNumber);
        len = MIN(tmp, len);

#if (MAX_NUBER_OF_ENTITIES < 256U)
        outputData[pos++] = (u8)(len & 0xFFU);
#else
        len &= 0xFFFFUL;
        ENTITY_BYTE_CPY(2, (u8*)&len, &outputData[pos]);
        pos += 2;
#endif /* MAX_NUBER_OF_ENTITIES < 256 */

        len += startEntityNumber;

        for(TYPEOF_STRUCT(EntityInfo, entities_count) i = startEntityNumber; i < len; ++i) {
            if((pos + ENTITY_DESCRIPTION_SIZE) < maxOutBufferSize) {
                ENTITY_BYTE_CPY(ENTITY_DESCRIPTION_SIZE, (u8 *)entityInfo.entities[i]->descr, &outputData[pos]);
                pos += ENTITY_DESCRIPTION_SIZE;
            } else {
                outputData[0] = ENTITY_ERROR;
                *size = 1;
                M_Assert_Break(M_ALWAYS, M_EMPTY, M_EMPTY, "read_entities_descriptions: description read size more than buffer");
                return ENTITY_ERROR;
            }
        }
    } else {
        outputData[pos++] = ENTITY_ERROR;
        *size = 1;
        return ENTITY_ERROR;
    }

    *size = pos;
    return ENTITY_OK;
}

int readEntityFields(TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, TYPEOF_STRUCT(Entity, fields_count) startFieldNumber, TYPEOF_STRUCT(Entity, fields_count) len, u8 *outputData, reg *size, reg maxOutBufferSize)
{	//send {entities[entityNum].fields[startFieldNum], ... entities[num].fields[entities[num].fields_count]}
    M_Assert_Break((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities)) || maxOutBufferSize < 4 || outputData == NULL || size == NULL), M_EMPTY, return ENTITY_ERROR, "read_entities_descriptions: No allocated entities or invalid input");
    reg pos = 0;

    if((entityNumber < entityInfo.entities_count) && (startFieldNumber < entityInfo.entities[entityNumber]->fields_count)) {
        outputData[pos++] = ENTITY_OK;
#if (MAX_NUBER_OF_FIELDS < 256U)
        outputData[pos++] = (u8)(startFieldNumber & 0xFFU);

        TYPEOF_STRUCT(Entity, fields_count) tmp = (entityInfo.entities[entityNumber]->fields_count - startFieldNumber);
        len = MIN(tmp, len);
        outputData[pos++] = (u8)(len & 0xFFU);

#else
        startFieldNumber &= 0xFFFFUL;
        ENTITY_BYTE_CPY(2, (u8*)&startFieldNumber, &outputData[pos]);
        pos += 2;

        len &= 0xFFFFUL;
        TYPEOF_STRUCT(Entity, fields_count) tmp = (entityInfo.entities[entityNumber]->fields_count - startFieldNumber);
        len = MIN(tmp, len);
        ENTITY_BYTE_CPY(2, (u8*)&len, &outputData[pos]);
        pos += 2;

#endif /* MAX_NUBER_OF_FIELDS < 256 */

        len += startFieldNumber;
        for(TYPEOF_STRUCT(Entity, fields_count) i = startFieldNumber; i < len; ++i) {
            if((pos + (sizeof(entityInfo.entities[entityNumber]->fields[i].bitFlags) + sizeof(entityInfo.entities[entityNumber]->fields[i].shift) + 1 + ENTITY_DESCRIPTION_SIZE)) < maxOutBufferSize) {
                ENTITY_BYTE_CPY(sizeof(entityInfo.entities[entityNumber]->fields[i].bitFlags), (u8 *)&entityInfo.entities[entityNumber]->fields[i].bitFlags, &outputData[pos]);
                pos += sizeof(entityInfo.entities[entityNumber]->fields[i].bitFlags);

                ENTITY_BYTE_CPY(sizeof(entityInfo.entities[entityNumber]->fields[i].shift), (u8 *)&entityInfo.entities[entityNumber]->fields[i].shift, &outputData[pos]);
                pos += sizeof(entityInfo.entities[entityNumber]->fields[i].shift);

                outputData[pos++] = (entityInfo.entities[entityNumber]->fields[i].type & 0xFFU);

                ENTITY_BYTE_CPY(ENTITY_DESCRIPTION_SIZE, (u8 *)&entityInfo.entities[entityNumber]->fields[i].descr, &outputData[pos]);
                pos += ENTITY_DESCRIPTION_SIZE;
            } else {
                outputData[0] = ENTITY_ERROR;
                *size = 1;
                M_Assert_Break(M_ALWAYS, M_EMPTY, M_EMPTY, "read_entity_fields: fields read size more than buffer");
                return ENTITY_ERROR;
            }
        }
    } else {
        outputData[pos++] = ENTITY_ERROR;
        *size = 1;
        return ENTITY_ERROR;
    }

    *size = pos;
    return ENTITY_OK;
}

void readFieldValue(TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, TYPEOF_STRUCT(Entity, fields_count) fieldNumber, u8 *outputData, reg *size, reg maxOutBufferSize)
{ //send {0x01 , prt + 0, ... , ptr + getTypeLen(entities[no].fields[fieldNo].type)}

}



#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */
