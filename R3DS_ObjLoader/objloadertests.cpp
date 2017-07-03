#include "objloadertests.h"
#include "objloader.h"

ObjLoaderTests::ObjLoaderTests(QObject *parent) : QObject(parent)
{

}

void ObjLoaderTests::testIsVertexLine01()
{
    QVERIFY(ObjLoader::isVerticesLine("v 1 2 3"));
    QVERIFY(ObjLoader::isVerticesLine("v 1.3 2.1 3.3"));
    QVERIFY(ObjLoader::isVerticesLine("v 1 2 3.3"));
    QVERIFY(!ObjLoader::isVerticesLine("v 1 2 3 4"));
    QVERIFY(!ObjLoader::isVerticesLine("v 1 2 abc"));
    QVERIFY(!ObjLoader::isVerticesLine("v1 2 3"));
    QVERIFY(!ObjLoader::isVerticesLine(""));
    QVERIFY(!ObjLoader::isVerticesLine("vt 1 2 3"));
    QVERIFY(!ObjLoader::isVerticesLine("vt 1 2"));
}

void ObjLoaderTests::testIsPolygonLine()
{
    QVERIFY(ObjLoader::isPolygonLine("f 1/2 2/2 3/3"));
    QVERIFY(ObjLoader::isPolygonLine("f 1 2 3"));
    QVERIFY(ObjLoader::isPolygonLine("f 1/2 2/3 3/3 5/3 7/5 3/4"));
    QVERIFY(ObjLoader::isPolygonLine("f 2/3 3/3 3/3/6"));
    QVERIFY(!ObjLoader::isPolygonLine("f1/2 1/3 3/3"));
    QVERIFY(!ObjLoader::isPolygonLine("v 2/3 3/3 3/3"));
    QVERIFY(!ObjLoader::isPolygonLine("f abs 3/1 3/3"));
    QVERIFY(!ObjLoader::isPolygonLine(""));
    QVERIFY(!ObjLoader::isPolygonLine("f 2/2/1 adsaa 2/2/1 2/4/2"));
    QVERIFY(!ObjLoader::isPolygonLine("f 2/аувауа/2 2/2/1 2/2/2"));
}

void ObjLoaderTests::testReadPoint3D()
{
    QVector3D testVector(5, 10, 15);

    QCOMPARE(ObjLoader::readPoint3D("v 5 10 15"), testVector);
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

