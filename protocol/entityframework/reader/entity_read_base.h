#ifndef ENTITY_READ_BASEH_H
#define ENTITY_READ_BASEH_H

#include "entity_port.h"

#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#ifdef USE_ENTITY_READ_SERVICE

#include "entity_manager.h"

typedef struct {
    const TYPEOF_STRUCT(Entity, fields_count) fieldNumber;
    const TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber;

    const TYPEOF_STRUCT(EntityField, type) type;
    const TYPEOF_STRUCT(EntityField, bitFlags) bitFlags;
    b onValueUpdated;
    const reg size;
    u8* data;
} EntityReadParent_t;

#define ENTITY_READ_CHIELD(type) EntityReadChield_ ## type ## _t
#define ENTITY_READ_CHIELD_CTOR_FUNC(type) ctorEntityReadChield_ ## type
#define ENTITY_READ_CHIELD_GET_FUNC(type) getEntityReadChield_ ## type
#define ENTITY_READ_CHIELD_SET_FUNC(type) setEntityReadChield_ ## type

#define DECLARE_ENTITY_READ_CHIELD_TYPE(type)                                                           \
    typedef struct EntityReadChield_ ## type       ENTITY_READ_CHIELD(type);                            \
    struct EntityReadChield_ ## type {                                                                  \
        EntityReadParent_t parent;                                                                      \
        int  (* const ctor)(ENTITY_READ_CHIELD(type) * const);                                          \
        type (* const get)(ENTITY_READ_CHIELD(type) * const);                                           \
        void (* const set)(ENTITY_READ_CHIELD(type)* const, const type);                                \
    };                                                                                                  \
    int ENTITY_READ_CHIELD_CTOR_FUNC(type) (ENTITY_READ_CHIELD(type) *const self);                      \
    type ENTITY_READ_CHIELD_GET_FUNC(type) (ENTITY_READ_CHIELD(type)* const self);                      \
    void ENTITY_READ_CHIELD_SET_FUNC(type) (ENTITY_READ_CHIELD(type)* const self, const type val);



// read chields declaration--------------------------------------------------------
//DECLARE_ENTITY_READ_CHIELD_TYPE(void);
DECLARE_ENTITY_READ_CHIELD_TYPE(u8);
DECLARE_ENTITY_READ_CHIELD_TYPE(u16);

DECLARE_ENTITY_READ_CHIELD_TYPE(u24);
DECLARE_ENTITY_READ_CHIELD_TYPE(u32);
DECLARE_ENTITY_READ_CHIELD_TYPE(u64);

DECLARE_ENTITY_READ_CHIELD_TYPE(c8);
DECLARE_ENTITY_READ_CHIELD_TYPE(i8);
DECLARE_ENTITY_READ_CHIELD_TYPE(i16);
DECLARE_ENTITY_READ_CHIELD_TYPE(i24);
DECLARE_ENTITY_READ_CHIELD_TYPE(i32);
DECLARE_ENTITY_READ_CHIELD_TYPE(i64);

DECLARE_ENTITY_READ_CHIELD_TYPE(f32);
DECLARE_ENTITY_READ_CHIELD_TYPE(f64);
DECLARE_ENTITY_READ_CHIELD_TYPE(f128);

DECLARE_ENTITY_READ_CHIELD_TYPE(b);
DECLARE_ENTITY_READ_CHIELD_TYPE(reg);
DECLARE_ENTITY_READ_CHIELD_TYPE(sreg);

#undef DECLARE_ENTITY_READ_CHIELD_TYPE


#endif /* USE_ENTITY_READ_SERVICE */

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */

#endif /* ENTITY_READ_BASEH_H */
