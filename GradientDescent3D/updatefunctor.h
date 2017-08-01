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
#include "transformation.h"
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <autodiff.h>


template<typename S>
using Vector3 = Eigen::Matrix<S, 3, 1> ;

class UpdateFunctor
{
public:
    virtual void update(const float &angle, const float &shiftX, const float &shiftY, const float &shiftZ, const Vector3<float> &axisProblem) = 0;
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

public:
    UpdateFunctorViewPort(GeometryStack2::GeomStackTriangulated *geom, Wrap::WGLDataGeomStackTriangulated *glData, Wrap::WGLObjectRenderer *glRenderer,
                          Wrap::WViewport *viewport, Wrap::WGLMaterialSurface *glMaterialSurface, Wrap::WGLMaterialWireframe *glMaterialWireframe);

    void update(const float &angle, const float &shiftX, const float &shiftY, const float &shiftZ, const Vector3<float> &axisProblem) override;
};

#endif // UPDATEFUNCTOR_H
