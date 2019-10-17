 
#ifndef _janela_h 
#define _janela_h

#include "tabela.h"
#include "diagrama.h"
#include "construtor.h"
#include "bloco.h"
#include "seta.h"
#include "historia.h"
#include "interface.h"
#include "vista_lista.h"
#include "vista_diagrama.h"
#include <utility>
#include <QGraphicsView>
#include <QDomDocument>
#include <QMainWindow>
#include <QFileDialog>
#include <QMenuBar>
#include <QAction>
#include <QString>
#include <QMenu>
#include <QTimer>
#include <QTextStream>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFont>

/**
 *  @class  Janela
 *  @brief  Controla toda representacao gráfica.
 */
class Janela : public QMainWindow
{
//  atributos
    Q_OBJECT
private:
    QMenu *m_arquivos;                      /**< menu Arquivo */
    QString m_scxml;                        /**< string com o caminho ao arquivo */
    VistaDiagrama *m_vista;                 /**< vista da janela */
    QList<Item*> m_copia;                   /**< copia do vetor de itens expostos */
    VistaLista* m_list;                     /**< lista de eventos */
    std::pair<Interface*,Interface*> m_par; /**< classe de comunicacao */
    QTimer *m_timer;                        /**< timer da janela */
    QMenu *m_ajuda;                         /**< menu de ajuda */

//  metodos
private:
    //  Cria as acoes suportadas pela janela.
    void criaAcoes();
    
public:
    //  Constroi uma janela.
    explicit Janela(QWidget *parente=0);
    ~Janela();

//  slots
public slots:
    /**
     *  @brief  Abre um aqruivo Scxml.
     */
    void abrirArq();
    /**
     *  @brief  Abre um aqruivo Txt.
     */
    void abrirArquivo();
    /**
     *  @brief  Volta uma acao.
     */
    void anterior();
    /**
     *  @brief  Avanca uma acao.
     */
    void proximo();
    /**
     *  @brief  Mensagem usada para relatar informacaoes ao usuario.
     */
    void mensagem(QString msg);
    /**
     *  @brief  Para o timer.
     */
    void para() { m_timer->stop(); }
    /**
     *  @brief  cria mensagem de ajuda.
     */
    void ajuda();
    /**
     *  @brief  Inicia o timer.
     */
    void comeca() { m_timer->start(1000); }
    
};      //  Janela

#endif  // _janela_h
