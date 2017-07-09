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



Node *KdTree::nearestNeighborSearch(QVector3D &point, Node *tree, int depth, int dimension, float lenBest, Node *best)
{
    int axis = tree->axis;
    Node *bestNode = best;
    float len = (tree->item.x()-point.x())*(tree->item.x()-point.x()) + (tree->item.y()-point.y())*(tree->item.y()-point.y());

    if (lenBest == -1)
        lenBest = len;
    else
        if (len < lenBest){
            lenBest = len;
            bestNode = tree;
        }


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

    if (toLeft)
        bestNode = KdTree::nearestNeighborSearch(point, tree->left, depth+1, dimension, lenBest, bestNode);
    if (toRight)
        bestNode = KdTree::nearestNeighborSearch(point, tree->right, depth+1, dimension, lenBest, bestNode);

    // НЕ РЕФАКТОРИЛ И НЕ ДОДЕЛАЛ

    if (tree->isLeaf && bestNode == tree){

        Node *nodePrevious = tree->previous;
        Node *nodeNow = tree;
        float x = nodePrevious->item.x();
        float y = nodePrevious->item.y();

        if (((nodePrevious->axis == 0) && (x > point.x()-len) && (point.x()>x)) || ((nodePrevious->axis == 1) && (y > point.y()-len) && (point.y()>y))){
            if (nodePrevious->left == nodeNow && nodePrevious->right != NULL)
                bestNode = KdTree::nearestNeighborSearch(point, nodePrevious->right, depth+1, dimension, lenBest, bestNode);
            if (nodePrevious->right == nodeNow && nodePrevious->left != NULL)
                bestNode = KdTree::nearestNeighborSearch(point, nodePrevious->left, depth+1, dimension, lenBest, bestNode);
        }

        if (((nodePrevious->axis == 0) && (x < point.x()+len) && (point.x()<x ))|| ((nodePrevious->axis == 1) && (y < point.y()+len) && (point.y()<y))){
            if (nodePrevious->left == nodeNow && nodePrevious->right != NULL)
                bestNode = KdTree::nearestNeighborSearch(point, nodePrevious->right, depth+1, dimension, lenBest, bestNode);
            if (nodePrevious->right == nodeNow && nodePrevious->left != NULL)
                bestNode = KdTree::nearestNeighborSearch(point, nodePrevious->left, depth+1, dimension, lenBest, bestNode);
        }

        if (tree->previous->previous != NULL){
            Node *nodePrevPrevious = tree->previous->previous;
            Node *nodePrevious = tree->previous;
            float x = nodePrevPrevious->item.x();
            float y = nodePrevPrevious->item.y();

            if (((nodePrevPrevious->axis == 0) && (x > point.x()-len) && (point.x()>x)) || ((nodePrevPrevious->axis == 1) && (y > point.y()-len) && (point.y()>y))){
                if (nodePrevPrevious->left == nodePrevious && nodePrevPrevious->right != NULL)
                    bestNode = KdTree::nearestNeighborSearch(point, nodePrevPrevious->right, depth+1, dimension, lenBest, bestNode);
                if (nodePrevPrevious->right == nodePrevious && nodePrevPrevious->left != NULL)
                    bestNode = KdTree::nearestNeighborSearch(point, nodePrevPrevious->left, depth+1, dimension, lenBest, bestNode);
            }

            if (((nodePrevPrevious->axis == 0) && (x < point.x()+len) && (point.x()<x ))|| ((nodePrevPrevious->axis == 1) && (y < point.y()+len) && (point.y()<y))){
                if (nodePrevPrevious->left == nodePrevious && nodePrevPrevious->right != NULL)
                    bestNode = KdTree::nearestNeighborSearch(point, nodePrevPrevious->right, depth+1, dimension, lenBest, bestNode);
                if (nodePrevPrevious->right == nodePrevious && nodePrevPrevious->left != NULL)
                    bestNode = KdTree::nearestNeighborSearch(point, nodePrevPrevious->left, depth+1, dimension, lenBest, bestNode);
            }
        }
    }

    if (!(tree->isLeaf) && bestNode == tree){
        Node *node = tree;
        float x = node->item.x();
        float y = node->item.y();

        if (((node->axis == 0) && (x > point.x()-len) && (point.x()>x)) || ((node->axis == 1) && (y > point.y()-len) && (point.y()>y))){
            if (node->right != NULL)
                bestNode = KdTree::nearestNeighborSearch(point, node->right, depth+1, dimension, lenBest, bestNode);
            if (node->left != NULL)
                bestNode = KdTree::nearestNeighborSearch(point, node->left, depth+1, dimension, lenBest, bestNode);
        }

        if (((node->axis == 0) && (x < point.x()+len) && (point.x()<x ))|| ((node->axis == 1) && (y < point.y()+len) && (point.y()<y))){
            if (node->right != NULL)
                bestNode = KdTree::nearestNeighborSearch(point, node->right, depth+1, dimension, lenBest, bestNode);
            if (node->left != NULL)
                bestNode = KdTree::nearestNeighborSearch(point, node->left, depth+1, dimension, lenBest, bestNode);
        }

        if (tree->previous != NULL){
            Node *node = tree->previous;
            float x = node->item.x();
            float y = node->item.y();

            if (((node->axis == 0) && (x > point.x()-len) && (point.x()>x)) || ((node->axis == 1) && (y > point.y()-len) && (point.y()>y))){
                if (node->right != NULL)
                    bestNode = KdTree::nearestNeighborSearch(point, node->right, depth+1, dimension, lenBest, bestNode);
                if (node->left != NULL)
                    bestNode = KdTree::nearestNeighborSearch(point, node->left, depth+1, dimension, lenBest, bestNode);
            }

            if (((node->axis == 0) && (x < point.x()+len) && (point.x()<x ))|| ((node->axis == 1) && (y < point.y()+len) && (point.y()<y))){
                if (node->right != NULL)
                    bestNode = KdTree::nearestNeighborSearch(point, node->right, depth+1, dimension, lenBest, bestNode);
                if (node->left != NULL)
                    bestNode = KdTree::nearestNeighborSearch(point, node->left, depth+1, dimension, lenBest, bestNode);
            }
        }

    }

    return bestNode;
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
