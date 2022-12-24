INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD	

include($$PWD/byte_order/byte_order.pri)
include($$PWD/convert/Cconvert.pri)
include($$PWD/crc/crc.pri)
include($$PWD/my_ctype/my_ctype.pri)
include($$PWD/raw_parser/raw_parser.pri)
include($$PWD/smart_assert/smart_assert.pri)
include($$PWD/preprocessor/preprocessor.pri)
include($$PWD/callback_manager/callback_manager.pri)
include($$PWD/callback_container/callback_container.pri)
include($$PWD/pool_container/pool_container.pri)
include($$PWD/entityframework/entityframework.pri)
include($$PWD/algorithms-in-c/c_algo.pri)
include($$PWD/slip/slip.pri)


SOURCES +=  \
    $$PWD/protocol_test.c

HEADERS += \
    $$PWD/compiler_predef.h \
    $$PWD/inline.h \
    $$PWD/protocol.h \
    $$PWD/protocol_test.h
