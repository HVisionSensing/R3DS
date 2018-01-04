#include "autodiff.h"

AutoDiff::AutoDiff()
{
}

AutoDiff::AutoDiff(float value, float derivative)
    :value(value), derivative(derivative)
{
}

AutoDiff::AutoDiff(float value)
    :value(value), derivative(0)
{
}



float AutoDiff::Value() const
{
    return value;
}



float AutoDiff::Derivative() const
{
    return derivative;
}



AutoDiff AutoDiff::IndependendVariable(float x)
{
    return AutoDiff(x,1);
}



AutoDiff AutoDiff::IndependendVariable(AutoDiff x)
{
    return AutoDiff(x.value, 1);
}



void AutoDiff::setDerivable(float x)
{
    derivative = x;
}



AutoDiff cos(const AutoDiff &x)
{
    return AutoDiff(cos(x.Value()), -sin(x.Value())*x.Derivative());
}



AutoDiff sin(const AutoDiff &x)
{
    return AutoDiff(sin(x.Value()), cos(x.Value())*x.Derivative());
}



AutoDiff sqrt(const AutoDiff &x)
{
    Q_ASSERT(sqrt(x.Value()) > -0.000001);
    return AutoDiff(sqrt(x.Value()), x.Derivative()/2/sqrt(x.Value()));
}



AutoDiff operator/(const AutoDiff &x1, const AutoDiff &x2)
{
    Q_ASSERT(fabs(x2.Value()) > 0.000001);
    return AutoDiff(x1.Value() / x2.Value(), (x1.Derivative() * x2.Value() - x1.Value() * x2.Derivative()) / x2.Value() / x2.Value());
}



AutoDiff operator-(const AutoDiff &x1, const AutoDiff &x2)
{
    return AutoDiff(x1.Value() - x2.Value(), x1.Derivative() - x2.Derivative());
}



AutoDiff operator+(const AutoDiff &x1, const AutoDiff &x2)
{
    return AutoDiff(x1.Value() + x2.Value(), x1.Derivative() + x2.Derivative());
}



AutoDiff operator*(const AutoDiff &x1, const AutoDiff &x2)
{
    return AutoDiff(x1.Value() * x2.Value(), x1.Derivative() * x2.Value() + x1.Value() * x2.Derivative());
}



Vector3<AutoDiff> operator*(const AutoDiff &x1, const Vector3<AutoDiff> &x2)
{
    return Vector3<AutoDiff>(x1*x2[0], x1*x2[1], x1*x2[2]);
}

