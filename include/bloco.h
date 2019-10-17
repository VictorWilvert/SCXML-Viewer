
#ifndef _bloco_h
#define _bloco_h

#include "item.h"
#include "painel.h"
#include <QGraphicsSceneMouseEvent>
#include <QList>
#include <utility>

/**
 *  @class  Bloco
 *  @brief  Classe que representa o <state> e sever de base para outras.
 */
class Bloco : public Item
{
//  atributos
protected:
    QList<Item*> m_filhos;      /**< filhos do Bloco */
    int m_blocos;               /**< quantidade de blocos */
    QPolygonF m_poligono;       /**< poligono do Bloco */
    QBrush m_pincel;            /**< pincel para a cor de fundo */
    qreal m_texto;              /**< tamanho da parte com text */
    int m_espacamento;          /**< espacamamento interno */
    Painel *m_painel;           /**< painel de informacoes */
    bool m_initial{false};      /**< identifica se o estado eh inicial */
    
//  metodos
protected:
    //  atualiza todos os blocos internos
    void atualiza();
    //  reimplementação de eventos
    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
    
public:
    //  construtores e destrutores
    explicit Bloco(Item *parente=0);
    virtual ~Bloco() override {}
    //  operacoes gerais
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    //  retorna o tipo do bloco
    /**
     *  @brief   Retorna o tipo do item.
     *  @return  QString com o tipo.
     */
    virtual QString tipo() override { return QString("bloco"); }
    //  retorna a quantidade de blocos
    /**
     *  @brief  Retorna o numero de blocos filhos.
     */
    int tamanhoFilhos() override { return m_blocos; }
    //  retorna a quantidade total de filhos
    /**
     *  @brief Retorna o total de filhos.
     */
    int tamanhoTotal() override { return m_filhos.size(); }
    //  remove um filhos
    /**
     *  @brief  Remove um filho.
     *  @param  filho   Filho a ser removido.
     */
    void removeFilho(Item *filho) override;
    //  insere um filho
    /**
     *  @brief  Insere um filho.
     *  @param  filho   Filho a ser inserido.
     */
    virtual void insereFilho(Item *filho) override;
    //  acessa um filho
    /**
     *  @brief  Da acesso a um dos filhos.
     *  @param  pos Numeracao do filho.
     *  @return Retorna um Item que eh filho.
     */
    virtual Item* acessaFilho(int pos) const override;
    //  chama o ajuste para as setas
    /**
     *  @brief Operacao geral de ajuste.
     */
    virtual void ajuste() override;
    //  mutator e acessor de initial
    void initial(bool novo_valor) override { m_initial=novo_valor; }
    bool initial() const override { return m_initial; }
    
};      //  Bloco

#endif  //  _bloco_h
