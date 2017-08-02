#include "kdtreenodes.h"

Leaf::Leaf(int itemIndex, QVector3D item)
{
    this->item = item;
    this->itemIndex = itemIndex;
    isLeaf = true;
}


Divider::Divider(Node *right, Node *left, int itemIndex, QVector3D item, int axis)
    :right(right), left(left), axis(axis)
{
    this->item = item;
    this->itemIndex = itemIndex;
    isLeaf = false;
}



void Leaf::nearestNeighborSearch(QVector3D &point, Node *&best)
{
    if (best == NULL){
        best = this;
        return;
    }

    float minLen = (best->item-point).lengthSquared();
    float len = (item-point).lengthSquared();

    if (len >= minLen)
        return;

    best = this;
    minLen = len;
}



void Divider::nearestNeighborSearch(QVector3D &point, Node *&best)
{
    int axis = this->axis;
    bool toLeft = false;
    bool toRight = (right != NULL);


    if (axis == 0)
        if (point.x() < item.x() && left != NULL)
            toLeft = true;

    if (axis == 1)
        if (point.y() < item.y() && left != NULL)
            toLeft = true;

    if (axis == 2)
        if (point.z() < item.z() && left != NULL)
            toLeft = true;

    if (toLeft){
       left->nearestNeighborSearch(point, best);
    }
    else if (toRight){
       right->nearestNeighborSearch(point, best);
    }

    if (best == NULL){
        best = this;
        return;
    }

    float minLen = (best->item-point).lengthSquared();
    float len = (item-point).lengthSquared();

    if (len < minLen){
        best = this;
        minLen = len;
    }

    float x = item.x();
    float y = item.y();
    float z = item.z();

    bool intersectionNodePreviousX = ((axis == 0) && (((x > point.x()-minLen) && (point.x()>x)) || ((x < point.x()+minLen) && (point.x()<x))));
    bool intersectionNodePreviousY = ((axis == 1) && (((y > point.y()-minLen) && (point.y()>y)) || ((y < point.y()+minLen) && (point.y()<y))));
    bool intersectionNodePreviousZ = ((axis == 2) && (((z > point.z()-minLen) && (point.z()>z)) || ((z < point.z()+minLen) && (point.z()<z))));

    bool toRightMove = (right != NULL) && (((x < point.x()+minLen) && (point.x()<x)) || ((y < point.y()+minLen) && (point.y()<y)) || ((z < point.z()+minLen) && (point.z()<z)));
    bool toLeftMove = (left != NULL) && (((x > point.x()-minLen) && (point.x()>x)) || ((y > point.y()-minLen) && (point.y()>y)) || ((z > point.z()-minLen) && (point.z()>z)));

    if (intersectionNodePreviousX || intersectionNodePreviousY || intersectionNodePreviousZ){
        if (toRightMove)
           right->nearestNeighborSearch(point, best);
        if (toLeftMove)
           left->nearestNeighborSearch(point, best);
    }
}
