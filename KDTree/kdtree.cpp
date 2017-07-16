#include "kdtree.h"

QVector<int> KdTree::buildIndexArray(QVector<QVector3D> &points)
{
    QVector<int> indexArray;
    for (int index = 0; index < points.size(); index++)
        indexArray.append(index);
    return indexArray;
}



Node *KdTree::kdTreeBuild(QVector<QVector3D> &points, QVector<int> &pointsIndex)
{
    if (pointsIndex.size() == 1){
        Node *leaf = new Leaf(pointsIndex[0], points[pointsIndex[0]]);
        return leaf;
    }

    QVector<QVector3D> points2;
    for (int i = 0; i < pointsIndex.size(); i++)
        points2.append(points[pointsIndex[i]]);

    QList<BorderAxis> borderAxis = KdTree::getBorder(points2);

    int axis = 0;
    float maxLen = borderAxis[0].len;
    for (int indexAxis = 0; indexAxis < borderAxis.size(); indexAxis++){
        if (borderAxis[indexAxis].len <= maxLen)
            continue;

        maxLen = borderAxis[indexAxis].len;
        axis = indexAxis;
    }

    auto xComparsion = [&](const int ind1, const int ind2){
        return points[ind1].x() < points[ind2].x();
    };
    auto yComparsion = [&](const int ind1, const int ind2){
        return points[ind1].y() < points[ind2].y();
    };
    auto zComparsion = [&](const int ind1, const int ind2){
        return points[ind1].z() < points[ind2].z();
    };

    if (axis == 0)
        std::sort(pointsIndex.begin(), pointsIndex.end(), xComparsion);
    if (axis == 1)
        std::sort(pointsIndex.begin(), pointsIndex.end(), yComparsion);
    if (axis == 2)
        std::sort(pointsIndex.begin(), pointsIndex.end(), zComparsion);

    int medianIndex = pointsIndex.size()/2;

    QVector<int> indexUntilMedian;
    QVector<int> indexAfterMedian;

    for (int i = 0; i < medianIndex; i++)
        indexUntilMedian.append(pointsIndex[i]);

    for (int i = medianIndex+1; i < pointsIndex.size(); i++)
        indexAfterMedian.append(pointsIndex[i]);

    Node *left = NULL;
    Node *right = NULL;

    if (indexUntilMedian.size())
        left = KdTree::kdTreeBuild(points, indexUntilMedian);

    if (indexAfterMedian.size())
        right = KdTree::kdTreeBuild(points, indexAfterMedian);

    Node *node = new Divider(right, left, pointsIndex[medianIndex], points[pointsIndex[medianIndex]], axis);
    return node;
}



QList<BorderAxis> KdTree::getBorder(QVector<QVector3D> points)
{
    QList<BorderAxis> bordList;

    std::sort(points.begin(), points.end(), KdTree::comparsionVectorsX);
    BorderAxis bordX(points.last().x(), points.first().x());
    bordList.push_back(bordX);

    std::sort(points.begin(), points.end(), KdTree::comparsionVectorsY);
    BorderAxis bordY(points.last().y(), points.first().y());
    bordList.push_back(bordY);

    std::sort(points.begin(), points.end(), KdTree::comparsionVectorsZ);
    BorderAxis bordZ(points.last().z(), points.first().z());
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
