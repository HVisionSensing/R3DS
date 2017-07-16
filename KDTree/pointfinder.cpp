#include "pointfinder.h"

int PointFinderNaive::findNearestPoint(QVector3D point) const
{
    QVector3D best = mesh.operator [](0);
    int index = -1;
    for (int indexPoints = 0; indexPoints < mesh.size(); indexPoints++)
        if ((mesh.at(indexPoints)-point).lengthSquared() < (best-point).lengthSquared()){
            best = mesh.operator [](indexPoints);
            index = indexPoints;
        }

    return index;
}



PointFinderNaive::PointFinderNaive(QVector<QVector3D> mesh)
    :mesh(mesh)
{

}



int PointFinderKDTree::findNearestPoint(QVector3D point) const
{
    Node *best = NULL;
    tree->nearestNeighborSearch(point, best);
    return best->itemIndex;
}



PointFinderKDTree::PointFinderKDTree(Node *tree)
    : tree(tree)
{

}
