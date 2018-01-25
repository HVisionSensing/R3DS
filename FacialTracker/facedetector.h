#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include "QDebug"
#include "faceshape.h"
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/opencv/cv_image.h>

#include "opencv2/opencv.hpp"

using namespace std;

class FaceDetector
{
public:
    void cutByLandmark(FaceShape &face) const;
    void cutByLandmark(vector<FaceShape> &faces) const;

    virtual void detect(vector<FaceShape> &faces) = 0;
    virtual void detect(FaceShape &face) = 0;

protected:
    void resizeDetectBox(cv::Rect &detectBox, float kX = -0.2, float kY = -0.1, float kWidth = 0.4, float kHeight = 0.3) const;
    bool isPointInBox(const cv::Rect &detectBox, const cv::KeyPoint &point) const;
    int getCountPointsInBox(const vector<cv::KeyPoint> &landmarks, const cv::Rect &detectBox) const;
    int getIndBoxWithFace(const vector<cv::Rect> &detectBox, const std::vector<cv::KeyPoint> &landmarks) const;
    vector<cv::Rect> convertDlibRectToOpenCVRect(const vector<dlib::rectangle> &dlibRects) const;

private:
    cv::Rect boundingBox(vector<cv::KeyPoint> marks) const;
};



class FaceDetectorOpenCV : public FaceDetector
{
public:
    FaceDetectorOpenCV(const string &haarcascade);
    void detect(vector<FaceShape> &faces) override;
    void detect(FaceShape &face) override;

private:
    cv::CascadeClassifier detector;
};


class FaceDetectorDlib : public FaceDetector
{
public:
    FaceDetectorDlib();
    void detect(vector<FaceShape> &faces) override;
    void detect(FaceShape &face) override;

private:
    dlib::frontal_face_detector detector;
};
#endif // FACEDETECTOR_H
