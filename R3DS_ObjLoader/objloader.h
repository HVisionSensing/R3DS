#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QString>
#include <QFile>
#include <QDebug>
#include <QVector>
#include <QVector3D>
#include <QStringList>
#include <QTextStream>
#include <QRegExp>

class ObjLoader
{
public:
    void polygonShow();

public:
    void loadObjFile(QString file);
    void loadObjFile(QFile &object);
    void readLine(const QString &line);

public:
    static bool isReadFile(QFile &file);
    static bool isPolygonLine(const QString &line);
    static bool isVerticesLine(const QString &line);
    static bool isNormalsLine(const QString &line);
    static bool isTexturesLine(const QString &line);
    static bool isGoodCoordinates(const QString &line);
    static bool isGoodPolygon(const QString &line);
    static QVector3D readPoint3D(const QString &line);
    static QString getCorrectLine(const QString &line);
    static void pointShow(const QVector<QVector3D> &list);
    static void objStringToFile(const QString &fileString, QFile &file);
    static int getSizePolygon(const QVector<int> &polygonStart, const int indexPolygon);
    static int getStartPolygon(const QVector<int> &polygonStart, const QString &line);
    static void pushPolygon(const QString &line, QVector<int> &polygonStart, QVector<int> &indexVertices, QVector<int> &indexNormals, QVector<int> &indexTextures,
                            QVector<QVector3D> &normalsList, QVector<QVector3D> &texturesList);
    static int getVerticesIndex(const QStringList &value);
    static int getNormalsIndex(const QStringList &value, const QVector<QVector3D> &normalsList, const QVector<QVector3D> &texturesList);
    static int getTexturesIndex(const QStringList &value, const QVector<QVector3D> &texturesList);

public:
    QVector<QVector3D> verticesList;
    QVector<QVector3D> normalsList;
    QVector<QVector3D> texturesList;

    QVector<int> polygonStart;
    QVector<int> indexVertices;
    QVector<int> indexNormals;
    QVector<int> indexTextures;
};

#endif // OBJLOADER_H
