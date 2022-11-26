#include "entity_packet.h"


#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#include "smart_assert.h"
#include "my_ctype_cast.h"

#ifdef USE_ENTITY_ATOMIC
#include "entity_atomic.h"
#endif /* USE_ENTITY_ATOMIC */




/*------------------- Packet formaters function`s ----------------------------------
 * basic`s
 */

#ifdef USE_ENTITY_PING

b entities_ping(void)
{
    return entityInfo.userInitReady;
}

#endif /* USE_ENTITY_PING */

int readEntitiesSizes(u8* const outputData, reg* const size, const reg maxOutBufferSize)
{
    M_Assert_Break((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities)) || (outputData == NULL) || (size == NULL)), M_EMPTY, return ENTITY_ERROR, "readEntitiesSizes: No allocated entities or invalid input");
    ENTITY_DBG_ASSERT_BUF(maxOutBufferSize < (8 + TYPE_ARRAY_LENGTH), M_EMPTY, return ENTITY_ERROR, "readEntitiesSizes: sizes read more than buffer");

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
    UNUSED(maxOutBufferSize);
    return ENTITY_OK;
}

int readEntitiesDescriptions(const TYPEOF_STRUCT(EntityInfo, entities_count) startEntityNumber, TYPEOF_STRUCT(EntityInfo, entities_count) len, u8* const outputData, reg* const size, const reg maxOutBufferSize)
{   //send {entities[0].descr, ... entities[entities_count].descr}

    M_Assert_Break((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities)) || (maxOutBufferSize < (2 + ENTITIES_SIZEOF)) || (outputData == NULL) || (size == NULL)), M_EMPTY, return ENTITY_ERROR, "readEntitiesDescriptions: No allocated entities or invalid input");

    if(startEntityNumber < entityInfo.entities_count) {
        reg pos = 0;

        outputData[pos++] = ENTITY_OK;
        outputData[pos++] = ENTITY_DESCRIPTION_SIZE;                // description sizeof /////////////////////////////// may be optimize??? ///////////////////////////////////////////////////

        TYPEOF_STRUCT(EntityInfo, entities_count) tmp = (entityInfo.entities_count - startEntityNumber);
        len = MIN(tmp, len);

        //#if (MAX_NUBER_OF_ENTITIES < 256U) // WARNING!!!!!!!!! DO NOT this SUPPORT ON OLD VERSION---------------------------------------------------------------------------------------------------------================================================-----===========================-------------------------------------------------------------------
        //        outputData[pos++] = (u8)(len & 0xFFU);
        //#else
        //        len &= 0xFFFFUL;
        //        ENTITY_BYTE_CPY(ENTITIES_SIZEOF, (u8*)&len, &outputData[pos]);
        //        pos += ENTITIES_SIZEOF;
        //#endif /* MAX_NUBER_OF_ENTITIES < 256 */
        outputData[pos++] = (u8)(len & 0xFFU); // support on old version (this is glitch!!!)
        //---------------------------================================----------------------------------==================================-----------------------------------================================---------------------------===========================---------------------------=========================------------------------
        len += startEntityNumber;

        for(TYPEOF_STRUCT(EntityInfo, entities_count) i = startEntityNumber; i < len; ++i) {

            ENTITY_DBG_ASSERT_BUF( (pos + ENTITY_DESCRIPTION_SIZE) > maxOutBufferSize, M_EMPTY, {
                                       goto error;
                                   }, "readEntitiesDescriptions: description read size more than buffer");


            ENTITY_BYTE_CPY(ENTITY_DESCRIPTION_SIZE, (u8 *)entityInfo.entities[i]->descr, &outputData[pos]);
            pos += ENTITY_DESCRIPTION_SIZE;
        }

        (*size) = pos;
    } else {
        goto error;
    }

    UNUSED(maxOutBufferSize);
    return ENTITY_OK;


    // error proceeding lable -------------------------------------------------------------------
    error:
        outputData[0] = ENTITY_ERROR;
        (*size) = 1;
        return ENTITY_ERROR;
}

int readEntityFields(const TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, const TYPEOF_STRUCT(Entity, fields_count) startFieldNumber, TYPEOF_STRUCT(Entity, fields_count) len, u8* const outputData, reg* const size, const reg maxOutBufferSize)
{   //send {entities[entityNum].fields[startFieldNum], ... entities[num].fields[entities[num].fields_count]}

    M_Assert_Break((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities)) || (maxOutBufferSize < (1 + (ENTITY_FIELD_SIZEOF << 1))) || (outputData == NULL) || (size == NULL)), M_EMPTY, return ENTITY_ERROR, "readEntityFields: No allocated entities or invalid input");

    const Entity* const entity = entityInfo.entities[entityNumber];

    if((entityNumber < entityInfo.entities_count) && (startFieldNumber < entity->fields_count)) {
        reg pos = 0;

        outputData[pos++] = ENTITY_OK;

#if (MAX_NUBER_OF_FIELDS < 256U)
        outputData[pos++] = (u8)(startFieldNumber & 0xFFU);                             /////////////////////////////// may be optimize??? ///////////////////////////////////////////////////

        TYPEOF_STRUCT(Entity, fields_count) tmp = (entity->fields_count - startFieldNumber);
        len = MIN(tmp, len);
        outputData[pos++] = (u8)(len & 0xFFU);

#else
        startFieldNumber &= 0xFFFFUL;
        ENTITY_BYTE_CPY(ENTITY_FIELD_SIZEOF, (u8*)&startFieldNumber, &outputData[pos]); /////////////////////////////// may be optimize??? ///////////////////////////////////////////////////
        pos += ENTITY_FIELD_SIZEOF;

        len &= 0xFFFFUL;
        TYPEOF_STRUCT(Entity, fields_count) tmp = (entity->fields_count - startFieldNumber);
        len = MIN(tmp, len);
        ENTITY_BYTE_CPY(ENTITY_FIELD_SIZEOF, (u8*)&len, &outputData[pos]);
        pos += ENTITY_FIELD_SIZEOF;

#endif /* MAX_NUBER_OF_FIELDS < 256 */

        len += startFieldNumber;

        for(TYPEOF_STRUCT(Entity, fields_count) i = startFieldNumber; i < len; ++i) {
            const EntityField* const field = &entity->fields[i];

            ENTITY_DBG_ASSERT_BUF( ((pos + (sizeof(field->bitFlags) + sizeof(field->shift) + 1 + ENTITY_DESCRIPTION_SIZE)) > maxOutBufferSize), M_EMPTY, {
                                       goto error;
                                   }, "readEntityFields: fields read size more than buffer");


//-----------------=============================-----------------------------------===========================-----------------------------=================================----OLD VERSION DO NOT SUPPORT---------------------------===========================----------------
//            ENTITY_BYTE_CPY(sizeof(field->bitFlags), (u8 *)&field->bitFlags, &outputData[pos]);
//            pos += sizeof(field->bitFlags);

//            ENTITY_BYTE_CPY(sizeof(field->shift), (u8 *)&field->shift, &outputData[pos]);
//            pos += sizeof(field->shift);

//            outputData[pos++] = (field->type & 0xFFU);

//            ENTITY_BYTE_CPY(ENTITY_DESCRIPTION_SIZE, (u8 *)&field->descr, &outputData[pos]);
//            pos += ENTITY_DESCRIPTION_SIZE;
//-----------------=============================-----------------------------------===========================-----------------------------=================================-------------------------------===========================-------------------======================
            // this is glitch
            ENTITY_BYTE_CPY(1, (u8 *)&field->bitFlags, &outputData[pos]);
            pos += 1;

            ENTITY_BYTE_CPY(2, (u8 *)&field->shift, &outputData[pos]);
            pos += 2;

            outputData[pos++] = (field->type & 0xFFU);

            ENTITY_BYTE_CPY(ENTITY_DESCRIPTION_SIZE, (u8 *)&field->descr, &outputData[pos]);
            pos += ENTITY_DESCRIPTION_SIZE;
//------------------------------=========================================--------------------------------------===================================--------------------------------------=============================---------------------------------========================

        }

        (*size) = pos;
    } else {
        goto error;
    }

    UNUSED(maxOutBufferSize);
    return ENTITY_OK;

    // error proceeding lable -------------------------------------------------------------------
    error:
        outputData[0] = ENTITY_ERROR;
        (*size) = 1;
        return ENTITY_ERROR;
}


/* ******************************************************************************************************************
 * READ one field value
 *
 */
int readFieldValue(const TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, const TYPEOF_STRUCT(Entity, fields_count) fieldNumber, u8* const outputData, reg* const size, const reg maxOutBufferSize)
{ //send {0x01 , prt + 0, ... , ptr + getTypeLen(entities[no].fields[fieldNo].type)}
    M_Assert_Break((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities)) || (maxOutBufferSize < 2) || (outputData == NULL) || (size == NULL)), M_EMPTY, return ENTITY_ERROR, "readFieldValue: No allocated entities or invalid input");

    Entity*         const       entity = entityInfo.entities[entityNumber];
    EntityField*    const       field = &entity->fields[fieldNumber];

    if((entityNumber < entityInfo.entities_count) && (fieldNumber < entity->fields_count)) {
        reg pos = 0;

        outputData[pos++] = ENTITY_OK;
        outputData[pos++] = field->type;    /////////////////////////////// may be optimize??? ///////////////////////////////////////////////////

        reg typeLen = getMYCTypeLen(field->type);

        ENTITY_DBG_ASSERT_BUF(((pos + typeLen) > maxOutBufferSize), M_EMPTY, {
                                  goto error;
                              }, "readFieldValue: field read size more than buffer");


        void* ptr = (entity->pointer + field->shift);

        proceedReadEntity(field->bitFlags, ptr, &outputData[pos], typeLen);


#ifdef USE_ENTITY_READ_CALLBACK
        if(field->rdCallback.entityCallback != NULLPTR(TYPEOF_STRUCT(entityCallbackContainer, entityCallback))) {
            field->rdCallback.entityCallback(entity, field, ptr, field->rdCallback.context);
        }
#endif /* USE_ENTITY_READ_CALLBACK */

        pos += typeLen;
        (*size) = pos;

    } else {
       goto error;
    }

    UNUSED(maxOutBufferSize);
    return ENTITY_OK;

    // error proceeding lable -------------------------------------------------------------------
    error:
        outputData[0] = ENTITY_ERROR;
        (*size) = 1;
        return ENTITY_ERROR;
}

/* ******************************************************************************************************************
 * READ several fields values
 *
 */

int readSeveralFieldsValues(u8* const inputData, u8* const outputData, reg* const size, const reg maxOutBufferSize)
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
        ENTITY_BYTE_CPY(ENTITY_FIELD_SIZEOF, &inputData[Rpos], (u8*)&fieldNumber);
        fieldNumber &= 0x0000FFFFUL;
        Rpos += ENTITY_FIELD_SIZEOF;
#endif /* (MAX_NUBER_OF_FIELDS < 256U) */


        Entity* const entity = entityInfo.entities[entityNumber];
        EntityField* const field = &entity->fields[fieldNumber];

        if((entityNumber < entityInfo.entities_count) && (fieldNumber < entity->fields_count)) {

            reg typeLen = getMYCTypeLen(field->type);

            ENTITY_DBG_ASSERT_BUF(((Wpos + typeLen) > maxOutBufferSize), M_EMPTY, {
                                     goto error;
                                  }, "readSeveralFieldsValues: field size more than buffer");


            ptr = (entity->pointer + field->shift);
            proceedReadEntity(field->bitFlags, ptr, &outputData[Wpos], typeLen);



#ifdef USE_ENTITY_READ_CALLBACK
            if(field->rdCallback.entityCallback != NULLPTR(TYPEOF_STRUCT(entityCallbackContainer, entityCallback))) {
                field->rdCallback.entityCallback(entity, field, ptr, field->rdCallback.context);
            }
#endif /* USE_ENTITY_READ_CALLBACK */

            Wpos += typeLen;

        } else {
            goto error;
        }
    }


    (*size) = Wpos;
    UNUSED(maxOutBufferSize);
    return ENTITY_OK;


    // error proceeding lable -------------------------------------------------------------------
    error:
        Wpos = 0;
        outputData[Wpos++] = ENTITY_ERROR;
        ENTITY_BYTE_CPY(ENTITIES_SIZEOF,     &outputData[Wpos], (u8*)&entityNumber);
        Wpos += ENTITIES_SIZEOF;

        ENTITY_BYTE_CPY(ENTITY_FIELD_SIZEOF, &outputData[Wpos], (u8*)&fieldNumber );
        Wpos += ENTITY_FIELD_SIZEOF;

        (*size) = Wpos;
        return ENTITY_ERROR;
}


/* ******************************************************************************************************************
 * WRITE one field value
 *
 */
int setFieldValue(const TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, const TYPEOF_STRUCT(Entity, fields_count) fieldNumber, u8* const inputData, const reg inputBufferSize)
{
    M_Assert_Break((entityInfo.entities == NULLPTR(TYPEOF_STRUCT(EntityInfo, entities)) || (inputData == NULL) || (inputBufferSize == 0)), M_EMPTY, return ENTITY_ERROR, "setFieldValue: No allocated entities or invalid input");

    Entity* const entity = entityInfo.entities[entityNumber];
    EntityField* const field = &entity->fields[fieldNumber];

    if((entityNumber < entityInfo.entities_count) && (fieldNumber < entity->fields_count) && ((field->bitFlags & ENTITY_READ_ONLY_MSK) == 0)) {
        void* ptr = (entity->pointer + field->shift);

        reg typeLen = getMYCTypeLen(field->type);

        ENTITY_DBG_ASSERT_BUF((typeLen > inputBufferSize), M_EMPTY, {
                                 return ENTITY_ERROR;
                              }, "setFieldValue: field size more than buffer");

        proceedWriteEntity(field->bitFlags, ptr, inputData, typeLen);


#ifdef USE_ENTITY_WRITE_CALLBACK
        if(field->wrCallback.entityCallback != NULLPTR(TYPEOF_STRUCT(entityCallbackContainer, entityCallback))) {
            field->wrCallback.entityCallback(entity, field, ptr, field->wrCallback.context);
        }
#endif /* USE_ENTITY_WRITE_CALLBACK */

    }

    UNUSED(inputBufferSize);
    return ENTITY_OK;
}



/* ******************************************************************************************************************
 * WRITE several fields values
 *
 */
int setSeveralFieldsValues(u8* const inputData, u8* const outputData, reg* const size, const reg maxOutBufferSize)
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
        ENTITY_BYTE_CPY(ENTITY_FIELD_SIZEOF, &inputData[Rpos], (u8*)&fieldNumber);
        fieldNumber &= 0x0000FFFFUL;
        Rpos += ENTITY_FIELD_SIZEOF;
#endif /* (MAX_NUBER_OF_FIELDS < 256U) */

        Entity* const entity = entityInfo.entities[entityNumber];
        EntityField* const field = &entity->fields[fieldNumber];

        if((entityNumber < entityInfo.entities_count) && (fieldNumber < entity->fields_count)) {
            reg typeLen = getMYCTypeLen(field->type);

            ENTITY_DBG_ASSERT_BUF(((Rpos + typeLen) > (*size)), M_EMPTY, {
                                      goto error;
                                  }, "setSeveralFieldsValues: field read size more than buffer");


            if((field->bitFlags & ENTITY_READ_ONLY_MSK) == 0) {

                ptr = (entity->pointer + field->shift);
                proceedWriteEntity(field->bitFlags, ptr, &inputData[Rpos], typeLen);


#ifdef USE_ENTITY_WRITE_CALLBACK
                if(field->wrCallback.entityCallback != NULLPTR(TYPEOF_STRUCT(entityCallbackContainer, entityCallback))) {
                    field->wrCallback.entityCallback(entity, field, ptr, field->wrCallback.context);
                }
#endif /* USE_ENTITY_WRITE_CALLBACK */


            }

            Rpos += typeLen;

        } else {
            goto error;
        }
    }


    (*size) = Wpos;
    UNUSED(maxOutBufferSize);
    return ENTITY_OK;


    // error proceeding lable -------------------------------------------------------------------
    error:
        Wpos = 0;
        outputData[Wpos++] = ENTITY_ERROR;
        ENTITY_BYTE_CPY(ENTITIES_SIZEOF,     &outputData[Wpos], (u8*)&entityNumber);
        Wpos += ENTITIES_SIZEOF;

        ENTITY_BYTE_CPY(ENTITY_FIELD_SIZEOF, &outputData[Wpos], (u8*)&fieldNumber );
        Wpos += ENTITY_FIELD_SIZEOF;

        (*size) = Wpos;
        return ENTITY_ERROR;
}

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */
