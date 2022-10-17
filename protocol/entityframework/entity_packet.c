#include "entity_packet.h"


#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#include "entity_manager.h"
#include "smart_assert.h"
#include "my_ctype_cast.h"

#ifdef USE_ENTITY_ATOMIC
#include "entity_atomic.h"
#endif //USE_ENTITY_ATOMIC


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
    M_Assert_Break((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities)) || (maxOutBufferSize < (8 + TYPE_ARRAY_LENGTH)) || outputData == NULL || size == NULL), M_EMPTY, return ENTITY_ERROR, "readEntitiesSizes: No allocated entities or invalid input");
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

    (*size) = pos;
    return ENTITY_OK;
}

int readEntitiesDescriptions(TYPEOF_STRUCT(EntityInfo, entities_count) startEntityNumber, TYPEOF_STRUCT(EntityInfo, entities_count) len, u8 *outputData, reg *size, reg maxOutBufferSize)
{   //send {entities[0].descr, ... entities[entities_count].descr}

#if (MAX_NUBER_OF_ENTITIES < 256U)
    M_Assert_Break((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities)) || (maxOutBufferSize < 3) || (outputData == NULL) || (size == NULL)), M_EMPTY, return ENTITY_ERROR, "readEntitiesDescriptions: No allocated entities or invalid input");
#else
    M_Assert_Break((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities)) || (maxOutBufferSize < 4) || (outputData == NULL) || (size == NULL)), M_EMPTY, return ENTITY_ERROR, "readEntitiesDescriptions: No allocated entities or invalid input");
#endif /* MAX_NUBER_OF_ENTITIES < 256 */

    if(startEntityNumber < entityInfo.entities_count) {
        reg pos = 0;

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

            M_Assert_BreakSaveCheck( (pos + ENTITY_DESCRIPTION_SIZE) > maxOutBufferSize, M_EMPTY, {
                                        outputData[0] = ENTITY_ERROR;
                                        (*size) = 1;
                                        return ENTITY_ERROR;
                                    }, "readEntitiesDescriptions: description read size more than buffer");

            ENTITY_BYTE_CPY(ENTITY_DESCRIPTION_SIZE, (u8 *)entityInfo.entities[i]->descr, &outputData[pos]);
            pos += ENTITY_DESCRIPTION_SIZE;
        }

        (*size) = pos;
    } else {
        outputData[0] = ENTITY_ERROR;
        (*size) = 1;
        return ENTITY_ERROR;
    }

    return ENTITY_OK;
}

int readEntityFields(TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, TYPEOF_STRUCT(Entity, fields_count) startFieldNumber, TYPEOF_STRUCT(Entity, fields_count) len, u8 *outputData, reg *size, reg maxOutBufferSize)
{   //send {entities[entityNum].fields[startFieldNum], ... entities[num].fields[entities[num].fields_count]}

#if (MAX_NUBER_OF_FIELDS < 256U)
    M_Assert_Break((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities)) || (maxOutBufferSize < 3) || (outputData == NULL) || (size == NULL)), M_EMPTY, return ENTITY_ERROR, "readEntityFields: No allocated entities or invalid input");
#else
    M_Assert_Break((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities)) || (maxOutBufferSize < 5) || (outputData == NULL) || (size == NULL)), M_EMPTY, return ENTITY_ERROR, "readEntityFields: No allocated entities or invalid input");
#endif /* MAX_NUBER_OF_FIELDS < 256 */

    if((entityNumber < entityInfo.entities_count) && (startFieldNumber < entityInfo.entities[entityNumber]->fields_count)) {
        reg pos = 0;

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

            M_Assert_BreakSaveCheck( ((pos + (sizeof(entityInfo.entities[entityNumber]->fields[i].bitFlags) + sizeof(entityInfo.entities[entityNumber]->fields[i].shift) + 1 + ENTITY_DESCRIPTION_SIZE)) > maxOutBufferSize), M_EMPTY, {
                               outputData[0] = ENTITY_ERROR;
                               (*size) = 1;
                               return ENTITY_ERROR;
                           }, "readEntityFields: fields read size more than buffer");

            ENTITY_BYTE_CPY(sizeof(entityInfo.entities[entityNumber]->fields[i].bitFlags), (u8 *)&entityInfo.entities[entityNumber]->fields[i].bitFlags, &outputData[pos]);
            pos += sizeof(entityInfo.entities[entityNumber]->fields[i].bitFlags);

            ENTITY_BYTE_CPY(sizeof(entityInfo.entities[entityNumber]->fields[i].shift), (u8 *)&entityInfo.entities[entityNumber]->fields[i].shift, &outputData[pos]);
            pos += sizeof(entityInfo.entities[entityNumber]->fields[i].shift);

            outputData[pos++] = (entityInfo.entities[entityNumber]->fields[i].type & 0xFFU);

            ENTITY_BYTE_CPY(ENTITY_DESCRIPTION_SIZE, (u8 *)&entityInfo.entities[entityNumber]->fields[i].descr, &outputData[pos]);
            pos += ENTITY_DESCRIPTION_SIZE;
        }

        (*size) = pos;
    } else {
        outputData[0] = ENTITY_ERROR;
        (*size) = 1;
        return ENTITY_ERROR;
    }

    return ENTITY_OK;
}

/* ******************************************************************************************************************
 * Read one field value
 *
 */
int readFieldValue(TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, TYPEOF_STRUCT(Entity, fields_count) fieldNumber, u8 *outputData, reg *size, reg maxOutBufferSize)
{ //send {0x01 , prt + 0, ... , ptr + getTypeLen(entities[no].fields[fieldNo].type)}
    M_Assert_Break((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities)) || (maxOutBufferSize < 2) || (outputData == NULL) || (size == NULL)), M_EMPTY, return ENTITY_ERROR, "readFieldValue: No allocated entities or invalid input");

    if((entityNumber < entityInfo.entities_count) && (fieldNumber < entityInfo.entities[entityNumber]->fields_count)) {
        reg pos = 0;

        outputData[pos++] = ENTITY_OK;
        outputData[pos++] = entityInfo.entities[entityNumber]->fields[fieldNumber].type;

        u8 typeLen = getMYCTypeLen(entityInfo.entities[entityNumber]->fields[fieldNumber].type);

        M_Assert_BreakSaveCheck(((pos + typeLen) > maxOutBufferSize), M_EMPTY, {
                           outputData[0] = ENTITY_ERROR;
                           (*size) = 1;
                           return ENTITY_ERROR;
                       }, "readFieldValue: field read size more than buffer");

        void* ptr = (entityInfo.entities[entityNumber]->pointer + entityInfo.entities[entityNumber]->fields[fieldNumber].shift);


#ifdef USE_ENTITY_ATOMIC
        ATOMIC_BLOCK_RESTORATE_COND((entityInfo.entities[entityNumber]->fields[fieldNumber].bitFlags & ENTITY_ATOMIC_MSK), {
#endif /* USE_ENTITY_ATOMIC */

#   if defined(USE_ENTITY_POINTER) && defined(USE_ENTITY_REGISTER)
                                        if(entityInfo.entities[entityNumber]->fields[fieldNumber].bitFlags & (ENTITY_POINTER_MSK | ENTITY_REGISTER_MSK)) {
                                            volatile reg* reg_ptr_from = (volatile reg*) (* REG_TYPE_DC(ptr));
                                            volatile reg* reg_ptr_to   = (volatile reg*) (  &outputData[pos]);
                                            if(reg_ptr_from) {
                                                ENTITY_REG_CPY(reg_ptr_from, reg_ptr_to);
                                            }
                                        }

                                        else

#   endif /* defined(USE_ENTITY_POINTER) && defined(USE_ENTITY_REGISTER) */


#   if defined(USE_ENTITY_REGISTER)

                                        if(entityInfo.entities[entityNumber]->fields[fieldNumber].bitFlags & ENTITY_REGISTER_MSK) {
                                            volatile reg* reg_ptr_from = ((volatile reg*)   ptr);
                                            volatile reg* reg_ptr_to   = ((volatile reg*) (&outputData[pos]));
                                            if(reg_ptr_from) {
                                                ENTITY_REG_CPY(reg_ptr_from, reg_ptr_to);
                                            }
                                        }

                                        else

#   endif /* defined(USE_ENTITY_REGISTER) */


#   if defined(USE_ENTITY_POINTER)

                                        if(entityInfo.entities[entityNumber]->fields[fieldNumber].bitFlags & ENTITY_POINTER_MSK) {
                                            u8* reg_ptr_from = (u8 *)(* REG_TYPE_DC(ptr));
                                            if(reg_ptr_from) {
                                                ENTITY_BYTE_CPY(typeLen, reg_ptr_from, &outputData[pos]);
                                            }
                                        }

                                        else

#   endif /* defined(USE_ENTITY_POINTER) */

                                        {
                                            ENTITY_BYTE_CPY(typeLen, (u8*)ptr, &outputData[pos]);
                                        }
#ifdef USE_ENTITY_ATOMIC
                                    });
#endif /* USE_ENTITY_ATOMIC */



#ifdef USE_ENTITY_READ_CALLBACK
        if(entityInfo.entities[entityNumber]->fields[fieldNumber].rdCallback.entityCallback != NULLPTR(TYPEOF_STRUCT(entityCallbackContainer, entityCallback))) {
            entityInfo.entities[entityNumber]->fields[fieldNumber].rdCallback.entityCallback(entityInfo.entities[entityNumber], &entityInfo.entities[entityNumber]->fields[fieldNumber], ptr, entityInfo.entities[entityNumber]->fields[fieldNumber].rdCallback.context);
        }
#endif /* USE_ENTITY_READ_CALLBACK */

        pos += typeLen;
        (*size) = pos;

    } else {
        outputData[0] = ENTITY_ERROR;
        (*size) = 1;
        return ENTITY_ERROR;
    }

    UNUSED(maxOutBufferSize);
    return ENTITY_OK;
}

/* ******************************************************************************************************************
 * WRITE one field value
 *
 */
void set_field_value(TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, TYPEOF_STRUCT(Entity, fields_count) fieldNumber, u8* data)
{

}



















#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */
