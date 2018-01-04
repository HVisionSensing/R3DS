#include "updatefunctor.h"

template<typename S>
using Vector3 = Eigen::Matrix<S, 3, 1> ;

UpdateFunctorViewPort::UpdateFunctorViewPort(GeometryStack2::GeomStackTriangulated *geom, Wrap::WGLDataGeomStackTriangulated *glData, Wrap::WGLObjectRenderer *glRenderer, Wrap::WViewport *viewport,
                                             Wrap::WGLMaterialSurface *glMaterialSurface, Wrap::WGLMaterialWireframe *glMaterialWireframe)
    : geom(geom), glData(glData), glRenderer(glRenderer), viewport(viewport), glMaterialSurface(glMaterialSurface), glMaterialWireframe(glMaterialWireframe)
{
}



void UpdateFunctorViewPort::update(const float &angle, const float &shiftX, const float &shiftY, const float &shiftZ, const Vector3<float> &axisProblem, const QVector<QVector3D> &mesh)
{
    for (int indexPoint = 0; indexPoint < mesh.size(); indexPoint++){
        float x =mesh[indexPoint].x();
        float y = mesh[indexPoint].y();
        float z = mesh[indexPoint].z();
        Vector3<float> point(x, y, z);
        Vector3<float> axisUnit = axisProblem;
        Vector3<float> rotatedPoint = Transformation::rotateQuaternion(point, axisUnit, angle);
        Vector3<float> shiftPoint = Transformation::shiftPoint(shiftX, shiftY, shiftZ, rotatedPoint);
        QVector3D newPoint(shiftPoint[0], shiftPoint[1], shiftPoint[2]);
        geom->v[indexPoint] = newPoint;
    }

    glData->setVertices(geom->v);
    viewport->update();
    QApplication::processEvents();
}
