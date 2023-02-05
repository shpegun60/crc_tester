INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD	

include($$PWD/raw_parser_dma/raw_parser_dma.pri)
include($$PWD/raw_parser_it/raw_parser_it.pri)
include($$PWD/raw_parser_general/raw_parser_general.pri)

SOURCES += \
    $$PWD/rawparser_macro.c

HEADERS += \
    $$PWD/rawparser_macro.h \
	$$PWD/rawparser_port.h
