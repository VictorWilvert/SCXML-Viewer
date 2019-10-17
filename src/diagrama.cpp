
#include "../include/diagrama.h"

Diagrama::Diagrama(Item* initial,Interface* prox)
    :   Interface(prox), m_atual{initial}
{
    if(!m_atual || m_atual->tipo()!="base") throw std::invalid_argument("atual invalido");
    //  busca o primeiro estado
    buscaInicio(m_atual);
    if(m_atual->nome().size()==0)
        throw std::invalid_argument("sem estado inicial");
    m_atual->ativo(true);
}

Diagrama::~Diagrama()
{}

void Diagrama::buscaInicio(Item *no)
{
    if(!no) return;
    //  caso se um estado initial
    if(no->nome()=="initial")
    {
        if(no->acessaFilho(0)->parente(1)->tipo()=="historia")
        {
            auto auxiliar=no->acessaFilho(0)->parente(1);
            while(auxiliar->tipo()=="historia")
                auxiliar=auxiliar->acessaFilho(0)->parente(1);
            if(auxiliar->tipo()=="bloco")
                m_atual=auxiliar;
            else
                throw std::invalid_argument("sem estado inicial");

        }
        else if(no->acessaFilho(0)->parente(1)->tipo()=="bloco")
        {
            m_atual=no->acessaFilho(0)->parente(1);
        }
        else
            throw std::invalid_argument("sem estado inicial");
    }
    //  caso o estado possua initial
    else if(no->initial())
    {
        if(no->tamanhoFilhos()>0)
        {
            for(int i=0;i<no->tamanhoFilhos();i++)
                buscaInicio(no->acessaFilho(i));
            return;
        }
        m_atual=no;
    }
    //  caso nao seja um no final
    else if(no->tamanhoFilhos()>0)
    {
        for(int i=0;i<no->tamanhoFilhos();i++)
            buscaInicio(no->acessaFilho(i));
    }
}

bool Diagrama::pedido(Acao acao)
{
    try
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
    }
    catch(...)
    {
        return false;
    }
    return true;
}

void Diagrama::avancar()
{
    if(!m_atual) throw std::domain_error("avanco invalido");
    for(int i=0;i<m_atual->tamanhoTotal();i++)
    {
        auto temp=m_atual->acessaFilho(i);
        if(temp->tipo()=="seta")
        {
            if(temp->valor()==texto())
            {
                m_anterior.append(m_atual);
                m_atual=temp->parente(1);
                m_atual->ativo(true);
                ajusta();
                return;
            }
        }
    }
    auto temp1=m_atual->parente(0);
    while(temp1)
    {
        for(int i=0;i<temp1->tamanhoTotal();i++)
        {
            auto temp=temp1->acessaFilho(i);
            if(temp->tipo()=="seta")
            {
                if(temp->valor()==texto())
                {
                    m_anterior.append(m_atual);
                    m_atual=temp->parente(1);
                    m_atual->ativo(true);
                    ajusta();
                    return;
                }
            }
        }  
        temp1=temp1->parente(0);
    }
    throw std::domain_error("avanco invalida");
}

void Diagrama::regredir()
{
    if(!m_atual) throw std::domain_error("regressao invalida");
    if(m_anterior.isEmpty())
    {
         throw std::domain_error("regressao invalida");;
    }
    else
    {
        m_atual->ativo(false);
        m_atual=m_anterior.back();
        m_anterior.pop_back();
        m_atual->ativo(true);
    }
}

void Diagrama::ajusta()
{
    if(!m_atual) return; // erro
    if(m_atual->tipo()=="historia")
    {
        //  como ele so pode ter um filho e ele deve ser seta
        auto temp=m_atual->acessaFilho(0);
        if(temp->parente(1))
        {
            m_atual=temp->parente(1);
            m_atual->ativo(true);
        }
    }

    if(m_atual->tamanhoFilhos()>0)
    {
        //  se tiver uma transicao para filho
        bool cond=false;
        int valor=m_atual->tamanhoTotal()-m_atual->tamanhoFilhos();
        for(int i=0;i<valor;i++)
        {
            auto temp=m_atual->acessaFilho(m_atual->tamanhoTotal()-1-i)->parente(1);
            for(int j=0;j<m_atual->tamanhoFilhos();j++)
            {
                if(temp==m_atual->acessaFilho(j))
                {
                    cond=true;
                    break;
                }
            }
        }
        if(!cond)
        {
            for(int j=0;j<m_atual->tamanhoFilhos();j++)
            {
                if(m_atual->acessaFilho(j)->initial())
                {
                    m_atual->ativo(false);
                    m_atual=m_atual->acessaFilho(j);
                    m_atual->ativo(true);
                    break;
                }
                if(m_atual->acessaFilho(j)->nome()=="initial")
                {
                    m_atual->ativo(false);
                    m_atual=m_atual->acessaFilho(j)->acessaFilho(0)->parente(1);
                    m_atual->ativo(true);
                    break;
                }
            }
        }

    }
    if(m_atual!=m_anterior.back())
        m_anterior.back()->ativo(false);
}
