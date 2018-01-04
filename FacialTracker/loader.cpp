#include "loader.h"



vector<FaceShape> Loader::loadFaces(const QString &dirDataSet, const int facesCount, bool random)
{
    vector<FaceShape> faces;
    srand(1);

    for (int indFace = 1; indFace <= facesCount; indFace++){
        QString faceName;
        QString marksName;

        int numOfFace = indFace;
        if (random)
            numOfFace = rand() %  900 + 1;

        QString numFace = QString::number(numOfFace);
        while (numFace.length() != 4)
            numFace = "0" + numFace;

        faceName = dirDataSet + "image_" + numFace + ".png";
        marksName = dirDataSet + "image_" + numFace + ".pts";

        if(!QFile::exists(faceName)){
            if (random)
                indFace--;
            continue;
        }

        FaceShape face = Loader::loadFace(faceName, marksName);
        faces.push_back(face);
        qDebug() <<"Num: " << faceName;
    }

    return faces;
}



FaceShape Loader::loadFace(const QString &faceName, const QString &marksName)
{
    vector<cv::KeyPoint> marks;
    cv::Mat faceImage;

    cv::String faceImageStr = faceName.toUtf8().constData();
    faceImage = cv::imread(faceImageStr, 1);
    marks = Loader::loadLandmarks(marksName);

    FaceShape face(faceImage, marks);
    return face;
}


vector<cv::KeyPoint> Loader::loadLandmarks(const QString &marksName)
{
    QFile file(marksName);
    Loader::isReadFile(file);
    vector<cv::KeyPoint> marks;
    while (!file.atEnd()){
        QString line = Loader::getCorrectLine(file.readLine());
        bool isX = false;
        bool isY = false;

        if (line.split(' ', QString::SkipEmptyParts).size() != 2)
            continue;

        float x = line.split(' ', QString::SkipEmptyParts).at(0).toFloat(&isX);
        float y = line.split(' ', QString::SkipEmptyParts).at(1).toFloat(&isY);

        if (!isX || !isY)
            continue;

        cv::KeyPoint landmark(x, y, 32);
        marks.push_back(landmark);
    }

    return marks;
}



QString Loader::getCorrectLine(const QString &line)
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



bool Loader::isReadFile(QFile &file)
{
    if (file.open(QIODevice::ReadOnly))
        return true;
    return false;
}



void Loader::saveFaces(const QString &dirSave, const QString &formatSave, const vector<FaceShape> &faces)
{
    for (int indFace = 0; indFace < faces.size(); indFace++){
        QString name = QString::number(indFace);
        QString saveName = dirSave+name+formatSave;
        saveFace(saveName, faces[indFace]);
    }
}



void Loader::saveFace(const QString &savePath, const FaceShape &face)
{
    cv::String saveName = savePath.toUtf8().constData();
    cv::imwrite(saveName, face.image);
}
