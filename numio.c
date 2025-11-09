#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "numio.h"
#include "numero.h"

int numio_gera_numero(struct Numero *num, unsigned long long num_blocos, int seed)
{
    if (num == NULL)
        return -2;

    if (seed == 0)
        srand((unsigned int)time(NULL));
    else
        srand((unsigned int)seed);

    if (numero_inicializa(num, num_blocos) != 0)
        return -1;

    unsigned long long i;
    for (i = 0; i < num_blocos - 1; i++)
        num->blocos_ptr[i] = (unsigned long long)(rand() % BLOCO_BASE);

    num->blocos_ptr[num_blocos - 1] = (unsigned long long)((rand() % (BLOCO_BASE - 1)) + 1);

    return 0;
}

int numio_print_numero(struct Numero *num)
{
    if (num == NULL)
        return -2;

    if (num->tamanho == 0)
    {
        printf("0\n");
        return 0;
    }

    if (num->sinal == -1)
        printf("-");
    printf("%llu", num->blocos_ptr[num->tamanho - 1]);

    unsigned long long i;
    for (i = num->tamanho - 1; i-- > 0;)
    {
        printf("%0*llu", DIGITOS_BLOCO, num->blocos_ptr[i]);
    }

    printf("\n");
    return 0;
}

unsigned long long numio_str_para_numero(const char *str, unsigned long long tam_str)
{
    unsigned long long valor = 0;

    unsigned long long i;
    for (i = 0; i < tam_str; i++)
        valor = valor * 10 + (str[i] - '0');

    return valor;
}

int numio_le_numero_txt(const char *caminho, struct Numero *num)
{
    if (caminho == NULL || num == NULL)
        return -2;

    FILE *fptr = fopen(caminho, "r");
    if (fptr == NULL)
        return -2;

    unsigned long long numero_digitos = 0;
    int c;

    while ((c = fgetc(fptr)) != EOF)
        if (isdigit(c))
            numero_digitos++;

    if (numero_digitos == 0)
    {
        fclose(fptr);
        return -2;
    }

    rewind(fptr);

    unsigned long long num_blocos = (numero_digitos + DIGITOS_BLOCO - 1) / DIGITOS_BLOCO;

    if (numero_inicializa(num, num_blocos) != 0)
    {
        fclose(fptr);
        return -1;
    }

    char bloco_texto[DIGITOS_BLOCO + 1];
    unsigned long long idxNUM = num->tamanho;
    unsigned long long digitos_restantes = numero_digitos;

    while (digitos_restantes > 0)
    {
        unsigned long long ler = (digitos_restantes >= DIGITOS_BLOCO) ? DIGITOS_BLOCO : digitos_restantes;
        unsigned long long i = 0;

        while (i < ler)
        {
            c = fgetc(fptr);
            if (isdigit(c))
                bloco_texto[i++] = c;
        }
        bloco_texto[ler] = '\0';

        num->blocos_ptr[--idxNUM] = numio_str_para_numero(bloco_texto, ler);
        digitos_restantes -= ler;
    }

    fclose(fptr);
    return 0;
}

