#ifndef ICP_H
#define ICP_H

#include "kdtree.h"
#include "problemvector.h"
#include "pointfinder.h"
#include "gradientdescent.h"
#include "updatefunctor.h"
#include "errorfunctions.h"
#include "rigidalignment.h"
#include "wgllines.h"

class ICP
{
public:
    static ProblemVector icp(PointFinder *finder, RigidAlignemnt *rigidAlignmen, Wrap::WGLLines *glLines, Wrap::WViewport *viewport);
};

#endif // ICP_H
