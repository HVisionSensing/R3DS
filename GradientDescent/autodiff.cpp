#include "autodiff.h"

AutoDiff::AutoDiff(float value, float derivative)
    :value(value), derivative(derivative)
{
}

AutoDiff::AutoDiff(float value)
    :value(value), derivative(0)
{
}

float AutoDiff::getValue()
{
    return value;
}



float AutoDiff::getDerivative()
{
    return derivative;
}



AutoDiff AutoDiff::operator+(AutoDiff func)
{
    return AutoDiff(value + func.value, derivative + func.derivative);
}



AutoDiff AutoDiff::operator-(AutoDiff func)
{
    return AutoDiff(value - func.value, derivative - func.derivative);
}



AutoDiff AutoDiff::operator*(AutoDiff func)
{
    return AutoDiff(value * func.value, derivative * func.value + value * func.derivative);
}



AutoDiff AutoDiff::operator/(AutoDiff func)
{
    return AutoDiff(value / func.value, (derivative * func.value - value * func.derivative) / func.value / func.value);
}



AutoDiff AutoDiff::coss(AutoDiff func)
{
    return AutoDiff(cos(func.value), -sin(func.value)*func.derivative);
}



AutoDiff AutoDiff::sinn(AutoDiff func)
{
    return AutoDiff(sin(func.value), cos(func.value)*func.derivative);
}



AutoDiff AutoDiff::IndependendVariable(float x)
{
    return AutoDiff(x,1);
}



AutoDiff cos(AutoDiff func)
{
    return AutoDiff::coss(func);
}



AutoDiff sin(AutoDiff func)
{
    return AutoDiff::sinn(func);
}
