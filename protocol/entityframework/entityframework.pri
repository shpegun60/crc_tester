INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD	

SOURCES += \
    $$PWD/entity_macro.c \
    $$PWD/entity_mail_service.c \
    $$PWD/entity_manager.c \
    $$PWD/entity_packet.c \
    $$PWD/entity_tests.c \
    $$PWD/reader/entity_read_base.c \
    $$PWD/reader/entity_read_parent_pool.c \
    $$PWD/reader/entity_read_system.c

HEADERS += \
    $$PWD/entity_atomic.h \
    $$PWD/entity_macro.h \
    $$PWD/entity_mail_service.h \
    $$PWD/entity_manager.h \
    $$PWD/entity_packet.h \
    $$PWD/entity_port.h \
    $$PWD/entity_tests.h \
    $$PWD/reader/entity_non-blocking.h \
    $$PWD/reader/entity_read_base.h \
    $$PWD/reader/entity_read_parent_pool.h \
    $$PWD/reader/entity_read_system.h
