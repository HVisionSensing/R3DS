#ifndef AUTODIFF_H
#define AUTODIFF_H

#include "math.h"

class AutoDiff
{
private:
    float value;
    float derivative;

public:
    AutoDiff(float value, float derivative);
    AutoDiff(float value);
    float getValue();
    float getDerivative();

    AutoDiff operator+(AutoDiff func);
    AutoDiff operator-(AutoDiff func);
    AutoDiff operator*(AutoDiff func);
    AutoDiff operator/(AutoDiff func);

    static AutoDiff coss(AutoDiff func);
    static AutoDiff sinn(AutoDiff func);

    static AutoDiff IndependendVariable(float x);
};

AutoDiff cos(AutoDiff func);
AutoDiff sin(AutoDiff func);

#endif // AUTODIFF_H
