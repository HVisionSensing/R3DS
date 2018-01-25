#ifndef DATASET_H
#define DATASET_H

#include "faceshape.h"

class DataSet
{
public:
    vector<FaceShape> dataSet;
    DataSet(vector<FaceShape> dataSet);
    ~DataSet();

    void resizeFaces(const cv::Size &size);
    void resizeImages(const cv::Size &size);
    void resizeLandmarks(const cv::Size &size);
    void scalingLandmarks(const float kx, const float ky);
    void drawLandmarks(const cv::Scalar &color);
    void drawLandmarks(const cv::Scalar &color, const vector<vector<cv::KeyPoint>> &landmarksOfSet);
    void transformLandmarks(const MatrixXf &transform);
    void transformLandmarks(const float x, const float y);
    void setLandmarks(const MatrixXf &landmarks);

    cv::Size getArgSize() const;
    MatrixXf getLandmarksMatrix() const;
    vector<vector<cv::KeyPoint>> getLandmarksVector() const;

};
#endif // DATASET_H
