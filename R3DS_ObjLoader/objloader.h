#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QString>
#include <QFile>
#include <QDebug>
#include <QVector>
#include <QVector3D>
#include <QStringList>

class ObjLoader
{
public:
    void polygonShow();

public:
    void loadObjFile(QString fileName);
    void loadObjFile(QFile &object);
    void readLine(const QString &line);

public:

    bool isNormals() const;
    bool isTextures() const;
    int getNormalsIndex(const QStringList &value) const;
    int getTexturesIndex(const QStringList &value) const;
    int getVerticesIndex(const QStringList &value) const;

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
    static int getSizePolygon(QVector<int> &polygonStart, int indexPolygon);
    static int getStartPolygon(QVector<int> &polygonStart, QString &line);

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
