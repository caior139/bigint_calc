#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "numero.h"
#include "numutil.h"

int numero_inicializa(struct Numero *num, uint64_t tamanho)
{
    if (num == NULL) return -2;

    if (num->blocos_ptr != NULL) numero_libera(num);

    if (tamanho == 0) tamanho = 1;

    num->blocos_ptr = (uint32_t *)calloc(tamanho, sizeof(uint32_t));
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

    uint64_t novo_tamanho = num->tamanho;
    if (novo_tamanho == 0) novo_tamanho = 1;

    if (novo_tamanho == num->alocado) return 0;

    uint32_t *novo_ptr = (uint32_t *)realloc(num->blocos_ptr, novo_tamanho * sizeof(uint32_t));
    if (novo_ptr == NULL) return -1;

    if (novo_tamanho > num->alocado)
        memset(novo_ptr + num->alocado, 0, (novo_tamanho - num->alocado) * sizeof(uint32_t));

    num->blocos_ptr = novo_ptr;
    num->alocado = novo_tamanho;
    if (num->tamanho > num->alocado) num->tamanho = num->alocado;

    return 0;
}

int numero_compacta(struct Numero *num)
{
    if (num == NULL || num->blocos_ptr == NULL) return -2;

    numutil_normaliza(num);

    uint64_t memoria_livre = num->alocado - num->tamanho;
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

int numero_set(struct Numero *num, uint64_t valor)
{
    if (num == NULL) return -1;

    uint64_t num_blocos = 0;
    uint64_t temporario = valor;

    do
    {
        num_blocos++;
        temporario /= BLOCO_BASE;
    } while (temporario > 0);

    int ret = numero_inicializa(num, num_blocos);
    if (ret != 0) return -2;

    for (uint64_t idx = 0; idx < num_blocos; idx++)
    {
        num->blocos_ptr[idx] = valor % BLOCO_BASE;
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
        memcpy(destino->blocos_ptr, origem->blocos_ptr, origem->tamanho * sizeof(uint32_t));

    destino->sinal = origem->sinal;

    return 0;
}
