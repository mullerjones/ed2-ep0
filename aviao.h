#ifndef AVIAO_H
#define AVIAO_H

#include <string>

const std::string empresas[] = {"AM", "IP", "EN", "CB", "WU"};
const std::string portos[] = {
    "ATL", "PEK", "LAX", "HND", "DXB",
    "ORD", "LHR", "HKG", "CDG", "GRU",
    "DFW", "CAN", "ICN", "AMS", "FRA",
    "SIN", "BKK", "DEN", "DEL", "CGK",
    "JFK", "KUL", "MAD", "SFO", "CTU",
    "SZX", "MCO", "LAS", "BCN", "YYZ"
};

class Aviao
{
public:
    std::string codigo;
    std::string orig_dest;
    bool isPouso;

    int combustivel;
    int tVoo;
    
    int tEspera;
    bool emergencia;
    bool prioridade;
    Aviao *prox;
    Aviao *ant;
    bool foiMovido;

    void init(int seed, int C, int V);
    Aviao();
    ~Aviao();
    bool checaPrio(int C);
    bool checaEmerg();
};

Aviao::Aviao() {}
Aviao::~Aviao() {}

/*Inicializa um aviao a partir da semente do gerador aleatorio*/
void Aviao::init(int seed, int C, int V)
{
    srand(seed); /*Inicializa gerador aleatorio*/

    /*Cria codigo do voo*/

    int emp = rand() % 5;         /*Seleciona uma das 5 empresas*/
    codigo = "";                  /*Inicializa string vazia*/
    codigo.append(empresas[emp]); /*Concatena nela o codigo da empresa*/
    /*Gera um numero entre 0 e 9 aleatoriamente, o converte   */
    /*para char e adiciona no fim do codigo, repetindo 3 vezes*/
    int aux;
    for (int i = 0; i < 3; i++)
    {
        aux = rand() % 10;
        codigo.push_back((char)aux + 48);
    }
    /*Decide o aeroporto de origem/destino*/
    orig_dest = portos[rand()%30];

    /*Decide se eh pouso ou decolagem*/
    isPouso = rand()%2;
    
    /*Cria valor de combustivel ou tempo de voo*/
    if(isPouso)
    {
        combustivel = rand() % C + 1;
        tVoo = -1;
    }
    else
    {
        tVoo = rand() % V +1;
        combustivel = -1;
    }

    /*Decide se voo eh de emergencia*/
    /*Escolhi uma proporcao de voos de*/
    /*emergencia de 1 a cada 20 voos*/
    emergencia = !(rand()%20);
    prox = nullptr;
    ant = nullptr;
    foiMovido = false;
}

bool Aviao::checaPrio(int C)
{
    if(emergencia) return true;
    if(isPouso)
    {
        if(combustivel<C/5)
        {
            prioridade = true;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if(tEspera>=tVoo%10)
        {
            prioridade = true;
            return true;
        }
    }
    
}

bool Aviao::checaEmerg()
{
    return emergencia;
}

#endif
