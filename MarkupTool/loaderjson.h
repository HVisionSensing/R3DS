#ifndef LOADERJSON_H
#define LOADERJSON_H

#include "QJsonDocument"
#include "QJsonObject"
#include "QJsonArray"
#include "landmark.h"
#include "QVector"
#include "QPointF"
#include "QFile"

#include "bodyparts.h"

namespace LoaderJson
{
    void saveJson(const QJsonDocument &doc, const QString &fileName);
    QJsonDocument createJson(const BodyParts &body);
    QJsonDocument loadJson(const QString &fileName);
    QVector<QPointF> getPoints(const QJsonArray &array);
    BodyParts getBodyParts(const QJsonDocument &doc);
    QJsonArray getArray(const QVector<QPointF> &points);
}

#endif // LOADERJSON_H
