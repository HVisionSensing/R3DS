#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QString>
#include <QFile>
#include <QDebug>
#include <QVector>
#include <QVector3D>
#include <QVector2D>
#include <QStringList>
#include <QTextStream>
#include <QRegExp>
#include <QDebug>
#include <QTextStream>
#include <QMessageBox>

class ObjLoader
{
public:
    bool loadFile(QTextStream &fileNameStream);
    bool loadObjFile(const QString &fileName);
    bool loadStringFile(const QString &fileName);
    bool readLine(const QString &line);
    void saveFile(QString fileWay, QVector<QVector3D> &verticesList, QVector<QVector3D> &normalsList, QVector<QVector2D> &texturesList,
                  QVector<int> &indexVertices, QVector<int> &indexNormals, QVector<int> &indexTextures);

public:
    static bool isReadFile(QFile &file);
    static bool isPolygonLine(const QString &line);
    static bool isGoodPolygon(const QString &line);
    static bool isVerticesLine(const QString &line);
    static bool isNormalsLine(const QString &line);
    static bool isTexturesLine(const QString &line);
    static bool isGoodCoordinates3D(const QString &line);
    static bool isGoodCoordinates2D(const QString &line);
    static QVector3D readPoint3D(const QString &line);
    static QVector2D readPoint2D(const QString &line);
    static QString getCorrectLine(const QString &line);
    static int getSizePolygon(const QVector<int> &polygonStart, const int indexPolygon);
    static bool pushPolygon(const QString &line, QVector<int> &polygonStart, QVector<int> &indexVertices, QVector<int> &indexNormals, QVector<int> &indexTextures,
                            QVector<QVector3D> &normalsList, QVector<QVector2D> &texturesList, QVector<QVector3D> &verticesList);
    static int getVerticesIndex(const QStringList &value);
    static int getNormalsIndex(const QStringList &value, const QVector<QVector2D> &texturesList);
    static int getTexturesIndex(const QStringList &value);
    static QVector<int> triangulation(QVector<int> &index, QVector<int> &polygonStart);

public:
    int getSizeVertices();
    int getSizeNormals();
    int getSizeTextures();


public:
    QVector<QVector3D> verticesList;
    QVector<QVector3D> normalsList;
    QVector<QVector2D> texturesList;

    QVector<int> polygonStart;
    QVector<int> indexVertices;
    QVector<int> indexNormals;
    QVector<int> indexTextures;

};

#endif // OBJLOADER_H
