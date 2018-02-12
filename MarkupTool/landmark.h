#ifndef LANDMARK_H
#define LANDMARK_H

#include "QDebug"
#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPen>
#include <QPointF>

class Landmark : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    explicit Landmark(qreal x, qreal y, qreal w, qreal h, int indPoint);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void changePosition(const QPointF &);

public:
    bool start = false;
    bool end = false;
    int indPoint;
    QPen pen;
    QBrush brush;
};

#endif // LANDMARK_H
