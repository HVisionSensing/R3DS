#ifndef KDTREE_H
#define KDTREE_H

#include <QVector>
#include <QVector3D>
#include <QList>
#include <QtMath>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <functional>
#include <time.h>

#include "kdtreenodes.h"
#include "borderaxis.h"

class KdTree
{
public:
    static QVector<int> buildIndexArray(QVector<QVector3D> &points);
    static Node *kdTreeBuild(QVector<QVector3D> &points, QVector<int> &pointsIndex);
    static QList<BorderAxis> getBorder(QVector<QVector3D> points);
    static bool comparsionVectorsX(QVector3D &pointsFirst, QVector3D &pointsSecond);
    static bool comparsionVectorsY(QVector3D &pointsFirst, QVector3D &pointsSecond);
    static bool comparsionVectorsZ(QVector3D &pointsFirst, QVector3D &pointsSecond);
};
#endif // KDTREE_H
