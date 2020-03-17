#ifndef FILA_H
#define FILA_H

#include "aviao.h"

class Fila
{
    private:
        Aviao* primeiro;
        Aviao* ultimo;
        int quantos;
    
    public:
        Fila()
        {
            primeiro = nullptr;
            ultimo = nullptr;
            quantos = 0;
        }
        ~Fila();
        Aviao* getPrimeiro();
        Aviao* getUltimo();
        int total();

        void remove(Aviao* av);
        void insereIni(Aviao* av);
        void insereFim(Aviao* av); 
        void insereDepois(Aviao* ref, Aviao* av);
        void inserePrimeiro(Aviao* av);
};

Fila::~Fila() {}


Aviao* Fila::getPrimeiro()
{
    return primeiro;
}

Aviao* Fila::getUltimo()
{
    return ultimo;
}

int Fila::total()
{
    return quantos;
}

void Fila::remove(Aviao* av)
{
    if(av == primeiro) primeiro = av->prox;
    if(av == ultimo) ultimo = av->ant;
    if(av->prox != nullptr) av->prox->ant = av->ant;
    if(av->ant != nullptr) av->ant->prox = av->prox;
    av->prox = nullptr;
    av->ant = nullptr;
    quantos -= 1;
    return;
}

void Fila::insereIni(Aviao* av)
{
    av->prox = primeiro;
    primeiro->ant = av;
    av->ant = nullptr;
    primeiro = av;
    quantos++;
    return;
}
void Fila::insereFim(Aviao* av)
{
    ultimo->prox = av;
    av->ant = ultimo;
    av->prox = nullptr;
    ultimo = av;
    quantos++;
    return;
}

void Fila::insereDepois(Aviao* ref, Aviao* av)
{
    av->prox = ref->prox;
    av->ant = ref;
    if(ref->prox!=nullptr) ref->prox->ant = av;
    ref->prox = av;
    quantos++;
}

void Fila::inserePrimeiro(Aviao* av)
{
    primeiro = av;
    ultimo = av;
    quantos++;
}

#endif