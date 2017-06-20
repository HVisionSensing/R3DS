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
    if(file.open(QIODevice::ReadOnly))
    {
        return true;
    }
    return true;
}

void Obj_loader::readLine(const QString &line)
{
    if(line.at(0) == 'v' && line.at(1) == ' ')
        verticesList.push_back(readVertices(getCorrectLine(line)));
    if(line.at(0) == 'v' && line.at(1) == 'n')
        normalsList.push_back(readVertices(getCorrectLine(line)));
    if(line.at(0) == 'v' && line.at(1) == 't')
        texturesList.push_back(readVertices(getCorrectLine(line)));
}

QString Obj_loader::getCorrectLine(const QString &line)
{
    QString new_line;

    for(int i = 0; i < line.size(); ++i)
    {
        if(isblank(line.at(i)))
        {
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

void Obj_loader::listShow(const QVector<QVector3D> &list)
{
    for (int i = 0; i < list.size(); i++){
        qDebug() << list.at(i).x() << list.at(i).y() << list.at(i).z();
    }
    qDebug() << "size: " << list.size();
}
