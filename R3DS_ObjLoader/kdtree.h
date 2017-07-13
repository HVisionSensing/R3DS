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

    virtual void nearestNeighborSearch(QVector3D &point, Node *&best) = 0;
};



class Leaf : public Node
{
public:
    Leaf(QVector3D item);

    virtual void nearestNeighborSearch(QVector3D &point, Node *&best) override;
};



class Divider : public Node
{
public:

    Node *right = NULL;
    Node *left = NULL;

    int axis = -1;

    Divider(Node *right, Node *left, QVector3D item, int axis);

    virtual void nearestNeighborSearch(QVector3D &point, Node *&best) override;
};

class KdTree
{
public:
    static QVector3D nearestNeighborSearchBasic(QVector3D point, QVector<QVector3D> &points);
    static QVector3D nearestNeighborSearchNotBasic(QVector3D point, QVector<QVector3D> &points);
    static float testingKnn(QVector<QVector3D> &meshFirst, QVector<QVector3D> &meshSecond, std::function<QVector3D (QVector3D, QVector<QVector3D>&)> func);
    static Node *kdTreeBuild(QVector<QVector3D> &points);
    static QList<BorderAxis> getBorder(QVector<QVector3D> points);
    static bool comparsionVectorsX(QVector3D &pointsFirst, QVector3D &pointsSecond);
    static bool comparsionVectorsY(QVector3D &pointsFirst, QVector3D &pointsSecond);
    static bool comparsionVectorsZ(QVector3D &pointsFirst, QVector3D &pointsSecond);

};

#endif // KDTREENEW_H
