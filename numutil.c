#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "numero.h"

void numutil_troca_ponteiros(struct Numero **num1, struct Numero **num2)
{
    struct Numero *temporario = *num1;
    *num1 = *num2;
    *num2 = temporario;
}

void numutil_normaliza(struct Numero *num)
{
    if (num == NULL || num->tamanho == 0)
        return;

    uint64_t novo_tamanho = num->tamanho;
    while (novo_tamanho > 0 && num->blocos_ptr[novo_tamanho - 1] == 0)
        novo_tamanho--;

    if (novo_tamanho == 0)
    {
        num->tamanho = 1;
        num->blocos_ptr[0] = 0;
        num->sinal = 0;
        return;
    }

    if (novo_tamanho < num->tamanho)
        num->tamanho = novo_tamanho;
}

int numutil_compara(const struct Numero *num1, const struct Numero *num2)
{
    if (num1 == NULL || num2 == NULL)
        return -2;

    if (num1->tamanho > num2->tamanho)
        return 1;
    if (num1->tamanho < num2->tamanho)
        return -1;

    for (uint64_t indice = num1->tamanho; indice-- > 0;)
    {
        if (num1->blocos_ptr[indice] > num2->blocos_ptr[indice])
            return 1;
        if (num1->blocos_ptr[indice] < num2->blocos_ptr[indice])
            return -1;
    }

    return 0;
}

int numutil_valida(const struct Numero *num)
{
    if (num == NULL)
        return 0;
    if (num->blocos_ptr == NULL)
        return 0;
    if (num->tamanho > num->alocado)
        return 0;
    return 1;
}

double numutil_estima_ln(const struct Numero *num)
{
    if (num == NULL)
        return -1;

    uint64_t tam = num->tamanho;

    if (num->sinal == 0 || tam == 0)
        return 0.0;

    uint64_t blk = num->blocos_ptr[tam - 1];

    uint64_t temp = blk;
    int blk_dig = 1;
    while (temp >= 10ULL)
    {
        temp /= 10ULL;
        blk_dig++;
    }

    double mantissa;
    double exp10;

    if (tam == 1)
    {
        exp10 = (double)(blk_dig - 1);
        mantissa = (double)blk / pow(10.0, blk_dig - 1);

        return log(mantissa) + exp10 * log(10.0);
    }

    uint64_t blk2 = num->blocos_ptr[tam - 2];
    double comb = (double)blk * 1e8 + (double)blk2;

    int comb_dig = blk_dig + DIGITOS_BLOCO;

    mantissa = comb / pow(10.0, comb_dig - 1);

    uint64_t total_dig = (tam - 1) * DIGITOS_BLOCO + blk_dig;

    exp10 = (double)(total_dig - 1);

    return log(mantissa) + exp10 * log(10.0);
}
