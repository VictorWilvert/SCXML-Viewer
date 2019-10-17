
#include "../include/janela.h"

Janela::Janela(QWidget *parente) 
    :   QMainWindow(parente), m_par(nullptr,nullptr)
{
    criaAcoes();
    QGraphicsScene *scene = new QGraphicsScene(this);
    m_vista=new VistaDiagrama(scene,this);
    m_vista->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
    m_vista->setMinimumSize(600,500);
    m_timer = new QTimer(this);
    m_list= new VistaLista(this);
    QVBoxLayout* l1 = new QVBoxLayout();
    l1->addWidget(m_list);
    m_list->setCurrentRow(0);
    // botoes
    QHBoxLayout* l2 = new QHBoxLayout();
    QList<QPushButton*> b;
    for(int i=0;i<4;i++){
        b.append(new QPushButton(this));
        b[i]->setMinimumSize(50,50);
        b[i]->setMaximumSize(50,50);
        l2->addWidget(b[i]);
        QFont f("Helvetica");
        f.setBold(true);
        f.setPointSize(20);
        b[i]->setFont(f);
        if(i==0){ 
            b[i]->setText("<");
            
        }
        if(i==1) b[i]->setText("||");
        if(i==2) b[i]->setText(">");
        if(i==3) b[i]->setText(">>");
    }
    // layouts
    QWidget* w1 = new QWidget(this);
    w1->setLayout(l2);
    l1->addWidget(w1);
    QWidget* w2 = new QWidget(this);
    w2->setLayout(l1);
    // layout final
    QHBoxLayout* l3 = new QHBoxLayout();
    l3->addWidget(w2);
    l3->addWidget(m_vista,2);
    setCentralWidget(m_vista);
    l3->setSizeConstraint(QLayout::SetMinAndMaxSize);
    // widget da janela
    QWidget* w3 = new QWidget;
    w3->setLayout(l3);
    setCentralWidget(w3);
    //  conecoes
    connect(b[0],&QPushButton::clicked,
            this,&Janela::anterior);
    connect(b[2],&QPushButton::clicked,
            this,&Janela::proximo);
    connect(m_timer,&QTimer::timeout,
            this,&Janela::proximo);
    connect(b[1],&QPushButton::clicked,
            this,&Janela::para);
    connect(b[3],&QPushButton::clicked,
            this,&Janela::comeca);
    m_par.second = new Tabela(m_list,nullptr);
    setWindowTitle(tr("Leitor de SCXML"));
}

Janela::~Janela()
{
    if(m_par.first) delete m_par.first;
    if(m_par.second) delete m_par.second;
}

void Janela::criaAcoes()
{
    m_arquivos = menuBar()->addMenu(tr("&Arquivo"));
    m_arquivos->addAction(tr("Abrir scxml"), this, &Janela::abrirArquivo, QKeySequence::Open);
    m_arquivos->addAction(tr("Abrir texto"), this, &Janela::abrirArq, tr("CTRL+I"));
    m_arquivos->addAction(tr("Sair"), this, &QWidget::close, QKeySequence::Quit);
    m_ajuda = menuBar()->addMenu(tr("A&juda"));
    m_ajuda->addAction(tr("Ajuda"),this,&Janela::ajuda,tr("CTRL+H"));
}

void Janela::abrirArquivo()
{
    QString caminho = QFileDialog::getOpenFileName(this,QString("Abrir Arquivo Scxml"),m_scxml,QString("*.scxml(*.scxml);;*.xml(*.xml);;"));
    if (!caminho.isEmpty()) {
        QFile arquivo(caminho);
        if (arquivo.open(QIODevice::ReadOnly)) {
            QDomDocument documento;
            QString erro;
            int coluna, linha;
            if (documento.setContent(&arquivo,&erro,&coluna,&linha)) {
                Construtor c(documento);
                c.leitura();
                //  caso tenha uma imagen na tela, deleta ela
                if(m_copia.size()) for(auto &ptr:m_copia) delete ptr;
                auto vetor =c.vetor();
                for(int i =0;i<vetor.size();i++)
                    m_vista->scene()->addItem(vetor[i]);
                m_copia=vetor;
                mensagem(c.erro());
                if(m_par.first)
                {
                    delete m_par.first;
                    m_par.first=nullptr;
                }
                try
                {
                    m_par.first=new Diagrama(c.raiz(),nullptr);
                }
                catch(std::exception& erro)
                {
                    delete m_par.first;
                    m_par.first=nullptr;
                    mensagem(QString(erro.what()));
                    for(auto&temp:m_copia)
                    {
                        delete temp;
                        temp=nullptr;
                    }
                }
                m_par.second->proximo(m_par.first);
                m_scxml = caminho;
            }
            if(erro.size()>0)
            {
                erro+=", coluna "+QString::number(coluna)+", linha "+QString::number(linha);
                mensagem(erro);
            }
            arquivo.close();
        }
    }
}

void Janela::abrirArq()
{
    QString caminho = QFileDialog::getOpenFileName(this,tr("Abrir Arquivo de Texto"),m_scxml,
                                                   tr("*.txt(*.txt);;"));
                                                   
    if (!caminho.isEmpty()) {
        QFile arquivo(caminho);
        if (arquivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&arquivo);
            m_list->reseta();
            while (!in.atEnd()) {
                QString line = in.readLine();
                m_list->addItem(line);
            }
            m_list->resetaPosicao();
            arquivo.close();
        }
    }
}

void Janela::ajuda()
{
    QMessageBox::about(this, tr("Ajuda"),tr("Programa de interpretacao e representacao de diagramas de estado contidos em arquivos da linguagem Scxml\n"
                                            "Para abrir um diagrama de estados deve-se ir na aba Arquivo e acessar o submenu Abrir scxml\n"
                                            "Para abrir uma lista de eventos deve-se ir na Aba Arquivo e acessar o submenu Abrir texto\n"
                                            "Apos a abertura do arquivo era mostrada a situacao do texto\n"
                                            "Se desejar realizar a animacao do diagrama deve-se ter o diagram e a lista abertos\n"
                                            "Funcionalidade dos botoes:\n"
                                            "-> O botao < representa voltar\n"
                                            "-> O botao || para o modo auto\n"
                                            "-> O botao > avanca\n"
                                            "-> O botao >> inicia o auto"));
}

void Janela::proximo()
{

    if(m_par.first)
    {
        if(!m_par.second->pedido(Interface::Avancar))
        {
            mensagem("acao invalida, evento nao encontrado.");
            para();
        }
    }
    else
    {
        mensagem("acao invalida, diagrama nao presente");
    }
    if(m_list->fim()) para();
}

void Janela::anterior()
{

    if(m_par.first)
    {
        if(!m_par.second->pedido(Interface::Regredir))
        {
            mensagem("acao invalida, nao a como regredir.");
            para();
        }
    }
    else
    {
        mensagem("acao invalida, diagrama nao presente");
        para();
    }
}

void Janela::mensagem(QString msg)
{
    msg+="\t";
    QMessageBox::about(this, tr("Diagrama"),msg);
}
