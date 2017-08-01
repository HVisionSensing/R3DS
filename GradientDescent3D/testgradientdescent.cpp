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
    return xd*xd*xd*a - cos(xd/2);
}

AutoDiff TestGradientDescent::funcTestAutoDiffTwo(float x, float a)
{
    AutoDiff xd = AutoDiff::IndependendVariable(x);
    return xd*xd*xd*a - sin(xd/2);
}

AutoDiff TestGradientDescent::funcTestAutoDiffThree(float x, float a)
{
    AutoDiff xd = AutoDiff::IndependendVariable(x);
    return sin(xd)*cos(xd)*a;
}

AutoDiff TestGradientDescent::funcTestAutoDiffFour(float x, float a)
{
    AutoDiff xd = AutoDiff::IndependendVariable(x);
    return (sin(xd)*cos(xd))/((xd*xd)*a);
}



void TestGradientDescent::testErrorFunctionQua()
{
    QVector<QVector3D> mesh1 = {QVector3D(1,1,1)};
    ErrorFunctionQua func;
    double testFunc = func(mesh1, mesh1, 0.0, 0.0, 0.0, 0.0, Vector3<double>(1,1,1));
    QCOMPARE(testFunc, 0.0);

    QVector<QVector3D> mesh2 = {QVector3D(1,1,1)};
    QVector<QVector3D> mesh22 = {QVector3D(2,2,2)};
    ErrorFunctionQua func2;
    double testFunc2 = func2(mesh2, mesh22, 0.0, 0.0, 0.0, 0.0, Vector3<double>(1,1,1));
    QCOMPARE(testFunc2, 3.0);
}



void TestGradientDescent::testErrorFunctionRod()
{
    QVector<QVector3D> mesh1 = {QVector3D(1,1,1)};
    ErrorFunctionRod func;
    double testFunc = func(mesh1, mesh1, 0.0, 0.0, 0.0, 0.0, Vector3<double>(1,1,1));
    QCOMPARE(testFunc, 0.0);

    QVector<QVector3D> mesh2 = {QVector3D(1,1,1)};
    QVector<QVector3D> mesh22 = {QVector3D(2,2,2)};
    ErrorFunctionRod func2;
    double testFunc2 = func2(mesh2, mesh22, 0.0, 0.0, 0.0, 0.0, Vector3<double>(1,1,1));
    QCOMPARE(testFunc2, 3.0);
}



void TestGradientDescent::testGradientAnalytical()
{
    QVector<QVector3D> meshOne = {QVector3D(10, 10, 10)};
    QVector<QVector3D> meshTwo = {QVector3D(2, 2, 2)};
    ProblemVector xOne(0, 2, 2, 2, Vector3<float>(1,1,1));
    ProblemVector xTwo = GradientDescent::gradientAnalytical(meshOne, meshTwo, xOne, ErrorFunctionRod());
    float len = xTwo.Len();
    QCOMPARE(len, 1200.0);

    QVector<QVector3D> meshOne2 = {QVector3D(1, 1, 1)};
    QVector<QVector3D> meshTwo2 = {QVector3D(2, 2, 2)};
    ProblemVector xOne2(M_PI/2.0, 0, 0, 0, Vector3<float>(1,1,1));
    ProblemVector xTwo2 = GradientDescent::gradientAnalytical(meshOne2, meshTwo2, xOne2, ErrorFunctionRod());
    float len2 = xTwo2.Len();
    QCOMPARE(len2, 588.0);

    QVector<QVector3D> meshOne3 = {QVector3D(1, 1, 1)};
    QVector<QVector3D> meshTwo3 = {QVector3D(2, 2, 2)};
    ProblemVector xOne3(M_PI/2.0, 0, 0, 0, Vector3<float>(0,1,0));
    ProblemVector xTwo3 = GradientDescent::gradientAnalytical(meshOne3, meshTwo3, xOne3, ErrorFunctionRod());
    float len3 = xTwo3.Len();
    QCOMPARE(len3, 236.0);


    QVector<QVector3D> meshOne4 = {QVector3D(10, 10, 10)};
    QVector<QVector3D> meshTwo4 = {QVector3D(2, 2, 2)};
    ProblemVector xOne4(0, 2, 2, 2, Vector3<float>(1,1,1));
    ProblemVector xTwo4 = GradientDescent::gradientAnalytical(meshOne4, meshTwo4, xOne4, ErrorFunctionQua());
    float len4 = xTwo4.Len();
    QCOMPARE(len4, 1200.0);

    QVector<QVector3D> meshOne5 = {QVector3D(1, 1, 1)};
    QVector<QVector3D> meshTwo5 = {QVector3D(2, 2, 2)};
    ProblemVector xOne5(M_PI/2.0, 0, 0, 0, Vector3<float>(1,1,1));
    ProblemVector xTwo5 = GradientDescent::gradientAnalytical(meshOne5, meshTwo5, xOne5, ErrorFunctionQua());
    float len5 = xTwo5.Len();
    QCOMPARE(len5, 12.0);

    QVector<QVector3D> meshOne6 = {QVector3D(1, 1, 1)};
    QVector<QVector3D> meshTwo6 = {QVector3D(2, 2, 2)};
    ProblemVector xOne6(M_PI, 0, 0, 0, Vector3<float>(0,1,0));
    ProblemVector xTwo6 = GradientDescent::gradientAnalytical(meshOne6, meshTwo6, xOne6, ErrorFunctionQua());
    float len6 = xTwo6.Len();
    QCOMPARE(len6, 2892.0);
}



void TestGradientDescent::testAutoDiff()
{
    AutoDiff test(0,0);
    float dif;
    float x;
    float a;

    x = 2;
    a = 2;
    test = funcTestAutoDiffOne(x, a);
    dif = 3*a*x*x+0.5*sin(x/2);
    QCOMPARE(test.Derivative(), dif);

    x = 3;
    a = 3;
    test = funcTestAutoDiffTwo(x, a);
    dif = 3*a*x*x-0.5*cos(x/2);
    QCOMPARE(test.Derivative(), dif);

    x = 4;
    a = 4;
    test = funcTestAutoDiffThree(x, a);
    dif = (cos(x)*cos(x)-sin(x)*sin(x))*a;
    QCOMPARE(test.Derivative(), dif);

    x = 5;
    a = 5;
    test = funcTestAutoDiffFour(x, a);
    dif = (((-2*cos(x)*sin(x))/(x*x*x))+(((cos(x)*cos(x)-sin(x)*sin(x))/(x*x))))/a;
    QCOMPARE(test.Derivative(), dif);

    x = 6;
    a = 6;
    test = funcTestAutoDiffMultip(x, a);
    dif = 2*a*x;
    QCOMPARE(test.Derivative(), dif);
}



void TestGradientDescent::testRodrigues()
{
    Vector3<float> pointFrom(1, 0, 0);
    Vector3<float> axisProblem(0, 0, 5);
    axisProblem = axisProblem/axisProblem.norm();
    float alpha = 1.5708;

    Vector3<float> rotated = Transformation::rotateRodrigues(pointFrom, axisProblem, alpha);
    QCOMPARE(rotated[1], 1.0);
}



void TestGradientDescent::testQuaternion()
{
    Vector3<AutoDiff> point1(1, 0, 0);
    Vector3<AutoDiff> axis1(0, 0, 5);
    axis1 = axis1/axis1.norm();
    AutoDiff alpha1 = 1.5708;
    Quaternion<AutoDiff> test1(AngleAxis<AutoDiff>(alpha1, axis1));
    Vector3<AutoDiff> rotated1 = test1*point1;
    QCOMPARE(rotated1[1].Value(), 1.0);

    Vector3<AutoDiff> point2(0, 1, 0);
    Vector3<AutoDiff> axis2(0, 0, 5);
    axis2 = axis2/axis2.norm();
    AutoDiff alpha2 = 1.5708;
    Quaternion<AutoDiff> test2(AngleAxis<AutoDiff>(alpha2, axis2));
    Vector3<AutoDiff> rotated2 = test2*point2;
    QCOMPARE(rotated2[0].Value(), -1.0);
}
