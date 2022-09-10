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

SOURCES +=  \
    $$PWD/protocol_test.c

HEADERS += \
    $$PWD/inline.h \
    $$PWD/protocol.h \
    $$PWD/protocol_test.h
