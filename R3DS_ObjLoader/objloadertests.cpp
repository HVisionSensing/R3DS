#include "objloadertests.h"
#include "objloader.h"

ObjLoaderTests::ObjLoaderTests(QObject *parent) : QObject(parent)
{

}

void ObjLoaderTests::testIsVertexLine01()
{
    QVERIFY(ObjLoader::isVerticesLine("v 1 2 3"));
    QVERIFY(ObjLoader::isVerticesLine("v 1.34 2.12 3.3453345"));
    QVERIFY(ObjLoader::isVerticesLine("v 1 2 3.3453345"));
    QVERIFY(!ObjLoader::isVerticesLine("v 1 2 3 4"));
    QVERIFY(!ObjLoader::isVerticesLine("v 1 2 abc"));
    QVERIFY(!ObjLoader::isVerticesLine("v1 2 3"));
    QVERIFY(!ObjLoader::isVerticesLine(""));
    QVERIFY(!ObjLoader::isVerticesLine("vt 1 2 3"));
    QVERIFY(!ObjLoader::isVerticesLine("vt 1 2"));
}

void ObjLoaderTests::testIsPolygonLine()
{
    QVERIFY(ObjLoader::isPolygonLine("f 12/23 23/23 33/33"));
    QVERIFY(ObjLoader::isPolygonLine("f 12/23 23/23 33/33 35/23 87/45 23/54"));
    QVERIFY(!ObjLoader::isPolygonLine("f12/23 13/23 23/33"));
    QVERIFY(!ObjLoader::isPolygonLine("v 12/23 23/23 23/33"));
    QVERIFY(!ObjLoader::isPolygonLine("f abs 23/11 23/33"));
    QVERIFY(!ObjLoader::isPolygonLine(""));
    QVERIFY(!ObjLoader::isPolygonLine("f 12/12/12 adsaa 12/12/31 23/24/2"));
    QVERIFY(!ObjLoader::isPolygonLine("f 12/аувауа/12 12/12/31 23/24/2"));
    QVERIFY(ObjLoader::isPolygonLine("f 12/23 23/23 33/33/6"));
}
