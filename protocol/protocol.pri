INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD	

include($$PWD/byte_order/byte_order.pri)
include($$PWD/convert/Cconvert.pri)
include($$PWD/crc/crc.pri)
include($$PWD/my_ctype/my_ctype.pri)
include($$PWD/raw_parser_dma/raw_parser_dma.pri)
include($$PWD/smart_assert/smart_assert.pri)
#include($$PWD/preprocessor_macro/preprocessor.pri)

SOURCES += 

HEADERS += \
    $$PWD/inline.h
