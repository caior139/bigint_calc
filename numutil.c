#include <stdio.h>
#include <stdlib.h>
#include "numero.h"

void numutil_troca_ponteiros(struct Numero **num1, struct Numero **num2)
{
    struct Numero *temporario = *num1;
    *num1 = *num2;
    *num2 = temporario;
}

void numutil_normaliza(struct Numero *num)
{
    if (num == NULL || num->tamanho == 0) return;

    unsigned long long novo_tamanho = num->tamanho;
    while (novo_tamanho > 0 && num->blocos_ptr[novo_tamanho - 1] == 0)
        novo_tamanho--;

    if (novo_tamanho == 0)
    {
        num->tamanho = 1;
        num->blocos_ptr[0] = 0;
        num->sinal = 0;
        return;
    }

    if (novo_tamanho < num->tamanho) num->tamanho = novo_tamanho;
}

int numutil_compara(struct Numero *num1, struct Numero *num2)
{
    if (num1 == NULL || num2 == NULL) return -2;

    if (num1->tamanho > num2->tamanho) return 1;
    if (num1->tamanho < num2->tamanho) return -1;

    for (unsigned long long indice = num1->tamanho; indice-- > 0;)
    {
        if (num1->blocos_ptr[indice] > num2->blocos_ptr[indice]) return 1;
        if (num1->blocos_ptr[indice] < num2->blocos_ptr[indice]) return -1;
    }

    return 0;
}

int numutil_valida(struct Numero *num)
{
    if (num == NULL) return 0;
    if (num->blocos_ptr == NULL) return 0;
    if (num->tamanho > num->alocado) return 0;
    return 1;
}
