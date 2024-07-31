QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    INIReader.cpp \
    deskwatch.cpp \
    ini.c \
    main.cpp \
    deskping.cpp

HEADERS += \
    INIReader.h \
    deskping.h \
    deskwatch.h \
    ini.h

FORMS += \
    deskping.ui \
    deskwatch.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    build/Desktop_Qt_6_7_1_MinGW_64_bit-Debug/debug/config/config.ini
