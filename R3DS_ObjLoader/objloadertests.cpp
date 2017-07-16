#include "objloadertests.h"
#include "objloader.h"

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





