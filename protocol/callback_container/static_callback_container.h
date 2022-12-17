#ifndef STATIC_CALLBACK_CONTAINER_H
#define STATIC_CALLBACK_CONTAINER_H

#include "my_ctypes.h"
#include "preprocessor_template.h"
#include "preprocessor_ctx.h"

/* fast calling callback container WARNING!!! --> if callback is NULL, MAY BE SEGMENTATION FAULT!!!!!!! */
#define SCC_CALL(desc)\
    (desc)->callback((desc)->context)

/* safe calling callback container */
#define SCC_SAFE_CALL(desc)                                                     \
    do{                                                                         \
        const TYPEOF_DATA((desc)->callback) _callback   = (desc)->callback;     \
        const TYPEOF_DATA((desc)->context)  _ctx        = (desc)->context;      \
        if(_callback) {                                                         \
            _callback(_ctx);                                                    \
        }                                                                       \
    }while(0L)

/* safe calling callback container with value getting */
#define SCC_SAFE_CALL_VAL(desc, value)                                          \
    do{                                                                         \
        const TYPEOF_DATA((desc)->callback) _callback   = (desc)->callback;     \
        const TYPEOF_DATA((desc)->context)  _ctx        = (desc)->context;      \
        if(_callback) {                                                         \
            (value) = _callback(_ctx);                                          \
        }                                                                       \
    }while(0L)

/* safe calling callback container with descriptor nullptr check */
#define SCC_SAFE_CALL_DESC(desc)                                                \
    do{                                                                         \
        const TYPEOF_DATA((desc)->callback) _callback   = (desc)->callback;     \
        const TYPEOF_DATA((desc)->context)  _ctx        = (desc)->context;      \
        if((desc) && _callback) {                                               \
            _callback(_ctx);                                                    \
        }                                                                       \
    }while(0L)

/* safe calling callback container with value getting and descriptor nullptr check */
#define SCC_SAFE_CALL_DESC_VAL(desc, value)                                     \
    do{                                                                         \
        const TYPEOF_DATA((desc)->callback) _callback   = (desc)->callback;     \
        const TYPEOF_DATA((desc)->context)  _ctx        = (desc)->context;      \
        if((desc) && _callback) {                                               \
            (value) = _callback(_ctx);                                          \
        }                                                                       \
    }while(0L)


/* fast get context macro */
#define SCC_GET_CONTEXT(desc)\
    (desc)->context

/* fast get callback macro */
#define SCC_GET_CALLBACK(desc)\
    (desc)->callback

#define HEADER_STATIC_CALLBACK_CONTAINER_MACRO(rT)                                                                      \
    typedef const struct {                                          /*  static callback container struct    */          \
        rT (* const callback)(PREPROCESSOR_CTX_TYPE(context));      /*  static callback function            */          \
        PREPROCESSOR_CTX_TYPE(context);                             /*  static callback context             */          \
    } TEMPLATE(StaticCallbackContainer, rT);                                                                            \


HEADER_STATIC_CALLBACK_CONTAINER_MACRO(void);
HEADER_STATIC_CALLBACK_CONTAINER_MACRO(u8);
HEADER_STATIC_CALLBACK_CONTAINER_MACRO(u16);
HEADER_STATIC_CALLBACK_CONTAINER_MACRO(u24);
HEADER_STATIC_CALLBACK_CONTAINER_MACRO(u32);
HEADER_STATIC_CALLBACK_CONTAINER_MACRO(u64);

HEADER_STATIC_CALLBACK_CONTAINER_MACRO(c8);
HEADER_STATIC_CALLBACK_CONTAINER_MACRO(i8);
HEADER_STATIC_CALLBACK_CONTAINER_MACRO(i16);
HEADER_STATIC_CALLBACK_CONTAINER_MACRO(i24);
HEADER_STATIC_CALLBACK_CONTAINER_MACRO(i32);
HEADER_STATIC_CALLBACK_CONTAINER_MACRO(i64);

HEADER_STATIC_CALLBACK_CONTAINER_MACRO(f32);
HEADER_STATIC_CALLBACK_CONTAINER_MACRO(f64);
HEADER_STATIC_CALLBACK_CONTAINER_MACRO(f128);

HEADER_STATIC_CALLBACK_CONTAINER_MACRO(b);
HEADER_STATIC_CALLBACK_CONTAINER_MACRO(reg);
HEADER_STATIC_CALLBACK_CONTAINER_MACRO(sreg);


#endif /* STATIC_CALLBACK_CONTAINER_H */
