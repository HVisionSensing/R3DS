#include "faceshape.h"



FaceShape::FaceShape()
{

}



FaceShape::FaceShape(const cv::Mat &image, const vector<cv::KeyPoint> &landmarks)
    : image(image), landmarks(landmarks)
{

}



cv::Rect FaceShape::checkBorders(cv::Rect cutBox) const
{
    if (cutBox.x < 0){
        cutBox.width -= qFabs(cutBox.x);
        cutBox.x = 0;
    }

    if (cutBox.y < 0){
        cutBox.height -= qFabs(cutBox.y);
        cutBox.y = 0;
    }

    if ((cutBox.x+cutBox.width) > image.cols)
        cutBox.width = image.cols - cutBox.x;
    if ((cutBox.y+cutBox.height) > image.rows)
        cutBox.height = image.rows - cutBox.y;

    return cutBox;
}



void FaceShape::resizeFace(const cv::Size &size)
{
    resizeLandmarks(size);
    resizeImage(size);
}



void FaceShape::resizeImage(const cv::Size &size)
{
    cv::Mat normalizedImg;
    cv::resize(image, normalizedImg, size);
    image = normalizedImg;
}



void FaceShape::resizeLandmarks(const cv::Size &size)
{
    float widthNew = size.width;
    float widthOld = image.cols;
    float heightNew = size.height;
    float heightOld = image.rows;

    float kWidth = widthNew / widthOld;
    float kHight = heightNew / heightOld;

    scalingLandmarks(kWidth, kHight);
}



void FaceShape::scalingLandmarks(const float kx, const float ky)
{
    for (int indexMark = 0; indexMark < landmarks.size(); indexMark++){
        landmarks[indexMark].pt.x *= kx;
        landmarks[indexMark].pt.y *= ky;
    }
}



void FaceShape::transformLandmarks(const float x, const float y)
{
    for (int index = 0; index < landmarks.size(); index++){
        landmarks[index].pt.x += x;
        landmarks[index].pt.y += y;
    }
}



void FaceShape::transformLandmarks(const MatrixXf &transform)
{
    for (int indMark = 0, indMarkAxis = 0; indMark < landmarks.size(); indMark++){
        landmarks[indMark].pt.x += transform(0, indMarkAxis++);
        landmarks[indMark].pt.y += transform(0, indMarkAxis++);
    }
}



void FaceShape::cutFace(const cv::Rect &cutBox)
{
    cv::Rect checkedBox = checkBorders(cutBox);
    transformLandmarks(-checkedBox.x, -checkedBox.y);
    image = image(checkedBox);
    cv::Mat cuttedImage(cutBox.height, cutBox.width, image.type());
    float transX = qFabs(cutBox.x) - qFabs(checkedBox.x);
    float transY = qFabs(cutBox.y) - qFabs(checkedBox.y);
    cv::Rect copyBox(transX, transY, checkedBox.width, checkedBox.height);
    transformLandmarks(transX, transY);
    image.copyTo(cuttedImage(copyBox));
    image = cuttedImage;
}



void FaceShape::drawLandmarks(const cv::Scalar &color)
{
    drawLandmarks(color, landmarks);
}



void FaceShape::drawLandmarks(const cv::Scalar &color, const vector<cv::KeyPoint> &landmarks)
{
    for (int indMark = 0; indMark < landmarks.size(); indMark++){
        cv::Point pt(landmarks.at(indMark).pt.x-0.7, landmarks.at(indMark).pt.y-0.7);
                cv::circle(image, pt, 2, color, -1, 8, 0);
                if (indMark+1 == landmarks.size()+1)
                    break;
                if (indMark+1 == 17 || indMark+1 == 22 || indMark+1 == 27 || indMark+1 == 31 || indMark+1 == 36)
                    continue;
                if (indMark == 41){
                    cv::Point pt2(landmarks.at(36).pt.x-0.7, landmarks.at(36).pt.y-0.7);
                    cv::line(image, pt, pt2, color);
                    continue;
                }
                if (indMark == 47){
                    cv::Point pt2(landmarks.at(42).pt.x-0.7, landmarks.at(42).pt.y-0.7);
                    cv::line(image, pt, pt2, color);
                    continue;
                }
                if (indMark == 59){
                    cv::Point pt2(landmarks.at(48).pt.x-0.7, landmarks.at(48).pt.y-0.7);
                    cv::line(image, pt, pt2, color);
                    continue;
                }
                if (indMark == 67){
                    cv::Point pt2(landmarks.at(60).pt.x-0.7, landmarks.at(60).pt.y-0.7);
                    cv::line(image, pt, pt2, color);
                    continue;
                }

                cv::Point pt2(landmarks.at(indMark+1).pt.x-0.7, landmarks.at(indMark+1).pt.y-0.7);
        cv::line(image, pt, pt2, color);

    }
}



void FaceShape::setLandmarks(const MatrixXf &landmarksMat)
{
    landmarks.clear();
    for (int indMark = 0, indMarkAxis = 0; indMark < landmarksMat.cols()/2; indMark++){
        int x = indMarkAxis++;
        int y = indMarkAxis++;
        landmarks.push_back(cv::KeyPoint(landmarksMat(0, x), landmarksMat(0, y), 32));
    }
}



MatrixXf FaceShape::getMatrixOfMarks() const
{
    MatrixXf marksOfFace(1, landmarks.size()*2);
    for (int indMark = 0, indMarkAxis = 0; indMark < landmarks.size(); indMark++){
        marksOfFace(0, indMarkAxis++) = landmarks.at(indMark).pt.x;
        marksOfFace(0, indMarkAxis++) = landmarks.at(indMark).pt.y;
    }

    return marksOfFace;
}



vector<cv::KeyPoint> FaceShape::convertVectorToLandmark(const VectorXf &vectorLandmarks)
{
    vector<cv::KeyPoint> marks;
    for (int indMark = 0, indMarkAxis = 0; indMark < vectorLandmarks.rows()/2; indMark++){
        int x = indMarkAxis++;
        int y = indMarkAxis++;
        marks.push_back(cv::KeyPoint(vectorLandmarks(x, 0), vectorLandmarks(y, 0), 32));
    }
    return marks;
}
