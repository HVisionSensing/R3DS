#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QMainWindow>
#include <QFileDialog>
#include <QVector3D>
#include <QVector>

struct Polygon{
    QVector<int> vertex;
    QVector<int> vertexTexture;
    QVector<int> normals;
    Polygon(QVector<int> normals, QVector<int> vertex, QVector<int> vertexTexture){
        this->normals = normals;
        this->vertex = vertex;
        this->vertexTexture = vertexTexture;
    }
};

namespace Ui {
class ObjLoader;
}

class ObjLoader : public QMainWindow
{
    Q_OBJECT

public:
    explicit ObjLoader(QWidget *parent = 0);
    ~ObjLoader();

private slots:
    void on_actionopen_triggered();
    void ObjReader();
    void LineReader(char *line);
    void OutInConsole(QVector<QVector3D*> out);
    void OutPolygonInConsole(QVector<Polygon*> outF);

private:
    Ui::ObjLoader *ui;

    QString objWay;
    QFile objFile;
    QVector<QVector3D*> vertex;
    QVector<QVector3D*> vertexTexture;
    QVector<QVector3D*> normals;
    QVector<Polygon*> faces;

};

#endif // OBJLOADER_H
