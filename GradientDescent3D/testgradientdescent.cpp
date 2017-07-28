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



void TestGradientDescent::testEigen()
{
    QVector3D point(1,2,3);
    QVector3D point2(0,1,2);
    Vector3d pointE(point.x(), point.y(), point.z());
    Vector3d point2E(point2.x(), point2.y(), point2.z());
    Vector3d point3E =  pointE.cross(point2E);
    float a = point3E[0];
    Vector3d f = 5*pointE;
    float l = f.squaredNorm();
    //AutoDiff o(2,5);
   // Vector3d p = o * pointE;

    QVector<QVector3D> mesh1 = {QVector3D(1,2,3),QVector3D(5,6,7),QVector3D(8,9,10),QVector3D(12,13,14)};
    //float func = GradientDescent::getFunctionRodrigues(mesh1, mesh1, 0,0,0,0,Vector3d(1,0,0));

    AutoDiff u(1,1);
    AutoDiff p = u*(-1);


    //template<typename Scalar>

    typedef Matrix<AutoDiff, 3, 1> Vector3d;
    Vector3d j(u,u,u);
    Vector3d x = j+j;
    //Vector3<AutoDiff> ft();
    Vector3<float> sa(1,2,3);
    Vector3<float> sk(0,1,2);
    Vector3<float> bs = sa.cross(sk);
    float bl = sa.dot(sk);
}



void TestGradientDescent::testRodrigues()
{
    Vector3<float> pointFrom(1, 0, 0);
    Vector3<float> pointTo(0, 1, 0);
    Vector3<float> axisProblem(0, 0, 1);
    axisProblem = axisProblem/axisProblem.norm();
    float alpha = 1.5708;

    Vector3<float> rotated = GradientDescent::getRotateRod(pointFrom, axisProblem, alpha);
    QCOMPARE(rotated[1], 1.0);
    int a = 5;
}
