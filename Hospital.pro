#-------------------------------------------------
#
# Project created by QtCreator 2019-10-08T22:07:23
#
#-------------------------------------------------

QT       += core gui sql
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Hospital
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        agregarUrgencia.cpp \
        agregaractividaddoctor.cpp \
        agregarcitaspaciente.cpp \
        agregartipremedio.cpp \
        agregarusuario.cpp \
        eliminarusuarios.cpp \
        main.cpp \
        mainwindow.cpp \
        mostrarhistoclinico.cpp \
        pagarcitaspaciente.cpp \
        pagointervenciones.cpp \
        registrodoctor.cpp \
        registropaciente.cpp \
        registrostaff.cpp \
        login.cpp \
        tipdeldia.cpp \
        ordenarintervencion.cpp \
        ordenarestudios.cpp

HEADERS += \
        agregarUrgencia.h \
        agregaractividaddoctor.h \
        agregarcitaspaciente.h \
        agregartipremedio.h \
        agregarusuario.h \
        eliminarusuarios.h \
        mainwindow.h \
        mostrarhistoclinico.h \
        pagarcitaspaciente.h \
        pagointervenciones.h \
        registrodoctor.h \
        registropaciente.h \
        registrostaff.h \
        login.h \
        tipdeldia.h \
        ordenarintervencion.h \
        ordenarestudios.h

FORMS += \
        agregarUrgencia.ui \
        agregaractividaddoctor.ui \
        agregarusuario.ui \
        eliminarusuarios.ui \
        mainwindow.ui \
        mostrarhistoclinico.ui \
        pagarcitaspaciente.ui \
        tipdeldia.ui \
        ordenarintervencion.ui \
        ordenarestudios.ui \
        pagointervenciones.ui

RC_ICONS = icono.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    imgs/imgs.qrc \
