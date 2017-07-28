#ifndef GRADIENTDESCENT_H
#define GRADIENTDESCENT_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QDebug>
#include "math.h"
#include "updatefunctor.h"
#include "autodiff.h"
#include "problemvector.h"
#include "settings.h"
#include "transformation.h"
#include <Eigen/Dense>
#include <Eigen/Core>

class GradientDescent
{
public:
    static ProblemVector gradientAnalytical(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const ProblemVector &x);
    static ProblemVector gradientNumerical(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const ProblemVector &x);
    static ProblemVector gradientDescent(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, UpdateFunctor *update = NULL);

    template< typename Scalar >
    static Scalar getFunctionRodrigues(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const Scalar &alpha, const Scalar &shiftX, const Scalar &shiftY, const Scalar &shiftZ, const Vector3<Scalar> &axisProblem);
};








template<typename Scalar>
Scalar GradientDescent::getFunctionRodrigues(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const Scalar &alpha, const Scalar &shiftX, const Scalar &shiftY, const Scalar &shiftZ, const Vector3<Scalar> &axisProblem)
{
    Vector3<Scalar> axis = axisProblem;

    Scalar function = 0;
    Vector3<Scalar> rotated;
    Vector3<Scalar> shiftLocal;
    Vector3<Scalar> difference;


    for (int indexPoint = 0; indexPoint < meshFrom.size(); indexPoint++){
        Vector3<Scalar> pointFrom(meshFrom[indexPoint].x(), meshFrom[indexPoint].y(), meshFrom[indexPoint].z());
        Vector3<Scalar> pointTo(meshTo[indexPoint].x(), meshTo[indexPoint].y(), meshTo[indexPoint].z());

        rotated = Transformation::rotateRodrigues(pointFrom, axis, alpha);
        shiftLocal = Transformation::shiftPoint(shiftX, shiftY, shiftZ, rotated);

        difference = pointTo - shiftLocal;
        function = function + difference.squaredNorm();

    }
    return function;
}

#endif // GRADIENTDESCENT_H
