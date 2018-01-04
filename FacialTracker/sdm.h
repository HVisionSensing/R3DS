#ifndef SDM_H
#define SDM_H

#include "facedescriptor.h"
#include "linearmodel.h"
#include "dataset.h"

using namespace Eigen;

class SupervisedDescentMethod
{

};

class SDM {
private:
    vector<vector<MatrixXf>> params;

private:
    LinearModel *linearModel;
    FaceDescriptor descriptor;
    vector<vector<cv::KeyPoint>> initFaces;
    float numberOfStages;

private:
    vector<FaceShape> getStartSet(const vector<FaceShape> &dataSet);
    vector<FaceShape> getFinishSet(const vector<FaceShape> &dataSet);

private:
    void train(MatrixXf landmarksOfStart, MatrixXf landmarksOfFinish, DataSet trainSetStart);

public:
    SDM(LinearModel *linearModel, FaceDescriptor &descriptor, vector<vector<cv::KeyPoint>> initFaces, float numberOfStages = 4);
    void fit(DataSet &trainSet);
    DataSet predict(const DataSet &dataSet);
    static float getError(const DataSet &truth, const DataSet &predict, bool draw = false);
};

#endif // SDM_H
