#QT       += opengl
#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += opengl
#TARGET = cloth

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#CONFIG -= qt  #when using qt library,maybe we need add qt

SOURCES += main.cpp \
    vector.cpp \
    particle.cpp \
    constrain.cpp \
    camera.cpp

INCLUDEPATH += /home/gu/Qt5.5.1/5.5/gcc_64/include

LIBS += -L /usr/lib/x86_64-linux-gnu -lglut -lGLU -lGL  -L/home/gu/Qt5.5.1/5.5/gcc_64/lib -lQt5Gui -licui18n -licuuc -licudata

HEADERS += \
    vector.h \
    particle.h \
    constrain.h \
    camera.h \
    key.h
