#ifndef FUNCTIONFUNCTOR_H
#define FUNCTIONFUNCTOR_H

#include <QVector>
#include <QVector3D>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include "transformation.h"

using namespace Eigen;

template<typename S>
using Vector3 = Eigen::Matrix<S, 3, 1> ;

class ErrorFunctionRod
{
public:
    template< typename Scalar >
    Scalar operator()(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const Scalar &angle,
                      const Scalar &shiftX, const Scalar &shiftY, const Scalar &shiftZ, const Vector3<Scalar> &axis) const;
};



class ErrorFunctionQua
{
public:
    template< typename Scalar >
    Scalar operator()(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const Scalar &angle,
                      const Scalar &shiftX, const Scalar &shiftY, const Scalar &shiftZ, const Vector3<Scalar> &axis) const;
};








template<typename Scalar>
Scalar ErrorFunctionQua::operator()(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const Scalar &angle, const Scalar &shiftX, const Scalar &shiftY, const Scalar &shiftZ, const Vector3<Scalar> &axis) const
{
    Scalar function = 0;
    Vector3<Scalar> rotated;
    Vector3<Scalar> shiftLocal;
    Vector3<Scalar> difference;

    for (int indexPoint = 0; indexPoint < meshFrom.size(); indexPoint++){
        Vector3<Scalar> pointFrom(meshFrom[indexPoint].x(), meshFrom[indexPoint].y(), meshFrom[indexPoint].z());
        Vector3<Scalar> pointTo(meshTo[indexPoint].x(), meshTo[indexPoint].y(), meshTo[indexPoint].z());

        rotated = Transformation::rotateQuaternion(pointFrom, axis, angle);
        shiftLocal = Transformation::shiftPoint(shiftX, shiftY, shiftZ, rotated);

        difference = pointTo - shiftLocal;
        function = function + difference.squaredNorm();

    }
    return function;
}



template<typename Scalar>
Scalar ErrorFunctionRod::operator()(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const Scalar &angle, const Scalar &shiftX, const Scalar &shiftY, const Scalar &shiftZ, const Vector3<Scalar> &axis) const
{
    Scalar function = 0;
    Vector3<Scalar> rotated;
    Vector3<Scalar> shiftLocal;
    Vector3<Scalar> difference;


    for (int indexPoint = 0; indexPoint < meshFrom.size(); indexPoint++){
        Vector3<Scalar> pointFrom(meshFrom[indexPoint].x(), meshFrom[indexPoint].y(), meshFrom[indexPoint].z());
        Vector3<Scalar> pointTo(meshTo[indexPoint].x(), meshTo[indexPoint].y(), meshTo[indexPoint].z());

        rotated = Transformation::rotateRodrigues(pointFrom, axis, angle);
        shiftLocal = Transformation::shiftPoint(shiftX, shiftY, shiftZ, rotated);

        difference = pointTo - shiftLocal;
        function = function + difference.squaredNorm();

    }
    return function;
}

#endif // FUNCTIONFUNCTOR_H
