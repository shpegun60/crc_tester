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

    ENTITY_MACRO_EXPAND_INIT(return ENTITY_ERROR, entityNumber, fieldNumber, entity, data_ptr, ENTITY_TUPLE);
    return 0;
}

#undef ENTITY_TUPLE
