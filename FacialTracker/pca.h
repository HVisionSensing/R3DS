#ifndef PCA_H
#define PCA_H

#include "Eigen/Core"
#include "Eigen/Dense"
#include "Eigen/Eigenvalues"
#include <MatOp/DenseSymMatProd.h>
#include <SymEigsSolver.h>
#include "QDebug"

using namespace std;
using namespace Eigen;

class PCA
{
public:
    PCA(const MatrixXf &data);    
    MatrixXf transform(const MatrixXf &data, const int reducedDimension) const;
    MatrixXf transformInverse(const MatrixXf &data) const;
    int getReducedDimension(const float varianceRetained) const;

    VectorXf getEigenValues() const;
    MatrixXf getEigenVectors() const;

private:
    MatrixXf eigenVectors;
    VectorXf eigenValues;
    VectorXf mean;

    MatrixXf computeCov(const MatrixXf &data) const;
};
#endif // PCA_H
