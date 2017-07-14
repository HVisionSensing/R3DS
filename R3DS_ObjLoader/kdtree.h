#ifndef KDTREENEW_H
#define KDTREENEW_H

#include <QVector>
#include <QVector3D>
#include <QList>
#include <QtMath>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <functional>
#include <time.h>

struct BorderAxis
{
public:
    float max;
    float min;
    float len;

    BorderAxis(float maxInConstruct, float minInConstruct):max(maxInConstruct), min(minInConstruct), len(maxInConstruct-minInConstruct){}
};



class Node
{
public:
    QVector3D item;
    bool isLeaf;

    virtual void nearestNeighborSearch(QVector3D &point, QVector3D *&best) = 0;
};



class Leaf : public Node
{
public:
    Leaf(QVector3D item);

    virtual void nearestNeighborSearch(QVector3D &point, QVector3D *&best) override;
};



class Divider : public Node
{
public:

    Node *right = NULL;
    Node *left = NULL;

    int axis = -1;

    Divider(Node *right, Node *left, QVector3D item, int axis);

    virtual void nearestNeighborSearch(QVector3D &point, QVector3D *&best) override;
};

class KdTree
{
public:
    static Node *kdTreeBuild(QVector<QVector3D> &points);
    static QList<BorderAxis> getBorder(QVector<QVector3D> points);
    static bool comparsionVectorsX(QVector3D &pointsFirst, QVector3D &pointsSecond);
    static bool comparsionVectorsY(QVector3D &pointsFirst, QVector3D &pointsSecond);
    static bool comparsionVectorsZ(QVector3D &pointsFirst, QVector3D &pointsSecond);

};

#endif // KDTREENEW_H
