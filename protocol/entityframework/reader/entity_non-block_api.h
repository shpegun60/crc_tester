#ifndef ENTITY_NONBLOCK_API_H
#define ENTITY_NONBLOCK_API_H

#include "entity_port.h"

#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#ifdef USE_ENTITY_READ_SERVICE
#include "entity_read_system.h"

#define ENTITY_READ_NONBLOCK_MAX_TIME_REQUEST 100
#define ENTITY_READ_NONBLOCK_READ_PACKETS_QUEUE 4
#define ENTITY_READ_NONBLOCK_MSK (ENTITY_READ_NONBLOCK_READ_PACKETS_QUEUE - 1)

typedef struct {
    u8* const outBuffer[ENTITY_READ_SYSTEM_BOARD_COUNT];
    reg* const size[ENTITY_READ_SYSTEM_BOARD_COUNT];
    const reg outBufferSize[ENTITY_READ_SYSTEM_BOARD_COUNT];
    const u32 time;
} EntityNonBlockReadLoopVariables_t;

void entityNonBlock_init(void);
void entityNonBlockReadLoop(EntityNonBlockReadLoopVariables_t* const var);
void entityNonBlockReceivePacket(const u16 boardNumber, u8* const inBuffer, const reg inBufferSize);


#if ((ENTITY_READ_NONBLOCK_READ_PACKETS_QUEUE - 1) & ENTITY_READ_NONBLOCK_READ_PACKETS_QUEUE)
#   error ENTITY_READ_NONBLOCK_READ_PACKETS_QUEUE must be is power of 2
#endif //check if power of 2 ENTITY_READ_NONBLOCK_READ_PACKETS_QUEUE

#if (ENTITY_READ_NONBLOCK_READ_PACKETS_QUEUE > 256)
#   error ENTITY_READ_NONBLOCK_READ_PACKETS_QUEUE must less than 256
#endif //check if power of 2 ENTITY_READ_NONBLOCK_READ_PACKETS_QUEUE

#if (ENTITY_READ_API_MTU > 256)
#   error ENTITY_READ_API_MTU must less than 256
#endif //check if power of 2 ENTITY_READ_API_MTU

#if (ENTITY_WRITE_API_MTU > 256)
#   error ENTITY_WRITE_API_MTU must less than 256
#endif //check if power of 2 ENTITY_WRITE_API_MTU


#endif /* USE_ENTITY_READ_SERVICE */

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */

#endif /* ENTITY_NONBLOCK_API_H */
