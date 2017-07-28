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

float AutoDiff::getValue() const
{
    return value;
}



float AutoDiff::getDerivative() const
{
    return derivative;
}



AutoDiff AutoDiff::coss(const AutoDiff &func)
{
    return AutoDiff(cos(func.value), -sin(func.value)*func.derivative);
}



AutoDiff AutoDiff::sinn(const AutoDiff &func)
{
    return AutoDiff(sin(func.value), cos(func.value)*func.derivative);
}



AutoDiff AutoDiff::sqrtt(const AutoDiff &func)
{
    Q_ASSERT(sqrt(func.value) > -0.0001);
    return AutoDiff(sqrt(func.value), func.derivative/2/sqrt(func.value));
}



AutoDiff AutoDiff::IndependendVariable(float x)
{
    return AutoDiff(x,1);
}



AutoDiff AutoDiff::IndependendVariable(AutoDiff x)
{
    return AutoDiff(x.value, 1);
}



AutoDiff cos(const AutoDiff &func)
{
    return AutoDiff::coss(func);
}



AutoDiff sin(const AutoDiff &func)
{
    return AutoDiff::sinn(func);
}



AutoDiff sqrt(const AutoDiff &func)
{
    return AutoDiff::sqrtt(func);
}
