
#ifndef _interface_h
#define _interface_h

#include <QString>
#include <stdexcept>

/**
 *  @class  Interface
 *  @brief  Interface de comunicacao entre duas classes.
 */
class Interface
{
//  atributos
private:
    QString m_texto;        /**< texto da interface */
    Interface* m_proximo;   /**< proximo na hierarquia de responsabilidade */

//  metodos
public:
    //  enum das acoes
    enum Acao { Avancar, Regredir };
    //  construtor e destrutor
    Interface(Interface *proximo=0);
    virtual ~Interface() {}
    //  acessores e mutatores
    QString texto() const { return m_texto; }
    void texto(QString novo_valor) { m_texto=novo_valor; }
    Interface* proximo() const { return m_proximo; }
    void proximo(Interface *prox) { m_proximo=prox; }
    /**
     *  @brief  Acao de pedido feita a classe.
     *  @param  acao    Acao qual foi pedida realizacao.
     *  @return true Se a acao foi executada, false caso contrario.
     */
    virtual bool pedido(Acao acao);

protected:
    //  avanca
    virtual void avancar() = 0;
    //  regride
    virtual void regredir() = 0;

};      //  Interface

#endif  //  _interface_h
