#-------------------------------------------------
#
# Project created by QtCreator 2011-02-08T17:54:32
#
#-------------------------------------------------

QT       += core gui

TARGET = gags
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    persona.cpp \
    miembro.cpp \
    ficha_medica.cpp \
    material.cpp \
    cuenta_miembro.cpp \
    contable_grupo.cpp \
    configuraciones.cpp \
    costos.cpp \
    conectarconbd.cpp \
    rautomaticpadres.cpp \
    evento.cpp

HEADERS  += mainwindow.h \
    persona.h \
    miembro.h \
    ficha_medica.h \
    material.h \
    cuenta_miembro.h \
    contable_grupo.h \
    configuraciones.h \
    costos.h \
    conectarconbd.h \
    rautomaticpadres.h \
    evento.h

FORMS    += mainwindow.ui \
    configuraciones.ui \
    rautomaticpadres.ui

RESOURCES += \
    icons.qrc \
    Manuales.qrc
