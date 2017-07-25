#include "updatefunctor.h"

UpdateFunctorQDebug::UpdateFunctorQDebug(QVector<QVector3D> mesh)
{
    this->mesh = mesh;
}



void UpdateFunctorQDebug::update(const float angle, const float shiftX, const float shiftY)
{
    QVector<QVector3D> newMesh = mesh;
    for (int index = 0; index < mesh.size(); index++){
        QVector3D point = UpdateFunctor::transformatePoint(angle, shiftX, shiftY, mesh[index]);
        newMesh[index] = point;
        qDebug() << point.x() << " " << point.y() << "\n";
    }
}



QVector3D UpdateFunctor::shiftPoint(const float &shiftX, const float &shiftY, const QVector3D &point)
{
    float newX = point.x()+shiftX;
    float newY = point.y()+shiftY;
    QVector3D newPoint(newX, newY, 0);
    return newPoint;
}



QVector3D UpdateFunctor::rotatePoint(const float &angle, const QVector3D &point)
{
    float newX = point.x()*cos(angle) - point.y()*sin(angle);
    float newY = point.x()*sin(angle) + point.y()*cos(angle);
    QVector3D newPoint(newX, newY, 0);
    return newPoint;

}



QVector3D UpdateFunctor::transformatePoint(const float &angle, const float &shiftX, const float &shiftY, const QVector3D &point)
{
    QVector3D rotatedPoint = UpdateFunctor::rotatePoint(angle, point);
    QVector3D transformatePoint = UpdateFunctor::shiftPoint(shiftX, shiftY, rotatedPoint);
    return transformatePoint;
}



UpdateFunctorViewPort::UpdateFunctorViewPort(GeometryStack2::GeomStackTriangulated *geom, Wrap::WGLDataGeomStackTriangulated *glData, Wrap::WGLObjectRenderer *glRenderer, Wrap::WViewport *viewport,
                                             Wrap::WGLMaterialSurface *glMaterialSurface, Wrap::WGLMaterialWireframe *glMaterialWireframe)
    : geom(geom), glData(glData), glRenderer(glRenderer), viewport(viewport), glMaterialSurface(glMaterialSurface), glMaterialWireframe(glMaterialWireframe)
{
}



void UpdateFunctorViewPort::update(const float angle, const float shiftX, const float shiftY)
{
    for (int indexPoint = 0; indexPoint < geom->v.size(); indexPoint++){
        QVector3D newPoint = UpdateFunctor::transformatePoint(angle, shiftX, shiftY, geom->v[indexPoint]);
        geom->v[indexPoint] = newPoint;
    }

    glData->setVertices(geom->v);
    viewport->update();
    QApplication::processEvents();

}
