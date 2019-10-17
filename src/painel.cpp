
#include "../include/painel.h" 

void Painel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    setZValue(scene()->items().size());
    painter->setPen(QPen(Qt::black,3,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
    painter->setBrush(QBrush(Qt::white));
    painter->drawRect(boundingRect());
    QGraphicsTextItem::paint(painter,option,widget);
    
}
