QT       += core gui widgets

TARGET = gameengine
TEMPLATE = app

SOURCES += main.cpp \
    GraphNode.cpp \
    PerlinNoise.cpp \
    Transform.cpp \
    debris.cpp \
    physicobject.cpp \
    terraingen.cpp \
    worm.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp

HEADERS += \
    BasicIO.h \
    GraphNode.h \
    PerlinNoise.h \
    Transform.h \
    debris.h \
    mainwidget.h \
    geometryengine.h \
    physicobject.h \
    terraingen.h \
    worm.h

RESOURCES += \
    model.qrc \
    shaders.qrc \
    textures.qrc

# install
target.path = $$[YOUR_PATH]
INSTALLS += target

DISTFILES += \
    sphere.obj \
    sphere.off
