 
#include "../include/base.h"

Base::Base(Item *parente)
    :   Bloco(parente)
{
    //  dimensoes
    largura(120);
    altura(80);
    //  definicao do poligono do Bloco
    m_poligono = shape().toFillPolygon();
    //  definindo flags do Bloco
    setFlag(QGraphicsItem::ItemIsSelectable, true);         
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true); 
    //  definindo a cor basica de fundo 
    m_pincel = QBrush(Qt::lightGray);
}

void Base::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //  parametros nao utilizados
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //  define o pintor
    painter->setBrush(m_pincel);
    painter->setPen(QPen(Qt::black,4,Qt::DashDotLine,Qt::RoundCap,Qt::RoundJoin));
    //  desenha as linhas
    painter->drawPolyline(m_poligono);
    //  pinta o fundo
    painter->setOpacity(0.2);
    painter->drawPolygon(m_poligono);
}
