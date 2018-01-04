#include "sdm.h"

vector<FaceShape> SDM::getStartSet(const vector<FaceShape> &dataSet)
{
    vector<FaceShape> startSet;
    for (int indFace = 0; indFace < dataSet.size(); indFace++)
        for (int indFaceBasic = 0; indFaceBasic < initFaces.size(); indFaceBasic++)
            startSet.push_back(FaceShape(dataSet[indFace].image, initFaces[indFaceBasic]));

    return startSet;
}



vector<FaceShape> SDM::getFinishSet(const vector<FaceShape> &dataSet)
{
    vector<FaceShape> finishSet;
    for (int indFace = 0; indFace < dataSet.size(); indFace++)
        for (int indFaceBasic = 0; indFaceBasic < initFaces.size(); indFaceBasic++)
            finishSet.push_back(FaceShape(dataSet[indFace].image, dataSet[indFace].landmarks));

    return finishSet;
}



void SDM::train(MatrixXf landmarksOfStart, MatrixXf landmarksOfFinish, DataSet trainSetStart)
{
    for (int indOfStg = 0; indOfStg < numberOfStages; indOfStg++){
        MatrixXf deltaLandmarks = landmarksOfFinish - landmarksOfStart;
        MatrixXf featuresDescriptor = descriptor.computeDescriptor(trainSetStart.dataSet);
        linearModel->fit(featuresDescriptor, deltaLandmarks);
        params.push_back(linearModel->getParams());
        MatrixXf predictDeltaLandmarks = linearModel->predict(featuresDescriptor);
        linearModel->deleteParams();
        landmarksOfStart = landmarksOfStart + predictDeltaLandmarks;
        trainSetStart.transformLandmarks(predictDeltaLandmarks);

        deltaLandmarks = landmarksOfFinish - landmarksOfStart;
        qDebug() << "Error on stage " << indOfStg << " = " << deltaLandmarks.norm();
    }
}



SDM::SDM(LinearModel *linearModel, FaceDescriptor &descriptor, vector<vector<cv::KeyPoint>> initFaces, float numberOfStages)
{
    this->linearModel = linearModel;
    this->descriptor = descriptor;
    this->initFaces = initFaces;
    this->numberOfStages = numberOfStages;
}



void SDM::fit(DataSet &trainSet)
{
    DataSet trainSetStart(getStartSet(trainSet.dataSet));
    DataSet trainSetFinish(getFinishSet(trainSet.dataSet));
    MatrixXf trainMarksFinish = trainSetFinish.getMatrixOfMarks();
    MatrixXf trainMarksStart = trainSetStart.getMatrixOfMarks();

    DataSet startSet(trainSetStart);
    trainSetStart.dataSet.clear();
    trainSetFinish.dataSet.clear();
    train(trainMarksStart, trainMarksFinish, startSet);
    startSet.dataSet.clear();
}



DataSet SDM::predict(const DataSet &dataSet)
{
    DataSet dataSetStart(getStartSet(dataSet.dataSet));
    MatrixXf dataSetMarksStart = dataSetStart.getMatrixOfMarks();
    MatrixXf predictedMarks = MatrixXf::Zero(dataSet.dataSet.size(), 136);

    for (int indOfStg = 0; indOfStg < numberOfStages; indOfStg++){
        MatrixXf featuresDescriptor = descriptor.computeDescriptor(dataSetStart.dataSet);
        linearModel->setParams(params[indOfStg]);
        MatrixXf predictDeltaLandmarks = linearModel->predict(featuresDescriptor);
        linearModel->deleteParams();
        dataSetMarksStart = dataSetMarksStart + predictDeltaLandmarks;
        dataSetStart.transformLandmarks(predictDeltaLandmarks);
    }

    dataSetStart.dataSet.clear();

    for (int indFace = 0; indFace < dataSet.dataSet.size(); indFace++)
        for (int step = indFace*initFaces.size(); step < indFace*initFaces.size()+initFaces.size(); step++)
            predictedMarks.row(indFace) = predictedMarks.row(indFace) + dataSetMarksStart.row(step);

    predictedMarks /= initFaces.size();
    DataSet newSet(dataSet.dataSet);
    newSet.setLandmarks(predictedMarks);
    return newSet;

}



float SDM::getError(const DataSet &truth, const DataSet &predict, bool draw)
{
    MatrixXf truthLandmark = truth.getMatrixOfMarks();
    MatrixXf predictLandmark = predict.getMatrixOfMarks();

    MatrixXf difference = truthLandmark - predictLandmark;
    float entireError = 0;

    for (int indFace = 0; indFace < difference.rows(); indFace++){
        float errorOnFace = difference.row(indFace).lpNorm<1>()*100/predictLandmark.row(indFace).lpNorm<1>();
        entireError += errorOnFace;
        if (draw == false)
            continue;

        cv::String errorOnFaceStr = "error = " + to_string(errorOnFace);
        cv::putText(predict.dataSet[indFace].image, errorOnFaceStr, cvPoint(0,280),
                    cv::FONT_HERSHEY_COMPLEX_SMALL, 0.6, cvScalar(255,0,0), 1, CV_AA);
    }

    entireError /= difference.rows();


    return entireError;
}
