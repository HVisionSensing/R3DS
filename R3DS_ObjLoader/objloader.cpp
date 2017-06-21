#include <objloader.h>

void Obj_loader::loadObjFile(QString fileName)
{
    QFile object;
    object.setFileName(fileName);

    if(isReadFile(object))
        while(!object.atEnd())
            readLine(object.readLine());

    object.close();
}

bool Obj_loader::isReadFile(QFile &file)
{
    if (file.open(QIODevice::ReadOnly))
        return true;
    return false;
}

void Obj_loader::readLine(const QString &line)
{
    if (isVerticesLine(line))
        verticesList.push_back(readVertices(getCorrectLine(line)));
    if (isNormalsLine(line))
        normalsList.push_back(readNormals(getCorrectLine(line)));
    if (isTexturesLine(line))
        texturesList.push_back(readTextures(getCorrectLine(line)));
    if(isPolygonLine(line))
        polygonStart.push_back(readFace(getCorrectLine(line)));
}

bool Obj_loader::isPolygonLine(const QString &line)
{
    if (line.at(0) == 'f' && line.at(1) == ' ')
        return true;
    return false;
}

bool Obj_loader::isVerticesLine(const QString &line)
{
    if (line.at(0) == 'v' && line.at(1) == ' ' && isGoodCoordinates(line))
        return true;
    return false;
}

bool Obj_loader::isNormalsLine(const QString &line)
{
    if (line.at(0) == 'v' && line.at(1) == 'n' && isGoodCoordinates(line))
        return true;
    return false;
}

bool Obj_loader::isTexturesLine(const QString &line)
{
    if (line.at(0) == 'v' && line.at(1) == 't' && isGoodCoordinates(line))
        return true;
    return false;
}

QString Obj_loader::getCorrectLine(const QString &line)
{
    QString new_line;

    for (int i = 0; i < line.size(); ++i){
        if (isblank(line.at(i))){
            new_line.push_back(' ');
        }
        else
            new_line.push_back(line.at(i));
    }
    return new_line;
}

bool Obj_loader::isblank(const QChar &ch)
{
    return (ch == ' ' || ch == '\r' || ch == '\n');
}

bool Obj_loader::isGoodCoordinates(const QString &line)
{
    QStringList list;
    list = line.split(' ', QString::SkipEmptyParts);
    if (list.size() != 4)
        return false;
    return true;
}

QVector3D Obj_loader::readVertices(const QString &line)
{
    QStringList list;
    QVector3D vert;

    list = line.split(' ', QString::SkipEmptyParts);

    //0 1 2 3
    //v 1 2 3
    vert.setX(list.at(1).toFloat());
    vert.setY(list.at(2).toFloat());
    vert.setZ(list.at(3).toFloat());

    return vert;
}

QVector3D Obj_loader::readNormals(const QString &line)
{
    isNormals = true;

    QStringList list;
    QVector3D norm;

    list = line.split(' ', QString::SkipEmptyParts);

    //0 1 2 3
    //vn 1 2 3
    norm.setX(list.at(1).toFloat());
    norm.setY(list.at(2).toFloat());
    norm.setZ(list.at(3).toFloat());

    return norm;
}

QVector3D Obj_loader::readTextures(const QString &line)
{
    isTextures = true;

    QStringList list;
    QVector3D tex;

    list = line.split(' ', QString::SkipEmptyParts);

    //0 1 2 3
    //vt 1 2 3
    tex.setX(list.at(1).toFloat());
    tex.setY(list.at(2).toFloat());
    tex.setZ(list.at(3).toFloat());

    return tex;
}


int Obj_loader::readFace(const QString &line)
{
    QStringList list;
    if (polygonStart.size() == 0)
        polygonStart.push_front(0);

    list = line.split(' ', QString::SkipEmptyParts);

    if (isNormals && isTextures){
        QStringList value;
        for (int i = 1; i < list.size(); i++){
            value = list.at(i).split('/', QString::SkipEmptyParts);

            indexVertices.push_back(value.at(0).toInt()-1);
            indexTextures.push_back(value.at(1).toInt()-1);
            indexNormals.push_back(value.at(2).toInt()-1);
        }
    }

    if (!isNormals && isTextures){
        QStringList value;
        for (int i = 1; i < list.size(); i++){
            value = list.at(i).split('/', QString::SkipEmptyParts);

            indexVertices.push_back(value.at(0).toInt()-1);
            indexTextures.push_back(value.at(1).toInt()-1);
        }
    }

    if (isNormals && !isTextures){
        QStringList value;
        for (int i = 1; i < list.size(); i++){
            value = list.at(i).split('/', QString::SkipEmptyParts);

            indexVertices.push_back(value.at(0).toInt()-1);
            indexNormals.push_back(value.at(1).toInt()-1);
        }
    }

    if (!isNormals && !isTextures){
        QStringList value;
        for (int i = 1; i < list.size(); i++){
            value = list.at(i).split('/', QString::SkipEmptyParts);

            indexVertices.push_back(value.at(0).toInt()-1);
        }
    }
    return polygonStart.at(polygonStart.size()-1) + list.size()-1;
}

void Obj_loader::pointShow(const QVector<QVector3D> &list)
{
    for (int i = 0; i < list.size(); i++){
        qDebug() << list.at(i).x() << list.at(i).y() << list.at(i).z();
    }
    qDebug() << "size: " << list.size();
}

int Obj_loader::getSizePolygon(QVector<int> &polygonStart, int indexPolygon)
{
    if ((indexPolygon+1) < polygonStart.size()-1)
        return polygonStart.at(indexPolygon+1)-polygonStart.at(indexPolygon);
    return polygonStart.at(polygonStart.size()-1)-polygonStart.at(indexPolygon);
}

void Obj_loader::polygonShow(QVector<int> &polygonStart)
{
    for (int i = 0; i < polygonStart.size()-1; i++){
        QString str = "f ";
        if (isNormals && isTextures)
            for (int j = 0; j < getSizePolygon(polygonStart, i); j++){
                str += QString::number(indexVertices.at(polygonStart.at(i)+j)) + "/" + QString::number(indexTextures.at(polygonStart.at(i)+j)) + "/" + QString::number(indexNormals.at(polygonStart.at(i)+j)) + " ";
            }
        if (!isNormals && isTextures)
            for(int j = 0; j < getSizePolygon(polygonStart, i); j++){
                str += QString::number(indexVertices.at(polygonStart.at(i)+j)) + "/" + QString::number(indexTextures.at(polygonStart.at(i)+j)) + " ";
            }
        if (isNormals && !isTextures)
            for (int j = 0; j < getSizePolygon(polygonStart, i); j++){
                str += QString::number(indexVertices.at(polygonStart.at(i)+j)) + "/" + QString::number(indexNormals.at(polygonStart.at(i)+j)) + " ";
            }
        if (!isNormals && !isTextures)
            for (int j = 0; j < getSizePolygon(polygonStart, i); j++){
                str += QString::number(indexVertices.at(polygonStart.at(i)+j)) + " ";
            }
        qDebug() << str;
    }
    qDebug() << "Size: " << polygonStart.size();
}

