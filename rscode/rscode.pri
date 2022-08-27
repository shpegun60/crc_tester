INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

SOURCES +=  \
    $$PWD/berlekamp.c \
    $$PWD/rs_ecc.c \
    $$PWD/galois.c \
    $$PWD/rs.c \
    $$PWD/rscode_test.c

HEADERS += \
	$$PWD/berlekamp.h \
    $$PWD/rs_ecc.h \
    $$PWD/galois.h \
    $$PWD/rs.h \
    $$PWD/rscode-config.h \
    $$PWD/rscode_test.h
