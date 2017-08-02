#include "pointfinder.h"

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
