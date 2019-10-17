
#ifndef _vista_lista_h
#define _vista_lista_h

#include <QListWidget>
#include <QMouseEvent>

/**
 *  @class  VistaLista
 *  @brief  Cuida da representacao grafica da lista de eventos.
 */
class VistaLista : public QListWidget
{
//  atributos
private:
    int m_atual;    /**< coluna atual */

//  metodos
protected:
    //  modificacao do evento de mouse
    void mousePressEvent(QMouseEvent *event);

public:
    VistaLista(QWidget *parente=0);
    /**
     *  @brief  Avanca um elemento na lista.
     */
    void avanca();
    /**
     *  @brief  Volta um elemento na lista.
     */
    void regride();
    /**
     *  @brief  Reseta a lista.
     */
    void reseta();
    /**
     *  @brief  Reseta a posicao da lista para o comeco.
     */
    void resetaPosicao();
    /**
     *  @brief  Retorna se eh o fim da lista.
     */
    bool fim();

public slots:
    /**
     *  @brief  mantem a posicao na lista em atual.
     */
    void mantem();

};      //  VistaLista

#endif  // _vista_lista_h
