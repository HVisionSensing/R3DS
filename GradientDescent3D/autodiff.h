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
    float getValue() const;
    float getDerivative() const;

    AutoDiff operator+(const AutoDiff &func) const{
        return AutoDiff(value + func.value, derivative + func.derivative);
    }

    AutoDiff operator-(const AutoDiff &func) const{
        return AutoDiff(value - func.value, derivative - func.derivative);
    }

    AutoDiff operator*(const AutoDiff &func) const{
        return AutoDiff(value * func.value, derivative * func.value + value * func.derivative);
    }

    bool operator <(const float func) const{
        return value < func;
    }

    Vector3<AutoDiff> operator*(Vector3<AutoDiff> &func){
        AutoDiff newP(value, derivative);
        AutoDiff newX = newP*func[0];
        AutoDiff newY = newP*func[1];
        AutoDiff newZ = newP*func[2];
        Vector3<AutoDiff> newVector(newX, newY, newZ);
        return newVector;
    }


    static AutoDiff coss(const AutoDiff &func);
    static AutoDiff sinn(const AutoDiff &func);
    static AutoDiff sqrtt(const AutoDiff &func);
    static AutoDiff IndependendVariable(float x);
    static AutoDiff IndependendVariable(AutoDiff x);
};

inline AutoDiff operator/(const AutoDiff &a, const AutoDiff &func)
{
    Q_ASSERT(fabs(func.getValue()) > 0.0001);

    return AutoDiff(a.getValue() / func.getValue(), (a.getDerivative() * func.getValue() - a.getValue() * func.getDerivative()) / func.getValue() / func.getValue());
}


AutoDiff cos(const AutoDiff &func);
AutoDiff sin(const AutoDiff &func);
AutoDiff sqrt(const AutoDiff &func);
#endif // AUTODIFF_H
