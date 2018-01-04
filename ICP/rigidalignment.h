#ifndef RIGIDALIGNMENT_H
#define RIGIDALIGNMENT_H

#include <QVector>
#include <QVector3D>
#include "updatefunctor.h"
#include "problemvector.h"
#include "gradientdescent.h"

class RigidAlignemnt
{
public:
    QVector<QVector3D> meshFrom;
    QVector<QVector3D> meshTo;

    virtual ProblemVector rigidAlignment(ProblemVector &x, bool isDebug) const = 0;
};

template<class ErrorFunction>
class RigidAlignemntFirst : public RigidAlignemnt
{
public:
    ErrorFunction errorFunc;
    UpdateFunctor *update;

    RigidAlignemntFirst(const QVector<QVector3D> &meshFrom, const QVector<QVector3D> &meshTo, const ErrorFunction errorFunc, UpdateFunctor *update)
        : errorFunc(errorFunc), update(update){
        this->meshFrom = meshFrom;
        this->meshTo = meshTo;
    }

    ProblemVector rigidAlignment(ProblemVector &x, bool isDebug) const {
        return GradientDescent::gradientDescentGaussNewton(meshFrom, meshTo, errorFunc, x, isDebug, update);
    }
};

#endif // RIGIDALIGNMENT_H

