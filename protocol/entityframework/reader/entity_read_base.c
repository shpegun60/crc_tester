#include "entity_read_base.h"


#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#ifdef USE_ENTITY_READ_SERVICE

#include <stdlib.h>

/*
 * ****************************************************************
 * Parent implement section
 * ****************************************************************
 */

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
    u8 sign = (input[self->size - 1] & 0x80U);
    for(reg i = self->size; i < sizeof(i64); ++i) {
        ((u8*)(self->data))[i] = sign ? 0xFFU : 0x00U;
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
    for(reg i = self->size; i < sizeof(u64); ++i) {
        ((u8*)(self->data))[i] = 0x00U;
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
    int ENTITY_READ_CHIELD_CTOR_FUNC(type) (ENTITY_READ_CHIELD(type) *const self) {                     \
        free(self->parent.data);                                                                        \
        self->parent.data = calloc(1, sizeof(opType));                                                  \
        if(self->parent.data == NULL) {                                                                 \
            return ENTITY_ERROR;                                                                        \
        }                                                                                               \
        return ENTITY_OK;                                                                               \
    }                                                                                                   \
                                                                                                        \
    opType ENTITY_READ_CHIELD_GET_FUNC(type) (ENTITY_READ_CHIELD(type)* const self) {                   \
        return *((opType*)self->parent.data);                                                           \
    }                                                                                                   \
                                                                                                        \
    void ENTITY_READ_CHIELD_SET_FUNC(type) (ENTITY_READ_CHIELD(type)* const self, const opType val) {   \
        if(self->parent.bitFlags & ENTITY_READ_ONLY_MSK) {                                              \
            return;                                                                                     \
        }                                                                                               \
        *((opType*)self->parent.data) = val;                                                            \
        self->parent.onValueUpdated = 1;                                                                \
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
