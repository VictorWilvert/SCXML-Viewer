 
#ifndef _historia_h
#define _historia_h

#include "bola.h"
#include <QFont>

/**
 *  @class  Historia
 *  @brief  Representativo da notacao <history>
 */
class Historia : public Bola
{
//  atributos
private:
    QList<Item*> m_pilha;   /**< pilha dos estados anteriores */
    Item* m_atual;          /**< ponteiro para o estado ativo */

//  metodos
private:
    //  cuida de manter a transicao no ultimo elemento
    void alteracao();

public:
    //  construtores e destrutores
    explicit Historia(Item *parente=0);
    ~Historia() override {}
    /**
     *  @brief   Retorna o tipo do item.
     *  @return  QString com o tipo.
     */
    QString tipo() override { return QString("historia"); }
    //  operacoes gerais
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void insereFilho(Item *filho) override;

};      //  Historia

#endif  //  _historia_h
