#include "testgradientdescent.h"

TestGradientDescent::TestGradientDescent(QObject *parent):QObject(parent)
{
}

AutoDiff TestGradientDescent::funcTestAutoDiffMultip(float x, float a)
{
    AutoDiff xd = AutoDiff::IndependendVariable(x);
    return xd*xd*a;
}

AutoDiff TestGradientDescent::funcTestAutoDiffOne(float x, float a)
{
    AutoDiff xd = AutoDiff::IndependendVariable(x);
    return xd*xd*xd*a - AutoDiff::coss(xd/2);
}

AutoDiff TestGradientDescent::funcTestAutoDiffTwo(float x, float a)
{
    AutoDiff xd = AutoDiff::IndependendVariable(x);
    return xd*xd*xd*a - AutoDiff::sinn(xd/2);
}

AutoDiff TestGradientDescent::funcTestAutoDiffThree(float x, float a)
{
    AutoDiff xd = AutoDiff::IndependendVariable(x);
    return AutoDiff::sinn(xd)*AutoDiff::coss(xd)*a;
}

AutoDiff TestGradientDescent::funcTestAutoDiffFour(float x, float a)
{
    AutoDiff xd = AutoDiff::IndependendVariable(x);
    return (AutoDiff::sinn(xd)*AutoDiff::coss(xd))/((xd*xd)*a);
}

void TestGradientDescent::testGradientDescent()
{
    AutoDiff test(0,0);
    float dif;
    float x;
    float a;

    x = 2;
    a = 2;
    test = funcTestAutoDiffOne(x, a);
    dif = 3*a*x*x+0.5*sin(x/2);
    QCOMPARE(test.getDerivative(), dif);

    x = 3;
    a = 3;
    test = funcTestAutoDiffTwo(x, a);
    dif = 3*a*x*x-0.5*cos(x/2);
    QCOMPARE(test.getDerivative(), dif);

    x = 4;
    a = 4;
    test = funcTestAutoDiffThree(x, a);
    dif = (cos(x)*cos(x)-sin(x)*sin(x))*a;
    QCOMPARE(test.getDerivative(), dif);

    x = 5;
    a = 5;
    test = funcTestAutoDiffFour(x, a);
    dif = (((-2*cos(x)*sin(x))/(x*x*x))+(((cos(x)*cos(x)-sin(x)*sin(x))/(x*x))))/a;
    QCOMPARE(test.getDerivative(), dif);

    x = 6;
    a = 6;
    test = funcTestAutoDiffMultip(x, a);
    dif = 2*a*x;
    QCOMPARE(test.getDerivative(), dif);
}
