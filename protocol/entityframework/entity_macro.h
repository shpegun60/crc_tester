#ifndef ENTITY_MACRO_H
#define ENTITY_MACRO_H


#include "preprocessor.h"
#include "my_ctypes.h"
#include "my_ctype_id.h"
#include "entity_manager.h"

// TUPLE((entityName, descr, isCustomSpace, isHeap, arg), (type1, name1, bitFlags1, descr1, incrementSize1, fieldInitEnable1, arrayStartNumber1 (if not array not use)) ... (typeN, nameN, bitFlagsN, descrN, incrementSizeN, fieldInitEnableN, arrayStartNumberN (if not array not use)))
// parameter: (breakExpr, entityNumber, fieldNumber, entityPtr, structObjectPointer, structName)

#define ENTITY_STRUCT_MAKE_VAR(x) PREPROCESSOR_GET_ARG(0)(PREPROCESSOR_UNTUPLE(x)) PREPROCESSOR_GET_ARG(1)(PREPROCESSOR_UNTUPLE(x))   /* for making struct fields (auto adds [] if this field is array)*/  \
                                  PREPROCESSOR_IF( PREPROCESSOR_PRIMITIVE_NOT(PREPROCESSOR_NAT_EQ(PREPROCESSOR_GET_ARG(4)(PREPROCESSOR_UNTUPLE(x)), 1)) )([PREPROCESSOR_GET_ARG(4)(PREPROCESSOR_UNTUPLE(x))])



#define ENTITY_STRUCT_COUNTER(x) PREPROCESSOR_GET_ARG(4)(PREPROCESSOR_UNTUPLE(x))                                                   // for counting fields

#define ENTITY_STRUCT_INIT_FIELD(par, x) if ((PREPROCESSOR_IF_ELSE(PREPROCESSOR_NAT_EQ(PREPROCESSOR_GET_ARG(4)(PREPROCESSOR_UNTUPLE(x)), 1))( \
                                            PREPROCESSOR_IF_ELSE( PREPROCESSOR_AND(PREPROCESSOR_BOOL(PREPROCESSOR_GET_ARG(4)(PREPROCESSOR_UNTUPLE(par))), PREPROCESSOR_BOOL(PREPROCESSOR_GET_ARG(5)(PREPROCESSOR_UNTUPLE(x)))) )( \
                                                                                        initField(     (PREPROCESSOR_GET_ARG(3)(PREPROCESSOR_UNTUPLE(par))),                                                                                                                                                    /* entityPtr */                                     \
                                                                                                                                            (&PREPROCESSOR_GET_ARG(2)(PREPROCESSOR_UNTUPLE(par))),                                                                                                              /* &fieldNumber */                                  \
                                                                                                                                            (PREPROCESSOR_GET_ARG(2)(PREPROCESSOR_UNTUPLE(x))),                                                                                                                 /* bitFlagsN */                                     \
                                                                                                                                            offsetof(PREPROCESSOR_GET_ARG(5)(PREPROCESSOR_UNTUPLE(par)), PREPROCESSOR_GET_ARG(1)(PREPROCESSOR_UNTUPLE(x))),                                                     /* offsetof(structName, nameN) */                   \
                                                                                                                                            (MY_CTYPE_GET_TYPE_ID(PREPROCESSOR_GET_ARG(0)(PREPROCESSOR_UNTUPLE(x)))),                                                                                           /* number of typeN */                               \
                                                                                                                                            (PREPROCESSOR_GET_ARG(3)(PREPROCESSOR_UNTUPLE(x))),                                                                                                                 /* descrN */                                        \
                                                                                                                                            &((PREPROCESSOR_GET_ARG(4)(PREPROCESSOR_UNTUPLE(par)))->PREPROCESSOR_GET_ARG(1)(PREPROCESSOR_UNTUPLE(x)))) ,                                                        /* &structObjectPointer->nameN */                   \
                                                                                                                                                                                                                                                                                                                                                                    \
                                                                                        initField(     (PREPROCESSOR_GET_ARG(3)(PREPROCESSOR_UNTUPLE(par))),                                                                                                                                                    /* entityPtr */                                     \
                                                                                                                                            (&PREPROCESSOR_GET_ARG(2)(PREPROCESSOR_UNTUPLE(par))),                                                                                                              /* &fieldNumber */                                  \
                                                                                                                                            (PREPROCESSOR_GET_ARG(2)(PREPROCESSOR_UNTUPLE(x))),                                                                                                                 /* bitFlagsN */                                     \
                                                                                                                                            offsetof(PREPROCESSOR_GET_ARG(5)(PREPROCESSOR_UNTUPLE(par)), PREPROCESSOR_GET_ARG(1)(PREPROCESSOR_UNTUPLE(x))),                                                     /* offsetof(structName, nameN) */                   \
                                                                                                                                            (MY_CTYPE_GET_TYPE_ID(PREPROCESSOR_GET_ARG(0)(PREPROCESSOR_UNTUPLE(x)))),                                                                                           /* number of typeN */                               \
                                                                                                                                            (PREPROCESSOR_GET_ARG(3)(PREPROCESSOR_UNTUPLE(x))),                                                                                                                 /* descrN */                                        \
                                                                                                                                            NULL)                                                                                                                                                               /* field_ptr */                                     \
                                            ),                                                                                                                                                                                                                                                                                                                      \
                                                                                                                                                                                                                                                                                                                                                                    \
                                            PREPROCESSOR_IF_ELSE( PREPROCESSOR_AND(PREPROCESSOR_BOOL(PREPROCESSOR_GET_ARG(4)(PREPROCESSOR_UNTUPLE(par))), PREPROCESSOR_BOOL(PREPROCESSOR_GET_ARG(5)(PREPROCESSOR_UNTUPLE(x)))) )(                                                                                                                                   \
                                                                                        initFieldArray( (PREPROCESSOR_GET_ARG(3)(PREPROCESSOR_UNTUPLE(par))),                                                                                                                                                   /* entityPtr */                                     \
                                                                                                                                            (&PREPROCESSOR_GET_ARG(2)(PREPROCESSOR_UNTUPLE(par))),                                                                                                              /* &fieldNumber */                                  \
                                                                                                                                            (PREPROCESSOR_GET_ARG(2)(PREPROCESSOR_UNTUPLE(x))),                                                                                                                 /* bitFlagsN */                                     \
                                                                                                                                            offsetof(PREPROCESSOR_GET_ARG(5)(PREPROCESSOR_UNTUPLE(par)), PREPROCESSOR_GET_ARG(1)(PREPROCESSOR_UNTUPLE(x))),                                                     /* offsetof(structName, nameN) */                   \
                                                                                                                                            (MY_CTYPE_GET_TYPE_ID(PREPROCESSOR_GET_ARG(0)(PREPROCESSOR_UNTUPLE(x)))),                                                                                           /* number of typeN */                               \
                                                                                                                                            (PREPROCESSOR_GET_ARG(4)(PREPROCESSOR_UNTUPLE(x))),                                                                                                                 /* incrementSizeN */                                \
                                                                                                                                            (PREPROCESSOR_GET_ARG(3)(PREPROCESSOR_UNTUPLE(x))),                                                                                                                 /* descrN */                                        \
                                                                                                                                            &((PREPROCESSOR_GET_ARG(4)(PREPROCESSOR_UNTUPLE(par)))->PREPROCESSOR_GET_ARG(1)(PREPROCESSOR_UNTUPLE(x))) ,                                                         /* &structObjectPointer->nameN */                   \
                                                                                                                                            PREPROCESSOR_GET_ARG(6)(PREPROCESSOR_UNTUPLE(x))),                                                                                                                  /* arrayStartNumber */                              \
                                                                                                                                                                                                                                                                                                                                                                    \
                                                                                        initFieldArray( (PREPROCESSOR_GET_ARG(3)(PREPROCESSOR_UNTUPLE(par))),                                                                                                                                                   /* entityPtr */                                     \
                                                                                                                                            (&PREPROCESSOR_GET_ARG(2)(PREPROCESSOR_UNTUPLE(par))),                                                                                                              /* &fieldNumber */                                  \
                                                                                                                                            (PREPROCESSOR_GET_ARG(2)(PREPROCESSOR_UNTUPLE(x))),                                                                                                                 /* bitFlagsN */                                     \
                                                                                                                                            offsetof(PREPROCESSOR_GET_ARG(5)(PREPROCESSOR_UNTUPLE(par)), PREPROCESSOR_GET_ARG(1)(PREPROCESSOR_UNTUPLE(x))),                                                     /* offsetof(structName, nameN) */                   \
                                                                                                                                            (MY_CTYPE_GET_TYPE_ID(PREPROCESSOR_GET_ARG(0)(PREPROCESSOR_UNTUPLE(x)))),                                                                                           /* number of typeN */                               \
                                                                                                                                            (PREPROCESSOR_GET_ARG(4)(PREPROCESSOR_UNTUPLE(x))),                                                                                                                 /* incrementSizeN */                                \
                                                                                                                                            (PREPROCESSOR_GET_ARG(3)(PREPROCESSOR_UNTUPLE(x))),                                                                                                                 /* descrN */                                        \
                                                                                                                                            NULL,                                                                                                                                                               /* field_ptr */                                     \
                                                                                                                                            PREPROCESSOR_GET_ARG(6)(PREPROCESSOR_UNTUPLE(x)))                                                                                                                   /* arrayStartNumber */                              \
                                            )                                                                                                                                                                                                                                                                                                                       \
                                        )) == ENTITY_NUMBER_ERROR) {                                                                                                                                                                                                                                                                                                \
                                            PREPROCESSOR_GET_ARG(0)(PREPROCESSOR_UNTUPLE(par));                                                                                                                                                                                                                                                                     \
                                        }







#define ENTITY_MACRO_EXPAND_STRUCT(...) ENTITY_MACRO_EXPAND_STRUCT_IMPL(__VA_ARGS__)
#define ENTITY_MACRO_EXPAND_STRUCT_IMPL(...)\
    typedef struct {\
        PREPROCESSOR_TUPLE_FOR_EACH(ENTITY_STRUCT_MAKE_VAR, PREPROCESSOR_COMMA_POINT, PREPROCESSOR_TUPLE_TAIL(__VA_ARGS__));\
    } PREPROCESSOR_GET_ARG(0)(PREPROCESSOR_UNTUPLE(PREPROCESSOR_GET_ARG(0)(PREPROCESSOR_UNTUPLE(__VA_ARGS__)))) /* entityName */


#define ENTITY_MACRO_EXPAND_INIT(breakExpr, entityNumber, fieldNumber, entityPtr, structObjectPointer, ...)\
    (entityNumber) = ENTITY_NUMBER_ERROR;\
    initEntity(&(entityNumber),\
               (PREPROCESSOR_TUPLE_FOR_EACH(ENTITY_STRUCT_COUNTER, PREPROCESSOR_PLUS, PREPROCESSOR_TUPLE_TAIL(__VA_ARGS__))),       /* (incrementSize1 + ... + incrementSizeN) */                                   \
               sizeof(PREPROCESSOR_GET_ARG(0)(PREPROCESSOR_UNTUPLE(PREPROCESSOR_GET_ARG(0)(PREPROCESSOR_UNTUPLE(__VA_ARGS__))))),   /* sizeof(entityName)   */                                                      \
               PREPROCESSOR_GET_ARG(1)(PREPROCESSOR_UNTUPLE(PREPROCESSOR_GET_ARG(0)(PREPROCESSOR_UNTUPLE(__VA_ARGS__)))),           /* descr                */                                                      \
               PREPROCESSOR_GET_ARG(2)(PREPROCESSOR_UNTUPLE(PREPROCESSOR_GET_ARG(0)(PREPROCESSOR_UNTUPLE(__VA_ARGS__)))),           /* isCustomSpace        */                                                      \
               PREPROCESSOR_GET_ARG(3)(PREPROCESSOR_UNTUPLE(PREPROCESSOR_GET_ARG(0)(PREPROCESSOR_UNTUPLE(__VA_ARGS__)))),           /* isHeap               */                                                      \
               PREPROCESSOR_GET_ARG(4)(PREPROCESSOR_UNTUPLE(PREPROCESSOR_GET_ARG(0)(PREPROCESSOR_UNTUPLE(__VA_ARGS__)))));          /* arg                  */                                                      \
    \
    if((entityNumber) == ENTITY_NUMBER_ERROR) {\
        breakExpr;\
    }\
    \
    \
    (entityPtr) = getEntityPointer(entityNumber);\
    if((entityPtr) == NULL) {\
        breakExpr;\
    }\
    \
    PREPROCESSOR_TUPLE_FOR_EACH_PARAMETER(\
        (breakExpr, entityNumber, fieldNumber, entityPtr, structObjectPointer, PREPROCESSOR_GET_ARG(0)(PREPROCESSOR_UNTUPLE(PREPROCESSOR_GET_ARG(0)(PREPROCESSOR_UNTUPLE(__VA_ARGS__))))), \
        ENTITY_STRUCT_INIT_FIELD, \
        PREPROCESSOR_EMPTY, \
        PREPROCESSOR_TUPLE_TAIL(__VA_ARGS__))



int entityMactoTest();


#endif /* ENTITY_MACRO_H */
