#-------------------------------------------------
#
# Project created by QtCreator 2017-06-22T15:16:24
#
#-------------------------------------------------

QT       += core gui opengl testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gradient
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

include(../Andrew/WViewport3D/WContext/WContextNavigation.pri)
include(../Andrew/Wrap3Framework/WGLGeomDataRendererTests/WGLMaterialSurface/WGLMaterialSurface.pri)
include(../Andrew/Wrap3Framework/WGLGeomDataRendererTests/WGLMaterialWireframe/WGLMaterialWireframe.pri)
include(../Andrew/Wrap3Framework/WGLGeomDataRendererTests/WGLDataGeomStackTriangulated/WGLDataGeomStackTriangulated.pri)
include(../Andrew/Wrap3Framework/WGLObjectRendererTests01/WGLObjectRenderer/WGLObjectRenderer.pri)
include(../Andrew/Wrap3Framework/WGLGridTests/WGLGrid/WGLGrid.pri)
include(../Andrew/Wrap3Framework/Shaders/WGLShaderConst/WGLShaderConst.pri)
include(../Andrew/Wrap3Framework/WGLDots/WGLDots/WGLDots.pri)

INCLUDEPATH += ../eigen-eigen-5a0156e40feb/

LIBS += -lopengl32 -lglu32

SOURCES += main.cpp\
        mainwindow.cpp \
    testgradientdescent.cpp \
    gradientdescent.cpp \
    updatefunctor.cpp \
    autodiff.cpp

HEADERS  += mainwindow.h \
    testgradientdescent.h \
    gradientdescent.h \
    updatefunctor.h \
    autodiff.h

FORMS    += mainwindow.ui
