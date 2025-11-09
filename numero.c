#include <stdlib.h>
#include <string.h>
#include "numero.h"
#include "numutil.h"

int numero_inicializa(struct Numero *num, unsigned long long tamanho)
{
    if (num == NULL) return -2;

    if (num->blocos_ptr != NULL) numero_libera(num);

    if (tamanho == 0) tamanho = 1;

    num->blocos_ptr = (unsigned long long *)calloc(tamanho, sizeof(unsigned long long));
    if (num->blocos_ptr == NULL) return -1;

    num->tamanho = tamanho;
    num->alocado = tamanho;
    num->sinal = 0;

    return 0;
}

void numero_libera(struct Numero *num)
{
    if (num == NULL) return;

    if (num->blocos_ptr != NULL)
    {
        free(num->blocos_ptr);
        num->blocos_ptr = NULL;
    }

    num->tamanho = 0;
    num->alocado = 0;
    num->sinal = 0;
}

int numero_resize(struct Numero *num)
{
    if (num == NULL) return -2;

    unsigned long long novo_tamanho = num->tamanho;
    if (novo_tamanho == 0) novo_tamanho = 1;

    if (novo_tamanho == num->alocado) return 0;

    unsigned long long *novo_ptr = (unsigned long long *)realloc(num->blocos_ptr, novo_tamanho * sizeof(unsigned long long));
    if (novo_ptr == NULL) return -1;

    if (novo_tamanho > num->alocado)
        memset(novo_ptr + num->alocado, 0, (novo_tamanho - num->alocado) * sizeof(unsigned long long));

    num->blocos_ptr = novo_ptr;
    num->alocado = novo_tamanho;
    if (num->tamanho > num->alocado) num->tamanho = num->alocado;

    return 0;
}

int numero_compacta(struct Numero *num)
{
    if (num == NULL || num->blocos_ptr == NULL) return -2;

    numutil_normaliza(num);

    unsigned long long memoria_livre = num->alocado - num->tamanho;
    if (memoria_livre > num->alocado / 2)
        return numero_resize(num);

    return 0;
}

void numero_cria_vazio(struct Numero *num)
{
    if (num == NULL) return;

    num->blocos_ptr = NULL;
    num->tamanho = 0;
    num->alocado = 0;
    num->sinal = 0;
}

int numero_set(struct Numero *num, unsigned long long valor)
{
    if (num == NULL) return -1;

    unsigned long long num_blocos = 0;
    unsigned long long temporario = valor;

    do
    {
        num_blocos++;
        temporario /= BLOCO_BASE;
    } while (temporario > 0);

    int ret = numero_inicializa(num, num_blocos);
    if (ret != 0) return -2;

    for (unsigned long long indice = 0; indice < num_blocos; indice++)
    {
        num->blocos_ptr[indice] = valor % BLOCO_BASE;
        valor /= BLOCO_BASE;
    }

    numutil_normaliza(num);
    if (num->blocos_ptr[0] == 0 && num->tamanho == 1)
        num->sinal = 0;
    else
        num->sinal = 1;

    return 0;
}

int numero_copia(const struct Numero *origem, struct Numero *destino)
{
    if (origem == NULL || destino == NULL) return -1;
    if (origem == destino) return 0;

    int ret = numero_inicializa(destino, origem->tamanho);
    if (ret != 0) return -2;

    if (origem->tamanho > 0)
        memcpy(destino->blocos_ptr, origem->blocos_ptr, origem->tamanho * sizeof(unsigned long long));

    destino->sinal = origem->sinal;

    return 0;
}
