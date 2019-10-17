
#include "../include/vista_lista.h"

VistaLista::VistaLista(QWidget *parente)
    :   QListWidget(parente), m_atual{0}
{
    connect(this,&QListWidget::currentRowChanged,
            this,&VistaLista::mantem);
}

void VistaLista::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
}

void VistaLista::mantem()
{
    setCurrentRow(m_atual);
    update();
}

void VistaLista::avanca()
{
    if(m_atual<count()) m_atual++;
    mantem();
}

void VistaLista::reseta()
{
    clear();
}

void VistaLista::resetaPosicao()
{
    m_atual=0;
    mantem();
}

void VistaLista::regride()
{
    if(m_atual!=0) m_atual--;
    mantem();
}

bool VistaLista::fim()
{
    if(m_atual==count()) return true;
    return false;
}
