#include "entity_tests.h"

#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#ifndef  ENTITY_TEST_DISABLE
#include "entity_manager.h"

#define EN_ASSERT_COUNTING(expression, counter)\
    if((expression)) {\
        ++counter;\
        return counter;\
    }


int generalTest(int testCnt)
{
    int testNotValid = 0;
    //------------------------------------------------------------------------------------------------------------------

    typedef struct {
        /* uni */ u8 _1; // do not check this type because void type is not exist, error type
        u8  _2;
        u16 _3;
        u24 _4;
        u32 _5;
        u64 _6;

        c8  _7;
        i8  _8;
        i16 _9;
        i24 _10;
        i32 _11;
        i64 _12;

        f32  _13;
        f64  _14;
        f128 _15; // platform depend type

        // bool type -------------------------
        b _16;

        // bus types defining (platform depend) ----------------------------------------------
        reg  _17;    // platform depend type
        sreg _18;   // platform depend type
        //------------------------------------------------------------------------------------

    } entirtyAllTest_t;

    EntityField* fields[TYPE_ARRAY_LENGTH];

    reg entityNumber = ENTITY_NUMBER_ERROR;
    reg fieldNumber = 0;
    Entity * entity;

    EN_ASSERT_COUNTING(initEntity(&entityNumber, TYPE_ARRAY_LENGTH, sizeof(entirtyAllTest_t), "E1  ", 0, 0, NULL) != ENTITY_OK, testNotValid);

    entity = getEntityPointer(entityNumber);
    entirtyAllTest_t* entityTest1 = getVoidPointer(entityNumber);
    EN_ASSERT_COUNTING((entity == NULL), testNotValid);

    EN_ASSERT_COUNTING(initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _1), UINT8_TYPE,                "F1", &entityTest1->_1)   != ENTITY_OK, testNotValid);
    EN_ASSERT_COUNTING(initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _2), UINT8_TYPE,                "F2", &entityTest1->_2)   != ENTITY_OK, testNotValid);
    EN_ASSERT_COUNTING(initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _3), UINT16_TYPE,               "F3", &entityTest1->_3)   != ENTITY_OK, testNotValid);
    EN_ASSERT_COUNTING(initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _4), UINT24_TYPE,               "F4", &entityTest1->_4)   != ENTITY_OK, testNotValid);
    EN_ASSERT_COUNTING(initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _5), UINT32_TYPE,               "F5", &entityTest1->_5)   != ENTITY_OK, testNotValid);
    EN_ASSERT_COUNTING(initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _6), UINT64_TYPE,               "F6", &entityTest1->_6)   != ENTITY_OK, testNotValid);
    EN_ASSERT_COUNTING(initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _7), CHAR_TYPE,                 "F7", &entityTest1->_7)   != ENTITY_OK, testNotValid);
    EN_ASSERT_COUNTING(initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _8), INT8_TYPE,                 "F8", &entityTest1->_8)   != ENTITY_OK, testNotValid);
    EN_ASSERT_COUNTING(initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _9), INT16_TYPE,                "F9", &entityTest1->_9)   != ENTITY_OK, testNotValid);
    EN_ASSERT_COUNTING(initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _10), INT24_TYPE,               "F10", &entityTest1->_10) != ENTITY_OK, testNotValid);
    EN_ASSERT_COUNTING(initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _11), INT32_TYPE,               "F11", &entityTest1->_11) != ENTITY_OK, testNotValid);
    EN_ASSERT_COUNTING(initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _12), INT64_TYPE,               "F12", &entityTest1->_12) != ENTITY_OK, testNotValid);
    EN_ASSERT_COUNTING(initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _13), FLOAT_TYPE,               "F13", &entityTest1->_13) != ENTITY_OK, testNotValid);
    EN_ASSERT_COUNTING(initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _14), DOUBLE_TYPE,              "F14", &entityTest1->_14) != ENTITY_OK, testNotValid);
    EN_ASSERT_COUNTING(initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _15), LONG_DOUBLE_TYPE,         "F15", &entityTest1->_15) != ENTITY_OK, testNotValid);
    EN_ASSERT_COUNTING(initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _16), BOOL_TYPE,                "F16", &entityTest1->_16) != ENTITY_OK, testNotValid);
    EN_ASSERT_COUNTING(initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _17), REG_TYPE,                 "F17", &entityTest1->_17) != ENTITY_OK, testNotValid);
    EN_ASSERT_COUNTING(initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _18), SREG_TYPE,                "F18", &entityTest1->_18) != ENTITY_OK, testNotValid);

    //container_of(entity, EntityField, fields);

    for(reg i = 0; i < TYPE_ARRAY_LENGTH; ++i) {
        fields[i] = getFieldPointer(entityNumber, i);
    }




    //------------------------------------------------------------------------------------------------------------------
    return testNotValid;
}

int entityTest(int randomSeed, int testCnt)
{
    int testNotValid = 0;
    //------------------------------------------------------------------------------------------------------------------

    // allocation entities
    EN_ASSERT_COUNTING(newEntities(10) != ENTITY_OK, testNotValid);

    testNotValid += generalTest(testCnt);



    //------------------------------------------------------------------------------------------------------------------
    return testNotValid;
}

#endif /* ENTITY_TEST_DISABLE */

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */

#undef EN_ASSERT_COUNTING
