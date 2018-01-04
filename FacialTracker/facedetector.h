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
private:
    cv::Rect boundingBox(vector<cv::KeyPoint> marks);

protected:
    void resizeDetectorBox(cv::Rect &detectedBox, float kX = -0.2, float kY = -0.2, float kWidth = 0.4, float kHeight = 0.4);

public:
    void cutByLandmark(FaceShape &face);
    void cutByLandmark(vector<FaceShape> &faces);

public:
    virtual void detect(vector<FaceShape> &faces) = 0;
    virtual void detect(FaceShape &face) = 0;
};



class FaceDetectorOpenCV : public FaceDetector
{
private:
    cv::CascadeClassifier detector;

public:
    FaceDetectorOpenCV(const string &haarcascade);
    void detect(vector<FaceShape> &faces) override;
    void detect(FaceShape &face) override;
};


class FaceDetectorDlib : public FaceDetector
{
private:
    dlib::frontal_face_detector detector;

public:
    FaceDetectorDlib();
    void detect(vector<FaceShape> &faces) override;
    void detect(FaceShape &face) override;
};
#endif // FACEDETECTOR_H
