#ifndef POINTFINDER_H
#define POINTFINDER_H

#include <QVector3D>
#include <QVector>
#include <QSharedPointer>

#include "kdtree.h"

class PointFinder
{
public:
    virtual int findNearestPoint(QVector3D point) const = 0;

};



class PointFinderKDTree : public PointFinder
{
public:
    Node *tree = NULL;

    virtual int findNearestPoint(QVector3D point) const override;

    PointFinderKDTree(Node *tree);

};

#endif // POINTFINDER_H
