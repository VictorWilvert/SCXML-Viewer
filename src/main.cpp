 
#include "../include/janela.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    Janela janela;
    // mostra a jaanela
    janela.show();
    return app.exec();
}
