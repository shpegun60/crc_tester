#include "entity_read_base.h"


#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#ifdef USE_ENTITY_READ_SERVICE

#include <stdlib.h>

#define DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(type)                                                      \
    int ENTITY_READ_CHIELD_CTOR_FUNC(type) (ENTITY_READ_CHIELD(type) *const self) {                     \
        free(self->parent.data);                                                                        \
        self->parent.data = (u8 *)calloc(1, sizeof(type));                                              \
        if(self->parent.data == NULL) {                                                                 \
            return ENTITY_ERROR;                                                                        \
        }                                                                                               \
        return ENTITY_OK;                                                                               \
    }                                                                                                   \
                                                                                                        \
    type ENTITY_READ_CHIELD_GET_FUNC(type) (ENTITY_READ_CHIELD(type)* const self) {                     \
        return *((type*)self->parent.data);                                                             \
    }                                                                                                   \
                                                                                                        \
    void ENTITY_READ_CHIELD_SET_FUNC(type) (ENTITY_READ_CHIELD(type)* const self, const type val) {     \
        *((type*)self->parent.data) = val;                                                              \
        self->parent.onValueUpdated = 1;                                                                \
    }

// read chields declaration--------------------------------------------------------
//DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(void);
DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(u8);
DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(u16);

DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(u24);
DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(u32);
DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(u64);

DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(c8);
DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(i8);
DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(i16);
DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(i24);
DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(i32);
DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(i64);

DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(f32);
DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(f64);
//DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(f128);

DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(b);
DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(reg);
DECLARE_ENTITY_READ_CHIELD_FUNCTIONS(sreg);

#undef DECLARE_ENTITY_READ_CHIELD_FUNCTIONS


#endif /* USE_ENTITY_READ_SERVICE */

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */
