#include "mainwindow.h"
#include "ui_mainwindow.h"

void runTests()
{
    FacialTrackerTest tests;
    QTest::qExec(&tests);
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    runTests();

    QString trainSetDir = "C:/Dataset/lfpw/trainset/";
    int trainSetCount = 900;
    cv::Size size;

    FaceDetectorDlib faceDetector;
    DataSet trainSet = Loader::loadFaces(trainSetDir, trainSetCount);
    faceDetector.cutByLandmark(trainSet.dataSet);
    size = trainSet.getArgSize();
    size.width *= 1.6;
    size.height *= 1.6;
    trainSet.resizeFaces(size);
    MatrixXf landmarksMatrix = trainSet.getLandmarksMatrix();
    pca = new PCA(landmarksMatrix);
    eigenVel = pca->getEigenValues().array().abs().sqrt();
    newFace = eigenVel * 0;
    eigenVec = pca->getEigenVectors();
    /*
    cv::Mat imageBefore(300, 300, CV_8UC3, cv::Scalar(0, 0, 0));
    FaceShape faceBefore(imageBefore, FaceShape::convertVectorToLandmark(landmarksMatrix.row(0)));
    faceBefore.drawLandmarks(cv::Scalar(255, 0, 0));
    cv::imshow("Before", faceBefore.image);

    PCA pca(landmarksMatrix);
    MatrixXf vectorr = pca.getEigenValues().array().abs().sqrt();
    MatrixXf transformFace = pca.transform(landmarksMatrix.row(0), 50);
    MatrixXf transformInverseFace = pca.transformInverse(transformFace);

    cv::Mat imageAfter(300, 300, CV_8UC3, cv::Scalar(0, 0, 0));
    FaceShape faceAfter(imageAfter, FaceShape::convertVectorToLandmark(transformInverseFace.row(0)));
    faceAfter.drawLandmarks(cv::Scalar(255, 0, 0));
    cv::imshow("After", faceAfter.image);
    cv::waitKey(0);
    */


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::draw(int num, float value)
{
    cv::destroyAllWindows();
    newFace(num) = 0.03*value*eigenVel(num);
    MatrixXf newShape = pca->transformInverse(newFace.transpose());
    cv::Mat imageAfter(300, 300, CV_8UC3, cv::Scalar(0, 0, 0));
    FaceShape faceAfter(imageAfter, FaceShape::convertVectorToLandmark(newShape.row(0)));
    faceAfter.drawLandmarks(cv::Scalar(255, 0, 0));
    cv::imshow("test", faceAfter.image);
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    draw(0, value);
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    draw(1, value);
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    draw(2, value);
}

void MainWindow::on_horizontalSlider_4_valueChanged(int value)
{
    draw(3, value);
}

void MainWindow::on_horizontalSlider_5_valueChanged(int value)
{
    draw(4, value);
}

void MainWindow::on_horizontalSlider_2_sliderReleased()
{

}

void MainWindow::on_horizontalSlider_sliderReleased()
{

}

void MainWindow::on_horizontalSlider_3_sliderReleased()
{

}

void MainWindow::on_horizontalSlider_4_sliderReleased()
{

}

void MainWindow::on_horizontalSlider_5_sliderReleased()
{

}
