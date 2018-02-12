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
    QJsonArray leftEyeCornerArray = getArray(body.leftEye.corner);
    QJsonArray leftEyeUpArray = getArray(body.leftEye.up);
    QJsonArray leftEyeDownArray = getArray(body.leftEye.down);

    QJsonArray rightEyeCornerArray = getArray(body.rightEye.corner);
    QJsonArray rightEyeUpArray = getArray(body.rightEye.up);
    QJsonArray rightEyeDownArray = getArray(body.rightEye.down);

    QJsonArray mouthEyeCornerArray = getArray(body.mouth.corner);
    QJsonArray mouthEyeUpArray = getArray(body.mouth.up);
    QJsonArray mouthEyeDownArray = getArray(body.mouth.down);

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
    body.leftEye.corner = LoaderJson::getPoints(leftEyeCornerArray);
    body.leftEye.up = LoaderJson::getPoints(leftEyeUpArray);
    body.leftEye.down = LoaderJson::getPoints(leftEyeDownArray);

    body.rightEye.corner = LoaderJson::getPoints(rightEyeCornerArray);
    body.rightEye.up = LoaderJson::getPoints(rightEyeUpArray);
    body.rightEye.down = LoaderJson::getPoints(rightEyeDownArray);

    body.mouth.corner = LoaderJson::getPoints(mouthEyeCornerArray);
    body.mouth.up = LoaderJson::getPoints(mouthEyeUpArray);
    body.mouth.down = LoaderJson::getPoints(mouthEyeDownArray);

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
