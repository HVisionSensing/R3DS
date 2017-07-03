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


    if (file.open(QIODevice::ReadOnly))
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



void ObjLoader::saveFile(QString fileWay, QVector<QVector3D> &verticesList, QVector<QVector3D> &normalsList, QVector<QVector3D> &texturesList,
                         QVector<int> &indexVertices, QVector<int> &indexNormals, QVector<int> &indexTextures)
{
    QFile fileSaved(fileWay);
    if (fileSaved.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream WriteStream(&fileSaved);

        if (verticesList.size()){
            for (int i = 0; i < verticesList.size(); i++){
                WriteStream << "v " << verticesList.at(i).x() << ' ' << verticesList.at(i).y() << ' ' << verticesList.at(i).z() << "\n";
            }

            WriteStream << "# " << verticesList.size() << " vertices" << "\n";
            WriteStream << "\n";
        }

        if (normalsList.size()){
            for (int i = 0; i < normalsList.size(); i++){
                WriteStream << "vn " << normalsList.at(i).x() << ' ' << normalsList.at(i).y() << ' ' << normalsList.at(i).z() << "\n";
            }

            WriteStream << "# " << normalsList.size() << " vertex normals" << "\n";
            WriteStream << "\n";
        }

        if (texturesList.size()){
            for (int i = 0; i < texturesList.size(); i++){
                WriteStream << "vt " << texturesList.at(i).x() << ' ' << texturesList.at(i).y() << ' ' << texturesList.at(i).z() << "\n";
            }

            WriteStream << "# " << texturesList.size() << " vertex normals" << "\n";
            WriteStream << "\n";
        }

        if (indexVertices.size() && indexTextures.size() && indexNormals.size()){
            for (int i = 0; i < indexVertices.size(); i+=3){
                WriteStream << "f " << indexVertices.at(i) << '/' << indexTextures.at(i) << '/' << indexNormals.at(i)
                            << ' '
                            << indexVertices.at(i+1) << '/' << indexTextures.at(i+1) << '/' << indexNormals.at(i+1)
                            << ' '
                            << indexVertices.at(i+2) << '/' << indexTextures.at(i+2) << '/' << indexNormals.at(i+2)
                            << "\n";
            }

            WriteStream << "# " << indexVertices.size()/3 << " polygons" << "\n";
            WriteStream << "\n";
        }

        if (indexVertices.size() && indexTextures.size() && !indexNormals.size()){
            for (int i = 0; i < indexVertices.size(); i+=3){
                WriteStream << "f " << indexVertices.at(i) << '/' << indexTextures.at(i)
                            << ' '
                            << indexVertices.at(i+1) << '/' << indexTextures.at(i+1)
                            << ' '
                            << indexVertices.at(i+2) << '/' << indexTextures.at(i+2)
                            << "\n";
            }

            WriteStream << "# " << indexVertices.size()/3 << " polygons" << "\n";
            WriteStream << "\n";
        }

        if (indexVertices.size() && !indexTextures.size() && indexNormals.size()){
            for (int i = 0; i < indexVertices.size(); i+=3){
                WriteStream << "f " << indexVertices.at(i) << '/' << indexNormals.at(i)
                            << ' '
                            << indexVertices.at(i+1) << '/' << indexNormals.at(i+1)
                            << ' '
                            << indexVertices.at(i+2) << '/' << indexNormals.at(i+2)
                            << "\n";
            }

            WriteStream << "# " << indexVertices.size()/3 << " polygons" << "\n";
            WriteStream << "\n";
        }

        if (indexVertices.size() && !indexTextures.size() && !indexNormals.size()){
            for (int i = 0; i < indexVertices.size(); i+=3){
                WriteStream << "f " << indexVertices.at(i)
                            << ' '
                            << indexVertices.at(i+1)
                            << ' '
                            << indexVertices.at(i+2)
                            << "\n";
            }

            WriteStream << "# " << indexVertices.size()/3 << " polygons" << "\n";
            WriteStream << "\n";
        }

        fileSaved.close();
    }
}



void ObjLoader::pushPolygon(const QString &line, QVector<int> &polygonStart, QVector<int> &indexVertices, QVector<int> &indexNormals, QVector<int> &indexTextures, QVector<QVector3D> &normalsList, QVector<QVector3D> &texturesList)
{
    QStringList listWithSlash;
    QStringList listWithoutSlash;
    listWithSlash = line.split(' ', QString::SkipEmptyParts);

    if (polygonStart.size() == 0){
       polygonStart.push_back(polygonStart.size());
       polygonStart.push_back(listWithSlash.size()-1);
    }
    else{
        polygonStart.push_back(polygonStart.at(polygonStart.size()-1) + listWithSlash.size()-1);
    }
    for (int i = 1; i < listWithSlash.size(); i++){
        listWithoutSlash = listWithSlash.at(i).split('/', QString::SkipEmptyParts);
         indexVertices.push_back(getVerticesIndex(listWithoutSlash));
         if (normalsList.size())
             indexNormals.push_back(getNormalsIndex(listWithoutSlash, normalsList, texturesList));
         if (texturesList.size())
             indexTextures.push_back(getTexturesIndex(listWithoutSlash, texturesList));
    }
}



int ObjLoader::getNormalsIndex(const QStringList &value, const QVector<QVector3D> &normalsList, const QVector<QVector3D> &texturesList)
{
    if (normalsList.size() && texturesList.size())
        return value.at(2).toInt();
    if (normalsList.size() && !texturesList.size())
        return value.at(1).toInt();
    return -1;
}



int ObjLoader::getTexturesIndex(const QStringList &value, const QVector<QVector3D> &texturesList)
{
    if (texturesList.size())
        return value.at(1).toInt();
    return -1;
}



int ObjLoader::getVerticesIndex(const QStringList &value)
{
    return value.at(0).toInt();
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



QVector<int> ObjLoader::triangulation(QVector<int> &index, QVector<int> &polygonStart)
{
    QVector<int> indexNew;

    for (int i = 0; i < polygonStart.size()-1; i++){

        if (getSizePolygon(polygonStart, i) > 3){

            QVector<int> support;

            for (int l = 0; l < getSizePolygon(polygonStart, i); l++){
                support.push_back(index.at(polygonStart.at(i)+l));
            }

            while(support.size() > 2){
                for (int j = 0; j < 3; j++){
                    indexNew.push_back(support.at(j));
                }
                support.remove(1);
            }
        }
        else {
            for (int j = polygonStart.at(i); j < polygonStart.at(i) + 3; j++){
            indexNew.push_back(index.at(j));
            }
        }
    }

    return indexNew;
}
