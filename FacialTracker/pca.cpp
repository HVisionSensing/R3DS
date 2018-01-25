#include "pca.h"

PCA::PCA(const MatrixXf &data)
{
    mean = data.colwise().mean();
    MatrixXf dataMean = data.rowwise() - mean.transpose();
    MatrixXf cov = computeCov(dataMean);
    dataMean.resize(0,0);
    qDebug() << "cov";
    EigenSolver<MatrixXf> eigenSolver(cov);
    eigenValues = eigenSolver.eigenvalues().real();
    eigenVectors = eigenSolver.eigenvectors().real();
}



MatrixXf PCA::transform(const MatrixXf &data, const int reducedDimension) const
{
    MatrixXf dataMean = data.rowwise() - mean.transpose();
    MatrixXf eigenVectorsReduced = eigenVectors.block(0, 0, eigenVectors.rows(), reducedDimension);
    MatrixXf reducedData = (eigenVectorsReduced.transpose() * dataMean.transpose()).transpose();
    return reducedData;
}



MatrixXf PCA::transformInverse(const MatrixXf &data) const
{
    MatrixXf eigenVectorsReduced = eigenVectors.block(0, 0, eigenVectors.rows(), data.cols());
    MatrixXf dataIncreased = (eigenVectorsReduced * data.transpose()).transpose();
    MatrixXf dataIncreasedUnMean = dataIncreased.rowwise() + mean.transpose();
    return dataIncreasedUnMean;
}



int PCA::getReducedDimension(const float varianceRetained) const
{
    float sumAllValues = eigenValues.sum();
    int countValues = eigenValues.rows();

    for (int countReducedValues = countValues; countReducedValues > 0; countReducedValues--){
        float sumReducedValues = eigenValues.block(0,0,countReducedValues,1).sum();
        float varianceRetainedLocal = sumReducedValues / sumAllValues;

        if (varianceRetainedLocal <= varianceRetained)
            return countReducedValues;
    }
}



VectorXf PCA::getEigenValues() const
{
    return eigenValues;
}



MatrixXf PCA::getEigenVectors() const
{
    return eigenVectors;
}



MatrixXf PCA::computeCov(const MatrixXf &data) const
{
    int countObserv = data.rows();
    MatrixXf dataTrans = data.transpose();
    MatrixXf multilpy = dataTrans * data;
    MatrixXf cov = multilpy / countObserv;
    return cov;
}
