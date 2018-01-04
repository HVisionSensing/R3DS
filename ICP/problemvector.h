#ifndef PROBLEMVECTOR_H
#define PROBLEMVECTOR_H

#include <Eigen/Dense>
#include <Eigen/Core>
#include <QVector>

using namespace Eigen;

template<typename S>
using Vector3 = Eigen::Matrix<S, 3, 1> ;

struct ProblemVector
{
public:
    float angle;
    float shiftX;
    float shiftY;
    float shiftZ;
    Vector3<float> axis;

public:
    ProblemVector(float angle, float shiftX, float shiftY, float shiftZ, Vector3<float> axis);
    float Len() const;

    ProblemVector operator *(const float &x) const;
    ProblemVector operator -(const ProblemVector &x) const;
    ProblemVector operator +(const ProblemVector &x) const;
    ProblemVector operator /(const float &x) const;
};
#endif // PROBLEMVECTOR_H
