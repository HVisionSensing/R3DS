#ifndef FACEDESCRIPTOR_H
#define FACEDESCRIPTOR_H

#include "Eigen/Core"
#include "opencv2/core/eigen.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "dataset.h"

using namespace Eigen;

class FaceDescriptor
{
public:
    cv::Ptr<cv::Feature2D> descriptor;

public:
    FaceDescriptor();
    FaceDescriptor(cv::Ptr<cv::Feature2D> descriptor);
    int getSize() const;

public:
    MatrixXf computeDescriptor(vector<FaceShape> &faces);
    MatrixXf computeDescriptor(FaceShape &face);
};

#endif // FACEDESCRIPTOR_H
