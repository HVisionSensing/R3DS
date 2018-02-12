#ifndef BODYPARTS_H
#define BODYPARTS_H

#include "QPointF"
#include "QVector"

struct Part
{
    QVector<QPointF> corner;
    QVector<QPointF> up;
    QVector<QPointF> down;

    QString name;
};

struct Body
{
    Part leftEye;
    Part rightEye;
    Part mouth;
};

#endif // BODYPARTS_H
