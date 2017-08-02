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

class GradientDescent
{
public:
    template <class ErrorFunction>
    static ProblemVector gradientAnalytical(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const ProblemVector &x, const ErrorFunction &errorFunc);
    template <class ErrorFunction>
    static ProblemVector gradientNumerical(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const ProblemVector &x, const ErrorFunction &errorFunc);
    template <class ErrorFunction>
    static ProblemVector gradientDescent(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const ErrorFunction &func, UpdateFunctor *update = NULL, Node *tree = NULL);
};








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

    float gradientLocalAlpha = errorFunc(meshFrom, meshTo, angle, shiftXLocal, shiftYLocal, shiftZLocal, axisProblemLocal).Derivative();
    float gradientLocalShiftX = errorFunc(meshFrom, meshTo, angleLocal, shiftX, shiftYLocal, shiftZLocal, axisProblemLocal).Derivative();
    float gradientLocalShiftY = errorFunc(meshFrom, meshTo, angleLocal, shiftXLocal, shiftY, shiftZLocal, axisProblemLocal).Derivative();
    float gradientLocalShiftZ = errorFunc(meshFrom, meshTo, angleLocal, shiftXLocal, shiftYLocal, shiftZ, axisProblemLocal).Derivative();
    float gradientLocalAxisProblemX = errorFunc(meshFrom, meshTo, angleLocal, shiftXLocal, shiftYLocal, shiftZLocal, axisProblemX).Derivative();
    float gradientLocalAxisProblemY = errorFunc(meshFrom, meshTo, angleLocal, shiftXLocal, shiftYLocal, shiftZLocal, axisProblemY).Derivative();
    float gradientLocalAxisProblemZ = errorFunc(meshFrom, meshTo, angleLocal, shiftXLocal, shiftYLocal, shiftZLocal, axisProblemZ).Derivative();
    Vector3<float> axisProblem(gradientLocalAxisProblemX, gradientLocalAxisProblemY, gradientLocalAxisProblemZ);
    ProblemVector gradientEntire(gradientLocalAlpha, gradientLocalShiftX, gradientLocalShiftY, gradientLocalShiftZ, axisProblem);
    return gradientEntire;
}



template<class ErrorFunction>
ProblemVector GradientDescent::gradientDescent(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const ErrorFunction &errorFunc, UpdateFunctor *update,  Node *tree)
{
    Settings settings(500000, 0.00001, 1.9, 0.01);
    float function = 0;
    ProblemVector entireGradient(0,0,0,0,Vector3<float>(0,0,0));
    ProblemVector entireGradientSecond(0,0,0,0,Vector3<float>(0,0,0));
    ProblemVector x(0,0,0,0,Vector3<float>(1,1,1));
    ProblemVector comparisonGradients(0,0,0,0,Vector3<float>(0,0,0));
    QVector<QVector3D> meshToNew = meshTo;

    for (int index = 0; index < settings.stepsCount; index++){

        if (tree != NULL){
            meshToNew.clear();
            PointFinderKDTree *finder = new PointFinderKDTree(tree);
            for (int indexPoints = 0; indexPoints < meshFrom.size(); indexPoints++){
                int index = finder->findNearestPoint(meshFrom[indexPoints]);
                meshToNew.push_back(meshTo[index]);
            }

        }

        Q_ASSERT(x.axis.norm() > 0.000001);
        x.axis = x.axis/x.axis.norm();
        function = errorFunc(meshFrom, meshToNew, x.angle, x.shiftX, x.shiftY, x.shiftZ, x.axis);
        entireGradient = GradientDescent::gradientAnalytical(meshFrom, meshToNew, x, errorFunc);
        entireGradientSecond = GradientDescent::gradientNumerical(meshFrom, meshToNew, x, errorFunc);
        comparisonGradients = entireGradient - entireGradientSecond;

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
        qDebug() << "-------------------------------------";

        x = x - (entireGradient * settings.stepLen);
        x.axis = x.axis - entireGradient.axis * settings.stepLenAxis;
        Q_ASSERT(x.axis.norm() > 0.000001);
        x.axis = x.axis/x.axis.norm();
        if(update != NULL)
            update->update(x.angle, x.shiftX, x.shiftY, x.shiftZ, x.axis);
        if (entireGradient.Len() < settings.stop)
            return x;
    }
    return x;
}

#endif // GRADIENTDESCENT_H
