QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


include(protocol/protocol.pri)

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h \
    preprocessor/preprocessor.h \
    preprocessor/preprocessor_arguments.h \
    preprocessor/preprocessor_arithmetic.h \
    preprocessor/preprocessor_base.h \
    preprocessor/preprocessor_boolean.h \
    preprocessor/preprocessor_char_eq.h \
    preprocessor/preprocessor_comparison.h \
    preprocessor/preprocessor_complement.h \
    preprocessor/preprocessor_concatenation.h \
    preprocessor/preprocessor_cycle.h \
    preprocessor/preprocessor_detection.h \
    preprocessor/preprocessor_div.h \
    preprocessor/preprocessor_if.h \
    preprocessor/preprocessor_keyword_eq.h \
    preprocessor/preprocessor_logical.h \
    preprocessor/preprocessor_map.h \
    preprocessor/preprocessor_nat_eq.h \
    preprocessor/preprocessor_serial.h \
    preprocessor/preprocessor_stringify.h \
    preprocessor/preprocessor_symbol.h \
    preprocessor/preprocessor_template.h \
    preprocessor/preprocessor_tuple.h \
    preprocessor_template.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
