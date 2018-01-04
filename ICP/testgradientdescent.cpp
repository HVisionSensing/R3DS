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



void TestGradientDescent::testInternalEntire()
{
    QVector<QVector3D> meshFrom = {QVector3D(1,1,0)};
    QVector<QVector3D> meshTo = {QVector3D(1,2,0)};
    ProblemVector x(0, 0, 1, 0, Vector3<float>(1,1,1));
    ErrorFunctionQua errorFunc;
    VectorS<float> internal = errorFunc.internalEntire(meshFrom, meshTo, x.angle, x.shiftX, x.shiftY, x.shiftZ, x.axis);
    QCOMPARE(internal.squaredNorm(), 0.0);

    QVector<QVector3D> meshFrom2 = {QVector3D(1,1,1)};
    QVector<QVector3D> meshTo2 = {QVector3D(2,2,2)};
    ProblemVector x2(0, 1, 1, 1, Vector3<float>(1,1,1));
    ErrorFunctionQua errorFunc2;
    VectorS<float> internal2 = errorFunc2.internalEntire(meshFrom2, meshTo2, x2.angle, x2.shiftX, x2.shiftY, x2.shiftZ, x2.axis);
    QCOMPARE(internal2.squaredNorm(), 0.0);

    QVector<QVector3D> meshFrom3 = {QVector3D(1,1,0), QVector3D(1,2,0), QVector3D(2,2,0), QVector3D(2,1,0)};
    QVector<QVector3D> meshTo3 = {QVector3D(-1,1,0), QVector3D(-2,1,0), QVector3D(-2,2,0), QVector3D(-1,2,0)};
    ProblemVector x3(1.5708, 0, 0, 0, Vector3<float>(0,0,1));
    ErrorFunctionQua errorFunc3;
    VectorS<float> internal3 = errorFunc3.internalEntire(meshFrom3, meshTo3, x3.angle, x3.shiftX, x3.shiftY, x3.shiftZ, x3.axis);
    QVERIFY(internal3.squaredNorm() < 0.00001);
}



void TestGradientDescent::testKDTree()
{
    QVector<QVector3D> listOfPoints;

    QVector3D v21(2, 3, 0);
    QVector3D v22(4, 2, 0);
    QVector3D v23(6, 5, 0);
    QVector3D v24(15, 4, 0);
    QVector3D v25(8, 7, 0);
    QVector3D v26(5.5, 10, 0);
    QVector3D v27(9, 12 , 0);

    listOfPoints.push_back(v21);
    listOfPoints.push_back(v22);
    listOfPoints.push_back(v23);
    listOfPoints.push_back(v24);
    listOfPoints.push_back(v25);
    listOfPoints.push_back(v26);
    listOfPoints.push_back(v27);

    QVector<int> listOfIndex;
    for (int i = 0; i < listOfPoints.size(); i++)
        listOfIndex.append(i);

    Node *tree = KdTree::kdTreeBuild(listOfPoints, listOfIndex);

    QVector3D point(9, 8, 0);
    Node *best = NULL;
    tree->nearestNeighborSearch(point, best);
    QCOMPARE(best->item, v25);

    QVector3D point2(1, 2, 0);
    Node *best2 = NULL;
    tree->nearestNeighborSearch(point2, best2);
    QCOMPARE(best2->item, v21);

    QVector3D point3(7, 2, 0);
    Node *best3 = NULL;
    tree->nearestNeighborSearch(point3, best3);
    QCOMPARE(best3->item, v22);

    QVector3D point4(7, 10, 0);
    Node *best4 = NULL;
    tree->nearestNeighborSearch(point4, best4);
    QCOMPARE(best4->item, v26);

    QVector3D point5(15, 8, 0);
    Node *best5 = NULL;
    tree->nearestNeighborSearch(point5, best5);
    QCOMPARE(best5->item, v24);

    QVector3D point6(11, 13, 0);
    Node *best6 = NULL;
    tree->nearestNeighborSearch(point6, best6);
    QCOMPARE(best6->item, v27);

    QVector3D v28(8, 9, 0);
    QVector3D v29(10, 10, 0);
    QVector3D v30(2, 5, 0);
    QVector3D v31(3, 7, 0);
    QVector3D v32(10, 2, 0);
    QVector3D v33(8, 2, 0);
    QVector3D v34(2, 6, 0);
    QVector3D v35(3, 8, 0);


    listOfPoints.push_back(v28);
    listOfPoints.push_back(v29);
    listOfPoints.push_back(v30);
    listOfPoints.push_back(v31);
    listOfPoints.push_back(v32);
    listOfPoints.push_back(v33);
    listOfPoints.push_back(v34);
    listOfPoints.push_back(v35);

    QVector<int> listOfIndex2;
    for (int i = 0; i < listOfPoints.size(); i++)
        listOfIndex2.append(i);

    Node *tree2 = KdTree::kdTreeBuild(listOfPoints, listOfIndex2);

    QVector3D point7(10, 13, 0);
    Node *best7 = NULL;
    tree2->nearestNeighborSearch(point7, best7);
    QCOMPARE(best7->item, v27);

    QVector3D point8(12, 11, 0);
    Node *best8 = NULL;
    tree2->nearestNeighborSearch(point8, best8);
    QCOMPARE(best8->item, v29);

    QVector3D point9(6.1, 9.8, 0);
    Node *best9 = NULL;
    tree2->nearestNeighborSearch(point9, best9);
    QCOMPARE(best9->item, v26);

    QVector3D point10(15, 8, 0);
    Node *best10 = NULL;
    tree2->nearestNeighborSearch(point10, best10);
    QCOMPARE(best10->item, v24);
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
