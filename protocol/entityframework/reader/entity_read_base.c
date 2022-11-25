#include "entity_read_base.h"

#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#ifdef USE_ENTITY_READ_SERVICE

#include <stdlib.h>


/*
 * ****************************************************************
 * Parent implement section
 * ****************************************************************
 */

EntityWritePoolContainer_t* entityWritePoolContainer_new(const reg pointerCount)
{
    if(pointerCount == 0) {
        return NULL;
    }

    EntityWritePoolContainer_t* m = calloc(1, sizeof(EntityWritePoolContainer_t));
    if(m == NULL) {
        return NULL;
    }
    entityWritePoolContainer_init(m, pointerCount);
    return m;
}

int entityWritePoolContainer_init(EntityWritePoolContainer_t* const self, const reg pointerCount)
{
    if(self == NULL || pointerCount == 0) {
        return ENTITY_ERROR;
    }

    self->writePool = calloc(pointerCount, sizeof(EntityReadParent_t*));
    if(self->writePool == NULL) {
        return ENTITY_ERROR;
    }

    self->allocatedFields = pointerCount;
    self->counter = 0;
    return ENTITY_OK;
}


 // universal write to parrent function
int setEntityReadParent_uni(EntityReadParent_t * const self, u8* input, const reg inputSize)
{
    if(self->size > inputSize) {
        return ENTITY_ERROR;
    }

    ENTITY_BYTE_CPY(self->size, input, self->data);
    return ENTITY_OK;
}

// unique function for sreg type
int ENTITY_READ_PARENT_SET_FUNC(sreg) (EntityReadParent_t * const self, u8* input, const reg inputSize)
{
    if(self->size > inputSize) {
        return ENTITY_ERROR;
    }

    ENTITY_BYTE_CPY(self->size, input, self->data);
    // copy sign
    u64 sign = (input[self->size - 1] & 0x80U) ? 0 : UINT64_MAX;

    reg i = self->size;
    while(i != sizeof(i64)) {
       ((u8*)(self->data))[i] = ((u8*)(&sign))[i];
        ++i;
    }

    return ENTITY_OK;
}

// unique function for reg type
int ENTITY_READ_PARENT_SET_FUNC(reg)(EntityReadParent_t * const self, u8* input, const reg inputSize)
{
    if(self->size > inputSize) {
        return ENTITY_ERROR;
    }

    ENTITY_BYTE_CPY(self->size, input, self->data);
    // other fill to 0
    reg i = self->size;
    while(i != sizeof(i64)) {
       ((u8*)(self->data))[i] = 0x00U;
        ++i;
    }
    return ENTITY_OK;
}


/*
 * ****************************************************************
 * Chield implement section
 * ****************************************************************
 */

#define DECLARE_ORDINARY_ENTITY_READ_CHIELD_FUNCTIONS(type) DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(type, type)
#define DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(type, opType)                                              \
    /* get / set (inside chield) */                                                                     \
    opType ENTITY_READ_CHIELD_GET_FUNC(type) (ENTITY_READ_CHIELD(type)* const self) {                   \
        return self->data;                                                                              \
    }                                                                                                   \
                                                                                                        \
    void ENTITY_READ_CHIELD_SET_FUNC(type) (ENTITY_READ_CHIELD(type)* const self, const opType val) {   \
        if(self->parent.bitFlags & ENTITY_READ_ONLY_MSK) {                                              \
            return;                                                                                     \
        }                                                                                               \
                                                                                                        \
        self->data = val;                                                                               \
                                                                                                        \
        /* --------------------------------------------------------------------------- */               \
        /* Write parent to container */                                                                 \
        /* --------------------------------------------------------------------------- */               \
                                                                                                        \
        /* move to cash */                                                                              \
        EntityWritePoolContainer_t *const writeCont = self->parent.writeContainer;                      \
        reg* const counter = &writeCont->counter;                                                       \
                                                                                                        \
        /* do logic */                                                                                  \
        if (((*counter) < writeCont->allocatedFields) && self->parent.onValueNotUpdated) {              \
            writeCont->writePool[(*counter)] = &self->parent;                                           \
            self->parent.onValueNotUpdated = 0;                                                         \
            ++(*counter);                                                                               \
        }                                                                                               \
    }

// read chields declaration--------------------------------------------------------
//DECLARE_ORDINARY_ENTITY_READ_CHIELD_FUNCTIONS(void);
DECLARE_ORDINARY_ENTITY_READ_CHIELD_FUNCTIONS(u8);
DECLARE_ORDINARY_ENTITY_READ_CHIELD_FUNCTIONS(u16);

DECLARE_ORDINARY_ENTITY_READ_CHIELD_FUNCTIONS(u24);
DECLARE_ORDINARY_ENTITY_READ_CHIELD_FUNCTIONS(u32);
DECLARE_ORDINARY_ENTITY_READ_CHIELD_FUNCTIONS(u64);

DECLARE_ORDINARY_ENTITY_READ_CHIELD_FUNCTIONS(c8);
DECLARE_ORDINARY_ENTITY_READ_CHIELD_FUNCTIONS(i8);
DECLARE_ORDINARY_ENTITY_READ_CHIELD_FUNCTIONS(i16);
DECLARE_ORDINARY_ENTITY_READ_CHIELD_FUNCTIONS(i24);
DECLARE_ORDINARY_ENTITY_READ_CHIELD_FUNCTIONS(i32);
DECLARE_ORDINARY_ENTITY_READ_CHIELD_FUNCTIONS(i64);

DECLARE_ORDINARY_ENTITY_READ_CHIELD_FUNCTIONS(f32);
DECLARE_ORDINARY_ENTITY_READ_CHIELD_FUNCTIONS(f64);
//DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(f128); // very hard this support on different platforms

DECLARE_ORDINARY_ENTITY_READ_CHIELD_FUNCTIONS(b);

DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(reg, u64);
DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(sreg, i64);

#undef DECLARE_ORDINARY_ENTITY_READ_CHIELD_FUNCTIONS
#undef DECLARE_ENTITY_READ_CHIELD_FUNCTIONS



#endif /* USE_ENTITY_READ_SERVICE */

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */
