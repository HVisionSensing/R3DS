#include "mainwindow.h"
#include "QApplication"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
    /*
    QString trainSetDirWare = "C:/Dataset/FaceWarehouse-img/FaceWarehouse/";
    int trainSetCount = 10;
    QString formatSave = ".jpg";
    QString dirSave = "C:/Dataset/test/";
    DataSet trainSet = Loader::loadFacesWarehouse(trainSetDirWare, trainSetCount);
    trainSet.drawLandmarks(cv::Scalar(255, 0, 0));
    Loader::saveFaces(dirSave, formatSave, trainSet.dataSet);
    */

    /*
    string haarcascade = "C:\\Soft\\opencv\\sources\\data\\haarcascades_cuda\\haarcascade_frontalface_alt.xml";
    string predictorForLandmarks = "C:\\Soft\\dlib\\shape_predictor_68_face_landmarks.dat";
    QString trainSetDirWare = "C:/Dataset/FaceWarehouse-img/FaceWarehouse/";
    QString testSetDirWare = "C:/Dataset/FaceWarehouse-img/FaceWarehouse/";
    QString trainSetDir = "C:/Dataset/lfpw/trainset/";
    QString testSetDir = "C:/Dataset/lfpw/testset/";
    QString dirSave = "C:/Dataset/test/";
    QString formatSave = ".jpg";
    int trainSetCount = 900;
    int testSetCount = 300;
    int numberOfStages = 0;
    int facesBasicCount = 5;
    cv::Size size;

    //Train
    FaceDetectorDlib faceDetector;
    DataSet trainSet = Loader::loadFaces(trainSetDir, trainSetCount);
    DataSet initFaces = Loader::loadFaces(trainSetDir, facesBasicCount);
    faceDetector.cutByLandmark(trainSet.dataSet);
    faceDetector.cutByLandmark(initFaces.dataSet);
    size = trainSet.getArgSize();
    size.width *= 1.6;
    size.height *= 1.6;

    trainSet.resizeFaces(size);
    initFaces.resizeFaces(size);

    FaceDescriptor descriptor(cv::xfeatures2d::SURF::create());
    LinearModel *linearModel = new LinearRegression();
    SDM sdm(linearModel, descriptor, initFaces.getLandmarksVector(), numberOfStages);
    sdm.fit(trainSet);

    //Test
    DataSet testSet = Loader::loadFaces(testSetDir, testSetCount);
    faceDetector.cutByLandmark(testSet.dataSet);
    testSet.resizeFaces(size);
    DataSet testSetPredicted = sdm.predict(testSet);

    SDMDlib sdmDlib(predictorForLandmarks);
    DataSet testSetPredictedDlib = sdmDlib.predict(testSet);


    testSetPredicted.drawLandmarks(cv::Scalar(0,0,255), testSet.getLandmarksVector());
    testSetPredicted.drawLandmarks(cv::Scalar(0,255,0), testSetPredictedDlib.getLandmarksVector());
    testSetPredicted.drawLandmarks(cv::Scalar(255, 0, 0));
    float errorOur = SDM::getError(testSet, testSetPredicted, cv::Size(0, 200), cv::Scalar(255,0,0));
    float errorDlib = SDM::getError(testSet, testSetPredictedDlib, cv::Size(0, 220), cv::Scalar(0,255,0));
    qDebug() << errorOur;
    qDebug() << errorDlib;
    Loader::saveFaces(dirSave, formatSave, testSetPredicted.dataSet);


    /*
    //Test Detector
    FaceDetectorOpenCV faceDetector(haarcascade);
    DataSet trainSet = Loader::loadFaces(trainSetDir, trainSetCount);
    DataSet initFaces = Loader::loadFaces(trainSetDir, facesBasicCount, true);
    faceDetector.cutByLandmark(trainSet.dataSet);
    faceDetector.cutByLandmark(initFaces.dataSet);
    initFaces.normalizeFaces(size);
    trainSet.normalizeFaces(size);
    trainSet.drawLandmarks(cv::Scalar(255, 0, 0));
    //trainSet.drawLandmarks(cv::Scalar(255, 0, 0), initFaces.getLandmarksOfSet()); //косяк
    Loader::saveFaces(dirSave, formatSave, trainSet.dataSet);
    */
}
