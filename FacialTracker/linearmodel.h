#ifndef LINEARREGRESSION_H
#define LINEARREGRESSION_H

#include "QDebug"
#include "QVector"
#include "Eigen/Core"
#include "Eigen/Dense"
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/SparseCore>
#include <Eigen/SparseCholesky>
#include <Eigen/SparseLU>
#include <Eigen/SparseQR>
#include <Eigen/Sparse>
#include "QElapsedTimer"

using namespace Eigen;
using namespace std;

class LinearModel
{
public:
    virtual void deleteParams() = 0;
    virtual void setParams(vector<MatrixXf> params) = 0;
    virtual void fit(const MatrixXf &x, const MatrixXf &y) = 0;

public:
    virtual vector<MatrixXf> getParams() const = 0;
    virtual MatrixXf predict(const MatrixXf &x) const = 0;
};



class LinearRegression : public LinearModel
{
private:
    vector<MatrixXf> params;

public:
    LinearRegression();
    void deleteParams() override;
    void setParams(const vector<MatrixXf> params) override;
    void fit(const MatrixXf &x, const MatrixXf &y) override;

public:
    vector<MatrixXf> getParams() const override;
    MatrixXf predict(const MatrixXf &x) const override;
};

#endif // LINEARREGRESSION_H
