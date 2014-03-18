QT       -= core
QT       -= gui

QMAKE_MAKEFILE = Makefile_creator

TARGET = term_device
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.c \
    TimeToEdge.c \
    termdevice.c \
    softuart_rs485.c \
    softuart_helper_rs485.c \
    rand_helper.c \
    protocol.c \
    debug_helper.c \
    buttons_handler.c \
    branchslavemanager.c

INCLUDEPATH += ../protoparser/
INCLUDEPATH += ../nrf51_sdk/Include \
    ../nrf51_sdk/Include/app_common

HEADERS += \
    ../protoparser/branchslavemanager_common.h
