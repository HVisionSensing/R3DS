#include "kdtree.h"

Node *KdTree::kdTreeBuild(QVector<QVector3D> &points)
{
    QList<BorderAxis> borderAxis = KdTree::getBorder(points);

    int axis = 0;
    float maxLen = borderAxis.at(0).len;
    for (int indexAxis = 0; indexAxis < borderAxis.size(); indexAxis++){
        if (borderAxis.at(indexAxis).len <= maxLen)
            continue;

        maxLen = borderAxis.at(indexAxis).len;
        axis = indexAxis;
    }

    if (axis == 0)
        std::sort(points.begin(), points.end(), KdTree::comparsionVectorsX);
    if (axis == 1)
        std::sort(points.begin(), points.end(), KdTree::comparsionVectorsY);
    if (axis == 2)
        std::sort(points.begin(), points.end(), KdTree::comparsionVectorsZ);

    int medianIndex = points.size()/2;

    QVector<QVector3D> pointsUntilMedian;
    QVector<QVector3D> pointsAfterMedian;

    for (int indexPoint = 0; indexPoint < medianIndex; indexPoint++)
        pointsUntilMedian.push_back(points.at(indexPoint));

    for (int indexPoint = medianIndex+1; indexPoint < points.size(); indexPoint++)
        pointsAfterMedian.push_back(points.at(indexPoint));

    bool isLeaf = true;

    for (int indexAxis = 0; indexAxis < borderAxis.size(); indexAxis++)
        if (borderAxis.at(indexAxis).len != 0){
            isLeaf = false;
            break;
        }

    if (isLeaf){
        Node *leaf = new Leaf(points.operator [](medianIndex));
        return leaf;
    }

    Node *left = NULL;
    Node *right = NULL;
    if (pointsUntilMedian.size())
        left = KdTree::kdTreeBuild(pointsUntilMedian);

    if (pointsAfterMedian.size())
        right = KdTree::kdTreeBuild(pointsAfterMedian);

    Node *node = new Divider(right, left, points.operator [](medianIndex), axis);
    return node;
}



QList<BorderAxis> KdTree::getBorder(QVector<QVector3D> points)
{
    QList<BorderAxis> bordList;

    std::sort(points.begin(), points.end(), KdTree::comparsionVectorsX);
    BorderAxis bordX(points.at(points.size()-1).x(), points.at(0).x());
    bordList.push_back(bordX);

    std::sort(points.begin(), points.end(), KdTree::comparsionVectorsY);
    BorderAxis bordY(points.at(points.size()-1).y(), points.at(0).y());
    bordList.push_back(bordY);

    std::sort(points.begin(), points.end(), KdTree::comparsionVectorsZ);
    BorderAxis bordZ(points.at(points.size()-1).z(), points.at(0).z());
    bordList.push_back(bordZ);


    return bordList;
}


bool KdTree::comparsionVectorsX(QVector3D &pointsFirst, QVector3D &pointsSecond)
{
    return pointsFirst.x() < pointsSecond.x();
}



bool KdTree::comparsionVectorsY(QVector3D &pointsFirst, QVector3D &pointsSecond)
{
    return pointsFirst.y() < pointsSecond.y();
}



bool KdTree::comparsionVectorsZ(QVector3D &pointsFirst, QVector3D &pointsSecond)
{
    return pointsFirst.z() < pointsSecond.z();
}



void Leaf::nearestNeighborSearch(QVector3D &point, QVector3D *&best)
{
    if (best == NULL){
        best = &this->item;
        return;
    }

    float minLen = (*best-point).lengthSquared();
    float len = (item-point).lengthSquared();

    if (len >= minLen)
        return;

    best = &this->item;
    minLen = len;
}



void Divider::nearestNeighborSearch(QVector3D &point, QVector3D *&best)
{
    int axis = this->axis;
    bool toLeft = false;
    bool toRight = (right != NULL);


    if (axis == 0)
        if (point.x() < item.x() && left != NULL) // Исправить
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
        best = &this->item;
        return;
    }

    float minLen = (*best-point).lengthSquared();
    float len = (item-point).lengthSquared();

    if (len < minLen){
        best = &this->item;
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



Leaf::Leaf(QVector3D item)
{
    this->item = item;
    isLeaf = true;
}



Divider::Divider(Node *right, Node *left, QVector3D item, int axis)
    :right(right), left(left), axis(axis)
{
    this->item = item;
    isLeaf = false;
}
