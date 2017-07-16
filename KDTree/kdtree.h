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
    int itemIndex;
    QVector3D item;
    bool isLeaf;

    virtual void nearestNeighborSearch(QVector3D &point, Node *&best) = 0;
};



class Leaf : public Node
{
public:
    Leaf(int itemIndex, QVector3D item);

    virtual void nearestNeighborSearch(QVector3D &point, Node *&best) override;
};



class Divider : public Node
{
public:

    Node *right = NULL;
    Node *left = NULL;

    int axis = -1;

    Divider(Node *right, Node *left, int itemIndex, QVector3D item, int axis);

    virtual void nearestNeighborSearch(QVector3D &point, Node *&best) override;
};

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
