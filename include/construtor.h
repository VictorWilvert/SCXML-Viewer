
#ifndef _construtor_h
#define _construtor_h

#include <QDomDocument>
#include <QList>
#include "seta.h"
#include "base.h"
#include "historia.h"
#include <algorithm>

/**
 *  @class  Construtor
 *  @brief  Responsavel pela construcao da representacao grafica do diagrama.
 */
class Construtor
{
//  atributos
private:
    QDomDocument m_documento;   /**< documento com o scxml */
    QList<Item*> m_itens;       /**< vetor com itens construidos */
    Item* m_raiz;               /**< raiz da arvore do scxml */
    QString m_erro;             /**< string contendo os erros */
    
//  metodos
private:
    //  realiza leitura de todos elementos
    QString leElemento(QDomElement elemento,Item* parente);
    //  le um atributo
    QString leAtributos(QDomElement elemento);
    //  le o texto
    QString leTexto(QDomElement elemento);
    //  realiza leitura de todas as transicoes
    void leTransicao(QDomElement elemento, Item *parente);
    //  valida se os elementos sao validos
    bool valida(Item *no);
    //  realiza a operacao de deletar um no e suas referencias
    void deleta(Item *no);
    //  busca um no por seu nome
    Item* busca(Item* no,QString nome);
    //  busca as transicoes que possue tal fim
    void buscaTransicao(Item *no,QString nome_fim);
    
public:
    // construtor
    Construtor(QDomDocument& documento);
    /**
     *  @brief  Realiza a leitura do QDomDocument.
     */
    void leitura();
    /**
     *  @brief  Da acesso a todos elementos criados na leitura.
     *  @return Qlist com os Itens.
     */
    QList<Item*> vetor() const { return m_itens; }
    /**
     *  @brief  Da acesso a condicao do documento.
     *  @return Retorna uma QString com a situacao.
     */
    QString erro() const { return m_erro; }
    /**
     *  @brief  Retorna a raiz do documento.
     */
    Item* raiz() const { return m_raiz; }
    
};      //  Construtor

#endif  //  _construtor_h
