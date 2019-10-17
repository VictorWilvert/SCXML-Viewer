 
#ifndef _bola_h
#define _bola_h

#include "bloco.h"

/**
 *  @class  Bola
 *  @brief  A classe Bola representa as notacoes <initial> e <final>.
 */
class Bola : public Bloco
{
//  metodos
public:
    //  construtores e destrutores
    explicit Bola(Item *parente=0);
    virtual ~Bola() override {}
    /**
     *  @brief   Retorna o tipo do item.
     *  @return  QString com o tipo.
     */
    virtual QString tipo() override { return QString("bola"); }
    //  operacoes gerais
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    
};      //  Bola

#endif  //  _bola_h
