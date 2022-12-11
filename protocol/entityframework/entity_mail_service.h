#ifndef ENTITY_MAIL_SERVICE_H
#define ENTITY_MAIL_SERVICE_H

#include "entity_port.h"

#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#ifdef USE_ENTITY_MAIL_SERVICE

#include "entity_manager.h"
#include "IntrusiveLinkedList.h"

#ifndef ENTITY_MAIL_MTU
#   define ENTITY_MAIL_MTU 10 // maximum number of entity per one transaction
#endif /* ENTITY_MAIL_MTU */

#ifndef ENTITY_MAIL_MINIMUM_TIME_SENDING_MS
#   define ENTITY_MAIL_MINIMUM_TIME_SENDING_MS 10 // minumum of milliseconds for sending
#endif /* ENTITY_MAIL_MINIMUM_TIME_SENDING_MS */


typedef struct {
    reg               mailDis;
    IntrusiveSListNode* lastStream;
    IntrusiveSList  head;
} EntityMailService_t;


EntityMailService_t* entityMailService_new(void);

int entityMailService_init(EntityMailService_t* const self);

int EntityMailService_subscribe(EntityMailService_t* const self,
                                const TYPEOF_STRUCT(EntityInfo, entities_count)   entityNumber,
                                const TYPEOF_STRUCT(Entity, fields_count)         fieldNumber, u32 updateTimeMs, u32 time);

int EntityMailService_unsubscribe(EntityMailService_t* const self,
                                  const TYPEOF_STRUCT(EntityInfo, entities_count)   entityNumber,
                                  const TYPEOF_STRUCT(Entity, fields_count)         fieldNumber);

void EntityMailService_getStream(EntityMailService_t* const self, u32 time,
                                 u8* const outputBuffer, reg* const size, const reg maxOutBufferSize);


// user defines check --------------------------------------------------
#if ENTITY_MAIL_MTU > 255
#   error ENTITY_MAIL_MTU must be less than 255
#endif /* ENTITY_MAIL_MTU > 255 */
//----------------------------------------------------------------------


#endif /* USE_ENTITY_MAIL_SERVICE */

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */

#endif /* ENTITY_MAIL_SERVICE_H */
