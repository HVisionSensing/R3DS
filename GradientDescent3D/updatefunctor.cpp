#include "updatefunctor.h"

template<typename S>
using Vector3 = Eigen::Matrix<S, 3, 1> ;

UpdateFunctorQDebug::UpdateFunctorQDebug(QVector<QVector3D> mesh)
{
    this->mesh = mesh;
}



void UpdateFunctorQDebug::update(const float angle, const float shiftX, const float shiftY, const float shiftZ, const Vector3<float> axisProblem)
{
    QVector<QVector3D> newMesh = mesh;
    for (int index = 0; index < mesh.size(); index++){
        QVector3D point = UpdateFunctor::transformatePoint(angle, axisProblem, shiftX, shiftY, shiftZ, mesh[index]);
        newMesh[index] = point;
        qDebug() << point.x() << " " << point.y() << " " << point.z() << "\n";
    }
}



QVector3D UpdateFunctor::shiftPoint(const float &shiftX, const float &shiftY, const float shiftZ, const QVector3D &point)
{
    float newX = point.x()+shiftX;
    float newY = point.y()+shiftY;
    float newZ = point.z()+shiftZ;
    QVector3D newPoint(newX, newY, newZ);
    return newPoint;
}



QVector3D UpdateFunctor::rotatePoint(const float &angle, const Vector3<float> axisProblem, const QVector3D &point)
{
    Vector3<float> dot(point.x(), point.y(), point.z());
    Vector3<float> axis = axisProblem/axisProblem.norm();
    Vector3<float> newPoint = ((cos(angle)-1)*(-1)) * (axis.dot(dot)) * axis + sin(angle)*(axis.cross(dot)) + cos(angle) * dot;

    QVector3D newDot(newPoint.x(), newPoint.y(), newPoint.z());
    return newDot;

}



QVector3D UpdateFunctor::transformatePoint(const float &angle, const Vector3<float> axisProblem, const float &shiftX, const float &shiftY, const float shiftZ, const QVector3D &point)
{
    QVector3D rotatedPoint = UpdateFunctor::rotatePoint(angle, axisProblem, point);
    QVector3D transformatePoint = UpdateFunctor::shiftPoint(shiftX, shiftY, shiftZ, rotatedPoint);
    return transformatePoint;
}



UpdateFunctorViewPort::UpdateFunctorViewPort(GeometryStack2::GeomStackTriangulated *geom, Wrap::WGLDataGeomStackTriangulated *glData, Wrap::WGLObjectRenderer *glRenderer, Wrap::WViewport *viewport,
                                             Wrap::WGLMaterialSurface *glMaterialSurface, Wrap::WGLMaterialWireframe *glMaterialWireframe)
    : geom(geom), glData(glData), glRenderer(glRenderer), viewport(viewport), glMaterialSurface(glMaterialSurface), glMaterialWireframe(glMaterialWireframe)
{
}



void UpdateFunctorViewPort::update(const float angle, const float shiftX, const float shiftY, const float shiftZ, const Vector3<float> axisProblem)
{
    for (int indexPoint = 0; indexPoint < geom->v.size(); indexPoint++){
        QVector3D newPoint = UpdateFunctor::transformatePoint(angle, axisProblem, shiftX, shiftY, shiftZ, geom->v[indexPoint]);
        geom->v[indexPoint] = newPoint;
    }

    glData->setVertices(geom->v);
    viewport->update();
    QApplication::processEvents();

}
