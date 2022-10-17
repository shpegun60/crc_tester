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
    outputData[pos++] = ENTITIES_SIZEOF;                        // entities count sizeof

    // fields sizes --------------------------------------------------------------------------------------
    outputData[pos++] = ENTITY_FIELD_SIZEOF;                    // fields count sizeof


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

    M_Assert_Break((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities)) || (maxOutBufferSize < (2 + ENTITIES_SIZEOF)) || (outputData == NULL) || (size == NULL)), M_EMPTY, return ENTITY_ERROR, "readEntitiesDescriptions: No allocated entities or invalid input");

    if(startEntityNumber < entityInfo.entities_count) {
        reg pos = 0;

        outputData[pos++] = ENTITY_OK;
        outputData[pos++] = ENTITY_DESCRIPTION_SIZE;                // description sizeof

        TYPEOF_STRUCT(EntityInfo, entities_count) tmp = (entityInfo.entities_count - startEntityNumber);
        len = MIN(tmp, len);

#if (MAX_NUBER_OF_ENTITIES < 256U) // WARNING!!!!!!!!! DO NOT SUPPORT ON OLD VERSION-----------------------------------------------------------------------------------------------------------------------------------------------------------------
        outputData[pos++] = (u8)(len & 0xFFU);
#else
        len &= 0xFFFFUL;
        ENTITY_BYTE_CPY(ENTITIES_SIZEOF, (u8*)&len, &outputData[pos]);
        pos += ENTITIES_SIZEOF;
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

    M_Assert_Break((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities)) || (maxOutBufferSize < (1 + (ENTITY_FIELD_SIZEOF << 1))) || (outputData == NULL) || (size == NULL)), M_EMPTY, return ENTITY_ERROR, "readEntityFields: No allocated entities or invalid input");

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
        ENTITY_BYTE_CPY(ENTITY_FIELD_SIZEOF, (u8*)&startFieldNumber, &outputData[pos]);
        pos += ENTITY_FIELD_SIZEOF;

        len &= 0xFFFFUL;
        TYPEOF_STRUCT(Entity, fields_count) tmp = (entityInfo.entities[entityNumber]->fields_count - startFieldNumber);
        len = MIN(tmp, len);
        ENTITY_BYTE_CPY(ENTITY_FIELD_SIZEOF, (u8*)&len, &outputData[pos]);
        pos += ENTITY_FIELD_SIZEOF;

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


STATIC_FORCEINLINE void proceedReadEntity(TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, void* ptr, u8* outputData, u8 typeLen)
{

#ifdef USE_ENTITY_ATOMIC
    ATOMIC_BLOCK_RESTORATE_COND((entityInfo.entities[entityNumber]->fields[fieldNumber].bitFlags & ENTITY_ATOMIC_MSK), {
                                #endif /* USE_ENTITY_ATOMIC */

                                #if defined(USE_ENTITY_POINTER) && defined(USE_ENTITY_REGISTER)
                                    if((bitFlags) & (ENTITY_POINTER_MSK | ENTITY_REGISTER_MSK)) {
                                        volatile reg* reg_ptr_from = (volatile reg*) (* REG_TYPE_DC(ptr));
                                        volatile reg* reg_ptr_to   = (volatile reg*) (  outputData);
                                        if(reg_ptr_from) {
                                            ENTITY_REG_CPY(reg_ptr_from, reg_ptr_to);
                                        }
                                    }

                                    else

                                #endif /* defined(USE_ENTITY_POINTER) && defined(USE_ENTITY_REGISTER) */


                                #if defined(USE_ENTITY_REGISTER)

                                    if((bitFlags) & ENTITY_REGISTER_MSK) {
                                        volatile reg* reg_ptr_from = ((volatile reg*)   ptr);
                                        volatile reg* reg_ptr_to   = ((volatile reg*) (outputData));
                                        if(reg_ptr_from) {
                                            ENTITY_REG_CPY(reg_ptr_from, reg_ptr_to);
                                        }
                                    }

                                    else

                                #endif /* defined(USE_ENTITY_REGISTER) */


                                #if defined(USE_ENTITY_POINTER)

                                    if((bitFlags) & ENTITY_POINTER_MSK) {
                                        u8* reg_ptr_from = (u8 *)(* REG_TYPE_DC(ptr));
                                        if(reg_ptr_from) {
                                            ENTITY_BYTE_CPY(typeLen, reg_ptr_from, outputData);
                                        }
                                    }

                                    else

                                #endif /* defined(USE_ENTITY_POINTER) */

                                    {
                                        ENTITY_BYTE_CPY(typeLen, (u8*)ptr, outputData);
                                    }
                                #ifdef USE_ENTITY_ATOMIC
                                });
#endif /* USE_ENTITY_ATOMIC */

}

STATIC_FORCEINLINE void proceedWriteEntity(TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, void* ptr, u8* inputData, u8 typeLen)
{
#ifdef USE_ENTITY_ATOMIC
    ATOMIC_BLOCK_RESTORATE_COND((entityInfo.entities[entityNumber]->fields[fieldNumber].bitFlags & ENTITY_ATOMIC_MSK), {
                                #endif /* USE_ENTITY_ATOMIC */

                                #if defined(USE_ENTITY_POINTER) && defined(USE_ENTITY_REGISTER)
                                    if((bitFlags) & (ENTITY_POINTER_MSK | ENTITY_REGISTER_MSK)) {
                                        volatile reg* reg_ptr_to        = (volatile reg*) (* REG_TYPE_DC(ptr));
                                        volatile reg* reg_ptr_from      = (volatile reg*) (  inputData);
                                        if(reg_ptr_to) {
                                            ENTITY_REG_CPY(reg_ptr_from, reg_ptr_to);
                                        }
                                    }

                                    else

                                #endif /* defined(USE_ENTITY_POINTER) && defined(USE_ENTITY_REGISTER) */


                                #if defined(USE_ENTITY_REGISTER)

                                    if((bitFlags) & ENTITY_REGISTER_MSK) {
                                        volatile reg* reg_ptr_to        = ((volatile reg*)   ptr);
                                        volatile reg* reg_ptr_from      = ((volatile reg*)  (inputData));
                                        if(reg_ptr_to) {
                                            ENTITY_REG_CPY(reg_ptr_from, reg_ptr_to);
                                        }
                                    }

                                    else

                                #endif /* defined(USE_ENTITY_REGISTER) */


                                #if defined(USE_ENTITY_POINTER)

                                    if((bitFlags) & ENTITY_POINTER_MSK) {
                                        u8* reg_ptr_to = (u8 *)(* REG_TYPE_DC(ptr));
                                        if(reg_ptr_to) {
                                            ENTITY_BYTE_CPY(typeLen, inputData, reg_ptr_to);
                                        }
                                    }

                                    else

                                #endif /* defined(USE_ENTITY_POINTER) */

                                    {
                                        ENTITY_BYTE_CPY(typeLen, inputData, (u8*)ptr);
                                    }
                                #ifdef USE_ENTITY_ATOMIC
                                });
#endif /* USE_ENTITY_ATOMIC */
}



/* ******************************************************************************************************************
 * READ one field value
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

        proceedReadEntity(entityInfo.entities[entityNumber]->fields[fieldNumber].bitFlags, ptr, &outputData[pos], typeLen);


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
 * READ several fields values
 *
 */

int readSeveralFieldsValues(u8 *inputData, u8 *outputData, reg *size, reg maxOutBufferSize)
{
    M_Assert_Break((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities)) || (maxOutBufferSize < (1 + (ENTITIES_SIZEOF + ENTITY_FIELD_SIZEOF))) || (outputData == NULL) || (inputData == NULL) || (size == NULL)), M_EMPTY, return ENTITY_ERROR, "readSeveralFieldsValues: No allocated entities or invalid input");

    reg Wpos = 0;
    reg Rpos = 0;
    void* ptr;
    TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber;
    TYPEOF_STRUCT(Entity, fields_count) fieldNumber;

    outputData[Wpos++] = ENTITY_OK;

    while((Rpos + (ENTITIES_SIZEOF + ENTITY_FIELD_SIZEOF)) < (*size)) {

#if (MAX_NUBER_OF_ENTITIES < 256U)
        entityNumber = inputData[Rpos++];
#else
        ENTITY_BYTE_CPY(ENTITIES_SIZEOF, &inputData[Rpos], (u8*)&entityNumber);
        entityNumber &= 0x0000FFFFUL;
        Rpos += ENTITIES_SIZEOF;
#endif /* (MAX_NUBER_OF_ENTITIES < 256U) */


#if (MAX_NUBER_OF_FIELDS < 256U)
        fieldNumber = inputData[Rpos++];
#else
        ENTITY_BYTE_CPY(ENTITIES_SIZEOF, &inputData[Rpos], (u8*)&fieldNumber);
        fieldNumber &= 0x0000FFFFUL;
        Rpos += ENTITY_FIELD_SIZEOF;
#endif /* (MAX_NUBER_OF_FIELDS < 256U) */

        if((entityNumber < entityInfo.entities_count) && (fieldNumber < entityInfo.entities[entityNumber]->fields_count)) {

            u8 typeLen = getMYCTypeLen(entityInfo.entities[entityNumber]->fields[fieldNumber].type);

            M_Assert_BreakSaveCheck(((Wpos + typeLen) > maxOutBufferSize), M_EMPTY, {
                                        outputData[0] = ENTITY_ERROR;
                                        (*size) = 1;
                                        return ENTITY_ERROR;
                                    }, "readSeveralFieldsValues: field read size more than buffer");


            ptr = (entityInfo.entities[entityNumber]->pointer + entityInfo.entities[entityNumber]->fields[fieldNumber].shift);
            proceedReadEntity(entityInfo.entities[entityNumber]->fields[fieldNumber].bitFlags, ptr, &outputData[Wpos], typeLen);



#ifdef USE_ENTITY_READ_CALLBACK
            if(entityInfo.entities[entityNumber]->fields[fieldNumber].rdCallback.entityCallback != NULLPTR(TYPEOF_STRUCT(entityCallbackContainer, entityCallback))) {
                entityInfo.entities[entityNumber]->fields[fieldNumber].rdCallback.entityCallback(entityInfo.entities[entityNumber], &entityInfo.entities[entityNumber]->fields[fieldNumber], ptr, entityInfo.entities[entityNumber]->fields[fieldNumber].rdCallback.context);
            }
#endif /* USE_ENTITY_READ_CALLBACK */

            Wpos += typeLen;

        } else {
            Wpos = 0;
            outputData[Wpos++] = ENTITY_ERROR;
            ENTITY_BYTE_CPY(ENTITIES_SIZEOF,     &outputData[Wpos], (u8*)&entityNumber);
            ENTITY_BYTE_CPY(ENTITY_FIELD_SIZEOF, &outputData[Wpos], (u8*)&fieldNumber );
            (*size) = Wpos;
            return ENTITY_ERROR;
        }
    }


    (*size) = Wpos;
    return ENTITY_OK;
}


/* ******************************************************************************************************************
 * WRITE one field value
 *
 */
int setFieldValue(TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, TYPEOF_STRUCT(Entity, fields_count) fieldNumber, u8* inputData, reg inputBufferSize)
{
    M_Assert_Break((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities)) || (inputData == NULL) || (inputBufferSize == 0)), M_EMPTY, return ENTITY_ERROR, "setFieldValue: No allocated entities or invalid input");

    if((entityNumber < entityInfo.entities_count) && (fieldNumber < entityInfo.entities[entityNumber]->fields_count) && ((entityInfo.entities[entityNumber]->fields[fieldNumber].bitFlags & ENTITY_READ_ONLY_MSK) == 0)) {
        void* ptr = (entityInfo.entities[entityNumber]->pointer + entityInfo.entities[entityNumber]->fields[fieldNumber].shift);

        u8 typeLen = getMYCTypeLen(entityInfo.entities[entityNumber]->fields[fieldNumber].type);

        if(typeLen > inputBufferSize) {
            return ENTITY_ERROR;
        }

        proceedWriteEntity(entityInfo.entities[entityNumber]->fields[fieldNumber].bitFlags, ptr, inputData, typeLen);


#ifdef USE_ENTITY_WRITE_CALLBACK
        if(entityInfo.entities[entityNumber]->fields[fieldNumber].wrCallback.entityCallback != NULLPTR(TYPEOF_STRUCT(entityCallbackContainer, entityCallback))) {
            entityInfo.entities[entityNumber]->fields[fieldNumber].wrCallback.entityCallback(entityInfo.entities[entityNumber], &entityInfo.entities[entityNumber]->fields[fieldNumber], ptr, entityInfo.entities[entityNumber]->fields[fieldNumber].wrCallback.context);
        }
#endif /* USE_ENTITY_WRITE_CALLBACK */

    }

    return ENTITY_OK;
}



/* ******************************************************************************************************************
 * WRITE several fields values
 *
 */
int setSeveralFieldsValues(u8 *inputData, u8 *outputData, reg *size, reg maxOutBufferSize)
{
    M_Assert_Break((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities)) || (maxOutBufferSize < (1 + (ENTITIES_SIZEOF + ENTITY_FIELD_SIZEOF))) || (outputData == NULL) || (inputData == NULL) || (size == NULL)), M_EMPTY, return ENTITY_ERROR, "setSeveralFieldsValues: No allocated entities or invalid input");

    reg Wpos = 0;
    reg Rpos = 0;
    void* ptr;
    TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber;
    TYPEOF_STRUCT(Entity, fields_count) fieldNumber;

    outputData[Wpos++] = ENTITY_OK;

    while((Rpos + (ENTITIES_SIZEOF + ENTITY_FIELD_SIZEOF)) < (*size)) {

#if (MAX_NUBER_OF_ENTITIES < 256U)
        entityNumber = inputData[Rpos++];
#else
        ENTITY_BYTE_CPY(ENTITIES_SIZEOF, &inputData[Rpos], (u8*)&entityNumber);
        entityNumber &= 0x0000FFFFUL;
        Rpos += ENTITIES_SIZEOF;
#endif /* (MAX_NUBER_OF_ENTITIES < 256U) */


#if (MAX_NUBER_OF_FIELDS < 256U)
        fieldNumber = inputData[Rpos++];
#else
        ENTITY_BYTE_CPY(ENTITIES_SIZEOF, &inputData[Rpos], (u8*)&fieldNumber);
        fieldNumber &= 0x0000FFFFUL;
        Rpos += ENTITY_FIELD_SIZEOF;
#endif /* (MAX_NUBER_OF_FIELDS < 256U) */

        if((entityNumber < entityInfo.entities_count) && (fieldNumber < entityInfo.entities[entityNumber]->fields_count)) {
            u8 typeLen = getMYCTypeLen(entityInfo.entities[entityNumber]->fields[fieldNumber].type);

            M_Assert_BreakSaveCheck(((Rpos + typeLen) > (*size)), M_EMPTY, {
                                        Wpos = 0;
                                        outputData[Wpos++] = ENTITY_ERROR;
                                        ENTITY_BYTE_CPY(ENTITIES_SIZEOF,     &outputData[Wpos], (u8*)&entityNumber);
                                        ENTITY_BYTE_CPY(ENTITY_FIELD_SIZEOF, &outputData[Wpos], (u8*)&fieldNumber );
                                        (*size) = Wpos;
                                        return ENTITY_ERROR;
                                    }, "setSeveralFieldsValues: field read size more than buffer");

            if((entityInfo.entities[entityNumber]->fields[fieldNumber].bitFlags & ENTITY_READ_ONLY_MSK) == 0) {

                ptr = (entityInfo.entities[entityNumber]->pointer + entityInfo.entities[entityNumber]->fields[fieldNumber].shift);
                proceedWriteEntity(entityInfo.entities[entityNumber]->fields[fieldNumber].bitFlags, ptr, &inputData[Rpos], typeLen);


#ifdef USE_ENTITY_READ_CALLBACK
                if(entityInfo.entities[entityNumber]->fields[fieldNumber].wrCallback.entityCallback != NULLPTR(TYPEOF_STRUCT(entityCallbackContainer, entityCallback))) {
                    entityInfo.entities[entityNumber]->fields[fieldNumber].wrCallback.entityCallback(entityInfo.entities[entityNumber], &entityInfo.entities[entityNumber]->fields[fieldNumber], ptr, entityInfo.entities[entityNumber]->fields[fieldNumber].wrCallback.context);
                }
#endif /* USE_ENTITY_READ_CALLBACK */


            }

            Rpos += typeLen;

        } else {
            Wpos = 0;
            outputData[Wpos++] = ENTITY_ERROR;
            ENTITY_BYTE_CPY(ENTITIES_SIZEOF,     &outputData[Wpos], (u8*)&entityNumber);
            ENTITY_BYTE_CPY(ENTITY_FIELD_SIZEOF, &outputData[Wpos], (u8*)&fieldNumber );
            (*size) = Wpos;
            return ENTITY_ERROR;
        }
    }


    (*size) = Wpos;
    return ENTITY_OK;
}



#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */
