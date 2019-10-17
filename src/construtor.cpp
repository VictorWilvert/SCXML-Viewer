 
#include "../include/construtor.h"

Construtor::Construtor(QDomDocument& documento)
    :   m_documento{documento}, m_raiz{nullptr}
{}

void Construtor::leitura()
{
    m_erro.clear();
    auto documento = m_documento.documentElement();
    if(documento.tagName()!="scxml")
    {
        m_erro=QString("tipo de XML invalido");
        return;
    }
    leElemento(documento,nullptr);
    leTransicao(documento,nullptr);
    valida(m_raiz);
    if(m_erro.size()==0) m_erro=QString("arquivo nao apresenta nenhum erro");
}

QString Construtor::leElemento(QDomElement elemento,Item* parente)
{
    QString valor;
    Item* auxiliar=nullptr;
    //  leitura dos estados que seram representados
    if(elemento.tagName()=="scxml") 
    {
        auxiliar=new Base(parente);
        m_raiz=auxiliar;
    }
    else if(elemento.tagName()=="state" || elemento.tagName()=="history")
    {
        if(elemento.hasAttribute("id"))
        {
            if(elemento.tagName()=="state") auxiliar=new Bloco(parente);
            else if(elemento.tagName()=="history") auxiliar=new Historia(parente);
            auxiliar->nome(elemento.attribute("id"));
        }
        else m_erro+=elemento.tagName()+" invalido.\n";
    }
    else if(elemento.tagName()=="initial" || elemento.tagName()=="final")
    {
        auxiliar=new Bola(parente);
        if(elemento.tagName()=="final" && elemento.hasAttribute("id"))
            auxiliar->nome(elemento.attribute("id"));
        else  auxiliar->nome(elemento.tagName());
    }
    if(auxiliar)
    {
        QString atributos=leAtributos(elemento);
        if(parente) parente->insereFilho(auxiliar);
        m_itens.prepend(auxiliar);
        auto no=elemento.firstChild();
        while(!no.isNull()) 
        {
            atributos+=leElemento(no.toElement(),auxiliar);
            no=no.nextSibling();
        }
        auxiliar->valor(atributos);
        if(elemento.hasAttribute("initial"))
        {
            if(elemento.tagName()=="scxml" || elemento.tagName()=="state")
            {
                if(auxiliar->tamanhoFilhos()!=0)
                {
                    int i;
                    for(i=0;i<auxiliar->tamanhoTotal();i++)
                    {
                        if(auxiliar->acessaFilho(i)->nome()==elemento.attribute("initial"))
                        {
                            auxiliar->acessaFilho(i)->initial(true);
                            auxiliar->ajuste();
                            break;
                        }
                    }
                }
                else m_erro+="atributo de initial invalido";
            }
            else m_erro+="atributo de initial invalido";
        }
        else if(elemento.tagName()=="state" || elemento.tagName()=="scxml")
        {
            //  definindo o estado inicial se nao tiver predefinido
            if(auxiliar->tamanhoFilhos()!=0)
            for(int i=0;i<auxiliar->tamanhoFilhos();i++)
            {
                if(auxiliar->acessaFilho(i)->tipo()=="bloco")
                {
                    auxiliar->acessaFilho(auxiliar->tamanhoFilhos()-1-(i))->initial(true);
                    break;
                }
            }
        }
    }
    else
    {
        valor+='\n'+elemento.tagName()+" :";
        valor+=leAtributos(elemento);
        auto no=elemento.firstChild();
        while(!no.isNull())
        {
            valor+=leTexto(no.toElement());
            no=no.nextSibling();
        }
    }
    return valor;
}

QString Construtor::leTexto(QDomElement elemento)
{
    QString retorno;
    retorno+='\n'+elemento.tagName()+" :";
    retorno+=leAtributos(elemento);
    auto no=elemento.firstChild();
    while(!no.isNull())
    {
        retorno+=leTexto(no.toElement());
        no=no.nextSibling();
    }
    return retorno;
}

QString Construtor::leAtributos(QDomElement elemento)
{
    QString retorno;
    auto atributos=elemento.attributes();
    for(int i=0;i<atributos.count();i++)
    {
        auto atributo=atributos.item(i);
        retorno+=atributo.nodeName()+"=\""+atributo.nodeValue()+"\", ";
    }
    return retorno;
}

void Construtor::leTransicao(QDomElement elemento,Item *parente)
{
    if(elemento.tagName()=="transition")
    {
        if(elemento.hasAttribute("target"))
        {
            Item *ptr=busca(m_raiz,elemento.attribute("target"));
            if(ptr)
            {
                auto auxiliar=new Seta(parente,ptr);
                parente->insereFilho(auxiliar);
                auxiliar->valor(elemento.attribute("event"));
                m_itens.prepend(auxiliar);
            }
        }
        else m_erro+=elemento.tagName()+" invalida. ";
    }
    else
    {
        auto nome=elemento.tagName();
        if(nome=="state" || nome=="scxml" || nome=="initial" || nome=="history")
        {
            if(!parente) parente=m_raiz;
            else
            {
                if(nome=="initial")
                {
                    for(int i=0;i<parente->tamanhoFilhos();i++)
                    {
                        if(parente->acessaFilho(i)->nome()=="initial")
                        {
                            parente=parente->acessaFilho(i);
                            break;
                        }
                    }
                }
                else parente=busca(m_raiz,elemento.attribute("id"));
            }
            auto no=elemento.firstChild();
            while(!no.isNull()) 
            {
                nome=no.toElement().tagName();
                leTransicao(no.toElement(),parente);
                no=no.nextSibling();
            }
        }
    }
}

Item* Construtor::busca(Item* no,QString nome)
{
    if(!no) return nullptr;
    if(no->nome()==nome) return no;
    else
    {
        for(int i=0;i<no->tamanhoFilhos();i++)
        {
            auto aux=busca(no->acessaFilho(i),nome);
            if(aux) return aux;
        }
        return nullptr;
    }
}

bool Construtor::valida(Item *no)
{
    if(no->tipo()=="base")
    {
        for(int i=0;i<no->tamanhoTotal();i++)
        {
            if(no->acessaFilho(i)->nome()=="initial")
            {
                deleta(no->acessaFilho(i));
                m_erro+="scxml invalido, possui um inicial. ";
                return true;
            }
            if(valida(no->acessaFilho(i)))
                i--;
        }
    }
    else if(no->tipo()=="bloco")
    {
        int cont=0;
        //  removendo states com nomes invalidos
        if(no->nome()=="initial")
        {
            deleta(no);
            m_erro+="estado invalido, nome invalido. ";
            return true;
        }
        for(int i=0;i<no->tamanhoTotal();i++)
        {
            //  removendo states com mais de um inicial
            if(no->acessaFilho(i)->nome()=="initial")
            {
                for(int j=0;j<no->tamanhoFilhos();j++)
                {
                    if(no->acessaFilho(j)->initial())
                    {
                        no->acessaFilho(j)->initial(false);
                    }
                }
                cont++;
            }
            if(cont>1)
            {
                deleta(no->acessaFilho(i));
                m_erro+="initial no state "+no->nome()+" invalido, multiplas declaracoes de initial. ";
                cont--;
                return true;
            }
            //  removendo transicoes sem eventos dentro de estados
            if(no->acessaFilho(i)->tipo()=="seta")
            {
                if(no->acessaFilho(i)->valor().size()==0)
                {
                    deleta(no->acessaFilho(i));
                    m_erro+="transicao invalida, sem evento. ";
                    return true;
                }
            }
            if(valida(no->acessaFilho(i)))
                i--;
        }
    }
    else if(no->tipo()=="bola")
    {
        //  removendo inicial sem transicao e final com transicao
        if((no->nome()=="initial" && no->tamanhoTotal()!=1) || (no->nome()=="final" && no->tamanhoTotal()!=0))
        {
            deleta(no);
            m_erro+=no->nome()+" invalido. ";
            return true;
        }
        //  removendo incial com algo alem de transicao
        if(no->nome()=="initial" && no->acessaFilho(0)->tipo()!="seta")
        {
            deleta(no);
            m_erro+=no->nome()+" invalido, filho que nao e transicao. ";
            return true;
        }

    }
    else if(no->tipo()=="historia")
    {
        if(no->tamanhoTotal()==0)
        {
            auto temp=new Seta(no,nullptr);
            no->insereFilho(temp);
            m_itens.prepend(temp);

        }
        //  removendo history com coisas alem de uma transicao
        else if(no->tamanhoTotal()!=1)
        {
            deleta(no);
            m_erro+="history invalido, possui multiplos filhos. ";
            return true;
        }
        //  removendo history com algo diferente de transicao
        if(no->tamanhoTotal()==1 && no->acessaFilho(0)->tipo()!="seta")
        {
            deleta(no);
            m_erro+=no->nome()+" invalido, possui um filho que nao e transicao. ";
            return true;
        }
    }
    else
    {
        //  remove setas que apontam para initial
        if(no->parente(1)->nome()=="initial")
        {
            deleta(no);
            m_erro+="transition invalido, aponta para inicial. ";
            return true;
        }
        return false;
    }
    return false;
}

void Construtor::buscaTransicao(Item *no,QString nome_fim)
{
    //  buscando uma transicao e a removendo
    if(!no) return;
    else if(no->tipo()=="seta")
    {
        if(no->parente(1)->nome()==nome_fim)
        {
            no->parente(0)->removeFilho(no);
            deleta(no);
        }
    }
    else
    {
        for(int i=0;i<no->tamanhoTotal();i++)
            buscaTransicao(no->acessaFilho(i),nome_fim);
    }
}

void Construtor::deleta(Item *no)
{
    //  deletando um no e seus filhos
    if(!no) return;
    else
    {
        if(no->tipo()!="seta")
        {
            while(no->tamanhoTotal())
                deleta(no->acessaFilho(0));
        }
        buscaTransicao(m_raiz,no->nome());
        no->parente(0)->removeFilho(no);
        m_itens.removeOne(no);
        delete no;
    }
}

