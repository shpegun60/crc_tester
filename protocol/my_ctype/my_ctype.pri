INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD	

SOURCES += \
	$$PWD/my_ctypes.c\
    $$PWD/runtime_cast/my_ctype_runtime_cast.c \
    $$PWD/runtime_cast/runtime_cast_template.c

HEADERS += \
    $$PWD/my_ctype_cast.h \
    $$PWD/my_ctype_id.h \
    $$PWD/my_ctypes.h\
    $$PWD/my_ctypes_def.h \
    $$PWD/runtime_cast/my_ctype_runtime_cast.h \
    $$PWD/runtime_cast/runtime_cast_template.h
