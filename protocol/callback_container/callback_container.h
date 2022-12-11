#include "preprocessor_template.h"
#include "my_ctypes.h"

#ifndef INIT_CALLBACK_CONTAINER_MACRO
#include <stdlib.h>


/* fast calling callback container WARNING!!! --> if callback is NULL, MAY BE SEGMENTATION FAULT!!!!!!! */
#define CC_CALL(desc)\
    (desc)->callback((desc)->context)

/* safe calling callback container */
#define CC_SAFE_CALL(desc)                                  \
    if((desc)->callback) {                                  \
        (desc)->callback((desc)->context);                  \
    }

/* safe calling callback container with value getting */
#define CC_SAFE_CALL_VAL(desc, value)                       \
    if((desc)->callback) {                                  \
        (value) = (desc)->callback((desc)->context);        \
    }

/* safe calling callback container with descriptor nullptr check */
#define CC_SAFE_CALL_DESC(desc)                             \
    if((desc) && (desc)->callback) {                        \
        (desc)->callback((desc)->context);                  \
    }

/* safe calling callback container with value getting and descriptor nullptr check */
#define CC_SAFE_CALL_DESC_VAL(desc, value)                  \
    if((desc) && (desc)->callback) {                        \
        (value) = (desc)->callback((desc)->context);        \
    }


/* fast get context macro */
#define CC_GET_CONTEXT(desc)\
    (desc)->context

/* fast get callback macro */
#define CC_GET_CALLBACK(desc)\
    (desc)->callback



#define INIT_CALLBACK_CONTAINER_MACRO(rT, cT)                                                                                                                               \
    typedef struct {                           /*  callback container struct    */                                                                                          \
        rT (*callback)(cT context);            /*  callback function            */                                                                                          \
        cT context;                            /*  callback context             */                                                                                          \
    } TEMPLATE(CallbackContainer, rT, cT);                                                                                                                                  \
                                                                                                                                                                            \
                                                                                                                                                                            \
/* new callback container with user context parameters */                                                                                                                   \
TEMPLATE(CallbackContainer, rT, cT)* TEMPLATE(newCallbackContainerUser, rT, cT) (rT (* const callback)(cT context), cT const context, b* const ok)                          \
{                                                                                                                                                                           \
    TEMPLATE(CallbackContainer, rT, cT) * m_container = (TEMPLATE(CallbackContainer, rT, cT) *) malloc(sizeof(TEMPLATE(CallbackContainer, rT, cT)));                        \
    if(m_container == NULL) {                                                                                                                                               \
        *ok = 0;                                                                                                                                                            \
        return NULL;                                                                                                                                                        \
    }                                                                                                                                                                       \
                                                                                                                                                                            \
    m_container->callback = callback;                                                                                                                                       \
    m_container->context  = context;                                                                                                                                        \
    *ok = 1;                                                                                                                                                                \
    return m_container;                                                                                                                                                     \
}                                                                                                                                                                           \
                                                                                                                                                                            \
/* new callback container with context allocation */                                                                                                                        \
TEMPLATE(CallbackContainer, rT, cT)* TEMPLATE(newCallbackContainerHeap, rT, cT) (rT (* const callback)(cT context), const reg heapContextSize, b* const ok)                 \
{                                                                                                                                                                           \
    TEMPLATE(CallbackContainer, rT, cT) * m_container = (TEMPLATE(CallbackContainer, rT, cT) *) malloc(sizeof(TEMPLATE(CallbackContainer, rT, cT)));                        \
    if(m_container == NULL) {                                                                                                                                               \
        *ok = 0;                                                                                                                                                            \
        return NULL;                                                                                                                                                        \
    }                                                                                                                                                                       \
                                                                                                                                                                            \
    m_container->context = (TYPEOF_STRUCT(TEMPLATE(CallbackContainer, rT, cT), context))calloc(1, heapContextSize);                                                         \
    if(m_container->context == NULL) {                                                                                                                                      \
        free(m_container);                                                                                                                                                  \
        m_container = NULL;                                                                                                                                                 \
        *ok = 0;                                                                                                                                                            \
        return NULL;                                                                                                                                                        \
    }                                                                                                                                                                       \
    m_container->callback = callback;                                                                                                                                       \
    *ok = 1;                                                                                                                                                                \
    return m_container;                                                                                                                                                     \
}                                                                                                                                                                           \
                                                                                                                                                                            \
/* init callback container with user context parameters */                                                                                                                  \
b TEMPLATE(initCallbackContainerUser, rT, cT) (TEMPLATE(CallbackContainer, rT, cT)* const self, rT (* const callback)(cT context), cT const context)                        \
{                                                                                                                                                                           \
    if(self == NULL) {                                                                                                                                                      \
        return 0;                                                                                                                                                           \
    }                                                                                                                                                                       \
                                                                                                                                                                            \
    self->callback = callback;                                                                                                                                              \
    self->context  = context;                                                                                                                                               \
    return 1;                                                                                                                                                               \
}                                                                                                                                                                           \
                                                                                                                                                                            \
/* init callback container with context allocation */                                                                                                                       \
b TEMPLATE(initCallbackContainerHeap, rT, cT) (TEMPLATE(CallbackContainer, rT, cT)* const self, rT (* const callback)(cT context), const reg heapContextSize)               \
{                                                                                                                                                                           \
    if(self == NULL) {                                                                                                                                                      \
        return 0;                                                                                                                                                           \
    }                                                                                                                                                                       \
    self->context = (TYPEOF_STRUCT(TEMPLATE(CallbackContainer, rT, cT), context))calloc(1, heapContextSize);                                                                \
    if(self->context == NULL) {                                                                                                                                             \
        return 0;                                                                                                                                                           \
    }                                                                                                                                                                       \
                                                                                                                                                                            \
    self->callback = callback;                                                                                                                                              \
    return 1;                                                                                                                                                               \
}                                                                                                                                                                           \
                                                                                                                                                                            \
                                                                                                                                                                            \
                                                                                                                                                                            \
/* safe invoke callback container */                                                                                                                                        \
rT TEMPLATE(callbackContainerCall, rT, cT) (TEMPLATE(CallbackContainer, rT, cT)* const self, b* const ok)                                                                   \
{                                                                                                                                                                           \
    if((self != NULL) && (self->callback != NULL)) {                                                                                                                        \
        *ok = 1;                                                                                                                                                            \
        return self->callback(self->context);                                                                                                                               \
    }                                                                                                                                                                       \
    *ok = 0;                                                                                                                                                                \
    return ((rT) 0);                                                                                                                                                        \
}                                                                                                                                                                           \
                                                                                                                                                                            \

#endif /* INIT_CALLBACK_CONTAINER_MACRO */

typedef void*  p1;
typedef void** p2;

// for p1 type--------------------------------------------------------
INIT_CALLBACK_CONTAINER_MACRO(void, p1);
INIT_CALLBACK_CONTAINER_MACRO(u8,   p1);
INIT_CALLBACK_CONTAINER_MACRO(u16,  p1);
INIT_CALLBACK_CONTAINER_MACRO(u24,  p1);
INIT_CALLBACK_CONTAINER_MACRO(u32,  p1);
INIT_CALLBACK_CONTAINER_MACRO(u64,  p1);

INIT_CALLBACK_CONTAINER_MACRO(c8,   p1);
INIT_CALLBACK_CONTAINER_MACRO(i8,   p1);
INIT_CALLBACK_CONTAINER_MACRO(i16,  p1);
INIT_CALLBACK_CONTAINER_MACRO(i24,  p1);
INIT_CALLBACK_CONTAINER_MACRO(i32,  p1);
INIT_CALLBACK_CONTAINER_MACRO(i64,  p1);

INIT_CALLBACK_CONTAINER_MACRO(f32,  p1);
INIT_CALLBACK_CONTAINER_MACRO(f64,  p1);
INIT_CALLBACK_CONTAINER_MACRO(f128, p1);

INIT_CALLBACK_CONTAINER_MACRO(b,    p1);
INIT_CALLBACK_CONTAINER_MACRO(reg,  p1);
INIT_CALLBACK_CONTAINER_MACRO(sreg, p1);

// for p2 type---------------------------------------------------------
INIT_CALLBACK_CONTAINER_MACRO(void, p2);
INIT_CALLBACK_CONTAINER_MACRO(u8,   p2);
INIT_CALLBACK_CONTAINER_MACRO(u16,  p2);
INIT_CALLBACK_CONTAINER_MACRO(u24,  p2);
INIT_CALLBACK_CONTAINER_MACRO(u32,  p2);
INIT_CALLBACK_CONTAINER_MACRO(u64,  p2);

INIT_CALLBACK_CONTAINER_MACRO(c8,   p2);
INIT_CALLBACK_CONTAINER_MACRO(i8,   p2);
INIT_CALLBACK_CONTAINER_MACRO(i16,  p2);
INIT_CALLBACK_CONTAINER_MACRO(i24,  p2);
INIT_CALLBACK_CONTAINER_MACRO(i32,  p2);
INIT_CALLBACK_CONTAINER_MACRO(i64,  p2);

INIT_CALLBACK_CONTAINER_MACRO(f32,  p2);
INIT_CALLBACK_CONTAINER_MACRO(f64,  p2);
INIT_CALLBACK_CONTAINER_MACRO(f128, p2);

INIT_CALLBACK_CONTAINER_MACRO(b,    p2);
INIT_CALLBACK_CONTAINER_MACRO(reg,  p2);
INIT_CALLBACK_CONTAINER_MACRO(sreg, p2);



#undef INIT_CALLBACK_CONTAINER_MACRO
