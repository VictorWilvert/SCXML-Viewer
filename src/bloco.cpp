 
#include "../include/bloco.h"
 
Bloco::Bloco(Item *parente)
    :   Item(parente), m_blocos{0}, m_texto{30}, m_espacamento{50}
{
    //  dimensoes
    largura(150);
    altura(90);
    //  definicao do poligono do Bloco
    m_poligono = shape().toFillPolygon();
    //  definindo flags do Bloco
    setFlag(QGraphicsItem::ItemIsSelectable, true);         
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true); 
    //  definindo a cor basica de fundo 
    m_pincel = QBrush(Qt::darkCyan);
}

QRectF Bloco::boundingRect() const 
{
    //  define a delimitacao do Bloco 
    return QRectF(-largura()/2,     //  int x
                  -altura()/2,      //  int y
                   largura(),       //  int width
                   altura());       //  int height
}

QPainterPath Bloco::shape() const 
{
    //  define o formato do Bloco
    QPainterPath cursor;
    qreal raio=90/std::log(largura());          //  raio das ellipses nos cantos do retangulo
    cursor.addRoundedRect(boundingRect(),       // QRect & rect
                          raio,                 //  qreal xRadius
                          raio);                //  qreal yRadius
    return cursor;
}
#include <QDebug>
void Bloco::insereFilho(Item *filho)
{
    //  adiciona o filho a lista de filhos
    if(!filho) return;
    if(filho->tipo()!="seta")
    {
        m_filhos.prepend(filho);
        m_blocos++;
    }
    else m_filhos.append(filho);
    atualiza();
    if(parente(0)) parente(0)->ajuste();
}

void Bloco::removeFilho(Item *filho)
{
    //  remove o filho desejado
    if(!filho) return;
    m_filhos.removeOne(filho);
    if(filho->tipo()!="seta") m_blocos--;
    atualiza();
    if(parente(0)) parente(0)->ajuste();
}

void Bloco::atualiza()
{
    if(!m_blocos) return;
    //  definicao do numero de colunas do grid de filhos
    int grade=std::ceil(std::sqrt(m_blocos));
    //  criacao de duas listas com os tamanhos dos filhos
    std::pair<QList<int>,QList<int>> tamanho;
    for(int i=0;i<m_blocos;i++)
    {
        tamanho.first.push_back(m_filhos[i]->largura());
        tamanho.second.push_back(m_filhos[i]->altura());
    }
    //  ordenacao das listas
    std::sort(tamanho.first.begin(),
              tamanho.first.end());
    std::sort(tamanho.second.begin(),
              tamanho.second.end());
    //  redimensionamento
    int nova_largura=m_espacamento;
    int nova_altura=m_espacamento+m_texto;
    int esp_h=tamanho.first.back()+m_espacamento;
    int esp_v=tamanho.second.back()+m_espacamento;
    for(int i=0;i<grade;i++)
    {
        if(i==(grade-1))
        {
            nova_largura+=tamanho.first[(tamanho.first.size()-1)-i]+m_espacamento;
            nova_altura+=tamanho.second[(tamanho.second.size()-1)-i]+m_espacamento;
        }
        else
        {
            nova_largura+=tamanho.first[(tamanho.first.size()-1)-i]+esp_h;
            nova_altura+=tamanho.second[(tamanho.second.size()-1)-i]+esp_v;
        }
    }
    largura(nova_largura);
    altura(nova_altura);
    //  definicao das posicoes dos filhos
    int x_pos;
    int y_pos;
    int cont_c=0;
    int cont_l=0;
    int pos=m_blocos-1;
    for(int i=0;i<m_blocos;i++)
    {
        if(cont_c==grade) 
        {   
            cont_c=0;
            cont_l++;
        }
        if(!cont_c) x_pos=x()-(largura()/2)+
                    (m_espacamento)+
                    (m_filhos[pos-i]->largura()/2);
        
        else x_pos=m_filhos[pos-i+1]->x()+
                   (m_filhos[pos-i+1]->largura()/2)+
                   (esp_h)+(m_filhos[pos-i]->largura()/2);
        
        if(!cont_l) y_pos=y()-(altura()/2)+
                          (m_espacamento+m_texto)+
                          (m_filhos[pos-i]->altura()/2);
        
        else y_pos=m_filhos[pos-i+grade]->y()+
                   (m_filhos[pos-i+grade]->altura()/2)+
                   (esp_v)+(m_filhos[pos-i]->altura()/2);
        
        m_filhos[pos-i]->setPos(x_pos,  //  qreal x
                                y_pos); //  qreal y
        cont_c++;
    }
    for(auto &valor : m_filhos)
    {
        valor->setZValue(zValue()+1);
        valor->ajuste();
    }
    m_poligono = shape().toFillPolygon();
}

void Bloco::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    //  parametros nao utilizados
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(livre()) setFlag(QGraphicsItem::ItemIsMovable, true);
    else setFlag(QGraphicsItem::ItemIsMovable, false);
    //  define o pintor
    painter->setBrush(m_pincel);
    painter->setPen(QPen(Qt::black,4,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
    //  desenha as linhas
    painter->drawPolyline(m_poligono);
    painter->drawLine(-largura()/2,-altura()/2+m_texto,largura()/2,-altura()/2+m_texto);
    //  desenha o nome
    QString bloco_nome = nome();
    if(bloco_nome.size()>10) 
    {
        bloco_nome.remove(9,bloco_nome.size()-10);
        bloco_nome.append("...");
    }
    painter->drawText(-largura()/2,-altura()/2,largura(),m_texto,Qt::AlignCenter,bloco_nome);
    //  ellipse de initial
    if(initial())
    {
        painter->setBrush(Qt::darkBlue);
        painter->drawEllipse(-largura()/2+5,-altura()/2+5,20,20);
        painter->setBrush(m_pincel);
    }
    if(ativo()) painter->setBrush(Qt::darkRed);
    painter->setOpacity(0.3);
    painter->drawPolygon(m_poligono);
    scene()->update();
}

void Bloco::ajuste()
{
    //  chama o ajuste para todos os filhos
    atualiza();
    for(auto &valor:m_filhos)
        if(valor->tipo()=="seta")
            valor->ajuste();
}

Item* Bloco::acessaFilho(int pos) const 
{
    if(!(pos<m_filhos.size())) throw std::out_of_range("fora de alcance");
    return m_filhos[pos];
}

void Bloco::mousePressEvent(QGraphicsSceneMouseEvent * event) 
{
    QGraphicsItem::mousePressEvent(event);
    m_painel = new Painel;
    m_painel->setPlainText(valor());
    scene()->addItem(m_painel);
    m_painel->setPos(event->scenePos());
}


void Bloco::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    delete m_painel;
    m_painel=nullptr;
}
