#ifndef MARKUPVIEW_H
#define MARKUPVIEW_H

#include "QGraphicsView"
#include "QGraphicsPixmapItem"
#include "QDebug"
#include "QWheelEvent"
#include "landmark.h"
#include "cmath"
#include "body.h"

class MarkupView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MarkupView(QWidget *parent = 0);

    void drawImage(const QImage &image);
    void updateBodyPart();
    void updateBlocks(Body newBody);
    void clearScene();
    void clearBodyPart();
    void clearBodyParts();

    QVector<QPointF> getBodyPart(int indBlock) const;
    QVector<QPointF> getUpPart(int indBLock) const;
    QVector<QPointF> getDownPart(int indBLock) const;
    QVector<QPointF> getCorner(int indBLock) const;
    void addPointInPart(Landmark *landmark);
    void changeBodyPart(int indBlock);

public slots:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void updatePart();

private:
    QGraphicsScene *scene = nullptr;
    QGraphicsItem *imageOpened = nullptr;

    Body *body = new Body();
};

#endif // MARKUPVIEW_H
