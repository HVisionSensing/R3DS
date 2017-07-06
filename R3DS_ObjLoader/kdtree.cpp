#include "kdtree.h"

bool KdTree::comparsionVectorsX(QVector3D &points1, QVector3D &points2)
{
    return points1.x() < points2.x();
}



bool KdTree::comparsionVectorsY(QVector3D &points1, QVector3D &points2)
{
    return points1.y() < points2.y();
}



bool KdTree::comparsionVectorsZ(QVector3D &points1, QVector3D &points2)
{
    return points1.z() < points2.z();
}



Node *KdTree::kdTreeBuild(QVector<QVector3D> &points, int depth, int dimension)
{
    int axis = depth % dimension;

    if (dimension == 2 && axis == 0)
            std::sort(points.begin(), points.end(), KdTree::comparsionVectorsY);

    if (dimension == 3){
        if (axis == 2)
            std::sort(points.begin(), points.end(), KdTree::comparsionVectorsY);
        if (axis == 0)
            std::sort(points.begin(), points.end(), KdTree::comparsionVectorsZ);
    }

    if (axis == 1)
        std::sort(points.begin(), points.end(), KdTree::comparsionVectorsX);


    int medianIndex = points.size()/2;

    QVector<QVector3D> pointsUntil;
    QVector<QVector3D> pointsAfter;;

    for (int i = 0; i < medianIndex; i++)
        pointsUntil.push_back(points.at(i));

    for (int i = medianIndex+1; i < points.size(); i++)
        pointsAfter.push_back(points.at(i));


    Node *tree = new Node;
    tree->item = points.operator [](medianIndex);

    if (pointsUntil.size()){
        tree->left = KdTree::kdTreeBuild(pointsUntil, depth+1, dimension);
    }
    else{
        tree->left = NULL;
    }

    if (pointsAfter.size()){
        tree->right = KdTree::kdTreeBuild(pointsAfter, depth+1, dimension);
    }
    else{
        tree->right = NULL;
    }

    return tree;
}



Node *KdTree::NearestNeighborSearch(QVector3D &point, Node *tree, int depth, int dimension)
{
    int axis = depth % dimension;

    Node *best = tree;

    bool toLeft = false;
    bool toRight = (tree->right != NULL);

    if (dimension == 3){
        if (axis == 2)
            if (KdTree::comparsionVectorsY(point, tree->item) && tree->left != NULL)
                toLeft = true;
        if (axis == 0)
            if (KdTree::comparsionVectorsZ(point, tree->item) && tree->left != NULL)
                toLeft = true;
    }

    if (dimension == 2 && axis == 0)
        if (KdTree::comparsionVectorsY(point, tree->item) && tree->left != NULL)
            toLeft = true;

    if (axis == 1)
        if (KdTree::comparsionVectorsX(point, tree->item) && tree->left != NULL)
            toLeft = true;


    if (toLeft)
        return KdTree::NearestNeighborSearch(point, tree->left, depth+1, dimension);
    else if (toRight)
        return KdTree::NearestNeighborSearch(point, tree->right, depth+1, dimension);

    return best;
}
