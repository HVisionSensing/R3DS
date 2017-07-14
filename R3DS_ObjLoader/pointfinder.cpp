#include "pointfinder.h"

QVector3D PointFinderNaive::findNearestPoint(QVector3D point) const
{
    QVector3D best = mesh.operator [](0);

    for (int indexPoints = 0; indexPoints < mesh.size(); indexPoints++)
        if ((mesh.at(indexPoints)-point).lengthSquared() < (best-point).lengthSquared())
            best = mesh.operator [](indexPoints);

    return best;
}



PointFinderNaive::PointFinderNaive(QVector<QVector3D> mesh)
    :mesh(mesh)
{

}



QVector3D PointFinderKDTree::findNearestPoint(QVector3D point) const
{
    QVector3D *best = NULL;
    tree->nearestNeighborSearch(point, best);
    return *best;
}



PointFinderKDTree::PointFinderKDTree(Node *tree)
    : tree(tree)
{

}
