#include "loaderjson.h"

void LoaderJson::saveJson(const QJsonDocument &doc, const QString &fileName)
{
    QFile file(fileName);
    file.open(QFile::WriteOnly);
    file.write(doc.toJson());
}


QJsonDocument LoaderJson::createJson(const BodyParts &body)
{
    QJsonObject obj;
    QJsonArray leftEyeCornerArray = getArray(body.leftEyeCorner);
    QJsonArray leftEyeUpArray = getArray(body.leftEyeUp);
    QJsonArray leftEyeDownArray = getArray(body.leftEyeDown);

    QJsonArray rightEyeCornerArray = getArray(body.rightEyeCorner);
    QJsonArray rightEyeUpArray = getArray(body.rightEyeUp);
    QJsonArray rightEyeDownArray = getArray(body.rightEyeDown);

    QJsonArray mouthEyeCornerArray = getArray(body.mouthCorner);
    QJsonArray mouthEyeUpArray = getArray(body.mouthUp);
    QJsonArray mouthEyeDownArray = getArray(body.mouthDown);

    obj.insert("LeftEyeCorners", leftEyeCornerArray);
    obj.insert("LeftEyeUp", leftEyeUpArray);
    obj.insert("LeftEyeDown", leftEyeDownArray);

    obj.insert("RightEyeCorners", rightEyeCornerArray);
    obj.insert("RightEyeUp", rightEyeUpArray);
    obj.insert("RightEyeDown", rightEyeDownArray);

    obj.insert("MouthCorners", mouthEyeCornerArray);
    obj.insert("MouthUp", mouthEyeUpArray);
    obj.insert("MouthDown", mouthEyeDownArray);

    QJsonDocument doc(obj);
    return doc;
}



QJsonDocument LoaderJson::loadJson(const QString &fileName)
{
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    return QJsonDocument().fromJson(file.readAll());
}



QVector<QPointF> LoaderJson::getPoints(const QJsonArray &array)
{
    QVector<QPointF> points;
    for (int indPoint = 0; indPoint < array.size();indPoint = indPoint+2){
        float x = array[indPoint].toDouble();
        float y = array[indPoint+1].toDouble();
        points.push_back(QPointF(x, y));
    }
    return points;
}


BodyParts LoaderJson::getBodyParts(const QJsonDocument &doc)
{
    QJsonObject obj = doc.object();

    QJsonArray leftEyeCornerArray = obj.value("LeftEyeCorners").toArray();
    QJsonArray leftEyeUpArray = obj.value("LeftEyeUp").toArray();
    QJsonArray leftEyeDownArray = obj.value("LeftEyeDown").toArray();

    QJsonArray rightEyeCornerArray = obj.value("RightEyeCorners").toArray();
    QJsonArray rightEyeUpArray = obj.value("RightEyeUp").toArray();
    QJsonArray rightEyeDownArray = obj.value("RightEyeDown").toArray();

    QJsonArray mouthEyeCornerArray = obj.value("MouthCorners").toArray();
    QJsonArray mouthEyeUpArray = obj.value("MouthUp").toArray();
    QJsonArray mouthEyeDownArray = obj.value("MouthDown").toArray();

    BodyParts body;
    body.leftEyeCorner = LoaderJson::getPoints(leftEyeCornerArray);
    body.leftEyeUp = LoaderJson::getPoints(leftEyeUpArray);
    body.leftEyeDown = LoaderJson::getPoints(leftEyeDownArray);

    body.rightEyeCorner = LoaderJson::getPoints(rightEyeCornerArray);
    body.rightEyeUp = LoaderJson::getPoints(rightEyeUpArray);
    body.rightEyeDown = LoaderJson::getPoints(rightEyeDownArray);

    body.mouthCorner = LoaderJson::getPoints(mouthEyeCornerArray);
    body.mouthUp = LoaderJson::getPoints(mouthEyeUpArray);
    body.mouthDown = LoaderJson::getPoints(mouthEyeDownArray);

    return body;
}


QJsonArray LoaderJson::getArray(const QVector<QPointF> &points)
{
    QJsonArray array;
    for (int indPoint = 0; indPoint < points.size(); indPoint++){
        array.push_back(points[indPoint].x());
        array.push_back(points[indPoint].y());
    }
    return array;
}
