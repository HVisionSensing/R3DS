#ifndef KDTREENEW_H
#define KDTREENEW_H

#include <QVector>
#include <QVector3D>
#include <QList>
#include <QtMath>

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
    QList<BorderAxis> bord;

    Node *right = NULL;
    Node *left = NULL;

    int axis = -1;
    bool isLeaf;
};



class KdTree
{
public:
    static Node *kdTreeBuild(QVector<QVector3D> &points, int depth, int dimension);
    static QList<BorderAxis> getBorder(QVector<QVector3D> points, int dimension);
    void nearestNeighborSearch(QVector3D &point, Node *tree, int dimension);
    static bool comparsionVectorsX(QVector3D &pointsFirst, QVector3D &pointsSecond);
    static bool comparsionVectorsY(QVector3D &pointsFirst, QVector3D &pointsSecond);
    static bool comparsionVectorsZ(QVector3D &pointsFirst, QVector3D &pointsSecond);
public:
    Node *best = NULL;
};

#endif // KDTREENEW_H
