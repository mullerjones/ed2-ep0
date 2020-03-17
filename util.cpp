#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "main.h"
#include "aviao.h"

void insereNoInicio(Aviao cab, Aviao novo)
{
    novo.prox = cab.prox;
    cab.prox = novo;
}