#include "facedetector.h"


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
    vector<cv::Rect> detectedFaces;
    detector.detectMultiScale(face.image, detectedFaces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));

    if (detectedFaces.size() == 0){
        cutByLandmark(face);
        return;
    }
    resizeDetectorBox(detectedFaces[0]);
    face.cutFace(detectedFaces[0]);
}


cv::Rect FaceDetector::boundingBox(vector<cv::KeyPoint> marks)
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



void FaceDetector::resizeDetectorBox(cv::Rect &detectedBox, float kX, float kY, float kWidth, float kHeight)
{
    float width = detectedBox.width;
    float height = detectedBox.height;
    detectedBox.x += width * kX;
    detectedBox.y += height * kY;
    detectedBox.width += width * kWidth;
    detectedBox.height += height * kHeight;
}



void FaceDetector::cutByLandmark(FaceShape &face)
{
    cv::Rect cutBox = boundingBox(face.landmarks);
    resizeDetectorBox(cutBox, -0.1, -0.1, 0.2, 0.2);
    face.cutFace(cutBox);
}



void FaceDetector::cutByLandmark(vector<FaceShape> &faces)
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

    cv::Rect detectedFace(boxs[0].left(), boxs[0].top(), boxs[0].width(), boxs[0].height());
    resizeDetectorBox(detectedFace);
    face.cutFace(detectedFace);
}
