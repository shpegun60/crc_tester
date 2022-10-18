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

int readEntitiesSizes(u8 *outputData, reg *size, reg maxOutBufferSize);
int readEntitiesDescriptions(TYPEOF_STRUCT(EntityInfo, entities_count) startEntityNumber, TYPEOF_STRUCT(EntityInfo, entities_count) len, u8 *outputData, reg *size, reg maxOutBufferSize);
int readEntityFields(TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, TYPEOF_STRUCT(Entity, fields_count) startFieldNumber, TYPEOF_STRUCT(Entity, fields_count) len, u8 *outputData, reg *size, reg maxOutBufferSize);


/* ******************************************************************************************************************
 * READ one field value
 *
 */
int readFieldValue(TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, TYPEOF_STRUCT(Entity, fields_count) fieldNumber, u8 *outputData, reg *size, reg maxOutBufferSize);

/* ******************************************************************************************************************
 * READ several fields values
 *
 */
int readSeveralFieldsValues(u8 *inputData, u8 *outputData, reg *size, reg maxOutBufferSize);

/* ******************************************************************************************************************
 * WRITE one field value
 *
 */
int setFieldValue(TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber, TYPEOF_STRUCT(Entity, fields_count) fieldNumber, u8* inputData, reg inputBufferSize);


/* ******************************************************************************************************************
 * WRITE several fields values
 *
 */
int setSeveralFieldsValues(u8 *inputData, u8 *outputData, reg *size, reg maxOutBufferSize);


#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */

#endif /* ENTITY_PACKET_H */
