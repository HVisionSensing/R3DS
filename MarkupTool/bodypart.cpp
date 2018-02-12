#include "bodypart.h"


BodyPart::BodyPart(QString nameBlock)
{
    //this->nameBlock = nameBlock;
}

void BodyPart::addPoint(Landmark *point, int indInsert)
{
    points.insert(indInsert, point);

    if (points.size() == 4){
        points[2]->end = true;
        points[0]->start = true;
    }
    for (int indPoint = indInsert; indPoint < points.size();indPoint++)
        points[indPoint]->indPoint = indPoint;
}

void BodyPart::loadPoint(Landmark *point, int indInsert)
{
    points.insert(indInsert, point);

    for (int indPoint = indInsert; indPoint < points.size();indPoint++)
        points[indPoint]->indPoint = indPoint;
}
