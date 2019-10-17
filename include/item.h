
#ifndef _item_h
#define _item_h

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainterPath>
#include <QStyleOption>
#include <QPolygon>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <stdexcept>
#include <cmath>

/**
 *  @interface  Item
 *  @brief  Funciona como componente do composite que forma o diagrama de estados, ou seja, eh uma classe 
 *          abstrata com as operacoes dar arvore composite.
 */
class Item : public QGraphicsItem
{
//  atributos
private:
    Item *m_parente;        /**< parente */
    QString m_nome;         /**< nomde do item */
    QString m_valor;        /**< valor do item */
    QSize m_tamanho;        /**< dimensoes do item */
    bool m_ativo{false};    /**< variavel representado estado do item */
    bool m_livre{false};    /**< variavel representando se ele eh inicial */
    
protected:
    static constexpr double pi(){ return std::atan(1)*4; }
    
//  metodos
public:
    //  construtores e destrutores
    explicit Item(Item *parente=0) : QGraphicsItem(parente), m_parente{parente} {}
    virtual ~Item() {}
    //  mutatores e acessores
    void largura(int novo_valor) { m_tamanho.setWidth(novo_valor); }
    int largura() const { return m_tamanho.width(); }
    void altura(int novo_valor) { m_tamanho.setHeight(novo_valor); }
    int altura() const { return m_tamanho.height(); }
    void nome(QString novo_valor) { m_nome=novo_valor; }
    QString nome() const { return m_nome; }
    void valor(QString novo_valor) { m_valor=novo_valor; }
    QString valor() const { return m_valor; }
    void ativo(bool novo_valor) { m_ativo=novo_valor; }
    bool ativo() const { return m_ativo; }
    void livre(bool novo_valor) { m_livre=novo_valor; }
    bool livre() const { return m_livre; }
    virtual void initial(bool novo_valor)
    { 
        Q_UNUSED(novo_valor); 
        throw std::domain_error("nao implementado"); 
    }
    virtual bool initial() const { return false; }
    virtual void    parente(int pos,Item* novo_valor) 
    { 
        Q_UNUSED(pos);
        Q_UNUSED(novo_valor);
        throw std::invalid_argument("alteracao invalida");
    }
    virtual Item*   parente(int pos) const 
    { 
        if(pos!=0) throw std::invalid_argument("posicao invalida"); 
        else return m_parente; 
    }
    //  operacoes gerais
    virtual void ajuste() = 0;
    virtual QString tipo() = 0;
    virtual QRectF boundingRect() const = 0;
    virtual QPainterPath shape() const = 0;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) = 0;
    //  acesso a filhos
    virtual int tamanhoFilhos() {  throw std::domain_error("operacao nao implementada"); }
    virtual int tamanhoTotal() {  throw std::domain_error("operacao nao implementada"); }
    virtual void removeFilho(Item *filho)
    {
        Q_UNUSED(filho);
        throw std::domain_error("operacao nao implementada");
    }
    virtual void insereFilho(Item *filho)
    {
        Q_UNUSED(filho); 
        throw std::domain_error("operacao nao implementada"); 
    }
    virtual Item* acessaFilho(int pos) const 
    { 
        Q_UNUSED(pos);
        throw std::domain_error("operacao nao implementada"); 
    }
    
};      //  Item

#endif  //  _item_h
