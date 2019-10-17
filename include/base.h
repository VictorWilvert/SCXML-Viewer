 
#ifndef _base_h
#define _base_h

#include "bloco.h"

/**
 *  @class  Base
 *  @brief  A classe Base representa a notacao <scxml>.
 */
class Base : public Bloco
{
//  metodos
public:
    //  construtores e destrutores
    explicit Base(Item *parente=0);
    ~Base() override {}
    //  operacoes gerais
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    /**
     *  @brief   Retorna o tipo do item.
     *  @return  QString com o tipo.
     */
    QString tipo() override { return QString("base"); }
    
};      //  Base

#endif  //  _base_h
