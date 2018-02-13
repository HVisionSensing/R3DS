#ifndef BODYPARTS_H
#define BODYPARTS_H

#include "QPointF"
#include "QVector"
#include "QGraphicsView"
#include "landmark.h"

struct Part
{
    void addPoint (Landmark* point, int indInsert);
    void loadPoint (Landmark* point, int indInsert);

    QGraphicsPathItem *path = nullptr;
    QList<Landmark*> points;

    QVector<QPointF> corner;
    QVector<QPointF> up;
    QVector<QPointF> down;
};

struct Body
{
    Body();
    QVector<Part> parts;
    int indActived = 0;
};

#endif // BODYPARTS_H
