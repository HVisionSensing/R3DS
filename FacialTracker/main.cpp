#include "QString"
#include "dataset.h"
#include "loader.h"
#include "facedetector.h"
#include "sdm.h"
#include "facedescriptor.h"

int main(int argc, char *argv[])
{
    string haarcascade = "C:\\Soft\\opencv\\sources\\data\\haarcascades_cuda\\haarcascade_frontalface_alt.xml";
    string predictorForLandmarks = "C:\\Soft\\dlib\\shape_predictor_68_face_landmarks.dat";
    QString trainSetDir = "C:/Dataset/lfpw/trainset/";
    QString testSetDir = "C:/Dataset/lfpw/testset/";
    QString dirSave = "C:/Dataset/test/";
    QString formatSave = ".jpg";
    int trainSetCount = 900;
    int testSetCount = 300;
    int numberOfStages = 4;
    int facesBasicCount = 100;
    cv::Size size(300, 300);

    //Train
    FaceDetectorOpenCV faceDetector(haarcascade);
    DataSet trainSet = Loader::loadFaces(trainSetDir, trainSetCount);
    DataSet initFaces = Loader::loadFaces(trainSetDir, facesBasicCount, true);
    faceDetector.cutByLandmark(trainSet.dataSet);
    faceDetector.cutByLandmark(initFaces.dataSet);
    trainSet.normalizeFaces(size);
    initFaces.normalizeFaces(size);

    FaceDescriptor descriptor(cv::xfeatures2d::SURF::create());
    LinearModel *linearModel = new LinearRegression();
    SDM sdm(linearModel, descriptor, initFaces.getLandmarksOfSet(), numberOfStages);
    sdm.fit(trainSet);

    //Test
    DataSet testSet = Loader::loadFaces(testSetDir, testSetCount);
    faceDetector.cutByLandmark(testSet.dataSet);
    testSet.normalizeFaces(size);
    DataSet testSetPredicted = sdm.predict(testSet);
    testSetPredicted.drawLandmarks(cv::Scalar(0,0,255), testSet.getLandmarksOfSet());
    testSetPredicted.drawLandmarks(cv::Scalar(255, 0, 0));
    float error = SDM::getError(testSet, testSetPredicted, true);
    qDebug() << error;
    Loader::saveFaces(dirSave, formatSave, testSetPredicted.dataSet);
}
