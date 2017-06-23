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
    void testIsVertexLine01();
    void testIsPolygonLine();
};

#endif // OBJLOADERTESTS_H
