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
    static Obj_loader& Instance()
    {
        static Obj_loader objLoader;
        return objLoader;
    }

    bool isReadFile(QFile &file);
    bool isblank(const QChar &ch);
    void loadObjFile(QString fileName);
    void readLine(const QString &line);
    void listShow(const QVector<QVector3D> &list);
    QString getCorrectLine(const QString &line);
    QVector3D readVertices(const QString &line);
    QVector3D readNormals(const QString &line);
    QVector3D readTextures(const QString &line);

public:
    QVector<QVector3D> verticesList;
    QVector<QVector3D> normalsList;
    QVector<QVector3D> texturesList;

private:
    Obj_loader() {}
    ~Obj_loader() {}

    // необходимо также запретить копирование
    Obj_loader(Obj_loader const&) = delete; // реализация не нужна
    Obj_loader& operator = (Obj_loader const&) = delete; // и тут

private:
    bool isNormals = false;
    bool isTextures = false;
};

#endif // OBJLOADER_H
