#ifndef BODYPARTS_H
#define BODYPARTS_H

#include "QPointF"
#include "QVector"

struct BodyParts
{
    QVector<QPointF> leftEyeCorner;
    QVector<QPointF> leftEyeUp;
    QVector<QPointF> leftEyeDown;

    QVector<QPointF> rightEyeCorner;
    QVector<QPointF> rightEyeUp;
    QVector<QPointF> rightEyeDown;

    QVector<QPointF> mouthCorner;
    QVector<QPointF> mouthUp;
    QVector<QPointF> mouthDown;
};

#endif // BODYPARTS_H
