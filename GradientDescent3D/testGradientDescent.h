#ifndef TESTKDTREE_H
#define TESTKDTREE_H

#include <QObject>
#include <QTest>
#include <QList>
#include <QDebug>
#include <QVector3D>
#include "gradientdescent.h"
#include "autodiff.h"
#include "Eigen/Dense"
#include "Eigen/Geometry"

using namespace Eigen;

class TestGradientDescent: public QObject
{
    Q_OBJECT
public:
    explicit TestGradientDescent(QObject *parent = 0);
public:
    static AutoDiff funcTestAutoDiffMultip(float x, float a);
    static AutoDiff funcTestAutoDiffOne(float x, float a);
    static AutoDiff funcTestAutoDiffTwo(float x, float a);
    static AutoDiff funcTestAutoDiffThree(float x, float a);
    static AutoDiff funcTestAutoDiffFour(float x, float a);
private slots:
    void testGradientDescent();
    void testEigen();
    void testRodrigues();
};
#endif // TESTKDTREE_H
