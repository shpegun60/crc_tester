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
 * Help functions for read and wrire entity if exists pointer to data, len and bit flags
 *
 */
STATIC_FORCEINLINE void proceedReadEntity(const TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, const void* const ptr, u8* const outputData, const reg typeLen)
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
                                        ENTITY_BYTE_CPY(typeLen, (u8*)ptr, outputData);
                                    }

#ifdef USE_ENTITY_ATOMIC
                            });
#endif /* USE_ENTITY_ATOMIC */

}

STATIC_FORCEINLINE void proceedWriteEntity(const TYPEOF_STRUCT(EntityField, bitFlags) bitFlags, void* const ptr, u8* const inputData, const reg typeLen)
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



#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */

#endif /* ENTITY_PACKET_H */
