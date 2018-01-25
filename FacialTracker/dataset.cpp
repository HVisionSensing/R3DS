#include "dataset.h"


DataSet::DataSet(vector<FaceShape> dataSet)
{
    this->dataSet = dataSet;
}



DataSet::~DataSet()
{
    dataSet.clear();
    dataSet.shrink_to_fit();
}



vector<vector<cv::KeyPoint>> DataSet::getLandmarksVector() const
{
    vector<vector<cv::KeyPoint>> landmarksOfSet;
    for (int indFace = 0; indFace < dataSet.size(); indFace++)
        landmarksOfSet.push_back(dataSet[indFace].landmarks);

    return landmarksOfSet;
}



void DataSet::resizeFaces(const cv::Size &size)
{
    for (int indFace = 0; indFace < dataSet.size(); indFace++)
        dataSet[indFace].resizeFace(size);
}



void DataSet::resizeImages(const cv::Size &size)
{
    for (int indFace = 0; indFace < dataSet.size(); indFace++)
        dataSet[indFace].resizeImage(size);
}



void DataSet::resizeLandmarks(const cv::Size &size)
{
    for (int indFace = 0; indFace < dataSet.size(); indFace++)
        dataSet[indFace].resizeLandmarks(size);
}



void DataSet::scalingLandmarks(const float kx, const float ky)
{
    for (int indFace = 0; indFace < dataSet.size(); indFace++)
        dataSet[indFace].scalingLandmarks(kx, ky);
}



void DataSet::drawLandmarks(const cv::Scalar &color)
{
    for (int indFace = 0; indFace < dataSet.size(); indFace++)
        dataSet[indFace].drawLandmarks(color);
}



void DataSet::drawLandmarks(const cv::Scalar &color, const vector<vector<cv::KeyPoint> > &landmarksOfSet)
{
    for (int indFace = 0; indFace < landmarksOfSet.size(); indFace++)
        dataSet[indFace].drawLandmarks(color, landmarksOfSet[indFace]);
}



void DataSet::transformLandmarks(const MatrixXf &transform)
{
    for (int indFace = 0; indFace < dataSet.size(); indFace++)
        dataSet[indFace].transformLandmarks(transform.row(indFace));
}



void DataSet::transformLandmarks(const float x, const float y)
{
    for (int indFace = 0; indFace < dataSet.size(); indFace++)
        dataSet[indFace].transformLandmarks(x, y);
}



void DataSet::setLandmarks(const MatrixXf &landmarks)
{
    for (int indFace = 0; indFace < dataSet.size(); indFace++)
        dataSet[indFace].setLandmarks(landmarks.row(indFace));
}



cv::Size DataSet::getArgSize() const
{
    int countShape = dataSet.size();
    vector<int> width;
    vector<int> height;

    for (int indFace = 0; indFace < countShape; indFace++){
        width.push_back(dataSet[indFace].image.cols);
        height.push_back(dataSet[indFace].image.rows);
    }

    sort(width.begin(), width.end());
    sort(height.begin(), height.end());

    int midWidth = width[width.size()/2];
    int midHeight = height[height.size()/2];
    cv::Size size(midWidth, midHeight);

    return size;
}



MatrixXf DataSet::getLandmarksMatrix() const
{
    int countMarks = 68;
    int countFaces = dataSet.size();
    MatrixXf landmarksMatrix(countFaces, 2*countMarks);

    for (int indFace = 0; indFace < dataSet.size(); indFace++)
        landmarksMatrix.block(indFace, 0, 1, 2*countMarks) = dataSet.at(indFace).getMatrixOfMarks();

    return landmarksMatrix;
}

