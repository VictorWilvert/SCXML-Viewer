
#ifndef _painel_h
#define _painel_h

#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QPainter>

/**
 *  @class  Painel
 *  @brief  painel de informacoes de um estado.
 */
class Painel : public QGraphicsTextItem
{
//  metodos
public:
    //  operacao geral
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    
};      //  Painel

#endif  //  _painel_h
