#include "dataset.h"


DataSet::DataSet(vector<FaceShape> dataSet)
{
    this->dataSet = dataSet;
}



vector<vector<cv::KeyPoint>> DataSet::getLandmarksOfSet() const
{
    vector<vector<cv::KeyPoint>> landmarksOfSet;
    for (int indFace = 0; indFace < dataSet.size(); indFace++)
        landmarksOfSet.push_back(dataSet[indFace].landmarks);

    return landmarksOfSet;
}



void DataSet::normalizeFaces(cv::Size size)
{
    for (int indFace = 0; indFace < dataSet.size(); indFace++)
        dataSet[indFace].normalizeFace(size);
}



void DataSet::drawLandmarks(const cv::Scalar &color)
{
    for (int indFace = 0; indFace < dataSet.size(); indFace++)
        dataSet[indFace].drawLandmarks(color);
}



void DataSet::drawLandmarks(const cv::Scalar &color, const vector<vector<cv::KeyPoint> > landmarksOfSet)
{
    for (int indFace = 0; indFace < dataSet.size(); indFace++)
        dataSet[indFace].drawLandmarks(color, landmarksOfSet[indFace]);
}



void DataSet::transformLandmarks(MatrixXf transform)
{
    for (int indFace = 0; indFace < dataSet.size(); indFace++)
        dataSet[indFace].transformLandmarks(transform.row(indFace));
}



void DataSet::setLandmarks(const MatrixXf &landmarksMat)
{
    for (int indFace = 0; indFace < dataSet.size(); indFace++)
        dataSet[indFace].setLandmarks(landmarksMat.row(indFace));
}



MatrixXf DataSet::getMatrixOfMarks() const
{
    int countMarks = 68;
    int countFaces = dataSet.size();
    MatrixXf matrixOfMarks(countFaces, 2*countMarks);

    for (int indFace = 0; indFace < dataSet.size(); indFace++)
        matrixOfMarks.block(indFace, 0, 1, 2*countMarks) = dataSet.at(indFace).getMatrixOfMarks();

    return matrixOfMarks;
}

