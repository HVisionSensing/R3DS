#include "problemvector.h"

ProblemVector::ProblemVector(float angle, float shiftX, float shiftY, float shiftZ, Vector3<float> axis)
    :angle(angle), shiftX(shiftX), shiftY(shiftY), shiftZ(shiftZ), axis(axis)
{
}



float ProblemVector::Len() const
{
    return (angle*angle + shiftX*shiftX + shiftY*shiftY + shiftZ*shiftZ + axis.squaredNorm());
}



ProblemVector ProblemVector::operator *(const float &x) const
{
    float newAngle = angle * x;
    float newShiftX = shiftX * x;
    float newShiftY = shiftY * x;
    float newShiftZ = shiftZ * x;
    Vector3<float> newAxis = axis * x;
    ProblemVector newProblemVector(newAngle, newShiftX, newShiftY, newShiftZ, newAxis);
    return newProblemVector;
}



ProblemVector ProblemVector::operator -(const ProblemVector &x) const
{
    float newAngle = angle - x.angle;
    float newShiftX = shiftX - x.shiftX;
    float newShiftY = shiftY - x.shiftY;
    float newShiftZ = shiftZ - x.shiftZ;
    Vector3<float> newAxis = axis - x.axis;
    ProblemVector newProblemVector(newAngle, newShiftX, newShiftY, newShiftZ, newAxis);
    return newProblemVector;
}



ProblemVector ProblemVector::operator +(const ProblemVector &x) const
{
    float newAngle = angle + x.angle;
    float newShiftX = shiftX + x.shiftX;
    float newShiftY = shiftY + x.shiftY;
    float newShiftZ = shiftZ + x.shiftZ;
    Vector3<float> newAxis = axis + x.axis;
    ProblemVector newProblemVector(newAngle, newShiftX, newShiftY, newShiftZ, newAxis);
    return newProblemVector;
}



ProblemVector ProblemVector::operator /(const float &x) const
{
    float newAngle = angle / x;
    float newShiftX = shiftX / x;
    float newShiftY = shiftY / x;
    float newShiftZ = shiftZ / x;
    Vector3<float> newAxis = axis / x;
    ProblemVector newProblemVector(newAngle, newShiftX, newShiftY, newShiftZ, newAxis);
    return newProblemVector;
}
