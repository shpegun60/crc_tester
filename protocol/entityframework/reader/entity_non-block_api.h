#ifndef ENTITY_NONBLOCK_API_H
#define ENTITY_NONBLOCK_API_H

#include "entity_port.h"

#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#ifdef USE_ENTITY_READ_SERVICE
#include "entity_read_system.h"

#define ENTITY_READ_NONBLOCK_MAX_TIME_REQUEST 100
#define ENTITY_READ_NONBLOCK_API_MTU 10
#define ENTITY_READ_NONBLOCK_READ_PACKETS_QUEUE 4
#define ENTITY_READ_NONBLOCK_MSK (ENTITY_READ_NONBLOCK_READ_PACKETS_QUEUE - 1)

typedef struct {
    u8* const outBuffer[ENTITY_READ_SYSTEM_BOARD_COUNT];
    reg* const size[ENTITY_READ_SYSTEM_BOARD_COUNT];
    const reg outBufferSize;
    const u32 time;
    void* const user_ctx;
} EntityNonBlockReadLoopVariables_t;

void entityNonBlock_init(void);
void entityNonBlockReadLoop(EntityNonBlockReadLoopVariables_t* const var);
void entityNonBlockReceivePacket(u8* const inBuffer, const reg inBufferSize);


#endif /* USE_ENTITY_READ_SERVICE */

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */

#endif /* ENTITY_NONBLOCK_API_H */
