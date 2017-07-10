#include "kdtree.h"

Node *KdTree::kdTreeBuild(QVector<QVector3D> &points, int depth, int dimension)
{
    QList<BorderAxis> borderAxis = KdTree::getBorder(points, dimension);

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

    Node *node = new Node;

    bool isLeaf = true;

    for (int indexAxis = 0; indexAxis < borderAxis.size(); indexAxis++)
        if (borderAxis.at(indexAxis).len != 0)
            isLeaf = false;


    node->item = points.operator [](medianIndex);
    node->bord = borderAxis;
    node->isLeaf = isLeaf;

    if (isLeaf)
        return node;

    node->axis = axis;
    if (pointsUntilMedian.size()){
        node->left = KdTree::kdTreeBuild(pointsUntilMedian, depth+1, dimension);
        node->left->previous = node;
    }

    if (pointsAfterMedian.size()){
        node->right = KdTree::kdTreeBuild(pointsAfterMedian, depth+1, dimension);
        node->right->previous = node;
    }

    return node;
}



QList<BorderAxis> KdTree::getBorder(QVector<QVector3D> points, int dimension)
{
    QList<BorderAxis> bordList;

    if (dimension == 2 || dimension == 3){
        std::sort(points.begin(), points.end(), KdTree::comparsionVectorsX);
        BorderAxis bordX(points.at(points.size()-1).x(), points.at(0).x());
        bordList.push_back(bordX);

        std::sort(points.begin(), points.end(), KdTree::comparsionVectorsY);
        BorderAxis bordY(points.at(points.size()-1).y(), points.at(0).y());
        bordList.push_back(bordY);
    }

    if (dimension == 3){
        std::sort(points.begin(), points.end(), KdTree::comparsionVectorsZ);
        BorderAxis bordZ(points.at(points.size()-1).z(), points.at(0).z());
        bordList.push_back(bordZ);
    }

    return bordList;
}



void KdTree::nearestNeighborSearch(QVector3D &point, Node *tree, int dimension)
{
    int axis = tree->axis;

    bool toLeft = false;
    bool toRight = (tree->right != NULL);


    if (axis == 0)
        if (KdTree::comparsionVectorsX(point, tree->item) && tree->left != NULL)
            toLeft = true;
    if (axis == 1)
        if (KdTree::comparsionVectorsY(point, tree->item) && tree->left != NULL)
            toLeft = true;

    if (dimension == 3 && axis == 2)
        if (KdTree::comparsionVectorsZ(point, tree->item) && tree->left != NULL)
            toLeft = true;

    if (toLeft){
       nearestNeighborSearch(point, tree->left, dimension);
    }
    else if (toRight){
       nearestNeighborSearch(point, tree->right, dimension);
    }

    if (tree->isLeaf && best == NULL){
        best = tree;
        return;
    }

    float minLen = (best->item.x()-point.x())*(best->item.x()-point.x()) + (best->item.y()-point.y())*(best->item.y()-point.y());
    float len = (tree->item.x()-point.x())*(tree->item.x()-point.x()) + (tree->item.y()-point.y())*(tree->item.y()-point.y());

    if (dimension == 3){
        minLen = (best->item.x()-point.x())*(best->item.x()-point.x()) + (best->item.y()-point.y())*(best->item.y()-point.y()) + (best->item.z()-point.z())*(best->item.z()-point.z());
        len = (tree->item.x()-point.x())*(tree->item.x()-point.x()) + (tree->item.y()-point.y())*(tree->item.y()-point.y()) + (tree->item.z()-point.z())*(tree->item.z()-point.z());
    }

    if (len < minLen){
        best = tree;
        minLen = len;
    }

    if (tree->isLeaf){
        return;
    }

    float x = tree->item.x();
    float y = tree->item.y();
    float z;

    if (dimension == 3)
        z = tree->item.z();

    bool intersectionNodePreviousX = ((tree->axis == 0) && (((x > point.x()-minLen) && (point.x()>x)) || ((x < point.x()+minLen) && (point.x()<x))));
    bool intersectionNodePreviousY = ((tree->axis == 1) && (((y > point.y()-minLen) && (point.y()>y)) || ((y < point.y()+minLen) && (point.y()<y))));
    bool intersectionNodePreviousZ = false;

    if (dimension == 3)
        intersectionNodePreviousZ = ((tree->axis == 2) && (((z > point.z()-minLen) && (point.z()>z)) || ((z < point.z()+minLen) && (point.z()<z)) ));

    bool toRightMove = (tree->right != NULL);
    bool toLeftMove = (tree->left != NULL);

    if (intersectionNodePreviousX || intersectionNodePreviousY || intersectionNodePreviousZ){
        if (toRightMove)
           nearestNeighborSearch(point, tree->right, dimension);
        if (toLeftMove)
           nearestNeighborSearch(point, tree->left, dimension);
    }
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
