/**
 * Entity packet formater
 *
 */

#ifndef ENTITY_PACKET_H
#define ENTITY_PACKET_H

#include "entity_port.h"

#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#include "entity_manager.h"


/*------------------- Packet formaters function`s ----------------------------------
 * basic`s
 */

#ifdef USE_ENTITY_PING

b entities_ping(void);

#endif /* USE_ENTITY_PING */

int readEntitiesSizes(u8* const outputData, reg* const size, const reg maxOutBufferSize);
int readEntitiesDescriptions(const TYPEOF_STRUCT(EntityInfo, entities_count) startEntityNumber, TYPEOF_STRUCT(EntityInfo, entities_count) len, u8* const outputData, reg* const size, const reg maxOutBufferSize);
int readEntityFields(const TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, const TYPEOF_STRUCT(Entity, fields_count) startFieldNumber, TYPEOF_STRUCT(Entity, fields_count) len, u8* const outputData, reg* const size, const reg maxOutBufferSize);

/* ******************************************************************************************************************
 * READ one field value
 *
 */
int readFieldValue(const TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, const TYPEOF_STRUCT(Entity, fields_count) fieldNumber, u8* const outputData, reg* const size, const reg maxOutBufferSize);

/* ******************************************************************************************************************
 * READ several fields values
 *
 */
int readSeveralFieldsValues(u8* const inputData, u8* const outputData, reg* const size, const reg maxOutBufferSize);

/* ******************************************************************************************************************
 * WRITE one field value
 *
 */
int setFieldValue(const TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, const TYPEOF_STRUCT(Entity, fields_count) fieldNumber, u8* const inputData, const reg inputBufferSize);

/* ******************************************************************************************************************
 * WRITE several fields values
 *
 */
int setSeveralFieldsValues(u8* const inputData, u8* const outputData, reg* const size, const reg maxOutBufferSize);

/* ******************************************************************************************************************
 * GLUED functions fields values
 *
 */
int proceedGluedValues(u8* const inputData, u8* const outputData, reg* const size, const reg maxOutBufferSize);


/* ******************************************************************************************************************
 * Help functions for read and wrire entity if exists pointer to data, len and bit flags
 *
 */
STATIC_FORCEINLINE void proceedReadEntity(const TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, void* ptr, u8* outputData, const reg typeLen)
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
                                        volatile reg* reg_ptr_from = ((volatile reg*) (ptr));
                                        volatile reg* reg_ptr_to   = ((volatile reg*) (outputData));
                                        ENTITY_REG_CPY(reg_ptr_from, reg_ptr_to);
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
                                        ENTITY_BYTE_CPY(typeLen, ptr, outputData);
                                    }

#ifdef USE_ENTITY_ATOMIC
                            });
#endif /* USE_ENTITY_ATOMIC */

}

STATIC_FORCEINLINE void proceedWriteEntity(const TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, void* ptr, u8* const inputData, reg typeLen)
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
                                        volatile reg* reg_ptr_to        = ((volatile reg*)  (ptr));
                                        volatile reg* reg_ptr_from      = ((volatile reg*)  (inputData));
                                        ENTITY_REG_CPY(reg_ptr_from, reg_ptr_to);
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


// write entity number to out buffer by next position
STATIC_FORCEINLINE void writeEntityNumberToBuf(const TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, u8* const outputData)
{
    /*******************************************************************************************************
     *  write entity number
     */
#if (MAX_NUBER_OF_ENTITIES < 256U)
    (*outputData) = entityNumber;
#else
    ENTITY_BYTE_CPY(ENTITIES_SIZEOF, (u8*)&entityNumber, outputData);
#endif /* (MAX_NUBER_OF_ENTITIES < 256U) */
    //----------------- END -------------------------------------------------------------------------------
}

// write field number to out buffer by next position
STATIC_FORCEINLINE void writeFieldNumberToBuf(const TYPEOF_STRUCT(Entity, fields_count) fieldNumber, u8* const outputData)
{
    /*******************************************************************************************************
     *  write field number
     */
#if (MAX_NUBER_OF_FIELDS < 256U)
    (*outputData) = fieldNumber;
#else
    ENTITY_BYTE_CPY(ENTITY_FIELD_SIZEOF, (u8*)&fieldNumber, outputData);
#endif /* (MAX_NUBER_OF_FIELDS < 256U) */
    //----------------- END -------------------------------------------------------------------------------
}


// write entity & field number to out buffer by next position
STATIC_FORCEINLINE void writeEntityFieldNumbersToBuf(const TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, const TYPEOF_STRUCT(Entity, fields_count) fieldNumber, u8* const outputData, reg* const Wpos)
{
    reg Wpos_internal = (*Wpos);

    /*******************************************************************************************************
     *  write entity number
     */
     writeEntityNumberToBuf(entityNumber, &outputData[Wpos_internal]);
     Wpos_internal += ENTITIES_SIZEOF;

    /*******************************************************************************************************
     *  write field number
     */
     writeFieldNumberToBuf(fieldNumber, &outputData[Wpos_internal]);
     Wpos_internal += ENTITY_FIELD_SIZEOF;

    //----------------- END -------------------------------------------------------------------------------
    (*Wpos) = Wpos_internal;
}


// read entity number from input buffer
STATIC_FORCEINLINE void readEntityNumberFromBuf(TYPEOF_STRUCT(EntityInfo, entities_count)* const entityNumber, u8* const inputData)
{
    /*******************************************************************************************************
     *  read entity number
     */
#if (MAX_NUBER_OF_ENTITIES < 256U)
    (*entityNumber) = *(inputData);
#else
    ENTITY_BYTE_CPY(ENTITIES_SIZEOF, inputData, (u8*)entityNumber);
    (*entityNumber) &= 0x0000FFFFUL;
#endif /* (MAX_NUBER_OF_ENTITIES < 256U) */
    //----------------- END --------------------------------------------------------------------------------
}

// read field number from input buffer
STATIC_FORCEINLINE void readFieldNumberFromBuf(TYPEOF_STRUCT(Entity, fields_count)* const fieldNumber, u8* const inputData)
{
    /*******************************************************************************************************
     *  read field number
     */
#if (MAX_NUBER_OF_FIELDS < 256U)
    (*fieldNumber) = *(inputData);
#else
    ENTITY_BYTE_CPY(ENTITY_FIELD_SIZEOF, inputData, (u8*)fieldNumber);
    (*fieldNumber) &= 0x0000FFFFUL;
#endif /* (MAX_NUBER_OF_FIELDS < 256U) */
    //----------------- END --------------------------------------------------------------------------------
}

// read entity & field number from input buffer by next position
STATIC_FORCEINLINE void readEntityFieldNumbersfromBuf(TYPEOF_STRUCT(EntityInfo, entities_count)* const entityNumber, TYPEOF_STRUCT(Entity, fields_count)* const fieldNumber, u8* const inputData, reg* const Rpos)
{
    reg Rpos_internal = (*Rpos);

    /*******************************************************************************************************
     *  read entity number
     */
    readEntityNumberFromBuf(entityNumber, &inputData[Rpos_internal]);
    Rpos_internal += ENTITIES_SIZEOF;

    /*******************************************************************************************************
     *  read field number
     */
    readFieldNumberFromBuf(fieldNumber, &inputData[Rpos_internal]);
    Rpos_internal += ENTITY_FIELD_SIZEOF;

    //----------------- END -----------------------------------------------
    (*Rpos) = Rpos_internal;
}




#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */

#endif /* ENTITY_PACKET_H */
