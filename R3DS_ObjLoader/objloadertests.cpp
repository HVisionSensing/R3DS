#include "objloadertests.h"
#include "objloader.h"
#include "kdtree.h"
#include "kdtree.h"

ObjLoaderTests::ObjLoaderTests(QObject *parent) : QObject(parent)
{

}



void ObjLoaderTests::testLoadStringFile()
{
    ObjLoader objLoader;
    QString fileNameTest1("v 1 2 3\n"
                          "vt 5 6 7\n"
                          "vn 9 7 6\n");

    QVERIFY(objLoader.loadStringFile(fileNameTest1));


    ObjLoader objLoader2;
    QString fileNameTest2("v 1 g 3\n"
                          "vt 5 6 7\n"
                          "vn 9 7 6\n");

    QVERIFY(!objLoader2.loadStringFile(fileNameTest2));


    ObjLoader objLoader3;
    QString fileNameTest3("v 1 2 3\n"
                          "v 2 3 4\n"
                          "v 4 5 6\n"
                          "f 1 2 3\n");

    QVERIFY(objLoader3.loadStringFile(fileNameTest3));


    ObjLoader objLoader4;
    QString fileNameTest4("v 1 2 3\n"
                          "v 2 3 4\n"
                          "v 4 5 6\n"
                          "f 1 g 3\n");

    QVERIFY(!objLoader4.loadStringFile(fileNameTest4));


    ObjLoader objLoader5;
    QString fileNameTest5("v 1 2 3\n"
                          "v 2 3 4\n"
                          "v 4 5 6\n"
                          "vt 1 2 3\n"
                          "vt 1 6 4\n"
                          "vt 6 3 9\n"
                          "f 1/1 2/3 3/3\n");

    QVERIFY(objLoader5.loadStringFile(fileNameTest5));


    ObjLoader objLoader6;
    QString fileNameTest6("v 1 2 3\n"
                          "v 2 3 4\n"
                          "v 4 5 6\n"
                          "vt 1 2 3\n"
                          "vt 1 6 4\n"
                          "vt 6 3 9\n"
                          "f 1/2 2/g 3/4\n");

    QVERIFY(!objLoader6.loadStringFile(fileNameTest6));


    ObjLoader objLoader7;
    QString fileNameTest7("v 1 2 3\n"
                          "v 2 3 4\n"
                          "v 4 5 6\n"
                          "vt 1 2 3\n"
                          "vt 1 6 4\n"
                          "vt 6 3 9\n"
                          "f 5/6 7/8 9/10\n");

    QVERIFY(!objLoader7.loadStringFile(fileNameTest7));
}



void ObjLoaderTests::testIsVertexLine()
{
    QVERIFY(ObjLoader::isVerticesLine("v 1 2 3"));
    QVERIFY(!ObjLoader::isVerticesLine("vt 1 2 3"));
    QVERIFY(!ObjLoader::isVerticesLine("v1 2 3"));
}



void ObjLoaderTests::testIsTexturesLine()
{
    QVERIFY(ObjLoader::isTexturesLine("vt 1 2 3"));
    QVERIFY(!ObjLoader::isTexturesLine("v 1 2"));
    QVERIFY(!ObjLoader::isTexturesLine("vt1 2 3"));
}



void ObjLoaderTests::testIsGoodCoordinates2D()
{
    QVERIFY(ObjLoader::isGoodCoordinates2D("vt 1 2"));
    QVERIFY(!ObjLoader::isGoodCoordinates2D("vt 1 2 3"));
    QVERIFY(!ObjLoader::isGoodCoordinates2D("vt 1 g"));
}



void ObjLoaderTests::testIsGoodCoordinates3D()
{
    QVERIFY(ObjLoader::isGoodCoordinates3D("v 1 2 3"));
    QVERIFY(!ObjLoader::isGoodCoordinates3D("v 1 2"));
    QVERIFY(!ObjLoader::isGoodCoordinates3D("v 1 2 g"));
}



void ObjLoaderTests::testIsGoodPolygon()
{
    QVERIFY(ObjLoader::isGoodPolygon("f 1/2 2/2 3/3"));
    QVERIFY(ObjLoader::isGoodPolygon("f 1 2 3"));
    QVERIFY(ObjLoader::isGoodPolygon("f 1/2 2/3 3/3 5/3 7/5 3/4"));
    QVERIFY(ObjLoader::isGoodPolygon("f 2/3 3/3 3/3/6"));
    QVERIFY(!ObjLoader::isGoodPolygon("f abs 3/1 3/3"));
    QVERIFY(!ObjLoader::isGoodPolygon("f 2/2/1 adsaa 2/2/1 2/4/2"));
    QVERIFY(!ObjLoader::isGoodPolygon("f 2/аувауа/2 2/2/1 2/2/2"));
}



void ObjLoaderTests::testIsPolygonLine()
{
    QVERIFY(ObjLoader::isPolygonLine("f 1/2 2/2 3/3"));
    QVERIFY(!ObjLoader::isPolygonLine("f1/2 1/3 3/3"));
    QVERIFY(!ObjLoader::isPolygonLine("v 2/3 3/3 3/3"));
    QVERIFY(!ObjLoader::isPolygonLine(""));
}



void ObjLoaderTests::testReadPoint3D()
{
    QVector3D testVector(5, 10, 0);

    QCOMPARE(ObjLoader::readPoint3D("v 5 10 0"), testVector);
}



void ObjLoaderTests::testReadPoint2D()
{
    QVector2D testVector(5, 10);

    QCOMPARE(ObjLoader::readPoint2D("vt 5 10"), testVector);
}



void ObjLoaderTests::testTriangulation()
{
    QVector<int> indexV;
    QVector<int> polygonSt;
    QVector<int> indexVnew;


    indexV = {0, 1, 2};
    polygonSt = {0, 3};

    indexVnew = {0, 1, 2};
    QCOMPARE(ObjLoader::triangulation(indexV, polygonSt), indexVnew);


    indexV = {0, 1, 2, 3};
    polygonSt = {0, 4};

    indexVnew = {0, 1, 2, 0, 2, 3};
    QCOMPARE(ObjLoader::triangulation(indexV, polygonSt), indexVnew);


    indexV = {0, 1, 2, 3, 3, 2, 4, 5, 5, 6, 7, 8};
    polygonSt = {0, 4, 8, 12};

    indexVnew = {0, 1, 2, 0, 2, 3, 3, 2, 4, 3, 4, 5, 5, 6, 7, 5, 7, 8};
    QCOMPARE(ObjLoader::triangulation(indexV, polygonSt), indexVnew);


    indexV = {0, 1, 2, 3, 4};
    polygonSt = {0, 5};

    indexVnew = {0, 1, 2, 0, 2, 3, 0, 3, 4};
    QCOMPARE(ObjLoader::triangulation(indexV, polygonSt), indexVnew);


    indexV = {0, 1, 2, 3, 4, 5, 6};
    polygonSt = {0, 7};

    indexVnew = {0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6};
    QCOMPARE(ObjLoader::triangulation(indexV, polygonSt), indexVnew);
}



void ObjLoaderTests::testGetCorrectionLine()
{
    QString testLine("v 1 2 3  ");
    QCOMPARE(ObjLoader::getCorrectLine("v 1 2 3\r\n"), testLine);
}



void ObjLoaderTests::testGetSizePolygon()
{
    QVector<int> polygonStart = {0, 3};
    QCOMPARE(ObjLoader::getSizePolygon(polygonStart, 0), 3);

    QVector<int> polygonStart2 = {0, 3, 6};
    QCOMPARE(ObjLoader::getSizePolygon(polygonStart2, 1), 3);

    QVector<int> polygonStart3 = {0, 10, 30};
    QCOMPARE(ObjLoader::getSizePolygon(polygonStart3, 1), 20);
}



void ObjLoaderTests::testKdTree()
{
    QVector<QVector3D> listOfPoints;

    QVector3D v21(2, 3, 7);
    QVector3D v22(4, 2, 15);
    QVector3D v23(6, 5, 23);
    QVector3D v24(15, 4, 2);
    QVector3D v25(8, 7, 3);
    QVector3D v26(5.5, 10, 4);
    QVector3D v27(9, 12 , 5);

    listOfPoints.push_back(v21);
    listOfPoints.push_back(v22);
    listOfPoints.push_back(v23);
    listOfPoints.push_back(v24);
    listOfPoints.push_back(v25);
    listOfPoints.push_back(v26);
    listOfPoints.push_back(v27);


    Node *tree = KdTree::kdTreeBuild(listOfPoints, 1, 2);

    QVector3D point(9, 8, 5);
    Node *best = NULL;
    KdTree::nearestNeighborSearch(point, tree, 2, best);
    QCOMPARE(best->item, v25);

    QVector3D point2(1, 2, 5);
    Node *best2 = NULL;
    KdTree::nearestNeighborSearch(point2, tree, 2, best2);
    QCOMPARE(best2->item, v21);

    QVector3D point3(7, 2, 5);
    Node *best3 = NULL;
    KdTree::nearestNeighborSearch(point3, tree, 2, best3);
    QCOMPARE(best3->item, v22);

    QVector3D point4(7, 10, 5);
    Node *best4 = NULL;
    KdTree::nearestNeighborSearch(point4, tree, 2, best4);
    QCOMPARE(best4->item, v26);

    QVector3D point5(15, 8, 5);
    Node *best5 = NULL;
    KdTree::nearestNeighborSearch(point5, tree, 2, best5);
    QCOMPARE(best5->item, v24);

    QVector3D point6(11, 13, 5);
    Node *best6 = NULL;
    KdTree::nearestNeighborSearch(point6, tree, 2, best6);
    QCOMPARE(best6->item, v27);

    QVector3D v28(8, 9, 7);
    QVector3D v29(10, 10, 3);
    QVector3D v30(2, 5, 11);
    QVector3D v31(3, 7, 23);
    QVector3D v32(10, 2, 1);
    QVector3D v33(8, 2, 3);
    QVector3D v34(2, 6, 6);
    QVector3D v35(3, 8, 3);


    listOfPoints.push_back(v28);
    listOfPoints.push_back(v29);
    listOfPoints.push_back(v30);
    listOfPoints.push_back(v31);
    listOfPoints.push_back(v32);
    listOfPoints.push_back(v33);
    listOfPoints.push_back(v34);
    listOfPoints.push_back(v35);

    Node *tree2 = KdTree::kdTreeBuild(listOfPoints, 1, 2);

    QVector3D point7(10, 13, 5);
    Node *best7 = NULL;
    KdTree::nearestNeighborSearch(point7, tree2, 2, best7);
    QCOMPARE(best7->item, v27);

    QVector3D point8(12, 11, 5);
    Node *best8 = NULL;
    KdTree::nearestNeighborSearch(point8, tree2, 2, best8);
    QCOMPARE(best8->item, v29);

    QVector3D point9(6.1, 9.8, 5);
    Node *best9 = NULL;
    KdTree::nearestNeighborSearch(point9, tree2, 2, best9);
    QCOMPARE(best9->item, v26);

    QVector3D point10(15, 8, 5);
    Node *best10 = NULL;
    KdTree::nearestNeighborSearch(point10, tree2, 2, best10);
    QCOMPARE(best10->item, v24);
}




