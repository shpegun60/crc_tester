#ifndef __ENTITY_CMD_H__
#define __ENTITY_CMD_H__

#include "entity_port.h"

#ifdef C_ENTITY_FRAMEWORK_LIB_ENA
#include "preprocessor_ctx.h"

void entityCMD(u8* const inputData, u8* const outputData, reg* const size, const reg maxOutBufferSize, PREPROCESSOR_CTX_TYPE(ctx));

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */

#endif /* __ENTITY_CMD_H__ */
