#include "landmark.h"


Landmark::Landmark(qreal x, qreal y, qreal w, qreal h, int indPoint)
    : QGraphicsEllipseItem(0, 0, w, h)
{
    this->setPos(x, y);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    this->setAcceptDrops(true);

    pen = QPen(Qt::red);
    brush = QBrush(Qt::red);
    this->indPoint = indPoint;
}



void Landmark::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (end){
        pen.setColor(Qt::blue);
        brush.setColor(Qt::blue);
    }

    if (start){
        pen.setColor(Qt::green);
        brush.setColor(Qt::green);
    }

    QRectF rect = this->boundingRect();
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawEllipse(rect);
    //QFont font=painter->font() ;
    //font.setPointSize ( 5);
    //painter->setFont(font);
    //painter->drawText(rect.center(), QString::number(indPoint));
}



void Landmark::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsEllipseItem::mouseMoveEvent(event);
    qDebug() << indPoint;
    emit changePosition(event->scenePos());
}

