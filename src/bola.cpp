 
#include "../include/bola.h"

Bola::Bola(Item *parente)
    :   Bloco(parente)
{
    largura(50);
    altura(50);
    // definicao das flags
    setFlag(QGraphicsItem::ItemIsSelectable, true);         
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true); 
    //  definindo a cor basica de fundo caso nao seja valido
    m_pincel = QBrush(Qt::red,Qt::Dense5Pattern);
}

QRectF Bola::boundingRect() const 
{
    return QRectF(-25,      //  int x
                  -25,      //  int y
                   50,      //  int width
                   50);     //  int height
}
QPainterPath Bola::shape() const 
{
    QPainterPath cursor;
    cursor.addEllipse(-25,  //  int x
                      -25,  //  int y
                      50,   //  int width
                      50);  //  int height
    return cursor;
}

void Bola::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QRadialGradient grad(0,0,25);
    if(nome()=="initial")
    {
        grad.setColorAt(0,Qt::blue);
        grad.setColorAt(0.7,Qt::darkBlue);
        m_pincel= QBrush(grad);
    }
    else
    {
        if(ativo())
        {
            grad.setColorAt(0,Qt::darkYellow);
            grad.setColorAt(0.7,Qt::yellow);
        }
        else
        {
            grad.setColorAt(0,Qt::darkGray);
            grad.setColorAt(0.7,Qt::black);
        }
        m_pincel = QBrush(grad);
    }
    painter->setPen(QPen(Qt::black,4,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
    painter->setBrush(m_pincel);
    painter->drawEllipse(-25,-25,50,50);
}
