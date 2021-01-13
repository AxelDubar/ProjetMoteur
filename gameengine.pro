QT       += core gui widgets

TARGET = gameengine
TEMPLATE = app

SOURCES += main.cpp \
    GraphNode.cpp \
    PerlinNoise.cpp \
    Transform.cpp \
    terraingen.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp

HEADERS += \
    BasicIO.h \
    GraphNode.h \
    PerlinNoise.h \
    Transform.h \
    mainwidget.h \
    geometryengine.h \
    terraingen.h

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
