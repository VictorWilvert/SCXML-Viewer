
#ifndef _vista_diagrama_h
#define _vista_diagrama_h

#include <QGraphicsView>
#include <QWheelEvent>

/**
 *  @class  VistaDiagrama
 *  @brief  Cuida da representacao grafica dos itens.
 */
class VistaDiagrama : public QGraphicsView
{
//  atributos
protected:
    qreal h11{1};
    qreal h22{1};
    void wheelEvent(QWheelEvent *event) override;
    
//  metodos
public:
    //  construtores
    explicit VistaDiagrama(QWidget *parente=0) : QGraphicsView(parente) {}
    explicit VistaDiagrama(QGraphicsScene *cena,QWidget *parente=0) :   QGraphicsView(cena,parente) {}
    
};      //  VistaDiagrama

#endif  //  _vista_diagrama_h
