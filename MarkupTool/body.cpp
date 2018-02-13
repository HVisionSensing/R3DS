#include "body.h"


void Part::addPoint(Landmark *point, int indInsert)
{
    points.insert(indInsert, point);

    if (points.size() == 4){
        points[2]->end = true;
        points[0]->start = true;
    }
    for (int indPoint = indInsert; indPoint < points.size();indPoint++)
        points[indPoint]->indPoint = indPoint;
}


void Part::loadPoint(Landmark *point, int indInsert)
{
    points.insert(indInsert, point);

    for (int indPoint = indInsert; indPoint < points.size();indPoint++)
        points[indPoint]->indPoint = indPoint;
}


Body::Body()
{
    Part leftEye;
    Part rightEye;
    Part mouth;

    parts.push_back(leftEye);
    parts.push_back(rightEye);
    parts.push_back(mouth);
}
