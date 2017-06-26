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

