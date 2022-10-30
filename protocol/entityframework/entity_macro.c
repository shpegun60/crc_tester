#include "entity_macro.h"

#define ENTITY_TUPLE PREPROCESSOR_TUPLE_MAKE(                                           \
            (entityName_t, "entt", 0, 0, NULL, (parent1 p1; parent2 p2; parent3 p3;)),                                         \
                                                                                        \
            (u8,  _name1, (0x00 | 0x01), "des1", 1, 1, 1, volatile, p1._name1),                              \
            (u32,  _name2, (0x00 | 0x01), "des1", 2, 1, 1,, p2._name2),                             \
            (u16, _nameN, (0x00),        "desN", 1, 1, 3,, p3._nameN)                               \
    )

ENTITY_MACRO_EXPAND_STRUCT(ENTITY_TUPLE);

int entityMactoTest()
{
    reg entityNumber;
    reg fieldNumber = 0;
    Entity * entity;

    entityName_t  data;
    entityName_t* data_ptr = &data;

    ENTITY_MACRO_EXPAND_INIT(return ENTITY_ERROR, entityNumber, fieldNumber, entity, data_ptr, ENTITY_TUPLE);
    return 0;
}

#undef ENTITY_OK
