#ifndef KDTREE_H
#define KDTREE_H

#include <QVector>
#include <QVector3D>

struct Node
{
    QVector3D item;
    Node *left;
    Node *right;
};



class KdTree
{
public:
    static Node *kdTreeBuild(QVector<QVector3D> &points, int depth, int dimension);
    static Node *NearestNeighborSearch(QVector3D &point, Node *tree, int depth, int dimension);
    static bool comparsionVectorsX(QVector3D &points1, QVector3D &points2);
    static bool comparsionVectorsY(QVector3D &points1, QVector3D &points2);
    static bool comparsionVectorsZ(QVector3D &points1, QVector3D &points2);
};



#endif // KDTREE_H
