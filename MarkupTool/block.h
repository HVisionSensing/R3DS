#ifndef BLOCK_H
#define BLOCK_H

#include "landmark.h"
#include "QGraphicsView"

class BodyPart
{
public:
    BodyPart(QString nameBlock);

    QGraphicsPathItem *path = nullptr;
    QList<Landmark*> points;

    void addPoint (Landmark* point, int indInsert);
    void loadPoint (Landmark* point, int indInsert);

    QString nameBlock;

};

#endif // BLOCK_H
