QT       += gui testlib
CONFIG -= app_bundle

TARGET = FaceAlignmentCore
DEFINES += QT_DEPRECATED_WARNINGS

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += main.cpp \
    faceshape.cpp \
    loader.cpp \
    facedetector.cpp \
    dataset.cpp \
    sdm.cpp \
    linearmodel.cpp \
    facedescriptor.cpp \
    pca.cpp \
    mainwindow.cpp \
    facialtrackertest.cpp

INCLUDEPATH += ../eigen-eigen-5a0156e40feb/
INCLUDEPATH += ../spectra-0.5.0/include
INCLUDEPATH += C:\Soft\opencv\build\install\include
INCLUDEPATH += C:\dlib\include

LIBS += C:\dlib\lib\libdlib.a

LIBS += C:\Soft\opencv\build\install\x86\mingw\lib\libopencv_xfeatures2d340.dll.a
LIBS += C:\Soft\opencv\build\install\x86\mingw\lib\libopencv_core340.dll.a
LIBS += C:\Soft\opencv\build\install\x86\mingw\lib\libopencv_highgui340.dll.a
LIBS += C:\Soft\opencv\build\install\x86\mingw\lib\libopencv_imgcodecs340.dll.a
LIBS += C:\Soft\opencv\build\install\x86\mingw\lib\libopencv_imgproc340.dll.a
LIBS += C:\Soft\opencv\build\install\x86\mingw\lib\libopencv_features2d340.dll.a
LIBS += C:\Soft\opencv\build\install\x86\mingw\lib\libopencv_calib3d340.dll.a
LIBS += C:\Soft\opencv\build\install\x86\mingw\lib\libopencv_objdetect340.dll.a

HEADERS += \
    loader.h \
    faceshape.h \
    facedetector.h \
    dataset.h \
    sdm.h \
    linearmodel.h \
    facedescriptor.h \
    pca.h \
    mainwindow.h \
    facialtrackertest.h

FORMS += \
    mainwindow.ui

