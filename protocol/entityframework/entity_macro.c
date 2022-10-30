#include "entity_macro.h"

#define ENTITY_TUPLE PREPROCESSOR_TUPLE_MAKE(                                           \
            (entityName_t, "entt", 0, 0, NULL),                                         \
                                                                                        \
            (u8,  _name1, (0x00 | 0x01), "des1", 1, 1, 1),                              \
            (u32,  _name2, (0x00 | 0x01), "des1", 1, 1, 1),                              \
            (u16, _nameN, (0x00),        "desN", 1, 1, 3)                               \
    )

ENTITY_MACRO_EXPAND_STRUCT(ENTITY_TUPLE);

int entityMactoTest()
{
    reg entityNumber;
    reg fieldNumber = 0;
    Entity * entity;

    entityName_t  data;
    entityName_t* data_ptr = &data;

    (entityNumber) = 0x010000UL;
    initEntity(&(entityNumber), (1 + 1 + 1), sizeof(entityName_t), "entt", 0, 0,
               ((void *)0));
    if ((entityNumber) == 0x010000UL) {
        return 0x00;
    }
    (entity) = getEntityPointer(entityNumber);
    if ((entity) == ((void *)0)) {
        return 0x00;
    }
    if ((initField((entity), (&fieldNumber), ((0x00 | 0x01)),
                   __builtin_offsetof(entityName_t, _name1), (1), ("des1"),
                   &((data_ptr)->_name1))) == 0x010000UL) {
        return 0x00;
    }
    if ((initField((entity), (&fieldNumber), ((0x00 | 0x01)),
                   __builtin_offsetof(entityName_t, _name2), (3), ("des1"),
                   &((data_ptr)->_name2))) == 0x010000UL) {
        return 0x00;
    }
    if ((initField((entity), (&fieldNumber), ((0x00)),
                   __builtin_offsetof(entityName_t, _nameN), (2), ("desN"),
                   &((data_ptr)->_nameN))) == 0x010000UL) {
        return 0x00;
    };
    return 0;
}

#undef ENTITY_TUPLE
