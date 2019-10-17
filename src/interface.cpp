
#include "../include/interface.h"

Interface::Interface(Interface *proximo)
    :   m_proximo{proximo}
{}

bool Interface::pedido(Acao acao)
{
    if(m_proximo)
        return m_proximo->pedido(acao);
    else return false;
}
