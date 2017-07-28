#ifndef GRADIENTDESCENT_H
#define GRADIENTDESCENT_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QDebug>
#include "math.h"
#include "updatefunctor.h"
#include "autodiff.h"
#include <Eigen/Dense>
#include <Eigen/Core>

using namespace Eigen;

template<typename S>
using Vector3 = Eigen::Matrix<S, 3, 1> ;

struct ProblemVector
{
    float alpha;
    float shiftX;
    float shiftY;
    float shiftZ;
    Vector3<float> axisProblem;
    ProblemVector(float alpha, float shiftX, float shiftY, float shiftZ, Vector3<float> axisProblem);

    float getLen(){return (alpha*alpha + shiftX*shiftX + shiftY*shiftY + shiftZ*shiftZ + axisProblem.squaredNorm());}

    ProblemVector operator *(float value){
        float newAlpha = alpha*value;
        float newShiftX = shiftX*value;
        float newShiftY = shiftY*value;
        float newShiftZ = shiftZ*value;
        Vector3<float> newAxisProblem = axisProblem*value;
        ProblemVector newInfo(newAlpha, newShiftX, newShiftY, newShiftZ, newAxisProblem);
        return newInfo;
    }

    ProblemVector operator -(ProblemVector value){
        float newAlpha = alpha-value.alpha;
        float newShiftX = shiftX-value.shiftX;
        float newShiftY = shiftY-value.shiftY;
        float newShiftZ = shiftZ-value.shiftZ;
        Vector3<float> newAxisProblem = axisProblem-value.axisProblem;
        ProblemVector newInfo(newAlpha, newShiftX, newShiftY, newShiftZ, newAxisProblem);
        return newInfo;
    }

    ProblemVector operator +(ProblemVector value){
        float newAlpha = alpha+value.alpha;
        float newShiftX = shiftX+value.shiftX;
        float newShiftY = shiftY+value.shiftY;
        float newShiftZ = shiftZ+value.shiftZ;
        Vector3<float> newAxisProblem = axisProblem+value.axisProblem;
        ProblemVector newInfo(newAlpha, newShiftX, newShiftY, newShiftZ, newAxisProblem);
        return newInfo;
    }
};



struct Settings
{
public:
    const int steps;
    const float lambda;
    const float epsilon;
    Settings(int steps, float lambda, float epsilon);
};



class GradientDescent
{
public:
    static ProblemVector getGradientAnalytical(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const ProblemVector &x);
    static ProblemVector getGradientNumerical(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const ProblemVector &x);
    static ProblemVector gradientDescent(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, UpdateFunctor *update = NULL);
    static ProblemVector comparisonGradients(ProblemVector &gradientFirst, ProblemVector &gradientSecond);
    template< typename Scalar >
    static Scalar getFunctionRodrigues(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const Scalar &alpha, const Scalar &shiftX, const Scalar &shiftY, const Scalar &shiftZ, const Vector3<Scalar> &axisProblem);
    template< typename Scalar >
    static Scalar getFunctionNaive(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const Scalar &alpha, const Scalar &shiftX, const Scalar &shiftY, const Scalar &shiftZ);
    template< typename Scalar >
    static Vector3<Scalar> getRotateRod(const Vector3<Scalar> &point, const Vector3<Scalar> &axis, const Scalar &angle);
};












template<typename Scalar>
Vector3<Scalar> GradientDescent::getRotateRod(const Vector3<Scalar> &point, const Vector3<Scalar> &axis, const Scalar &angle)
{
    Vector3<Scalar> rotatedPoint = ((cos(angle)-1)*(-1)) * (axis.dot(point)) * axis +
                                    sin(angle)*(axis.cross(point)) + cos(angle) * point;
    return rotatedPoint;
}



template<typename Scalar>
Scalar GradientDescent::getFunctionRodrigues(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const Scalar &alpha, const Scalar &shiftX, const Scalar &shiftY, const Scalar &shiftZ, const Vector3<Scalar> &axisProblem)
{

    if (axisProblem.squaredNorm() < 0.00001)
        return -1;
    Vector3<Scalar> axis = axisProblem;

    Scalar function = 0;
    Vector3<Scalar> rotated;
    Vector3<Scalar> shift(shiftX, shiftY, shiftZ);
    Vector3<Scalar> shiftLocal;
    Vector3<Scalar> difference;


    for (int indexPoint = 0; indexPoint < meshFrom.size(); indexPoint++){
        Vector3<Scalar> pointFrom(meshFrom[indexPoint].x(), meshFrom[indexPoint].y(), meshFrom[indexPoint].z());
        Vector3<Scalar> pointTo(meshTo[indexPoint].x(), meshTo[indexPoint].y(), meshTo[indexPoint].z());

        //rotated = ((cos(alpha)-1)*(-1)) * (axisProblem.dot(pointFrom)) * axisProblem + sin(alpha)*(axisProblem.cross(pointFrom)) + cos(alpha) * pointFrom;
        rotated = GradientDescent::getRotateRod(pointFrom, axis, alpha);

        shiftLocal = rotated + shift;
        difference = shiftLocal - pointTo;

        function = function + difference.squaredNorm();

    }
    return function;
}



template<typename Scalar>
Scalar GradientDescent::getFunctionNaive(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const Scalar &alpha, const Scalar &shiftX, const Scalar &shiftY, const Scalar &shiftZ)
{
    Scalar function = 0;
    for (int indexPoint = 0; indexPoint < meshFrom.size(); indexPoint++){
        Scalar rotatedXOne = meshFrom[indexPoint].x();
        Scalar rotatedYOne = cos(alpha)*meshFrom[indexPoint].y() + sin(alpha)*meshFrom[indexPoint].z();
        Scalar rotatedZOne = sin(alpha)*meshFrom[indexPoint].y()*(-1) + cos(alpha)*meshFrom[indexPoint].z();

        Scalar rotatedXTwo = cos(alpha)*rotatedXOne + sin(alpha)*rotatedZOne;
        Scalar rotatedYTwo = rotatedYOne;
        Scalar rotatedZTwo = sin(alpha)*rotatedXOne*(-1) + cos(alpha)*rotatedZOne;

        Scalar rotatedXTree = cos(alpha)*rotatedXTwo - sin(alpha)*rotatedYTwo;
        Scalar rotatedYTree = sin(alpha)*rotatedXTwo + cos(alpha)*rotatedYTwo;
        Scalar rotatedZTree = rotatedZTwo;

        Scalar shiftXlocal = rotatedXTree + shiftX;
        Scalar shiftYlocal = rotatedYTree + shiftY;
        Scalar shiftZlocal = rotatedZTree + shiftZ;

        Scalar differenceX = shiftXlocal - meshTo[indexPoint].x();
        Scalar differenceY = shiftYlocal - meshTo[indexPoint].y();
        Scalar differenceZ = shiftZlocal - meshTo[indexPoint].z();
        function = function + (differenceX*differenceX + differenceY*differenceY + differenceZ*differenceZ);
    }
    return function;
}

#endif // GRADIENTDESCENT_H
