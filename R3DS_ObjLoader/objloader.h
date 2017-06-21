#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QString>
#include <QFile>
#include <QDebug>
#include <QVector>
#include <QVector3D>
#include <QStringList>

class Obj_loader
{
public:
    static Obj_loader& Instance(){
        static Obj_loader objLoader;
        return objLoader;
    }

    bool isReadFile(QFile &file);
    bool isblank(const QChar &ch);
    bool isVerticesLine(const QString &line);
    bool isNormalsLine(const QString &line);
    bool isTexturesLine(const QString &line);
    bool isGoodCoordinates(const QString &line);
    bool isPolygonLine(const QString &line);
    void loadObjFile(QString fileName);
    void readLine(const QString &line);
    void pointShow(const QVector<QVector3D> &list);
    void polygonShow(QVector<int> &polygonStart);
    int readFace(const QString &line);
    int getSizePolygon(QVector<int> &polygonStart, int indexPolygon);
    QString getCorrectLine(const QString &line);
    QVector3D readVertices(const QString &line);
    QVector3D readNormals(const QString &line);
    QVector3D readTextures(const QString &line);

public:
    QVector<QVector3D> verticesList;
    QVector<QVector3D> normalsList;
    QVector<QVector3D> texturesList;

    QVector<int> polygonStart;
    QVector<int> indexVertices;
    QVector<int> indexNormals;
    QVector<int> indexTextures;

private:
    Obj_loader() {}
    ~Obj_loader() {}

    // необходимо также запретить копирование
    Obj_loader(Obj_loader const&) = delete; // реализация не нужна
    Obj_loader& operator = (Obj_loader const&) = delete; // и тут

private:
    bool isNormals;
    bool isTextures;
};

#endif // OBJLOADER_H
