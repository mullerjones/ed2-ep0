#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "main.h"
#include "aviao.h"
#include "fila.h"

void entradaManual(int *K, int *V, int *C, int *T, int *semente);
void geraNovos(int K, int C, int V, Fila *fil);
void pulaLinha();
void listaAvioes(Fila *fil);

int main(int argc, char **argv)
{
    int K, V, C, T, semente;
    int tempo, i;
    int pistas[3];

    pistas[0] = pistas[1] = pistas[2] = 0;

    double totalCombEsperando = 0.0;
    double totalCombSobrando = 0.0;
    int pousaram = 0;
    int decolaram = 0;
    double totalEsperaPouso = 0.0;
    double totalEsperaDecolagem = 0.0;
    int quantosEmergencia = 0;

    if (argc != 6)
    {
        std::cout << argc << std::endl;
        entradaManual(&K, &V, &C, &T, &semente);
    }
    else
    {
        K = atoi(argv[1]);
        V = atoi(argv[2]);
        C = atoi(argv[3]);
        T = atoi(argv[4]);
        semente = atoi(argv[5]);
    }

    srand(semente);

    Fila q;
    Aviao *aux;
    Aviao *aux2;
    aux = nullptr;
    aux2 = nullptr;

    /*Comeca simulacao*/
    for (tempo = 0; tempo < T; tempo++)
    {
        std::cout << "Simulacao para instante " << tempo << std::endl;
        std::cout << "\n";
        /*Novos avioes*/
        geraNovos(K, C, V, &q);

        pulaLinha();
        /*Reseta flag para novos movimentos*/
        for (aux = q.getPrimeiro(); aux != q.getUltimo(); aux = aux->prox)
        {
            aux->foiMovido = false;
        }

        /*Confere prioridades e ordena fila*/
        for (aux = q.getPrimeiro(); aux != q.getUltimo(); aux = aux->prox)
        {
            if (!(aux->foiMovido) && aux->checaPrio())
            {
                if (aux->checaEmerg())
                {
                    q.remove(aux);
                    q.insereIni(aux);
                    aux->foiMovido = true;
                }
                else
                {
                    aux2 = q.getPrimeiro();
                    while (aux2->checaEmerg())
                    {
                        aux2 = aux2->prox;
                    }
                    if (aux2 != aux)
                    {
                        q.remove(aux);
                        q.insereDepois(aux2, aux);
                        aux->foiMovido = true;
                    }
                }
            }
        }
        //Esvazia a fila o quanto possivel
        for (i = 0; i < 3; i++)
        {
            if (i != 3)
            {
                if (pistas[i] == 0)
                {
                    /*Tira primeiro da fila pra essa pista*/
                    aux = q.getPrimeiro();
                    q.remove(aux);
                    std::string pous;
                    if (aux->isPouso)
                    {
                        pous = " pousou na pista ";
                        pousaram++;
                        totalEsperaPouso += (double)aux->tEspera;
                        totalCombSobrando += (double)aux->combustivel;
                    }
                    else
                    {
                        pous = " decolou na pista ";
                        totalEsperaDecolagem += (double)aux->tEspera;
                        decolaram++;
                    }
                    std::cout << "Voo " << aux->codigo << pous << i + 1 << std::endl;
                    /*Coloca tempo de seguranca da pista*/
                    pistas[i] = 2;
                    /*Deleta aviao que ja foi processado*/
                    delete aux;
                    aux = nullptr;
                }
                else
                {
                    pistas[i]--;
                }
            }
            else
            {
                if (q.getPrimeiro()->checaEmerg())
                {
                    aux = q.getPrimeiro();
                    q.remove(aux);
                    std::string pous;
                    if (aux->isPouso)
                    {
                        pous = " pousou na pista ";
                        pousaram++;
                        totalEsperaPouso += (double)aux->tEspera;
                        totalCombSobrando += (double)aux->combustivel;
                    }
                    else
                    {
                        pous = " decolou na pista ";
                        totalEsperaDecolagem += (double)aux->tEspera;
                        decolaram++;
                    }
                    std::cout << "Voo " << aux->codigo << pous << i + 1 << " por uma emergencia" << std::endl;
                    /*Coloca tempo de seguranca da pista*/
                    pistas[i] = 2;
                    /*Deleta aviao que ja foi processado*/
                    delete aux;
                    aux = nullptr;
                    quantosEmergencia++;
                }
                else
                {
                    aux2 = q.getPrimeiro();
                    while (aux2->isPouso && aux2 != nullptr)
                    {
                        aux2 = aux2->prox;
                    }
                    //aux2 tem o primeiro de decolagem (se ha algum)
                    if (aux2 != nullptr)
                    {
                        q.remove(aux2);
                        std::string pous;
                        pous = " decolou na pista ";
                        std::cout << "Voo " << aux->codigo << pous << i + 1 << std::endl;
                        decolaram++;
                        totalEsperaDecolagem += (double)aux2->tEspera;
                        /*Coloca tempo de seguranca da pista*/
                        pistas[i] = 2;
                        /*Deleta aviao que ja foi processado*/
                        delete aux2;
                        aux2 = nullptr;
                    }
                }
            }
        }

        pulaLinha();

        //Checa se restou voo emergencial na fila
        //Se sim, redireciona
        aux2 = q.getPrimeiro();
        while (aux2->emergencia)
        {
            //Manda pra outro
            q.remove(aux2);
            std::string pous;
            pous = " redirecionado para outro aeroporto por uma emergencia";
            std::cout << "Voo " << aux->codigo << pous << std::endl;
            /*Coloca tempo de seguranca da pista*/
            /*Deleta aviao que ja foi processado*/
            delete aux2;
            /*Verifica do comeco da fila novamente*/
            aux2 = q.getPrimeiro();
        }
        pulaLinha();

        /*Diminui em 1 o combustivel de todos os avioes na fila*/
        /*Aumenta em 1 o tempo de espera de cada aviao*/
        for (aux = q.getPrimeiro(); aux != nullptr; aux = aux->prox)
        {
            aux->combustivel--;
            aux->tEspera++;
        }

        /*SAIDAS PEDIDAS*/
        /*Lista de avioes esperando*/
        listaAvioes(&q);
        /*Medias de espera*/
        std::cout << "Media de espera para pouso = " << totalEsperaPouso / pousaram << std::endl;
        std::cout << "Media de espera para decolagem = " << totalEsperaDecolagem / decolaram << std::endl;
        /*Media de combustivel em espera*/
        totalCombEsperando = 0.0;
        i = 0;
        for (aux = q.getPrimeiro(); aux != nullptr; aux = aux->prox)
        {
            if (aux->isPouso)
            {
                totalCombEsperando += (double)aux->combustivel;
                i++;
            }
        }
        std::cout << "Media de combustivel dos avioes em espera = " << totalCombEsperando/i << std::endl;
        /*Media de combustivel nos que desceram*/
        std::cout << "Media de combustivel dos avioes que pousaram = " << totalCombSobrando/pousaram << std::endl;
        /*Quantos em emergencia*/
        std::cout << "Total de pousos de emergencia = " << quantosEmergencia << std::endl;
    }
    return 0;
}

void entradaManual(int *K, int *V, int *C, int *T, int *semente)
{

    std::cout << "Insira tempo maximo de simulacao (T): " << std::endl;
    std::cin >> *T;
    std::cout << "Insira maximo de combustivel (C): " << std::endl;
    std::cin >> *C;
    std::cout << "Insira numero maximo de avioes em comunicao (K): " << std::endl;
    std::cin >> *K;
    std::cout << "Insira maximo para tempo estimado de voo (V): " << std::endl;
    std::cin >> *V;
    std::cout << "Insira a semente para o gerador de numeros aleatorios: " << std::endl;
    std::cin >> *semente;
}

void geraNovos(int K, int C, int V, Fila *fil)
{
    int i, numNovos;
    numNovos = rand() % K;
    Aviao *aux;
    std::cout << numNovos << " novos avioes entraram em contato" << std::endl;
    for (i = 0; i < numNovos; i++)
    {
        aux = new Aviao();       //Cria novo aviao
        aux->init(rand(), C, V); //Inicializa esse aviao
        if (fil->total() == 0)
            fil->inserePrimeiro(aux);
        else
            fil->insereFim(aux); //O insere na lista de avioes
        std::cout << "Voo " << aux->codigo << " entrou em contato com a torre" << std::endl;
        aux = nullptr; //Limpa apontador auxiliar para proximo aviao
    }
}

void pulaLinha()
{
    std::cout << "\n";
}

void listaAvioes(Fila *fil)
{
    Aviao *aux;
    std::cout << "Lista dos avioes em espera:" << std::endl;
    std::string decpouso = "";
    for (aux = fil->getPrimeiro(); aux != nullptr; aux = aux->prox)
    {
        if (aux->isPouso)
            decpouso = " - pouso";
        else
            decpouso = " - decolagem";
        std::cout << aux->codigo << decpouso << std::endl;
    }
}

/*Foi*/