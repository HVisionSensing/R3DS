#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <Eigen/Dense>
#include <Eigen/Core>
#include <QVector3D>

using namespace Eigen;

template<typename S>
using Vector3 = Eigen::Matrix<S, 3, 1> ;

class Transformation
{
public:
    template< typename Scalar >
    static Vector3<Scalar> shiftPoint(const Scalar &shiftX, const Scalar &shiftY, const Scalar shiftZ, const Vector3<Scalar> &point);
    template< typename Scalar >
    static Vector3<Scalar> rotateRodrigues(const Vector3<Scalar> &point, const Vector3<Scalar> &axis, const Scalar &angle);
};





template<typename Scalar>
Vector3<Scalar> Transformation::shiftPoint(const Scalar &shiftX, const Scalar &shiftY, const Scalar shiftZ, const Vector3<Scalar> &point)
{
    return Vector3<Scalar>(point.x()+shiftX, point.y()+shiftY, point.z()+shiftZ);
}



template<typename Scalar>
inline Vector3<Scalar> Transformation::rotateRodrigues(const Vector3<Scalar> &point, const Vector3<Scalar> &axis, const Scalar &angle)
{
    Vector3<Scalar> rotatedPoint = (axis.dot(point))*(1-cos(angle))*axis + (axis.cross(point))*sin(angle) + point*cos(angle);
    return rotatedPoint;
}


#endif // TRANSFORMATION_H
