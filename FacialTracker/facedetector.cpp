#include "facedetector.h"


int FaceDetector::getIndBoxWithFace(const vector<cv::Rect> &detectBox, const vector<cv::KeyPoint> &landmarks) const
{
    int countBox = detectBox.size();
    int countPointsInBoxs[countBox];
    int indBoxWithFace = 0;

    for (int indBox = 0; indBox < countBox; indBox++){
        int countPointsInBox = getCountPointsInBox(landmarks, detectBox[indBox]);
        countPointsInBoxs[indBox] = countPointsInBox;
        if (indBox == 0)
            continue;
        if (countPointsInBox > countPointsInBoxs[indBoxWithFace])
            indBoxWithFace = indBox;
    }

    return indBoxWithFace;
}



FaceDetectorOpenCV::FaceDetectorOpenCV(const string &haarcascade)
{
    detector.load(haarcascade);
}



void FaceDetectorOpenCV::detect(vector<FaceShape> &faces)
{
    for (int indFace = 0; indFace < faces.size(); indFace++)
        detect(faces[indFace]);
}



void FaceDetectorOpenCV::detect(FaceShape &face)
{
    vector<cv::Rect> detectedBox;
    detector.detectMultiScale(face.image, detectedBox, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));

    if (detectedBox.size() == 0){
        cutByLandmark(face);
        return;
    }

    int indBoxWithFace = getIndBoxWithFace(detectedBox, face.landmarks);
    resizeDetectBox(detectedBox[indBoxWithFace]);
    face.cutFace(detectedBox[indBoxWithFace]);
}



cv::Rect FaceDetector::boundingBox(vector<cv::KeyPoint> marks) const
{
    float xMax, xMin, yMax, yMin;

    std::sort(marks.begin(), marks.end(),
              [](cv::KeyPoint pointOne, cv::KeyPoint pointTwo) { return pointOne.pt.x < pointTwo.pt.x; });

    xMin = marks[0].pt.x;
    xMax = marks[marks.size()-1].pt.x;

    std::sort(marks.begin(), marks.end(),
              [](cv::KeyPoint pointOne, cv::KeyPoint pointTwo) { return pointOne.pt.y < pointTwo.pt.y; });

    yMin = marks[0].pt.y;
    yMax = marks[marks.size()-1].pt.y;

    return cv::Rect(xMin, yMin, xMax - xMin, yMax - yMin);
}



void FaceDetector::resizeDetectBox(cv::Rect &detectBox, float kX, float kY, float kWidth, float kHeight) const
{
    float width = detectBox.width;
    float height = detectBox.height;
    detectBox.x += width * kX;
    detectBox.y += height * kY;
    detectBox.width += width * kWidth;
    detectBox.height += height * kHeight;
}



bool FaceDetector::isPointInBox(const cv::Rect &detectBox, const cv::KeyPoint &point) const
{
    int leftX = detectBox.tl().x;
    int rightX = detectBox.br().x;

    int topY = detectBox.tl().y;
    int bottomY = detectBox.br().y;

    bool locateForX = (leftX < point.pt.x) && (point.pt.x < rightX);
    bool locateForY = (topY < point.pt.y) && (point.pt.y < bottomY);

    if (locateForX && locateForY)
        return true;
    return false;
}



int FaceDetector::getCountPointsInBox(const vector<cv::KeyPoint> &landmarks, const cv::Rect &detectBox) const
{
    int countPointsInBox = 0;
    for (int indPoint = 0; indPoint < landmarks.size(); indPoint++){
        bool pointLocatedInBox = isPointInBox(detectBox, landmarks[indPoint]);
        if (pointLocatedInBox)
            countPointsInBox++;
    }
    return countPointsInBox;
}



vector<cv::Rect> FaceDetector::convertDlibRectToOpenCVRect(const vector<dlib::rectangle> &dlibRects) const
{
    vector<cv::Rect> openCVRects;
    for (int indRect = 0; indRect < dlibRects.size(); indRect++){
        int x = dlibRects[indRect].left();
        int y = dlibRects[indRect].top();
        int width = dlibRects[indRect].width();
        int height = dlibRects[indRect].height();

        cv::Rect openCVRect(x, y, width, height);
        openCVRects.push_back(openCVRect);
    }

    return openCVRects;
}



void FaceDetector::cutByLandmark(FaceShape &face) const
{
    cv::Rect cutBox = boundingBox(face.landmarks);
    resizeDetectBox(cutBox, -0.1, -0.1, 0.2, 0.2);
    face.cutFace(cutBox);
}



void FaceDetector::cutByLandmark(vector<FaceShape> &faces) const
{
    for (int indFace = 0; indFace < faces.size(); indFace++)
        cutByLandmark(faces[indFace]);
}



FaceDetectorDlib::FaceDetectorDlib()
{
    detector = dlib::get_frontal_face_detector();
}



void FaceDetectorDlib::detect(vector<FaceShape> &faces)
{
    for (int indFace = 0; indFace < faces.size(); indFace++)
        detect(faces[indFace]);

}



void FaceDetectorDlib::detect(FaceShape &face)
{
    dlib::array2d<dlib::rgb_pixel> img;
    dlib::assign_image(img, dlib::cv_image<dlib::rgb_pixel>(face.image));
    vector<dlib::rectangle> boxs = detector(img);

    if (boxs.size() == 0){
        cutByLandmark(face);
        return;
    }

    vector<cv::Rect> openCVBoxs = convertDlibRectToOpenCVRect(boxs);
    int indBoxWithFace = getIndBoxWithFace(openCVBoxs, face.landmarks);
    resizeDetectBox(openCVBoxs[indBoxWithFace]);
    face.cutFace(openCVBoxs[indBoxWithFace]);
}
