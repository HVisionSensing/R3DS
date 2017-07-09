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
//    QVector<QVector3D> list;

//    QVector3D v1(1,6,6);
//    QVector3D v2(6,3,1);
//    QVector3D v3(2,4,9);
//    QVector3D v4(3,12,12);
//    QVector3D v5(10,24,24);
//    QVector3D v6(1,8,45);
//    QVector3D v7(7,4,5);
//    QVector3D v8(9,2,31);
//    QVector3D v9(32,54,12);
//    QVector3D v10(3,162,14);
//    QVector3D v11(10,24,53);
//    QVector3D v12(1,7,2);

//    list.push_back(v1);
//    list.push_back(v2);
//    list.push_back(v3);
//    list.push_back(v4);
//    list.push_back(v5);
//    list.push_back(v6);
//    list.push_back(v7);
//    list.push_back(v8);
//    list.push_back(v9);
//    list.push_back(v10);
//    list.push_back(v11);
//    list.push_back(v12);

//    Node *node = KdTree::kdTreeBuild(list, 1, 3);

    QVector<QVector3D> listOfPoints;

    QVector3D v21(2, 3, 5);
    QVector3D v22(4, 2, 5);
    QVector3D v23(6, 5, 5);
    QVector3D v24(15, 4, 5);
    QVector3D v25(8, 7, 5);
    QVector3D v26(5.5, 10, 5);
    QVector3D v27(9, 12 ,5);

    listOfPoints.push_back(v21);
    listOfPoints.push_back(v22);
    listOfPoints.push_back(v23);
    listOfPoints.push_back(v24);
    listOfPoints.push_back(v25);
    listOfPoints.push_back(v26);
    listOfPoints.push_back(v27);

   // BorderAxis bdX(4, 2);
    //BorderAxis bdY(6, 2);

    //QList<BorderAxis> bdList;
   // bdList.push_back(bdX);
   // bdList.push_back(bdY);

    //QList<BorderAxis> bdTest = KdTreeNew::getBorder(listOfPoints, 2);

    //NodeNew testNode(v21, bdTest, NULL, NULL, true);
   // QList<BorderAxis> bd = KdTree::getBorder(listOfPoints, 2);
    Node *tree = KdTree::kdTreeBuild(listOfPoints, 1, 2);

    QVector3D point(9, 8, 5);
    Node *best = KdTree::nearestNeighborSearch(point, tree, 1, 2,-1, tree);
    QCOMPARE(best->item, v25);

    QVector3D point2(1, 2, 5);
    Node *best2 = KdTree::nearestNeighborSearch(point2, tree, 1, 2,-1, tree);
    QCOMPARE(best2->item, v21);

    QVector3D point3(7, 2, 5);
    Node *best3 = KdTree::nearestNeighborSearch(point3, tree, 1, 2,-1, tree);
    QCOMPARE(best3->item, v22);

    QVector3D point4(7, 10, 5);
    Node *best4 = KdTree::nearestNeighborSearch(point4, tree, 1, 2,-1, tree);
    QCOMPARE(best4->item, v26);

    QVector3D point5(15, 8, 5);
    Node *best5 = KdTree::nearestNeighborSearch(point5, tree, 1, 2,-1, tree);
    QCOMPARE(best5->item, v24);

    QVector3D point6(11, 13, 5);
    Node *best6 = KdTree::nearestNeighborSearch(point6, tree, 1, 2,-1, tree);
    QCOMPARE(best6->item, v27);
}




