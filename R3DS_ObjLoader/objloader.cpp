#include <objloader.h>

void ObjLoader::loadObjFile(QString file)
{   
    QRegExp isPath("^(.+):(\\.*)*(\\.*).(.*)$");

    if (!isPath.exactMatch(file)){
        QFile object("Obj.txt");
        objStringToFile(file, object);

        loadObjFile(object);
    }
    else {
        QFile object;
        object.setFileName(file);

        loadObjFile(object);
    }
}



void ObjLoader::objStringToFile(const QString &fileString, QFile &file)
{
    QStringList list = fileString.split("\n", QString::SkipEmptyParts);


    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        for(int i =0; i < list.size(); i++){
        stream << list.at(i) << endl;
        }
    }
    file.close();
}



void ObjLoader::loadObjFile(QFile &object)
{
    if(isReadFile(object))
        while(!object.atEnd()){
            readLine(object.readLine());
        }

    object.close();
}



bool ObjLoader::isReadFile(QFile &file)
{
    if (file.open(QIODevice::ReadOnly))
        return true;
    qDebug() << "Error read file";
    return false;
}



void ObjLoader::readLine(const QString &loadLine)
{
    QString line = getCorrectLine(loadLine);

    if (isVerticesLine(line))
        verticesList.push_back(readPoint3D(line));

    if (isNormalsLine(line))
        normalsList.push_back(readPoint3D(line));

    if (isTexturesLine(line))
        texturesList.push_back(readPoint3D(line));

    if(isPolygonLine(line)){
        pushPolygon(line, polygonStart, indexVertices, indexNormals, indexTextures, normalsList, texturesList);
        }
}



void ObjLoader::pushPolygon(const QString &line, QVector<int> &polygonStart, QVector<int> &indexVertices, QVector<int> &indexNormals, QVector<int> &indexTextures, QVector<QVector3D> &normalsList, QVector<QVector3D> &texturesList)
{
    QStringList listWithSlash;
    QStringList listWithoutSlash;
    listWithSlash = line.split(' ', QString::SkipEmptyParts);

    polygonStart.push_back(getStartPolygon(polygonStart, line));

    for (int i = 1; i < listWithSlash.size(); i++){
        listWithoutSlash = listWithSlash.at(i).split('/', QString::SkipEmptyParts);
        indexVertices.push_back(getVerticesIndex(listWithoutSlash));
        indexNormals.push_back(getNormalsIndex(listWithoutSlash, normalsList, texturesList));
        indexTextures.push_back(getTexturesIndex(listWithoutSlash, texturesList));
    }
}



int ObjLoader::getNormalsIndex(const QStringList &value, const QVector<QVector3D> &normalsList, const QVector<QVector3D> &texturesList)
{
    if (normalsList.size() && texturesList.size())
        return value.at(2).toInt()-1;
    if (normalsList.size() && !texturesList.size())
        return value.at(1).toInt()-1;
    return -1;
}



int ObjLoader::getTexturesIndex(const QStringList &value, const QVector<QVector3D> &texturesList)
{
    if (texturesList.size())
        return value.at(1).toInt()-1;
    return -1;
}



int ObjLoader::getVerticesIndex(const QStringList &value)
{
    return value.at(0).toInt()-1;
}



QString ObjLoader::getCorrectLine(const QString &line)
{
    QString new_line;

    for (int i = 0; i < line.size(); ++i){
        if ((line.at(i) == ' ' || line.at(i) == '\r' || line.at(i) == '\n')){
            new_line.push_back(' ');
        }
        else
            new_line.push_back(line.at(i));
    }
    return new_line;
}



bool ObjLoader::isVerticesLine(const QString &line)
{
    if (line.size() != 0 && line.at(0) == 'v' && line.at(1) == ' ' && isGoodCoordinates(line))
        return true;
    return false;
}



bool ObjLoader::isNormalsLine(const QString &line)
{
    if (line.size() != 0 && line.at(0) == 'v' && line.at(1) == 'n' && isGoodCoordinates(line))
        return true;
    return false;
}



bool ObjLoader::isTexturesLine(const QString &line)
{
    if (line.size() != 0 && line.at(0) == 'v' && line.at(1) == 't' && isGoodCoordinates(line))
        return true;
    return false;
}



bool ObjLoader::isPolygonLine(const QString &line)
{
    if (isGoodPolygon(line))
        return true;
    return false;
}



bool ObjLoader::isGoodPolygon(const QString &line)
{
    if(!(line.size() !=0 && line.at(0) == 'f' && line.at(1) == ' '))
        return false;

    QStringList listWithSlash;
    listWithSlash = line.split(' ', QString::SkipEmptyParts);

    QStringList listWithoutSlash;
    bool error;

    for (int i = 1; i < listWithSlash.size(); i++){
        listWithoutSlash = listWithSlash.at(i).split('/', QString::SkipEmptyParts);

        for(int i = 0; i < listWithoutSlash.size(); i++){
            listWithoutSlash.at(i).toFloat(&error);
            if(!error)
                return false;
        }
    }

    return true;
}



bool ObjLoader::isGoodCoordinates(const QString &line)
{
    QStringList list;
    list = line.split(' ', QString::SkipEmptyParts);
    bool error;

    for(int i = 1; i < list.size(); i++){
        list.at(i).toFloat(&error);
        if(!error || (list.size() != 4))
            return false;
    }
    return true;
}



QVector3D ObjLoader::readPoint3D(const QString &line)
{
    QStringList list;
    QVector3D vert;

    list = line.split(' ', QString::SkipEmptyParts);

    /*
     0 1 2 3
     v 1 2 3
    vn 1 2 3
    vt 1 2 3
    */

    vert.setX(list.at(1).toFloat());
    vert.setY(list.at(2).toFloat());
    vert.setZ(list.at(3).toFloat());

    return vert;
}



int ObjLoader::getSizePolygon(const QVector<int> &polygonStart, const int indexPolygon)
{
    if ((indexPolygon+1) < polygonStart.size()-1)
        return polygonStart.at(indexPolygon+1)-polygonStart.at(indexPolygon);
    return polygonStart.at(polygonStart.size()-1)-polygonStart.at(indexPolygon);
}



int ObjLoader::getStartPolygon(const QVector<int> &polygonStart, const QString &line)
{
    QStringList listWithSlash;
    listWithSlash = line.split(' ', QString::SkipEmptyParts);

    if (polygonStart.size() == 0)
        return polygonStart.size();
    return (polygonStart.at(polygonStart.size()-1) + listWithSlash.size()-1);
}



void ObjLoader::pointShow(const QVector<QVector3D> &list)
{
    for (int i = 0; i < list.size(); i++){
        qDebug() << list.at(i).x() << list.at(i).y() << list.at(i).z();
    }
    qDebug() << "size: " << list.size();
}



void ObjLoader::polygonShow()
{
    for (int i = 0; i < polygonStart.size()-1; i++){
        QString str = "f ";
        if (normalsList.size() && texturesList.size())
            for (int j = 0; j < getSizePolygon(polygonStart, i); j++){
                str += QString::number(indexVertices.at(polygonStart.at(i)+j)) + "/" + QString::number(indexTextures.at(polygonStart.at(i)+j)) + "/" + QString::number(indexNormals.at(polygonStart.at(i)+j)) + " ";
            }
        if (!normalsList.size() && texturesList.size())
            for(int j = 0; j < getSizePolygon(polygonStart, i); j++){
                str += QString::number(indexVertices.at(polygonStart.at(i)+j)) + "/" + QString::number(indexTextures.at(polygonStart.at(i)+j)) + " ";
            }
        if (normalsList.size() && !texturesList.size())
            for (int j = 0; j < getSizePolygon(polygonStart, i); j++){
                str += QString::number(indexVertices.at(polygonStart.at(i)+j)) + "/" + QString::number(indexNormals.at(polygonStart.at(i)+j)) + " ";
            }
        if (!normalsList.size() && !texturesList.size())
            for (int j = 0; j < getSizePolygon(polygonStart, i); j++){
                str += QString::number(indexVertices.at(polygonStart.at(i)+j)) + " ";
            }
        qDebug() << str;
    }
    qDebug() << "Size: " << polygonStart.size();
}

