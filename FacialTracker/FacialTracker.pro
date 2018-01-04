QT       += gui
CONFIG -= app_bundle

TARGET = FaceAlignmentCore
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp \
    faceshape.cpp \
    loader.cpp \
    facedetector.cpp \
    dataset.cpp \
    sdm.cpp \
    linearmodel.cpp \
    facedescriptor.cpp

INCLUDEPATH += ../eigen-eigen-5a0156e40feb/
INCLUDEPATH += C:\Soft\opencv\build\install\include
INCLUDEPATH += C:\dlib\include

LIBS += C:\dlib\lib\libdlib.a

LIBS += C:\Soft\opencv\build\install\x64\mingw\lib\libopencv_xfeatures2d330.dll.a
LIBS += C:\Soft\opencv\build\install\x64\mingw\lib\libopencv_core330.dll.a
LIBS += C:\Soft\opencv\build\install\x64\mingw\lib\libopencv_highgui330.dll.a
LIBS += C:\Soft\opencv\build\install\x64\mingw\lib\libopencv_imgcodecs330.dll.a
LIBS += C:\Soft\opencv\build\install\x64\mingw\lib\libopencv_imgproc330.dll.a
LIBS += C:\Soft\opencv\build\install\x64\mingw\lib\libopencv_features2d330.dll.a
LIBS += C:\Soft\opencv\build\install\x64\mingw\lib\libopencv_calib3d330.dll.a
LIBS += C:\Soft\opencv\build\install\x64\mingw\lib\libopencv_objdetect330.dll.a

HEADERS += \
    loader.h \
    faceshape.h \
    facedetector.h \
    dataset.h \
    sdm.h \
    linearmodel.h \
    facedescriptor.h

