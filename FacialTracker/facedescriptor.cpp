#include "facedescriptor.h"


FaceDescriptor::FaceDescriptor()
{
}



FaceDescriptor::FaceDescriptor(cv::Ptr<cv::Feature2D> descriptor)
{
    this->descriptor = descriptor;
}



int FaceDescriptor::getSize() const
{
    return descriptor->descriptorSize();
}



MatrixXf FaceDescriptor::compute(vector<FaceShape> &faces)
{
    MatrixXf features(faces.size(), getSize() * 68);
    for (int indFace = 0; indFace < faces.size(); indFace++)
        features.row(indFace) = compute(faces[indFace]);

    return features;
}



MatrixXf FaceDescriptor::compute(FaceShape &face)
{

    cv::Mat descriptedFace;
    int sizeDescriptor = getSize();
    MatrixXf features(1, sizeDescriptor * 68);
    descriptor->compute(face.image, face.landmarks, descriptedFace);
    MatrixXf descriptedFaceEigen(descriptedFace.rows, descriptedFace.cols);
    cv::cv2eigen(descriptedFace, descriptedFaceEigen);
    for (int indMark = 0; indMark < descriptedFaceEigen.rows(); indMark++)
        features.block(0, sizeDescriptor*indMark, 1, sizeDescriptor) = descriptedFaceEigen.row(indMark);
    return features;
}
