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



class Leaf;
class Divider;



class Node
{
public:
    virtual ~Node() = 0;
    QVector3D item;
    bool isLeaf;
    virtual Leaf *toLeaf() = 0;
    virtual Divider *toDivider() = 0;
};



class Leaf : public Node
{
public:
    Leaf(QVector3D item);

    virtual Leaf *toLeaf(){
        return this;
    }
    virtual Divider *toDivider(){
        return NULL;
    }
};



class Divider : public Node
{
public:

    Node *right = NULL;
    Node *left = NULL;

    int axis = -1;

    virtual Leaf *toLeaf(){
        return NULL;
    }
    virtual Divider *toDivider(){
        return this;
    }

    Divider(Node *right, Node *left, QVector3D item, int axis);
};



class KdTree
{
public:
    static Node *kdTreeBuild(QVector<QVector3D> &points, int depth, int dimension);
    static QList<BorderAxis> getBorder(QVector<QVector3D> points, int dimension);
    static void nearestNeighborSearch(QVector3D &point, Node *tree, int dimension, Node *&best);
    static bool comparsionVectorsX(QVector3D &pointsFirst, QVector3D &pointsSecond);
    static bool comparsionVectorsY(QVector3D &pointsFirst, QVector3D &pointsSecond);
    static bool comparsionVectorsZ(QVector3D &pointsFirst, QVector3D &pointsSecond);
};

#endif // KDTREENEW_H
