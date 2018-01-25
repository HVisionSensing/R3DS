#ifndef FACESHAPE_H
#define FACESHAPE_H

#include "QDebug"
#include "qmath.h"
#include "Eigen/Core"
#include <Eigen/Dense>
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"

using namespace Eigen;
using namespace std;

class FaceShape
{
public:
    cv::Mat image;
    vector<cv::KeyPoint> landmarks;

    FaceShape ();
    FaceShape (const cv::Mat &image, const vector<cv::KeyPoint> &landmarks);

    void resizeFace(const cv::Size &size);
    void resizeImage(const cv::Size &size);
    void resizeLandmarks(const cv::Size &size);
    void scalingLandmarks(const float kx, const float ky);
    void cutFace(const cv::Rect &cutBox);
    void drawLandmarks(const cv::Scalar &color);
    void transformLandmarks(const MatrixXf &transform);
    void transformLandmarks(const float x, const float y);
    void drawLandmarks(const cv::Scalar &color, const vector<cv::KeyPoint> &landmarks);
    void setLandmarks(const MatrixXf &landmarksMat);

    MatrixXf getMatrixOfMarks() const;
    static vector<cv::KeyPoint> convertVectorToLandmark(const VectorXf &vectorLandmarks);

private:
    cv::Rect checkBorders(cv::Rect cutBox) const;

};
#endif // FACESHAPE_H
