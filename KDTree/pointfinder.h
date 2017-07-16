#ifndef POINTFINDER_H
#define POINTFINDER_H

#include <QVector3D>
#include <QVector>
#include "kdtree.h"
#include <QSharedPointer>

class PointFinder
{
public:
    virtual int findNearestPoint(QVector3D point) const = 0;

};



class PointFinderNaive : public PointFinder
{
public:
    QVector<QVector3D> mesh;

    virtual int findNearestPoint(QVector3D point) const override;

    PointFinderNaive(QVector<QVector3D> mesh);

};


class PointFinderKDTree : public PointFinder
{
public:
    Node *tree = NULL;

    virtual int findNearestPoint(QVector3D point) const override;

    PointFinderKDTree(Node *tree);

};

#endif // POINTFINDER_H
