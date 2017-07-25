#ifndef GRADIENTDESCENT_H
#define GRADIENTDESCENT_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QDebug>
#include "math.h"
#include "updatefunctor.h"
#include "autodiff.h"

struct ProblemVector
{
    float alpha;
    float shiftX;
    float shiftY;
    ProblemVector(float alpha, float shiftX, float shiftY);

    float getLen(){return (alpha*alpha + shiftX*shiftX + shiftY*shiftY);}

    ProblemVector operator *(float value){
        float newAlpha = alpha*value;
        float newShiftX = shiftX*value;
        float newShiftY = shiftY*value;
        ProblemVector newInfo(newAlpha, newShiftX, newShiftY);
        return newInfo;
    }

    ProblemVector operator -(ProblemVector value){
        float newAlpha = alpha-value.alpha;
        float newShiftX = shiftX-value.shiftX;
        float newShiftY = shiftY-value.shiftY;
        ProblemVector newInfo(newAlpha, newShiftX, newShiftY);
        return newInfo;
    }

    ProblemVector operator +(ProblemVector value){
        float newAlpha = alpha+value.alpha;
        float newShiftX = shiftX+value.shiftX;
        float newShiftY = shiftY+value.shiftY;
        ProblemVector newInfo(newAlpha, newShiftX, newShiftY);
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
    static Scalar getFunction(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const Scalar &alpha, const Scalar &shiftX, const Scalar &shiftY);
};











template<typename Scalar>
Scalar GradientDescent::getFunction(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const Scalar &alpha, const Scalar &shiftX, const Scalar &shiftY)
{
    Scalar function = 0;
    Scalar rotatedY = 0;
    Scalar rotatedX = 0;
    Scalar shiftXlocal = 0;
    Scalar shiftYlocal = 0;
    Scalar differenceX = 0;
    Scalar differenceY = 0;
    for (int indexPoint = 0; indexPoint < meshFrom.size(); indexPoint++){
        rotatedX = cos(alpha)*meshFrom[indexPoint].x() - sin(alpha)*meshFrom[indexPoint].y();
        rotatedY = sin(alpha)*meshFrom[indexPoint].x() + cos(alpha)*meshFrom[indexPoint].y();
        shiftXlocal = rotatedX + shiftX;
        shiftYlocal = rotatedY + shiftY;
        differenceX = shiftXlocal - meshTo[indexPoint].x();
        differenceY = shiftYlocal - meshTo[indexPoint].y();
        function = function + differenceX*differenceX + differenceY*differenceY;
    }

    return function;
}

#endif // GRADIENTDESCENT_H
