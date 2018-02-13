#include "markupview.h"



MarkupView::MarkupView(QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene();
    this->setScene(scene);
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
}



void MarkupView::drawImage(const QImage &image)
{
    scene->clear();
    clearBodyParts();
    QPixmap pixmap = QPixmap::fromImage(image);
    imageOpened = new QGraphicsPixmapItem(pixmap);
    scene->addItem(imageOpened);
}


void MarkupView::updateBodyPart()
{
    for (int indPoint = 0; indPoint < body->parts[body->indActived].points.size();indPoint++)
        scene->addItem(body->parts[body->indActived].points[indPoint]);
    updatePart();
}



void MarkupView::clearScene()
{
    scene->removeItem(body->parts[body->indActived].path);
    for (int indItem = 0; indItem < body->parts[body->indActived].points.size(); indItem++)
        scene->removeItem(body->parts[body->indActived].points[indItem]);
}



void MarkupView::updatePart()
{
    if (body->parts[body->indActived].points.size() < 4)
        return;
    scene->removeItem(body->parts[body->indActived].path);
    QPainterPath pathLeftEye(body->parts[body->indActived].points[0]->scenePos());
    for (int indLeftEye = 1; indLeftEye < body->parts[body->indActived].points.size(); indLeftEye++)
        pathLeftEye.lineTo(body->parts[body->indActived].points[indLeftEye]->scenePos());
    pathLeftEye.lineTo(body->parts[body->indActived].points[0]->scenePos());
    body->parts[body->indActived].path = new QGraphicsPathItem(pathLeftEye);
    body->parts[body->indActived].path->setPen(QPen(Qt::red));
    scene->addItem(body->parts[body->indActived].path);
}



void MarkupView::updateBlocks(Body newBody)
{
    clearScene();
    body = new Body();

    // Всё ровно это все еще говно, как и куча кода до этого, вот нахера я торопился?!

    for (int indPart = 0; indPart < newBody.parts.size(); indPart++){
        int indPoint = 0;
        if (newBody.parts[indPart].corner.size() == 0)
            continue;
        Landmark *landmark = new Landmark(newBody.parts[indPart].corner[indPoint].x()-0, newBody.parts[indPart].corner[indPoint].y()-0, 0.5, 0.5, indPoint);
        landmark->start = true;
        connect(landmark, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
        body->parts[indPart].loadPoint(landmark, indPoint);

        for (int indPointUp = 0; indPointUp < newBody.parts[indPart].up.size(); indPointUp++){
            indPoint++;
            Landmark *landmark = new Landmark(newBody.parts[indPart].up[indPointUp].x()-0, newBody.parts[indPart].up[indPointUp].y()-0, 0.5, 0.5, indPoint);
            connect(landmark, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
            body->parts[indPart].loadPoint(landmark, indPoint);
        }

        indPoint++;
        Landmark *landmark2 = new Landmark(newBody.parts[indPart].corner[1].x()-0, newBody.parts[indPart].corner[1].y()-0, 0.5, 0.5, indPoint);
        landmark2->end = true;
        connect(landmark2, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
        body->parts[indPart].loadPoint(landmark2, indPoint);

        for (int indPointDown = 0; indPointDown < newBody.parts[indPart].down.size(); indPointDown++){
            indPoint++;
            Landmark *landmark = new Landmark(newBody.parts[indPart].down[indPointDown].x()-0, newBody.parts[indPart].down[indPointDown].y()-0, 0.5, 0.5, indPoint);
            connect(landmark, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
            body->parts[indPart].loadPoint(landmark, indPoint);
        }
    }
    /*
    // TODO О БОЖЕ КАКОЕ ТУТ ГОВНО
    int indLeftEye = 0;
    if (blks.leftEye.corner.size() > 0){
        Landmark *landmark = new Landmark(blks.leftEye.corner[0].x()-0, blks.leftEye.corner[0].y()-0, 0.5, 0.5, indLeftEye);
        landmark->start = true;
        connect(landmark, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
        leftEyeBlock->loadPoint(landmark, indLeftEye);

        for (int indPointUp = 0; indPointUp < blks.leftEye.up.size(); indPointUp++){
            indLeftEye++;
            Landmark *landmark = new Landmark(blks.leftEye.up[indPointUp].x()-0, blks.leftEye.up[indPointUp].y()-0, 0.5, 0.5, indLeftEye);
            connect(landmark, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
            leftEyeBlock->loadPoint(landmark, indLeftEye);
        }
        indLeftEye++;
        Landmark *landmark2 = new Landmark(blks.leftEye.corner[1].x()-0, blks.leftEye.corner[1].y()-0, 0.5, 0.5, indLeftEye);
        landmark2->end = true;
        connect(landmark2, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
        leftEyeBlock->loadPoint(landmark2, indLeftEye);

        for (int indPointDown = 0; indPointDown < blks.leftEye.down.size(); indPointDown++){
            indLeftEye++;
            Landmark *landmark = new Landmark(blks.leftEye.down[indPointDown].x()-0, blks.leftEye.down[indPointDown].y()-0, 0.5, 0.5, indLeftEye);
            connect(landmark, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
            leftEyeBlock->loadPoint(landmark, indLeftEye);
        }
    }
    // ИСПРАВИТЬ БЕЗВОЗРАЖЕНИЙ

    indLeftEye = 0;
    if (blks.rightEye.corner.size() > 0){
        Landmark *landmark3 = new Landmark(blks.rightEye.corner[0].x()-0, blks.rightEye.corner[0].y()-0, 0.5, 0.5, indLeftEye);
        landmark3->start = true;
        connect(landmark3, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
        rightEyeBlock->loadPoint(landmark3, indLeftEye);

        for (int indPointUp = 0; indPointUp < blks.rightEye.up.size(); indPointUp++){
            indLeftEye++;
            Landmark *landmark = new Landmark(blks.rightEye.up[indPointUp].x()-0, blks.rightEye.up[indPointUp].y()-0, 0.5, 0.5, indLeftEye);
            connect(landmark, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
            rightEyeBlock->loadPoint(landmark, indLeftEye);
        }
        indLeftEye++;
        Landmark *landmark4 = new Landmark(blks.rightEye.corner[1].x()-0, blks.rightEye.corner[1].y()-0, 0.5, 0.5, indLeftEye);
        landmark4->end = true;
        connect(landmark4, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
        rightEyeBlock->loadPoint(landmark4, indLeftEye);

        for (int indPointDown = 0; indPointDown < blks.rightEye.down.size(); indPointDown++){
            indLeftEye++;
            Landmark *landmark = new Landmark(blks.rightEye.down[indPointDown].x()-0, blks.rightEye.down[indPointDown].y()-0, 0.5, 0.5, indLeftEye);
            connect(landmark, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
            rightEyeBlock->loadPoint(landmark, indLeftEye);
        }
    }
    // ИСПРАВИТЬ БЕЗВОЗРАЖЕНИЙ

    indLeftEye = 0;
    if (blks.mouth.corner.size() > 0){
        Landmark *landmark5 = new Landmark(blks.mouth.corner[0].x()-0, blks.mouth.corner[0].y()-0, 0.5, 0.5, indLeftEye);
        landmark5->start = true;
        connect(landmark5, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
        mouthBlock->loadPoint(landmark5, indLeftEye);

        for (int indPointUp = 0; indPointUp < blks.mouth.up.size(); indPointUp++){
            indLeftEye++;
            Landmark *landmark = new Landmark(blks.mouth.up[indPointUp].x()-0, blks.mouth.up[indPointUp].y()-0, 0.5, 0.5, indLeftEye);
            connect(landmark, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
            mouthBlock->loadPoint(landmark, indLeftEye);
        }
        indLeftEye++;
        Landmark *landmark6 = new Landmark(blks.mouth.corner[1].x()-0, blks.mouth.corner[1].y()-0, 0.5, 0.5, indLeftEye);
        landmark6->end = true;
        connect(landmark6, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
        mouthBlock->loadPoint(landmark6, indLeftEye);

        for (int indPointDown = 0; indPointDown < blks.mouth.down.size(); indPointDown++){
            indLeftEye++;
            Landmark *landmark = new Landmark(blks.mouth.down[indPointDown].x()-0, blks.mouth.down[indPointDown].y()-0, 0.5, 0.5, indLeftEye);
            connect(landmark, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
            mouthBlock->loadPoint(landmark, indLeftEye);
        }
    }
    /*
    for (int indPoint = 0; indPoint < blks.leftEye.size();indPoint++){
        Landmark *landmark = new Landmark(blks.leftEye[indPoint].x()-0, blks.leftEye[indPoint].y()-0, 0.5, 0.5, indPoint);
        connect(landmark, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
        leftEyeBlock->addPoint(landmark, indPoint);
    }

    for (int indPoint = 0; indPoint < blks.rightEye.size();indPoint++){
        Landmark *landmark = new Landmark(blks.rightEye[indPoint].x()-0, blks.rightEye[indPoint].y()-0, 0.5, 0.5, indPoint);
        connect(landmark, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
        rightEyeBlock->addPoint(landmark, indPoint);
    }

    for (int indPoint = 0; indPoint < blks.mouth.size();indPoint++){
        Landmark *landmark = new Landmark(blks.mouth[indPoint].x()-0, blks.mouth[indPoint].y()-0, 0.5, 0.5, indPoint);
        connect(landmark, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
        mouthBlock->addPoint(landmark, indPoint);
    }
    */
    //block = blocks[0];

    updateBodyPart();
}



void MarkupView::clearBodyPart()
{
    scene->removeItem(body->parts[body->indActived].path);
    for (int indItem = 0; indItem < body->parts[body->indActived].points.size(); indItem++)
        scene->removeItem(body->parts[body->indActived].points[indItem]);

    body->parts[body->indActived].path = nullptr;
    body->parts[body->indActived].points.clear();
}



void MarkupView::clearBodyParts()
{
    for (int indPart = 0; indPart < body->parts.size(); indPart++){
        scene->removeItem(body->parts[indPart].path);
        for (int indItem = 0; indItem < body->parts[indPart].points.size(); indItem++)
            scene->removeItem(body->parts[indPart].points[indItem]);

        body->parts[indPart].path = nullptr;
        body->parts[indPart].points.clear();
    }
}



QVector<QPointF> MarkupView::getBodyPart(int indBlock) const
{
    QVector<QPointF> leftEye;
    for (int indPoint = 0; indPoint < body->parts[indBlock].points.size(); indPoint++)
        leftEye.push_back(body->parts[indBlock].points[indPoint]->scenePos());
    return leftEye;
}



QVector<QPointF> MarkupView::getUpPart(int indBLock) const
{
    QVector<QPointF> upPart;
    for (int indPoint = 1; indPoint < body->parts[indBLock].points.size(); indPoint++){
        if (body->parts[indBLock].points[indPoint]->end == true)
            break;
        upPart.push_back(body->parts[indBLock].points[indPoint]->scenePos());
    }
    return upPart;
}



QVector<QPointF> MarkupView::getDownPart(int indBLock) const
{
    QVector<QPointF> downPart;

    int indEnd;
    for (int indPoint = 0; indPoint < body->parts[indBLock].points.size(); indPoint++)
        if (body->parts[indBLock].points[indPoint]->end == true)
            indEnd = indPoint;

    for (int indPoint = indEnd+1; indPoint < body->parts[indBLock].points.size(); indPoint++){
        if (body->parts[indBLock].points[indPoint]->start == true)
            break;
        downPart.push_back(body->parts[indBLock].points[indPoint]->scenePos());
    }
    return downPart;
}



QVector<QPointF> MarkupView::getCorner(int indBLock) const
{
    QVector<QPointF> corners;

    for (int indPoint = 0; indPoint < body->parts[indBLock].points.size(); indPoint++)
        if (body->parts[indBLock].points[indPoint]->start == true)
            corners.push_back(body->parts[indBLock].points[indPoint]->scenePos());

    for (int indPoint = 0; indPoint < body->parts[indBLock].points.size(); indPoint++)
        if (body->parts[indBLock].points[indPoint]->end == true)
            corners.push_back(body->parts[indBLock].points[indPoint]->scenePos());

    return corners;
}


void MarkupView::addPointInPart(Landmark *landmark)
{
    int indInsert = 0;
    float lineMin = 100000000;
    float indMin = 0;

    for (int indPoint = 0; indPoint < body->parts[body->indActived].points.size(); indPoint++){
        int prev = indPoint;
        int next = indPoint+1;
        if (next == body->parts[body->indActived].points.size())
            next = 0;
        float x1 = body->parts[body->indActived].points[prev]->x();
        float x2 = body->parts[body->indActived].points[next]->x();
        float y1 = body->parts[body->indActived].points[prev]->y();
        float y2 = body->parts[body->indActived].points[next]->y();
        float x = landmark->x();
        float y = landmark->y();
        indInsert = prev+1;

        float line = (y1-y2)*x + (x2-x1)*y + (x1*y2-x2*y1);
        if (line < lineMin){
            lineMin = line;
            indMin = prev+1;
        }

        bool amongX = ((x1 < x) && (x < x2)) || ((x1 > x) && (x > x2));
        bool amongY = ((y1 > y) && (y >y2)) || ((y1 < y) && (y < y2));
        if (amongX && amongY){
            body->parts[body->indActived].addPoint(landmark, indPoint+1);
            return;
        }
    }

    body->parts[body->indActived].addPoint(landmark, indMin);
}



void MarkupView::changeBodyPart(int indBlock)
{
    clearScene();
    body->indActived = indBlock;
    updateBodyPart();
}



void MarkupView::wheelEvent(QWheelEvent *event)
{
    const double scaleFactor = 1.15;
    if (event->delta() > 0)
        scale(scaleFactor, scaleFactor);
    else
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
}



void MarkupView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    viewport()->setCursor(Qt::CrossCursor);
    if (event->button() != Qt::RightButton)
        return;

    QPointF position = mapToScene(event->pos());
    Landmark *landmark = new Landmark(position.x()-0, position.y()-0, 0.5, 0.5, 0);
    connect(landmark, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
    addPointInPart(landmark);
    scene->addItem(landmark);

    updatePart();

}



void MarkupView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
    viewport()->setCursor(Qt::CrossCursor);
    //qDebug() << mapToScene(event->pos());
}
