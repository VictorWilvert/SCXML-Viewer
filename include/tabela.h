
#ifndef _tabela_h
#define _tabela_h

#include "interface.h"
#include "vista_lista.h"

/**
 *  @class  Tabela
 *  @brief  Cuida mas movimentacoes na lista de eventos
 */
class Tabela : public Interface
{
//  atributos
private:
    VistaLista *m_lista;    /**< lista de eventos */

//  metodos
public:
    //  construtor e destrutor
    Tabela(VistaLista *vista, Interface* prox);
    ~Tabela() override;
    /**
     *  @brief  Acao de pedido feita a classe.
     *  @param  acao    Acao qual foi pedida realizacao.
     *  @return true Se a acao foi executada, false caso contrario.
     */
    bool pedido(Acao acao) override;

protected:
    // alteraçao de avanco na lista
    void avancar() override;
    // alteraçao de regressao na lista
    void regredir() override;

};      //  Tabela

#endif  // _tabela_h
