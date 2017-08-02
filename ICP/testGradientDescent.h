#ifndef TESTKDTREE_H
#define TESTKDTREE_H

#include <QObject>
#include <QTest>
#include <QList>
#include <QDebug>
#include <QVector3D>

#include "gradientdescent.h"
#include "autodiff.h"
#include "transformation.h"
#include "errorfunctions.h"
#include "kdtree.h"
#include "pointfinder.h"
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
    void testErrorFunctionQua();
    void testErrorFunctionRod();
    void testGradientAnalytical();
    void testKDTree();
    void testAutoDiff();
    void testRodrigues();
    void testQuaternion();
};
#endif // TESTKDTREE_H
