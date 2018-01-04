#ifndef GRADIENTDESCENT_H
#define GRADIENTDESCENT_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QDebug>
#include <Eigen/Dense>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include "transformation.h"
#include "problemvector.h"
#include "updatefunctor.h"
#include "math.h"
#include "autodiff.h"
#include "settings.h"
#include "kdtree.h"
#include "pointfinder.h"

template<typename S>
using MatrixS = Eigen::Matrix<S, Dynamic, Dynamic>;

template<typename S>
using VectorS = Eigen::Matrix<S, Dynamic, 1>;

class GradientDescent
{
public:
    template <class ErrorFunction>
    static ProblemVector gradientAnalytical(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const ProblemVector &x, const ErrorFunction &errorFunc);
    template <class ErrorFunction>
    static ProblemVector gradientNumerical(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const ProblemVector &x, const ErrorFunction &errorFunc);
    template <class ErrorFunction>
    static ProblemVector gradientDescent(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const ErrorFunction &errorFunc, ProblemVector &probVec, bool isDebug = false, UpdateFunctor *update = NULL);
    template <class ErrorFunction>
    static ProblemVector gradientDescentGaussNewton(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const ErrorFunction &errorFunc, ProblemVector &probVec, bool isDebug = false, UpdateFunctor *update = NULL);
    template <class ErrorFunction>
    static MatrixS<float> jacobi(const ErrorFunction &errorFunc, const ProblemVector &x, const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo);
    template <class ErrorFunction>
    static MatrixS<float> jacobiNumerical(const ErrorFunction &errorFunc, const ProblemVector &x, const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo);

    static void getDerivativeCol(const int &col, const VectorS<AutoDiff> &corCol, MatrixS<float> &j);
    static void getValueCol(const int &col, const VectorS<float> &corCol, MatrixS<float> &j);
};






template<class ErrorFunction>
MatrixS<float> GradientDescent::jacobiNumerical(const ErrorFunction &errorFunc, const ProblemVector &x, const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo)
{
    MatrixS<float> j(meshFrom.size()*3, 7);
    float delta = 0.0001;
    float angleWithDelta = x.angle + delta;
    float shiftXWithDelta = x.shiftX + delta;
    float shiftYWithDelta = x.shiftY + delta;
    float shiftZWithDelta = x.shiftZ + delta;
    float axisXWithDelta = x.axis[0] + delta;
    float axisYWithDelta = x.axis[1] + delta;
    float axisZWithDelta = x.axis[2] + delta;
    Vector3<float> axisProblemX (axisXWithDelta, x.axis[1], x.axis[2]);
    Vector3<float> axisProblemY (x.axis[0], axisYWithDelta, x.axis[2]);
    Vector3<float> axisProblemZ (x.axis[0], x.axis[1], axisZWithDelta);

    VectorS<float> migratedAlphaGradient = (errorFunc.internalEntire(meshFrom, meshTo, angleWithDelta, x.shiftX, x.shiftY, x.shiftZ, x.axis)
                                   - errorFunc.internalEntire(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, x.shiftZ, x.axis))/delta;
    VectorS<float> migratedXGradient = (errorFunc.internalEntire(meshFrom, meshTo, x.angle, shiftXWithDelta, x.shiftY, x.shiftZ, x.axis)
                               - errorFunc.internalEntire(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, x.shiftZ, x.axis))/delta;
    VectorS<float> migratedYGradient = (errorFunc.internalEntire(meshFrom, meshTo, x.angle, x.shiftX, shiftYWithDelta, x.shiftZ, x.axis)
                               - errorFunc.internalEntire(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, x.shiftZ, x.axis))/delta;
    VectorS<float> migratedZGradient = (errorFunc.internalEntire(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, shiftZWithDelta, x.axis)
                               - errorFunc.internalEntire(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, x.shiftZ, x.axis))/delta;
    VectorS<float> migratedAxisProblemXGradient = (errorFunc.internalEntire(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, x.shiftZ, axisProblemX)
                               - errorFunc.internalEntire(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, x.shiftZ, x.axis))/delta;
    VectorS<float> migratedAxisProblemYGradient = (errorFunc.internalEntire(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, x.shiftZ, axisProblemY)
                               - errorFunc.internalEntire(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, x.shiftZ, x.axis))/delta;
    VectorS<float> migratedAxisProblemZGradient = (errorFunc.internalEntire(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, x.shiftZ, axisProblemZ)
                               - errorFunc.internalEntire(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, x.shiftZ, x.axis))/delta;

    GradientDescent::getValueCol(0, migratedAlphaGradient, j);
    GradientDescent::getValueCol(1, migratedXGradient, j);
    GradientDescent::getValueCol(2, migratedYGradient, j);
    GradientDescent::getValueCol(3, migratedZGradient, j);
    GradientDescent::getValueCol(4, migratedAxisProblemXGradient, j);
    GradientDescent::getValueCol(5, migratedAxisProblemYGradient, j);
    GradientDescent::getValueCol(6, migratedAxisProblemZGradient, j);

    return j;
}

template <class ErrorFunction>
ProblemVector GradientDescent::gradientNumerical(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const ProblemVector &x, const ErrorFunction &errorFunc)
{
    float delta = 0.001;
    float angleWithDelta = x.angle + delta;
    float shiftXWithDelta = x.shiftX + delta;
    float shiftYWithDelta = x.shiftY + delta;
    float shiftZWithDelta = x.shiftZ + delta;
    float axisXWithDelta = x.axis[0] + delta;
    float axisYWithDelta = x.axis[1] + delta;
    float axisZWithDelta = x.axis[2] + delta;
    Vector3<float> axisProblemX (axisXWithDelta, x.axis[1], x.axis[2]);
    Vector3<float> axisProblemY (x.axis[0], axisYWithDelta, x.axis[2]);
    Vector3<float> axisProblemZ (x.axis[0], x.axis[1], axisZWithDelta);
    Vector3<float> axisProblem =  x.axis;

    float migratedAlphaGradient = (errorFunc(meshFrom, meshTo, angleWithDelta, x.shiftX, x.shiftY, x.shiftZ, axisProblem)
                                   - errorFunc(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, x.shiftZ, axisProblem))/delta;
    float migratedXGradient = (errorFunc(meshFrom, meshTo, x.angle, shiftXWithDelta, x.shiftY, x.shiftZ, axisProblem)
                               - errorFunc(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, x.shiftZ, axisProblem))/delta;
    float migratedYGradient = (errorFunc(meshFrom, meshTo, x.angle, x.shiftX, shiftYWithDelta, x.shiftZ, axisProblem)
                               - errorFunc(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, x.shiftZ, axisProblem))/delta;
    float migratedZGradient = (errorFunc(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, shiftZWithDelta, axisProblem)
                               - errorFunc(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, x.shiftZ, axisProblem))/delta;
    float migratedAxisProblemXGradient = (errorFunc(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, x.shiftZ, axisProblemX)
                               - errorFunc(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, x.shiftZ, axisProblem))/delta;
    float migratedAxisProblemYGradient = (errorFunc(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, x.shiftZ, axisProblemY)
                               - errorFunc(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, x.shiftZ, axisProblem))/delta;
    float migratedAxisProblemZGradient = (errorFunc(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, x.shiftZ, axisProblemZ)
                               - errorFunc(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, x.shiftZ, axisProblem))/delta;

    Vector3<float> axisProblemNew(migratedAxisProblemXGradient, migratedAxisProblemYGradient, migratedAxisProblemZGradient);
    ProblemVector gradientEntire(migratedAlphaGradient, migratedXGradient, migratedYGradient, migratedZGradient, axisProblemNew);
    return gradientEntire;
}



template <class ErrorFunction>
ProblemVector GradientDescent::gradientAnalytical(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const ProblemVector &x, const ErrorFunction &errorFunc)
{
    AutoDiff shiftXLocal(x.shiftX);
    AutoDiff shiftYLocal(x.shiftY);
    AutoDiff shiftZLocal(x.shiftZ);
    AutoDiff angleLocal(x.angle);
    AutoDiff angle = AutoDiff::IndependendVariable(x.angle);
    AutoDiff shiftX = AutoDiff::IndependendVariable(x.shiftX);
    AutoDiff shiftY = AutoDiff::IndependendVariable(x.shiftY);
    AutoDiff shiftZ = AutoDiff::IndependendVariable(x.shiftZ);
    AutoDiff axisX = AutoDiff::IndependendVariable(x.axis[0]);
    AutoDiff axisY = AutoDiff::IndependendVariable(x.axis[1]);
    AutoDiff axisZ = AutoDiff::IndependendVariable(x.axis[2]);
    Vector3<AutoDiff> axisProblemLocal(x.axis[0], x.axis[1], x.axis[2]);
    Vector3<AutoDiff> axisProblemX (axisX, axisProblemLocal[1], axisProblemLocal[2]);
    Vector3<AutoDiff> axisProblemY (axisProblemLocal[0], axisY, axisProblemLocal[2]);
    Vector3<AutoDiff> axisProblemZ (axisProblemLocal[0], axisProblemLocal[1], axisZ);

    float gradientLocalAlpha = (errorFunc(meshFrom, meshTo, angle, shiftXLocal, shiftYLocal, shiftZLocal, axisProblemLocal)).Derivative();
    float gradientLocalShiftX = (errorFunc(meshFrom, meshTo, angleLocal, shiftX, shiftYLocal, shiftZLocal, axisProblemLocal)).Derivative();
    float gradientLocalShiftY = (errorFunc(meshFrom, meshTo, angleLocal, shiftXLocal, shiftY, shiftZLocal, axisProblemLocal)).Derivative();
    float gradientLocalShiftZ = (errorFunc(meshFrom, meshTo, angleLocal, shiftXLocal, shiftYLocal, shiftZ, axisProblemLocal)).Derivative();
    float gradientLocalAxisProblemX = (errorFunc(meshFrom, meshTo, angleLocal, shiftXLocal, shiftYLocal, shiftZLocal, axisProblemX)).Derivative();
    float gradientLocalAxisProblemY = (errorFunc(meshFrom, meshTo, angleLocal, shiftXLocal, shiftYLocal, shiftZLocal, axisProblemY)).Derivative();
    float gradientLocalAxisProblemZ = (errorFunc(meshFrom, meshTo, angleLocal, shiftXLocal, shiftYLocal, shiftZLocal, axisProblemZ)).Derivative();
    Vector3<float> axisProblem(gradientLocalAxisProblemX, gradientLocalAxisProblemY, gradientLocalAxisProblemZ);
    ProblemVector gradientEntire(gradientLocalAlpha, gradientLocalShiftX, gradientLocalShiftY, gradientLocalShiftZ, axisProblem);
    return gradientEntire;
}



template<class ErrorFunction>
ProblemVector GradientDescent::gradientDescent(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const ErrorFunction &errorFunc, ProblemVector &probVec, bool isDebug, UpdateFunctor *update)
{
    // Head to Head (5000, 0.0000001, 0.00001, 100)
    // Head to ScanHead(500, 0.000001, 0.00001, 50)
    // Monkey to Monkey(5000, 0.001, 0.1, 0.0001)

    Settings settings(500, 0.0001, 0.1, 0.00001);
    float function = 0;
    ProblemVector entireGradient(0,0,0,0,Vector3<float>(1,1,1));
    ProblemVector entireGradientSecond(0,0,0,0,Vector3<float>(0,0,0));
    ProblemVector x = probVec;
    ProblemVector comparisonGradients(0,0,0,0,Vector3<float>(0,0,0));
    QVector<QVector3D> meshFromLocal = meshFrom;
    QVector<QVector3D> meshToLocal = meshTo;
    for (int index = 0; index < settings.stepsCount; index++){
        Q_ASSERT(x.axis.norm() > 0.000001);
        x.axis = x.axis/x.axis.norm();
        function = errorFunc(meshFromLocal, meshToLocal, x.angle, x.shiftX, x.shiftY, x.shiftZ, x.axis);
        entireGradient = GradientDescent::gradientAnalytical(meshFromLocal, meshToLocal, x, errorFunc);
        entireGradientSecond = GradientDescent::gradientNumerical(meshFromLocal, meshToLocal, x, errorFunc);
        comparisonGradients = entireGradient - entireGradientSecond;

        if(isDebug){
            qDebug() << "count step: " << index;
            qDebug() << "function: " << function;
            qDebug() << "axis Problem X: " << x.axis[0];
            qDebug() << "axis Problem Y: " << x.axis[1];
            qDebug() << "axis Problem Z: " << x.axis[2];
            qDebug() << "comparsion gradients: " << comparisonGradients.Len();
            qDebug() << "comparsion angle: " << comparisonGradients.angle;
            qDebug() << "comparsion axis X: " << comparisonGradients.axis[0];
            qDebug() << "comparsion axis Y: " << comparisonGradients.axis[1];
            qDebug() << "comparsion axis Z: " << comparisonGradients.axis[2];
            qDebug() << "comparsion X: " << comparisonGradients.shiftX;
            qDebug() << "comparsion Y: " << comparisonGradients.shiftY;
            qDebug() << "comparsion Z: " << comparisonGradients.shiftZ;
            qDebug() << "gradient len: " << entireGradient.Len();
            qDebug() << "gradient X: " << entireGradient.axis[0];
            qDebug() << "gradient Y: " << entireGradient.axis[1];
            qDebug() << "gradient Z: " << entireGradient.axis[2];
            qDebug() << "--------------------------------------";
        }

        x.angle = x.angle - entireGradient.angle*settings.stepLen;
        x.shiftX = x.shiftX - entireGradient.shiftX*settings.stepLen;
        x.shiftY = x.shiftY - entireGradient.shiftY*settings.stepLen;
        x.shiftZ = x.shiftZ - entireGradient.shiftZ*settings.stepLen;
        x.axis = x.axis - entireGradient.axis*settings.stepLenAxis;

        Q_ASSERT(x.axis.norm() > 0.000001);
        x.axis = x.axis/x.axis.norm();

        if(update != NULL)
            update->update(x.angle, x.shiftX, x.shiftY, x.shiftZ, x.axis, meshFromLocal);
        if (entireGradient.Len() < settings.stop)
            return x;
    }
    return x;
}


template<class ErrorFunction>
ProblemVector GradientDescent::gradientDescentGaussNewton(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const ErrorFunction &errorFunc  , ProblemVector &probVec, bool isDebug, UpdateFunctor *update)
{
    float function = 0;
    ProblemVector x = probVec;
    QVector<QVector3D> meshFromLocal = meshFrom;
    QVector<QVector3D> meshToLocal = meshTo;
    float step = 3;
    for (int index = 0; index < 4; index++){
        Q_ASSERT(x.axis.norm() > 0.000001);
        x.axis = x.axis/x.axis.norm();
        function = errorFunc(meshFromLocal, meshToLocal, x.angle, x.shiftX, x.shiftY, x.shiftZ, x.axis);

        MatrixS<float> j = GradientDescent::jacobi(errorFunc, x, meshFrom, meshTo);
        VectorS<float> f = errorFunc.internalEntire(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, x.shiftZ, x.axis);

        MatrixS<float> b = j.transpose()*f;

        MatrixS<float> a = j.transpose()*j + MatrixS<float>::Identity(j.cols(), j.cols()) * step;
        MatrixS<float> res = a.llt().solve(-b);

        if(isDebug){
            qDebug() << "count step: " << index;
            qDebug() << "function: " << function;
            qDebug() << "axis Problem X: " << x.axis[0];
            qDebug() << "axis Problem Y: " << x.axis[1];
            qDebug() << "axis Problem Z: " << x.axis[2];
            qDebug() << "step: " << step;
        }

        x.angle = x.angle + res(0);
        x.shiftX = x.shiftX + res(1);
        x.shiftY = x.shiftY + res(2);
        x.shiftZ = x.shiftZ + res(3);
        Vector3<float> axisLocal(res(4), res(5), res(6));
        x.axis = x.axis + axisLocal;
        Q_ASSERT(x.axis.norm() > 0.000001);
        x.axis = x.axis/x.axis.norm();

        if(update != NULL)
            update->update(x.angle, x.shiftX, x.shiftY, x.shiftZ, x.axis, meshFromLocal);
    }
    return x;
}

template<class ErrorFunction>
MatrixS<float> GradientDescent::jacobi(const ErrorFunction &errorFunc, const ProblemVector &x, const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo)
{
    MatrixS<float> j(meshFrom.size()*3, 7);
    AutoDiff shiftXLocal(x.shiftX);
    AutoDiff shiftYLocal(x.shiftY);
    AutoDiff shiftZLocal(x.shiftZ);
    AutoDiff angleLocal(x.angle);
    AutoDiff axisX (x.axis[0]);
    AutoDiff axisY (x.axis[1]);
    AutoDiff axisZ (x.axis[2]);
    Vector3<AutoDiff> axisProblemLocal(axisX, axisY, axisZ);

    /*VectorS<AutoDiff> problemVector(7, 1); //= {angleLocal, shiftXLocal, shiftYLocal, shiftZLocal, axisX, axisY, axisZ};
    problemVector(0) = angleLocal;
    problemVector(1) = shiftXLocal;
    problemVector(2) = shiftYLocal;
    problemVector(3) = shiftZLocal;
    problemVector(4) = axisX;
    problemVector(5) = axisY;
    problemVector(6) = axisZ;
    for (int index = 0; index < 7; index++){
        problemVector(index).setDerivable(1);
        Vector3<AutoDiff> axisProblemLocal(axisX, axisY, axisZ);
        VectorS<AutoDiff> curCol = errorFunc.internalEntire(meshFrom, meshTo, angleLocal, shiftXLocal, shiftYLocal, shiftZLocal, axisProblemLocal);
        problemVector(index).setDerivable(0);
        GradientDescent::getDerivativeCol(index, curCol, j);
    }*/

    angleLocal.setDerivable(1);
    VectorS<AutoDiff> curColAngle = errorFunc.internalEntire(meshFrom, meshTo, angleLocal, shiftXLocal, shiftYLocal, shiftZLocal, axisProblemLocal);
    angleLocal.setDerivable(0);
    GradientDescent::getDerivativeCol(0, curColAngle, j);

    shiftXLocal.setDerivable(1);
    VectorS<AutoDiff> curColX = errorFunc.internalEntire(meshFrom, meshTo, angleLocal, shiftXLocal, shiftYLocal, shiftZLocal, axisProblemLocal);
    shiftXLocal.setDerivable(0);
    GradientDescent::getDerivativeCol(1, curColX, j);

    shiftYLocal.setDerivable(1);
    VectorS<AutoDiff> curColY = errorFunc.internalEntire(meshFrom, meshTo, angleLocal, shiftXLocal, shiftYLocal, shiftZLocal, axisProblemLocal);
    shiftYLocal.setDerivable(0);
    GradientDescent::getDerivativeCol(2, curColY, j);

    shiftZLocal.setDerivable(1);
    VectorS<AutoDiff> curColZ = errorFunc.internalEntire(meshFrom, meshTo, angleLocal, shiftXLocal, shiftYLocal, shiftZLocal, axisProblemLocal);
    shiftZLocal.setDerivable(0);
    GradientDescent::getDerivativeCol(3, curColZ, j);

    axisProblemLocal(0).setDerivable(1);
    VectorS<AutoDiff> curColAxisX = errorFunc.internalEntire(meshFrom, meshTo, angleLocal, shiftXLocal, shiftYLocal, shiftZLocal, axisProblemLocal);
    axisProblemLocal(0).setDerivable(0);
    GradientDescent::getDerivativeCol(4, curColAxisX, j);

    axisProblemLocal(1).setDerivable(1);
    VectorS<AutoDiff> curColAxisY = errorFunc.internalEntire(meshFrom, meshTo, angleLocal, shiftXLocal, shiftYLocal, shiftZLocal, axisProblemLocal);
    axisProblemLocal(1).setDerivable(0);
    GradientDescent::getDerivativeCol(5, curColAxisY, j);

    axisProblemLocal(2).setDerivable(1);
    VectorS<AutoDiff> curColAxisZ = errorFunc.internalEntire(meshFrom, meshTo, angleLocal, shiftXLocal, shiftYLocal, shiftZLocal, axisProblemLocal);
    axisProblemLocal(2).setDerivable(0);
    GradientDescent::getDerivativeCol(6, curColAxisZ, j);

    return j;
}
#endif // GRADIENTDESCENT_H
