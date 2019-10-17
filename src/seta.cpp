
#include "../include/seta.h" 

Seta::Seta(Item *parente,Item *fim)
    :   Item(parente), m_fim{fim}, m_seta(20)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    ajuste();
    m_caneta=QPen(QPen(Qt::black,4,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
}

void Seta::ajuste()
{
    QLineF linha(mapFromItem(parente(0), 0, 0), mapFromItem(m_fim, 0, 0));
    m_linha=linha;
}

void Seta::parente(int pos,Item* novo_valor)
{
    if(pos==0) Item::parente(pos,novo_valor);
    else if(pos==1) m_fim=novo_valor;
    else throw std::invalid_argument("posicao invalida");
}

Item* Seta::parente(int pos) const
{
    if(pos>=2) throw std::invalid_argument("posicao invalida");
    else if(pos==0) return Item::parente(0);
    else return m_fim;
}

QPainterPath Seta::shape() const
{
    QPolygonF poligono;
    qreal offsetSelecao=20.0;
    qreal angulo = m_linha.angle()* pi() / 180;
    qreal dx=offsetSelecao*sin(angulo);
    qreal dy=offsetSelecao*cos(angulo);
    QPointF offset1=QPointF(dx, dy);
    QPointF offset2=QPointF(-dx, -dy);
    poligono << m_linha.p1()+offset1
             << m_linha.p1()+offset2
             << m_linha.p2()+offset2
             << m_linha.p2()+offset1;
    QPainterPath cursor;
    cursor.addPolygon(poligono);
    cursor.addPolygon(m_seta);
    return cursor;
}

QRectF Seta::boundingRect() const
{
    if (!parente(0) || !m_fim) return QRectF();
    qreal extra = (m_caneta.width() + 20) / 2.0;
    return QRectF(m_linha.p1(), 
                  QSizeF(m_linha.p2().x() - m_linha.p1().x(),
                         m_linha.p2().y() - m_linha.p1().y()))
                  .normalized()
                  .adjusted(-extra, -extra, extra, extra);
}

void Seta::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (!parente(0) || !m_fim) return;
    QPointF inicio = (parente(0)->pos());
    QPointF fim    = (m_fim->pos());
    qreal arrowSize = 20;
    painter->setPen(m_caneta);
    painter->setBrush(Qt::black);
    if(parente(0)->collidesWithItem(m_fim)) inicio-=QPointF(0,parente(0)->altura());
    if(parente(0)==parente(1)) inicio-=QPointF(0,parente(0)->altura());
    QLineF centerLine(inicio, fim);
    // buscando ponto de intersecao poligono final
    QPolygonF endPolygon = m_fim->shape().toFillPolygon();
    QPointF p1 = endPolygon.first() + fim;
    QPointF p2;
    QPointF intersectPoint;
    QLineF polyLine;
    for (int i = 1; i < endPolygon.count(); ++i) {
        p2 = endPolygon.at(i) + fim;
        polyLine = QLineF(p1, p2);
        QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &intersectPoint);
        if (intersectType == QLineF::BoundedIntersection) break;
        p1 = p2;
    }
    // buscando ponto de intersecao poligono initial
    QPolygonF startPolygon = parente(0)->shape().toFillPolygon();
    QPointF p11 = startPolygon.first() + inicio;
    QPointF p22;
    QPointF intersectPoint1;
    QLineF polyLine1;
    for (int i = 1; i < startPolygon.count(); ++i) {
        p22 = startPolygon.at(i) + inicio;
        polyLine1 = QLineF(p11, p22);
        QLineF::IntersectType intersectType1 = polyLine1.intersect(centerLine, &intersectPoint1);
        if (intersectType1 == QLineF::BoundedIntersection) break;
        p11 = p22;
    }
    // definindo a linha
    if(parente(0)->collidesWithItem(m_fim)){
        QPointF fi(fim.x(),fim.y()-parente(0)->largura()/2);
        m_linha=QLineF(inicio, fim);
        m_linha.setLength(50);
    }
    m_linha=QLineF(intersectPoint, intersectPoint1);
    // fazendo o angulo da seta
    double angle = std::acos(m_linha.dx() / m_linha.length());
    if (m_linha.dy() >= 0) angle = (pi() * 2) - angle;
    QPointF arrowP1 = m_linha.p1() + QPointF(sin(angle + pi() / 3) * arrowSize, cos(angle + pi() / 3) * arrowSize);
    QPointF arrowP2 = m_linha.p1() + QPointF(sin(angle + pi() - pi() / 3) * arrowSize, cos(angle + pi() - pi() / 3) * arrowSize);
    m_seta.clear();
    m_seta << m_linha.p1() << arrowP1 << arrowP2;
    if(parente(0)!=parente(1)) painter->drawLine(m_linha);
    painter->drawPolygon(m_seta);
    // linha de contorno
    if (isSelected()) {
        painter->setPen(QPen(Qt::black, 1, Qt::DashLine));
        QLineF myLine = m_linha;
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }
    angle = std::acos(m_linha.dy() / m_linha.length());
    QPointF meio = (m_linha.p1() + m_linha.p2())/2;
    painter->translate(meio);
    int tam_x = 0;
    int tam_y = 0;
    QString str(valor());
    if(str.size()>10) 
    {
        str.remove(9,str.size()-10);
        str.append("...");
    }
    if(parente(0)==parente(1))
    {
        painter->rotate(0);
        tam_x = -str.size()*3;
        tam_y = 15;
    }
    else if(m_linha.dx()==0) {
        painter->rotate(90);
        tam_x = -str.size()*3;
        tam_y = (m_linha.dy()>0) ? 15 : -5;
    }
    else if(m_linha.dy()==0) {
        painter->rotate(0);
        tam_x = -str.size()*3;
        tam_y = (m_linha.dx()>0) ? 15 : -5;
    }
    else if(m_linha.dx()<0 && m_linha.dy()<0) {
        painter->rotate(-90+(angle*57.2859));
        tam_x = -str.size()*3;
        tam_y = 15;
    }
    else if(m_linha.dx()>0 && m_linha.dy()<0) {
        painter->rotate(90-(angle*57.2859));
        tam_x = -str.size()*3;
        tam_y = -5;
    }
    else if(m_linha.dx()>0 && m_linha.dy()>0) {
        painter->rotate(90-(angle*57.2859));
        tam_x = -str.size()*3;
        tam_y = -5;
    }
    else if(m_linha.dx()<0 && m_linha.dy()>0) {
        painter->rotate(-90+(angle*57.2859));
        tam_x = -str.size()*3;
        tam_y = 15;
    }
    painter->setPen(QPen(Qt::darkRed));
    // usar q graphcis item
    painter->setFont(QFont("Times",12,QFont::Bold));
    painter->drawText(tam_x,tam_y,str);
    update();
} 
