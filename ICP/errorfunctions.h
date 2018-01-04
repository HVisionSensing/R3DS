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

template<typename S>
using VectorS = Eigen::Matrix<S, Dynamic, 1>;

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

    template< typename Scalar >
    Vector3<Scalar> internal(const QVector3D &pointFrom, const QVector3D &pointTo, const Scalar &angle,
                      const Scalar &shiftX, const Scalar &shiftY, const Scalar &shiftZ, const Vector3<Scalar> &axis) const;
    template< typename Scalar >
    VectorS<Scalar> internalEntire(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const Scalar &angle,
                      const Scalar &shiftX, const Scalar &shiftY, const Scalar &shiftZ, const Vector3<Scalar> &axis) const;
};








template<typename Scalar>
Scalar ErrorFunctionQua::operator()(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const Scalar &angle, const Scalar &shiftX, const Scalar &shiftY, const Scalar &shiftZ, const Vector3<Scalar> &axis) const
{
    Scalar function = 0;
    Vector3<Scalar> difference;

    for (int indexPoint = 0; indexPoint < meshFrom.size(); indexPoint++){
        difference = internal(meshFrom[indexPoint], meshTo[indexPoint], angle, shiftX, shiftY, shiftZ, axis);
        function = function + difference.squaredNorm();
    }
    return function;
}

template<typename Scalar>
Vector3<Scalar> ErrorFunctionQua::internal(const QVector3D &pointFrom, const QVector3D &pointTo, const Scalar &angle, const Scalar &shiftX, const Scalar &shiftY, const Scalar &shiftZ, const Vector3<Scalar> &axis) const
{
    Vector3<Scalar> pointFromEigen(pointFrom.x(), pointFrom.y(), pointFrom.z());
    Vector3<Scalar> pointToEigen(pointTo.x(), pointTo.y(), pointTo.z());

    Vector3<Scalar> rotated = Transformation::rotateQuaternion(pointFromEigen, axis, angle);
    Vector3<Scalar> shiftLocal = Transformation::shiftPoint(shiftX, shiftY, shiftZ, rotated);

    Vector3<Scalar> difference = pointToEigen - shiftLocal;

    return difference;
}

template<typename Scalar>
VectorS<Scalar> ErrorFunctionQua::internalEntire(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const Scalar &angle, const Scalar &shiftX, const Scalar &shiftY, const Scalar &shiftZ, const Vector3<Scalar> &axis) const
{
    VectorS<Scalar> internalEntireFunc(meshFrom.size()*3);
    int indexElem = 0;
    for (int index = 0; index < meshFrom.size(); index++){
        Vector3<Scalar> pointFromEigen(meshFrom[index].x(), meshFrom[index].y(), meshFrom[index].z());
        Vector3<Scalar> pointToEigen(meshTo[index].x(), meshTo[index].y(), meshTo[index].z());

        Vector3<Scalar> rotated = Transformation::rotateQuaternion(pointFromEigen, axis, angle);
        Vector3<Scalar> shiftLocal = Transformation::shiftPoint(shiftX, shiftY, shiftZ, rotated);

        Vector3<Scalar> difference = pointToEigen - shiftLocal;
        internalEntireFunc(indexElem) = difference(0);
        internalEntireFunc(indexElem+1) = difference(1);
        internalEntireFunc(indexElem+2) = difference(2);
        indexElem += 3;
    }
    return internalEntireFunc;
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
