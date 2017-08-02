#ifndef KDTREENODES_H
#define KDTREENODES_H

#include <QVector>
#include <QVector3D>

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

    void nearestNeighborSearch(QVector3D &point, Node *&best) override;
};



class Divider : public Node
{
public:
    Node *right = NULL;
    Node *left = NULL;
    int axis = -1;

    Divider(Node *right, Node *left, int itemIndex, QVector3D item, int axis);

    void nearestNeighborSearch(QVector3D &point, Node *&best) override;
};
#endif // KDTREENODES_H
