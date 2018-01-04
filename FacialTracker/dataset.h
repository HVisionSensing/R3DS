#ifndef DATASET_H
#define DATASET_H

#include "faceshape.h"

class DataSet
{
public:
    vector<FaceShape> dataSet;
    DataSet(vector<FaceShape> dataSet);

public:
    void normalizeFaces(cv::Size size);
    void drawLandmarks(const cv::Scalar &color);
    void drawLandmarks(const cv::Scalar &color, const vector<vector<cv::KeyPoint>> landmarksOfSet);
    void transformLandmarks(MatrixXf transform);
    void setLandmarks(const MatrixXf &landmarksMat);

public:
    MatrixXf getMatrixOfMarks() const;

public:
    vector<vector<cv::KeyPoint>> getLandmarksOfSet() const;

};
#endif // DATASET_H
