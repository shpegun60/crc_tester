#ifndef ENTITY_READ_BASEH_H
#define ENTITY_READ_BASEH_H

#include "entity_port.h"

#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#ifdef USE_ENTITY_READ_SERVICE

#include "entity_manager.h"

/*
 * ****************************************************************
 * Parent section
 * ****************************************************************
 */
#define ENTITY_READ_PARENT_SET_FUNC(type) setEntityReadParent_ ## type

typedef struct EntityReadParent EntityReadParent_t;
struct EntityReadParent {
    const u16 boardNumber;
    const TYPEOF_STRUCT(EntityInfo, entities_count) entityNumber;
    const TYPEOF_STRUCT(Entity, fields_count) fieldNumber;

    const TYPEOF_STRUCT(EntityField, type) type;
    const TYPEOF_STRUCT(EntityField, bitFlags) bitFlags;
    b onValueUpdated;
    const reg size;
    void* data;

    int (* const set)(EntityReadParent_t * const self, u8* input, const reg inputSize);
};

int setEntityReadParent_uni(EntityReadParent_t * const self, u8* input, const reg inputSize);

#define setEntityReadParent_u8  setEntityReadParent_uni
#define setEntityReadParent_u16 setEntityReadParent_uni
#define setEntityReadParent_u24 setEntityReadParent_uni
#define setEntityReadParent_u32 setEntityReadParent_uni
#define setEntityReadParent_u64 setEntityReadParent_uni

#define setEntityReadParent_c8  setEntityReadParent_uni
#define setEntityReadParent_i8  setEntityReadParent_uni
#define setEntityReadParent_i16 setEntityReadParent_uni
#define setEntityReadParent_i24 setEntityReadParent_uni
#define setEntityReadParent_i32 setEntityReadParent_uni
#define setEntityReadParent_i64 setEntityReadParent_uni

#define setEntityReadParent_f32 setEntityReadParent_uni
#define setEntityReadParent_f64 setEntityReadParent_uni
//#define setEntityReadParent_f128 setEntityReadParent_uni // very hard this support on different platforms

#define setEntityReadParent_b setEntityReadParent_uni
int ENTITY_READ_PARENT_SET_FUNC(sreg)(EntityReadParent_t * const self, u8* input, const reg inputSize); // unique function for sreg type
int ENTITY_READ_PARENT_SET_FUNC(reg)(EntityReadParent_t * const self, u8* input, const reg inputSize);  // unique function for reg type

/*
 * ****************************************************************
 * Chield section
 * ****************************************************************
 */
#define ENTITY_READ_CHIELD(type) EntityReadChield_ ## type ## _t
#define ENTITY_READ_CHIELD_CTOR_FUNC(type) ctorEntityReadChield_ ## type
#define ENTITY_READ_CHIELD_GET_FUNC(type) getEntityReadChield_ ## type
#define ENTITY_READ_CHIELD_SET_FUNC(type) setEntityReadChield_ ## type

#define DECLARE_ORDINARY_ENTITY_READ_CHIELD_TYPE(type) DECLARE_ENTITY_READ_CHIELD_TYPE(type, type)
#define DECLARE_ENTITY_READ_CHIELD_TYPE(type, opType)                                                   \
    typedef struct EntityReadChield_ ## type       ENTITY_READ_CHIELD(type);                            \
    struct EntityReadChield_ ## type {                                                                  \
        EntityReadParent_t parent;                                                                      \
        int     (* const ctor)(ENTITY_READ_CHIELD(type) * const);                                       \
        opType  (* const get)(ENTITY_READ_CHIELD(type) * const);                                        \
        void    (* const set)(ENTITY_READ_CHIELD(type) * const, const opType);                          \
    };                                                                                                  \
    int ENTITY_READ_CHIELD_CTOR_FUNC(type) (ENTITY_READ_CHIELD(type) *const self);                      \
    opType ENTITY_READ_CHIELD_GET_FUNC(type) (ENTITY_READ_CHIELD(type)* const self);                    \
    void ENTITY_READ_CHIELD_SET_FUNC(type) (ENTITY_READ_CHIELD(type)* const self, const opType val);



// read chields declaration--------------------------------------------------------
//DECLARE_ENTITY_READ_CHIELD_TYPE(void);
DECLARE_ORDINARY_ENTITY_READ_CHIELD_TYPE(u8);
DECLARE_ORDINARY_ENTITY_READ_CHIELD_TYPE(u16);

DECLARE_ORDINARY_ENTITY_READ_CHIELD_TYPE(u24);
DECLARE_ORDINARY_ENTITY_READ_CHIELD_TYPE(u32);
DECLARE_ORDINARY_ENTITY_READ_CHIELD_TYPE(u64);

DECLARE_ORDINARY_ENTITY_READ_CHIELD_TYPE(c8);
DECLARE_ORDINARY_ENTITY_READ_CHIELD_TYPE(i8);
DECLARE_ORDINARY_ENTITY_READ_CHIELD_TYPE(i16);
DECLARE_ORDINARY_ENTITY_READ_CHIELD_TYPE(i24);
DECLARE_ORDINARY_ENTITY_READ_CHIELD_TYPE(i32);
DECLARE_ORDINARY_ENTITY_READ_CHIELD_TYPE(i64);

DECLARE_ORDINARY_ENTITY_READ_CHIELD_TYPE(f32);
DECLARE_ORDINARY_ENTITY_READ_CHIELD_TYPE(f64);
//DECLARE_ORDINARY_ENTITY_READ_CHIELD_TYPE(f128);  // very hard this support on different platforms

DECLARE_ORDINARY_ENTITY_READ_CHIELD_TYPE(b);
DECLARE_ENTITY_READ_CHIELD_TYPE(reg, u64);
DECLARE_ENTITY_READ_CHIELD_TYPE(sreg, i64);


#undef DECLARE_ORDINARY_ENTITY_READ_CHIELD_TYPE
#undef DECLARE_ENTITY_READ_CHIELD_TYPE

/*
 * ****************************************************************
 * creation Read Chield macro
 * ****************************************************************
 */

// init chield without external size
#define ENTITY_CREATE_READ_CHIELD(type, name, boardNumber, entityNumber, fieldNumber, bitFlags) ENTITY_CREATE_READ_CHIELD_SIZE(type, name, boardNumber, entityNumber, fieldNumber, bitFlags, sizeof(type))
// init chield with external size
#define ENTITY_CREATE_READ_CHIELD_SIZE(type, name, boardNumber, entityNumber, fieldNumber, bitFlags, size)                                                          \
    ENTITY_READ_CHIELD(type) name = {                                                                                                                               \
    /* parent initialization */                                                                                                                                     \
{boardNumber, entityNumber, fieldNumber, MY_CTYPE_GET_TYPE_ID(type), bitFlags, 0, size, NULL, ENTITY_READ_PARENT_SET_FUNC(type)},                                   \
    /* chield initialization */                                                                                                                                     \
    ENTITY_READ_CHIELD_CTOR_FUNC(type),                                                                                                                             \
    ENTITY_READ_CHIELD_GET_FUNC(type),                                                                                                                              \
    ENTITY_READ_CHIELD_SET_FUNC(type)}

#endif /* USE_ENTITY_READ_SERVICE */

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */

#endif /* ENTITY_READ_BASEH_H */
