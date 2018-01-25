#ifndef SDM_H
#define SDM_H

#include "facedescriptor.h"
#include "linearmodel.h"
#include "dataset.h"
#include "pca.h"

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_core/gui_core_kernel_1.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv/cv_image.h>

using namespace Eigen;

class SDM {
public:
    SDM(LinearModel *model, FaceDescriptor &descriptor, vector<vector<cv::KeyPoint>> initFaces, float numOfStages);
    void fit(const DataSet &trainSet);
    DataSet predict(const DataSet &dataSet);
    static float getError(const DataSet &truth, const DataSet &predict, const cv::Point &position, const cv::Scalar &color);

private:
    LinearModel *model;
    FaceDescriptor descriptor;
    vector<vector<cv::KeyPoint>> initFaces;
    vector<vector<MatrixXf>> params;
    vector<PCA> pca;
    vector<int> reducedDimensions;
    float numOfStages;

    vector<FaceShape> getStartSet(const vector<FaceShape> &dataSet) const;
    vector<FaceShape> getFinishSet(const vector<FaceShape> &dataSet) const;

    void train(MatrixXf landmarksOfStart, MatrixXf landmarksOfFinish, DataSet trainSetStart);
};



class SDMDlib{
public:
    SDMDlib(const string &predictorLandmarks);
    DataSet predict(DataSet dataSet);

private:
    dlib::shape_predictor shapePredictor;
};
#endif // SDM_H
