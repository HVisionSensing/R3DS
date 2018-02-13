#include "loaderjson.h"

void LoaderJson::saveJson(const QJsonDocument &doc, const QString &fileName)
{
    QFile file(fileName);
    file.open(QFile::WriteOnly);
    file.write(doc.toJson());
}


QJsonDocument LoaderJson::createJson(const Body &body)
{
    QJsonObject obj;
    QJsonArray leftEyeCornerArray = getArray(body.parts[0].corner);
    QJsonArray leftEyeUpArray = getArray(body.parts[0].up);
    QJsonArray leftEyeDownArray = getArray(body.parts[0].down);

    QJsonArray rightEyeCornerArray = getArray(body.parts[1].corner);
    QJsonArray rightEyeUpArray = getArray(body.parts[1].up);
    QJsonArray rightEyeDownArray = getArray(body.parts[1].down);

    QJsonArray mouthEyeCornerArray = getArray(body.parts[2].corner);
    QJsonArray mouthEyeUpArray = getArray(body.parts[2].up);
    QJsonArray mouthEyeDownArray = getArray(body.parts[2].down);

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


Body LoaderJson::getBodyParts(const QJsonDocument &doc)
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

    Body body;
    body.parts[0].corner = LoaderJson::getPoints(leftEyeCornerArray);
    body.parts[0].up = LoaderJson::getPoints(leftEyeUpArray);
    body.parts[0].down = LoaderJson::getPoints(leftEyeDownArray);

    body.parts[1].corner = LoaderJson::getPoints(rightEyeCornerArray);
    body.parts[1].up = LoaderJson::getPoints(rightEyeUpArray);
    body.parts[1].down = LoaderJson::getPoints(rightEyeDownArray);

    body.parts[2].corner = LoaderJson::getPoints(mouthEyeCornerArray);
    body.parts[2].up = LoaderJson::getPoints(mouthEyeUpArray);
    body.parts[2].down = LoaderJson::getPoints(mouthEyeDownArray);

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
