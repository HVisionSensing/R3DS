#ifndef LOADER_H
#define LOADER_H
#include "QDebug"
#include "QVector"
#include "QString"
#include "QFile"
#include "QDir"
#include "qmath.h"
#include <stdlib.h>

#include "faceshape.h"

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/opencv/cv_image.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "Eigen/Geometry"

namespace Loader
{
    using namespace std;

    vector<FaceShape> loadFaces(const QString &dirDataSet, const int facesCount, bool random = false);
    FaceShape loadFace(const QString &faceName, const QString &marksName);
    vector<FaceShape> loadFacesWarehouse(const QString &dirDataSet, const int facesCount, const int start = 1);
    vector<cv::KeyPoint> loadLandmarks(const QString &marksName);
    QString getCorrectLine(const QString &line);
    bool isReadFile(QFile &file);
    void saveFaces(const QString &dirSave, const QString &formatSave, const vector<FaceShape> &faces);
    void saveFace(const QString &savePath, const FaceShape &face);
}
#endif // LOADER_H
