 
#include "../include/historia.h"

Historia::Historia(Item *parente)
    :   Bola(parente), m_atual{nullptr}
{
    QRadialGradient grad(0,0,25);
    grad.setColorAt(0,Qt::green);
    grad.setColorAt(0.7,Qt::darkGreen);
    setZValue(1000);
    m_pincel = QBrush(grad);
}

void Historia::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    alteracao();
    painter->setPen(QPen(Qt::black,4,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
    painter->setBrush(m_pincel);
    painter->drawEllipse(-25,-25,50,50);
    painter->setFont(QFont("Times",24,QFont::Bold));
    painter->drawText(boundingRect(),Qt::AlignCenter,"H");
}

void Historia::insereFilho(Item *filho)
{
    Bloco::insereFilho(filho);
    if(filho->tipo()=="seta")
        m_atual=filho->parente(1);
    filho->ajuste();
}

void Historia::alteracao()
{
    if(!tamanhoTotal()) return;
    for(int i=0;i<parente(0)->tamanhoFilhos();i++)
    {
        auto temp=parente(0)->acessaFilho(i);
        if(temp->ativo() && temp->tipo()=="bloco" && temp!=m_atual)
        {
            m_pilha.append(m_atual);
            acessaFilho(0)->parente(1,m_atual);
            m_atual=temp;
            acessaFilho(0)->ajuste();
            return;
        }
    }
    if(m_atual && !m_atual->ativo())
    {
        acessaFilho(0)->parente(1,m_atual);
        acessaFilho(0)->ajuste();
    }
}
