
#ifndef _diagrama_h
#define _diagrama_h

#include <QList>
#include "item.h"
#include "interface.h"

/**
 *  @class  Diagrama
 *  @brief  A classe Diagrama controla a animacao do diagram de estados.
 */
class Diagrama : public Interface
{
//  atributos
private:
    Item *m_atual;              /**< aponta para o item atual */
    QList<Item*> m_anterior;    /**< pilha com os itens passados */

//  metodos
public:
    // construtor e destrutor
    Diagrama(Item* initial,Interface* prox);
    ~Diagrama() override;
    /**
     *  @brief  Acao de pedido feita a classe.
     *  @param  acao    Acao qual foi pedida realizacao.
     *  @return true Se a acao foi executada, false caso contrario.
     */
    bool pedido(Acao acao) override;
    /**
     *  @brief  Avanca no diagrama.
     */
    void avancar() override;
    /**
     *  @brief  Regride no diagrama.
     */
    void regredir() override;

private:
    //  ajusta casos especiais
    void ajusta();
    //  busca o comeco da animacao
    void buscaInicio(Item *no);

};      //  Diagrama

#endif  // _diagrama_h
