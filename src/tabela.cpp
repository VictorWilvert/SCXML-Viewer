
#include "../include/tabela.h"

Tabela::Tabela(VistaLista *vista, Interface *prox)
    :   Interface(prox), m_lista{vista}
{
    if(!vista) throw std::invalid_argument("lista invalida");
}

Tabela::~Tabela()
{}

bool Tabela::pedido(Acao acao)
{
    if(m_lista->count()==0 || (acao==Interface::Avancar && m_lista->currentRow()==-1)) return false;
    if(acao==Interface::Avancar) proximo()->texto(m_lista->currentItem()->text());
    if(Interface::pedido(acao))
    {
        switch (acao)
        {
        case Avancar:
            avancar();
            break;
        case Regredir:
            regredir();
            break;
        default:
            break;
        }
        return true;
    }
    return false;
}

void Tabela::avancar()
{
    m_lista->avanca();
}

void Tabela::regredir()
{
    m_lista->regride();
}
