#include "sdm.h"

vector<FaceShape> SDM::getStartSet(const vector<FaceShape> &dataSet) const
{
    vector<FaceShape> startSet;
    for (int indFace = 0; indFace < dataSet.size(); indFace++)
        for (int indFaceBasic = 0; indFaceBasic < initFaces.size(); indFaceBasic++)
            startSet.push_back(FaceShape(dataSet[indFace].image, initFaces[indFaceBasic]));

    return startSet;
}



vector<FaceShape> SDM::getFinishSet(const vector<FaceShape> &dataSet) const
{
    vector<FaceShape> finishSet;
    for (int indFace = 0; indFace < dataSet.size(); indFace++)
        for (int indFaceBasic = 0; indFaceBasic < initFaces.size(); indFaceBasic++)
            finishSet.push_back(FaceShape(dataSet[indFace].image, dataSet[indFace].landmarks));

    return finishSet;
}



void SDM::train(MatrixXf landmarksOfStart, MatrixXf landmarksOfFinish, DataSet trainSetStart)
{
    for (int indOfStg = 0; indOfStg < numOfStages; indOfStg++){
        MatrixXf deltaLandmarks = landmarksOfFinish - landmarksOfStart;
        MatrixXf featuresDescriptor = descriptor.compute(trainSetStart.dataSet);
//        if (indOfStg == 0){
//            PCA pcaOnStage(featuresDescriptor);
//            qDebug() << "PCA created";
//            pca.push_back(pcaOnStage);
//            int reducedDimension = pcaOnStage.getReducedDimension(0.99);
//            qDebug() << reducedDimension;
//            reducedDimensions.push_back(reducedDimension);
//        }
        //MatrixXf featuresDescriptorReduced = pca[0].transform(featuresDescriptor, reducedDimensions[0]);
        model->fit(featuresDescriptor, deltaLandmarks);
        params.push_back(model->getParams());
        MatrixXf predictDeltaLandmarks = model->predict(featuresDescriptor);
        model->deleteParams();
        landmarksOfStart = landmarksOfStart + predictDeltaLandmarks;
        trainSetStart.transformLandmarks(predictDeltaLandmarks);

        deltaLandmarks = landmarksOfFinish - landmarksOfStart;
        qDebug() << "Error on stage " << indOfStg << " = " << deltaLandmarks.norm();
    }
}



SDM::SDM(LinearModel *model, FaceDescriptor &descriptor, vector<vector<cv::KeyPoint> > initFaces, float numOfStages)
{
    this->model = model;
    this->descriptor = descriptor;
    this->initFaces = initFaces;
    this->numOfStages = numOfStages;
}


void SDM::fit(const DataSet &trainSet)
{
    DataSet trainSetStart(getStartSet(trainSet.dataSet));
    DataSet trainSetFinish(getFinishSet(trainSet.dataSet));
    MatrixXf trainMarksFinish = trainSetFinish.getLandmarksMatrix();
    MatrixXf trainMarksStart = trainSetStart.getLandmarksMatrix();

    DataSet startSet(trainSetStart);
    trainSetStart.~DataSet();
    trainSetFinish.~DataSet();
    train(trainMarksStart, trainMarksFinish, startSet);
    startSet.~DataSet();
}



DataSet SDM::predict(const DataSet &dataSet)
{
    DataSet dataSetStart(getStartSet(dataSet.dataSet));
    MatrixXf dataSetMarksStart = dataSetStart.getLandmarksMatrix();
    MatrixXf predictedMarks = MatrixXf::Zero(dataSet.dataSet.size(), 68*2);

    for (int indOfStg = 0; indOfStg < numOfStages; indOfStg++){
        MatrixXf featuresDescriptor = descriptor.compute(dataSetStart.dataSet);
        //MatrixXf featuresDescriptorReduced = pca[0].transform(featuresDescriptor, reducedDimensions[0]);
        model->setParams(params[indOfStg]);
        MatrixXf predictDeltaLandmarks = model->predict(featuresDescriptor);
        model->deleteParams();
        dataSetMarksStart = dataSetMarksStart + predictDeltaLandmarks;
        dataSetStart.transformLandmarks(predictDeltaLandmarks);
    }

    dataSetStart.dataSet.clear();

    for (int indFace = 0; indFace < dataSet.dataSet.size(); indFace++)
        for (int step = indFace*initFaces.size(); step < indFace*initFaces.size()+initFaces.size(); step++)
            predictedMarks.row(indFace) = predictedMarks.row(indFace) + dataSetMarksStart.row(step);

    predictedMarks /= initFaces.size();
    DataSet newSet(dataSet.dataSet);
    newSet.setLandmarks(predictedMarks);
    return newSet;

}



float SDM::getError(const DataSet &truth, const DataSet &predict, const cv::Point &position, const cv::Scalar &color)
{
    MatrixXf truthLandmark = truth.getLandmarksMatrix();
    MatrixXf predictLandmark = predict.getLandmarksMatrix();

    MatrixXf difference = truthLandmark - predictLandmark;
    float entireError = 0;

    for (int indFace = 0; indFace < difference.rows(); indFace++){
        float errorOnFace = difference.row(indFace).lpNorm<1>()*100/predictLandmark.row(indFace).lpNorm<1>();
        entireError += errorOnFace;

        cv::String errorOnFaceStr = "error = " + to_string(errorOnFace);
        cv::putText(predict.dataSet[indFace].image, errorOnFaceStr, position,
                    cv::FONT_HERSHEY_COMPLEX_SMALL, 0.6, color, 1, CV_AA);
    }

    entireError /= difference.rows();


    return entireError;
}



SDMDlib::SDMDlib(const string &predictorLandmarks)
{
    dlib::deserialize(predictorLandmarks) >> shapePredictor;
}



DataSet SDMDlib::predict(DataSet dataSet)
{
    int dataSize = dataSet.dataSet.size();

    for (int indFace = 0; indFace < dataSize; indFace++){
        dlib::array2d<dlib::rgb_pixel> faceShape;
        dlib::assign_image(faceShape, dlib::cv_image<dlib::rgb_pixel>(dataSet.dataSet[indFace].image));
        dlib::rectangle dets(0,0,dataSet.dataSet[indFace].image.cols, dataSet.dataSet[indFace].image.rows);
        dlib::full_object_detection shape = shapePredictor(faceShape, dets);

        vector<cv::KeyPoint> points;
        for (int ind = 0; ind < shape.num_parts(); ind++)
        {
            float x = shape.part(ind)(0);
            float y = shape.part(ind)(1);
            points.push_back(cv::KeyPoint(x,y,32));
        }
        dataSet.dataSet[indFace].landmarks = points;
    }

    return dataSet;
}
