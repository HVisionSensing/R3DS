#include "icp.h"

ProblemVector ICP::icp(PointFinder *finder, RigidAlignemnt *rigidAlignment, Wrap::WGLLines *glLines, Wrap::WViewport *viewport)
{
        QVector<QVector3D> meshTo = rigidAlignment->meshTo;
        QVector<QVector3D> meshToLocal = rigidAlignment->meshTo;
        QVector<QVector3D> meshFromLocal = rigidAlignment->meshFrom;
        QVector<QVector3D> meshFromLocalChanged = rigidAlignment->meshFrom;
        QVector<QVector3D> vertices;
        ProblemVector x(0,0,0,0,Vector3<float>(1,0,0));
        Q_ASSERT(x.axis.norm() > 0.000001);
        x.axis = x.axis/x.axis.norm();
        for (int index = 0; index < 25; index++){
            meshTo.clear();
            for (int indexPoints = 0; indexPoints < meshFromLocalChanged.size(); indexPoints++){
                int index = finder->findNearestPoint(meshFromLocalChanged[indexPoints]);
                meshTo.push_back(meshToLocal[index]);
            }

            vertices.clear();
            for (int index = 0; index < meshFromLocalChanged.size(); index++){
                vertices.append(meshFromLocalChanged[index]);
                vertices.append(meshTo[index]);
            }
            glLines->updateVertices(vertices);
            viewport->update();

            rigidAlignment->meshTo = meshTo;
            x = rigidAlignment->rigidAlignment(x, true);

            for (int index = 0; index < meshFromLocal.size(); index++){
                Vector3<float> point(meshFromLocal[index].x(), meshFromLocal[index].y(), meshFromLocal[index].z());
                point = Transformation::rotateQuaternion(point, x.axis, x.angle);
                point = Transformation::shiftPoint(x.shiftX, x.shiftY, x.shiftZ, point);
                QVector3D newPoint(point(0), point(1), point(2));
                meshFromLocalChanged[index] = newPoint;
            }
        }
        return x;
}
/*QVector<QVector3D> vertices = { QVector3D(0,0,0), QVector3D(1,0,0)};
glLines = new WGLLines(vertices);
glLines->setColor(Qt::yellow);
glLines->setLineWidth(1);
viewport->addObject(glLines,WViewport::Overlay);*/
