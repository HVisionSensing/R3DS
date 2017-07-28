#ifndef UPDATEFUNCTOR_H
#define UPDATEFUNCTOR_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QDebug>
#include "math.h"
#include "wcontextnavigation.h"
#include "wgldatageomstacktriangulated.h"
#include "wglmaterialsurface.h"
#include "wglmaterialwireframe.h"
#include "wglobjectrenderer.h"
#include "wglgrid.h"
#include "wgldots.h"
#include <Eigen/Core>
#include <autodiff.h>


template<typename S>
using Vector3 = Eigen::Matrix<S, 3, 1> ;

class UpdateFunctor
{
public:
    virtual void update(const float angle, const float shiftX, const float shiftY, const float shiftZ, const Vector3<float> axisProblem) = 0;
public:
    static QVector3D shiftPoint(const float &shiftX, const float &shiftY, const float shiftZ, const QVector3D &point);
    static QVector3D rotatePoint(const float &angle, const Vector3<float> axisProblem, const QVector3D &point);
    static QVector3D transformatePoint(const float &angle, const Vector3<float> axisProblem, const float &shiftX, const float &shiftY, const float shiftZ, const QVector3D &point);
};



class UpdateFunctorQDebug : public UpdateFunctor
{
public:
    QVector<QVector3D> mesh;
    UpdateFunctorQDebug(QVector<QVector3D> mesh);
    virtual void update(const float angle, const float shiftX, const float shiftY, const float shiftZ, const Vector3<float> axisProblem) override;
};



class UpdateFunctorViewPort : public UpdateFunctor
{
public:
    Wrap::WGLMaterialSurface *glMaterialSurface = nullptr;
    Wrap::WGLMaterialWireframe *glMaterialWireframe = nullptr;
    GeometryStack2::GeomStackTriangulated *geom = nullptr;
    Wrap::WGLDataGeomStackTriangulated *glData = nullptr;
    Wrap::WGLObjectRenderer *glRenderer = nullptr;
    Wrap::WViewport *viewport = nullptr;

    UpdateFunctorViewPort(GeometryStack2::GeomStackTriangulated *geom, Wrap::WGLDataGeomStackTriangulated *glData, Wrap::WGLObjectRenderer *glRenderer, Wrap::WViewport *viewport, Wrap::WGLMaterialSurface *glMaterialSurface, Wrap::WGLMaterialWireframe *glMaterialWireframe);
    virtual void update(const float angle, const float shiftX, const float shiftY, const float shiftZ, const Vector3<float> axisProblem) override;
};

#endif // UPDATEFUNCTOR_H
