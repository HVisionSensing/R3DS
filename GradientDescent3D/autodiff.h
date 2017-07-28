#ifndef AUTODIFF_H
#define AUTODIFF_H

#include <QtGlobal>
#include "math.h"
#include <Eigen/Dense>
#include <Eigen/Geometry>

using namespace Eigen;

template<typename S>
using Vector3 = Eigen::Matrix<S, 3, 1> ;

class AutoDiff
{
private:
    float value;
    float derivative;

public:
    AutoDiff();
    AutoDiff(float value, float derivative);
    AutoDiff(float value);
    float Value() const;
    float Derivative() const;

public:
    static AutoDiff IndependendVariable(float x);
    static AutoDiff IndependendVariable(AutoDiff x);
};

Vector3<AutoDiff> operator*(const AutoDiff &x1, const Vector3<AutoDiff> &x2);
AutoDiff operator+(const AutoDiff &x1, const AutoDiff &x2);
AutoDiff operator-(const AutoDiff &x1, const AutoDiff &x2);
AutoDiff operator*(const AutoDiff &x1, const AutoDiff &x2);
AutoDiff operator/(const AutoDiff &x1, const AutoDiff &x2);
AutoDiff cos(const AutoDiff &x);
AutoDiff sin(const AutoDiff &x);
AutoDiff sqrt(const AutoDiff &x);
#endif // AUTODIFF_H
