#ifndef OBJLOADERTESTS_H
#define OBJLOADERTESTS_H

#include <QObject>
#include <QTest>

class ObjLoaderTests : public QObject
{
    Q_OBJECT
public:
    explicit ObjLoaderTests(QObject *parent = 0);

private slots:
    void testLoadStringFile();
    void testIsVertexLine();
    void testIsTexturesLine();
    void testIsGoodCoordinates2D();
    void testIsGoodCoordinates3D();
    void testIsGoodPolygon();
    void testIsPolygonLine();
    void testReadPoint3D();
    void testReadPoint2D();
    void testTriangulation();
    void testGetCorrectionLine();
    void testGetSizePolygon();
};


#endif // OBJLOADERTESTS_H
