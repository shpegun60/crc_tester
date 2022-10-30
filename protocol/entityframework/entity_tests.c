#include "entity_tests.h"
#include "entity_manager.h"

#define EN_ASSERT_COUNTING(expression, counter)\
    if(((expression) != ENTITY_OK)) {\
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

    entirtyAllTest_t entityTest1;



    reg entityNumber = ENTITY_NUMBER_ERROR;
    reg fieldNumber = 0;
    Entity * entity;

    EN_ASSERT_COUNTING(initEntity(&entityNumber, TYPE_ARRAY_LENGTH, sizeof(entirtyAllTest_t), "E1  ", 0, 0, NULL), testNotValid);

    entity = getEntityPointer(entityNumber);
    EN_ASSERT_COUNTING((entity == NULL), testNotValid);

    initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _1), UINT8_TYPE,        "F1", &entityTest1._1);
    initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _2), UINT8_TYPE,        "F2", &entityTest1._2);
    initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _3), UINT16_TYPE,       "F3", &entityTest1._3);
    initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _4), UINT24_TYPE,       "F4", &entityTest1._4);
    initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _5), UINT32_TYPE,       "F5", &entityTest1._5);
    initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _6), UINT64_TYPE,       "F6", &entityTest1._6);
    initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _7), CHAR_TYPE,         "F7", &entityTest1._7);
    initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _8), INT8_TYPE,         "F8", &entityTest1._8);
    initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _9), INT16_TYPE,        "F9", &entityTest1._9);
    initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _10), INT24_TYPE,       "F10", &entityTest1._10);
    initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _11), INT32_TYPE,       "F11", &entityTest1._11);
    initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _12), INT64_TYPE,       "F12", &entityTest1._12);
    initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _13), FLOAT_TYPE,       "F13", &entityTest1._13);
    initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _14), DOUBLE_TYPE,       "F14", &entityTest1._14);
    initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _15), LONG_DOUBLE_TYPE,       "F15", &entityTest1._15);
    initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _16), BOOL_TYPE,       "F16", &entityTest1._16);
    initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _17), REG_TYPE,       "F17", &entityTest1._17);
    initField(entity, &fieldNumber, (ENTITY_EMPTY_FLAG), offsetof(entirtyAllTest_t, _18), SREG_TYPE,       "F18", &entityTest1._18);



    //------------------------------------------------------------------------------------------------------------------
    return testNotValid;
}

int entityTest(int randomSeed, int testCnt)
{
    int testNotValid = 0;
    //------------------------------------------------------------------------------------------------------------------

    // allocation entities
    EN_ASSERT_COUNTING(newEntities(10), testNotValid);

    testNotValid += generalTest(testCnt);



    //------------------------------------------------------------------------------------------------------------------
    return testNotValid;
}


#undef EN_ASSERT_COUNTING
