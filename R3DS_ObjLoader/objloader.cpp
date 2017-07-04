#include <objloader.h>

bool ObjLoader::isReadFile(QFile &file)
{
    if (file.open(QIODevice::ReadOnly))
        return true;
    return false;
}



bool ObjLoader::loadFile(QTextStream &fileNameStream)
{
    QString fileName = fileNameStream.readAll();

    QRegExp isPath("^(.+):(\\.*)*(\\.*).(.*)$");
    bool isCorrespondsPath = isPath.exactMatch(fileName);

    if (loadObjFile(fileName))
        return true;
    if (loadStringFile(fileName) && !isCorrespondsPath)
        return true;

    return false;
}



bool ObjLoader::loadStringFile(const QString &fileName)
{
    QStringList listLine = fileName.split("\n", QString::SkipEmptyParts);
    for (int lineIndex = 0; lineIndex < listLine.size(); lineIndex++)
        if (!readLine(listLine.at(lineIndex)))
            return false;

    return true;
}



bool ObjLoader::loadObjFile(const QString &fileName)
{
    QFile object;
    object.setFileName(fileName);

    if (!isReadFile(object))
        return false;

    while (!object.atEnd())
        if (!readLine(object.readLine()))
            return false;

    return true;
}



bool ObjLoader::readLine(const QString &loadLine)
{
    QString line = getCorrectLine(loadLine);

    if (isVerticesLine(line)){
        if (!isGoodCoordinates3D(line))
            return false;
        verticesList.push_back(readPoint3D(line));
    }

    if (isNormalsLine(line)){
        if (!isGoodCoordinates3D(line))
            return false;
        normalsList.push_back(readPoint3D(line));
    }

    if (isTexturesLine(line)){
        if (!(isGoodCoordinates2D(line) || isGoodCoordinates3D(line)))
            return false;
        texturesList.push_back(readPoint2D(line));
    }

    if(isPolygonLine(line)){
        if(!isGoodPolygon(line))
            return false;
        pushPolygon(line, polygonStart, indexVertices, indexNormals, indexTextures, normalsList, texturesList);
    }

    return true;
}



void ObjLoader::saveFile(QString fileWay, QVector<QVector3D> &verticesList, QVector<QVector3D> &normalsList, QVector<QVector2D> &texturesList,
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
                WriteStream << "vt " << texturesList.at(i).x() << ' ' << texturesList.at(i).y() << "\n";
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
                WriteStream << "f " << indexVertices.at(i) << "//" << indexNormals.at(i)
                            << ' '
                            << indexVertices.at(i+1) << "//" << indexNormals.at(i+1)
                            << ' '
                            << indexVertices.at(i+2) << "//" << indexNormals.at(i+2)
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



void ObjLoader::pushPolygon(const QString &line, QVector<int> &polygonStart, QVector<int> &indexVertices, QVector<int> &indexNormals, QVector<int> &indexTextures, QVector<QVector3D> &normalsList, QVector<QVector2D> &texturesList)
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

    for (int pointIndex = 1; pointIndex < listWithSlash.size(); pointIndex++){
        listWithoutSlash = listWithSlash.at(pointIndex).split('/', QString::SkipEmptyParts);
        indexVertices.push_back(getVerticesIndex(listWithoutSlash));
        if (normalsList.size())
            indexNormals.push_back(getNormalsIndex(listWithoutSlash, texturesList));
        if (texturesList.size())
            indexTextures.push_back(getTexturesIndex(listWithoutSlash));
    }
}



int ObjLoader::getNormalsIndex(const QStringList &value, const QVector<QVector2D> &texturesList)
{
    if (texturesList.size())
        return value.at(2).toInt();
    return value.at(1).toInt();
}



int ObjLoader::getTexturesIndex(const QStringList &value)
{
    return value.at(1).toInt();
}



int ObjLoader::getVerticesIndex(const QStringList &value)
{
    return value.at(0).toInt();
}



QString ObjLoader::getCorrectLine(const QString &line)
{
    QString new_line;

    for (int charInLineIndex = 0; charInLineIndex < line.size(); ++charInLineIndex){
        if ((line.at(charInLineIndex) == ' ' || line.at(charInLineIndex) == '\r' || line.at(charInLineIndex) == '\n')){
            new_line.push_back(' ');
            continue;
        }
        new_line.push_back(line.at(charInLineIndex));
    }
    return new_line;
}



bool ObjLoader::isVerticesLine(const QString &line)
{
    if (line.size() != 0 && line.at(0) == 'v' && line.at(1) == ' ')
        return true;
    return false;
}



bool ObjLoader::isNormalsLine(const QString &line)
{
    if (line.size() != 0 && line.at(0) == 'v' && line.at(1) == 'n' && line.at(2) == ' ')
        return true;
    return false;
}



bool ObjLoader::isTexturesLine(const QString &line)
{
    if (line.size() != 0 && line.at(0) == 'v' && line.at(1) == 't' && line.at(2) == ' ')
        return true;
    return false;
}


bool ObjLoader::isGoodPolygon(const QString &line)
{
    QStringList listWithSlash;
    listWithSlash = line.split(' ', QString::SkipEmptyParts);

    QStringList listWithoutSlash;
    bool error;

    for (int pointIndex = 1; pointIndex < listWithSlash.size(); pointIndex++){
        listWithoutSlash = listWithSlash.at(pointIndex).split('/', QString::SkipEmptyParts);

        for(int sortIndex = 0; sortIndex < listWithoutSlash.size(); sortIndex++){
            listWithoutSlash.at(sortIndex).toFloat(&error);
            if(!error)
                return false;
        }
    }

    return true;
}



bool ObjLoader::isPolygonLine(const QString &line)
{
    if (!(line.size() !=0 && line.at(0) == 'f' && line.at(1) == ' '))
        return false;
    return true;
}



bool ObjLoader::isGoodCoordinates3D(const QString &line)
{
    QStringList list;
    list = line.split(' ', QString::SkipEmptyParts);
    bool error;

    for (int coordinateIndex = 1; coordinateIndex < list.size(); coordinateIndex++){
        list.at(coordinateIndex).toFloat(&error);
        if (!error || (list.size() != 4))
            return false;
    }
    return true;
}



bool ObjLoader::isGoodCoordinates2D(const QString &line)
{
    QStringList list;
    list = line.split(' ', QString::SkipEmptyParts);
    bool error;

    for(int coordinateIndex = 1; coordinateIndex < list.size(); coordinateIndex++){
        list.at(coordinateIndex).toFloat(&error);
        if(!error || (list.size() != 3))
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



QVector2D ObjLoader::readPoint2D(const QString &line)
{
    QStringList list;
    QVector2D vert;

    list = line.split(' ', QString::SkipEmptyParts);

    /*
    vt 1 2
    */

    vert.setX(list.at(1).toFloat());
    vert.setY(list.at(2).toFloat());

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

    for (int polygonInd = 0; polygonInd < polygonStart.size()-1; polygonInd++) {
        bool isTriangle = getSizePolygon(polygonStart, polygonInd) == 3;
        if (isTriangle) {
            for (int j = polygonStart.at(polygonInd); j < polygonStart.at(polygonInd) + 3; j++)
                indexNew.push_back(index.at(j));
            continue;
        }
        QVector<int> support;
        for (int polygonVertexIndex = 0; polygonVertexIndex < getSizePolygon(polygonStart, polygonInd); polygonVertexIndex++)
            support.push_back(index.at(polygonStart.at(polygonInd)+polygonVertexIndex));

        while(support.size() > 2){
            for (int j = 0; j < 3; j++)
                indexNew.push_back(support.at(j));
            support.remove(1);
        }
    }

    return indexNew;
}



int ObjLoader::getSizeVertices()
{
    return verticesList.size();
}



int ObjLoader::getSizeNormals()
{
    return normalsList.size();
}



int ObjLoader::getSizeTextures()
{
    return texturesList.size();
}
