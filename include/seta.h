 
#ifndef _seta_h
#define _seta_h

#include "item.h"

/**
 *  @class  Seta
 *  @brief  A classe Seta representa a notacao <transition>.
 */
class Seta : public Item
{
//  atributos
private:
    Item *m_fim;            /**< item do final da seta */
    QPolygonF m_seta;       /**< poligono da cabeca da seta */
    QLineF m_linha;         /**< linha da Seta */
    QPen m_caneta;          /**< caneta atual */
    
//  metodos
public:
    //  construtor
    Seta(Item *parente,Item *fim);
    //  operacoes gerais
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    /**
     *  @brief  Ajusta posicao da reta.
     */
    void ajuste() override;
    /**
     *  @brief   Retorna o tipo do item.
     *  @return  QString com o tipo.
     */
    QString tipo() override { return "seta"; }
    /**
     *  @brief Modifica o acesso para o segundo parente.
     */
    void parente(int pos,Item* novo_valor) override;
    Item* parente(int pos) const override;
    
};      //  Seta

#endif  //  _seta_h
