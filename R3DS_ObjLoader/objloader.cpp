#include "objloader.h"
#include "ui_objloader.h"
#include <QDebug>
#include <algorithm>

ObjLoader::ObjLoader(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ObjLoader)
{
    ui->setupUi(this);
}

ObjLoader::~ObjLoader()
{
    delete ui;
}

void ObjLoader::on_actionopen_triggered()
{
    objWay = QFileDialog::getOpenFileName(this, "Open Dialog", "", "");
    objFile.setFileName(objWay);
    ObjReader();
    OutInConsole(vertex);
    OutInConsole(vertexTexture);
    OutInConsole(normals);
    OutPolygonInConsole(faces);
}

void ObjLoader::ObjReader()
{
    if (objFile.open(QFile::ReadOnly)) {
        char line[256];
        while (!objFile.atEnd()){
            objFile.readLine(line, sizeof(line));
            LineReader(line);
        }
    }
    objFile.close();
}

void ObjLoader::LineReader(char *line)
{
    if (line[0] == 'v' && line[1] == ' ') {
        float x, y, z;
        sscanf(line, "v %f %f %f", &x, &y, &z);

        vertex.push_back(new QVector3D(x, y, z));
    }
    else if (line[0] == 'v' && line[1] == 'n') {
        float x, y, z;
        sscanf(line, "vn %f %f %f", &x, &y, &z);

        normals.push_back(new QVector3D(x, y, z));
    }
    else if (line[0] == 'v' && line[1] == 't') {
        float x, y, z;
        sscanf(line, "vt %f %f %f", &x, &y, &z);

        vertexTexture.push_back(new QVector3D(x, y, z));
    }
    else if (line[0] == 'f' && line[1] == ' ') {
        QString lineStr(line);
        int countOfVertex;
        countOfVertex = std::count(lineStr.begin(), lineStr.end(),' ') - 1;

        QVector<int> vertex(countOfVertex);
        QVector<int> vertexTexture(countOfVertex);
        QVector<int> normal(countOfVertex);

        if (countOfVertex == 4){
            sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &vertex[0], &vertexTexture[0], &normal[0], &vertex[1], &vertexTexture[1], &normal[1],
                   &vertex[2], &vertexTexture[2], &normal[2], &vertex[3], &vertexTexture[3], &normal[3]);
        }
        if (countOfVertex == 3){
            sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &vertex[0], &vertexTexture[0], &normal[0], &vertex[1], &vertexTexture[1], &normal[1],
                   &vertex[2], &vertexTexture[2], &normal[2]);
        }

        faces.push_back(new Polygon(normal, vertex, vertexTexture));
    }
}

void ObjLoader::OutInConsole(QVector<QVector3D*> out)
{
    foreach (QVector3D* v, out) {
        qDebug() << v->x() << v->y() << v->z();
    }
    qDebug() << out.size();
}

void ObjLoader::OutPolygonInConsole(QVector<Polygon*> outF)
{
    foreach (Polygon* f, outF) {
        if (f->vertex.size() == 3)
            qDebug() << f->vertex.at(0) << "/" << f->vertexTexture.at(0) << "/" << f->normals.at(0) << " "
                     << f->vertex.at(1) << "/" << f->vertexTexture.at(1) << "/" << f->normals.at(1) << " "
                     << f->vertex.at(2) << "/" << f->vertexTexture.at(2) << "/" << f->normals.at(2) << " ";
        if (f->vertex.size() == 4)
            qDebug() << f->vertex.at(0) << "/" << f->vertexTexture.at(0) << "/" << f->normals.at(0) << " "
                     << f->vertex.at(1) << "/" << f->vertexTexture.at(1) << "/" << f->normals.at(1) << " "
                     << f->vertex.at(2) << "/" << f->vertexTexture.at(2) << "/" << f->normals.at(2) << " "
                     << f->vertex.at(3) << "/" << f->vertexTexture.at(3) << "/" << f->normals.at(3) << " ";
    }
    qDebug() << outF.size();
}

