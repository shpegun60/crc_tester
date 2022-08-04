INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD	

SOURCES += \
    $$PWD/crc_test.c\
	$$PWD/crc8.c\
	$$PWD/crc16.c\
	$$PWD/crc32.c\
	$$PWD/crc64.c\

HEADERS += \
    $$PWD/crc_test.h\
	$$PWD/crc8.h\
	$$PWD/crc16.h\
	$$PWD/crc32.h\
	$$PWD/crc64.h\
    $$PWD/my_crc_port.h
