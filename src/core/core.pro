#-------------------------------------------------
#
# Project created by QtCreator 2014-06-17T23:02:40
#
#-------------------------------------------------

QT       -= core gui

TARGET = core
TEMPLATE = lib
CONFIG += staticlib

SOURCES += asm.c \
           breakpnt.c \
           disas.c \
           event.c \
           gui.c \
           i8080.c \
           mpsys.c \
           myglib.c

HEADERS += asm.h \
           breakpnt.h \
           disas.h \
           event.h \
           gui.h \
           i8080.h \
           i8080_macros.h \
           mpsys.h \
           myglib.h \
           plugin.h \
           types.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
