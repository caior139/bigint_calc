#ifndef NUMUTIL_H
#define NUMUTIL_H

#include "numero.h"

/*
 * Biblioteca de utilidades para o tipo abstrato Numero
 */

/*
 * Troca os ponteiros de dois números
 *
 * Input:
 *   struct Numero **ptrA : Ponteiro para o primeiro número
 *   struct Numero **ptrB : Ponteiro para o segundo número
 *
 * Output:
 *   VOID
 */
void numutil_troca_ponteiros(struct Numero **ptrA, struct Numero **ptrB);

/*
 * Remove blocos nulos à esquerda (normaliza o número)
 *
 * Input:
 *   struct Numero *num : Número a ser normalizado
 *
 * Output:
 *   VOID
 */
void numutil_normaliza(struct Numero *num);

/*
 * Compara dois números A e B
 *
 * Input:
 *   struct Numero *A : Primeiro número
 *   struct Numero *B : Segundo número
 *
 * Output:
 *   1   - Se A > B
 *   0   - Se A == B
 *  -1   - Se A < B
 *  -2   - Argumento NULL
 */
int numutil_compara(struct Numero *A, struct Numero *B);

/*
 * Verifica se o número é válido (memória, tamanho e ponteiro)
 *
 * Input:
 *   struct Numero *num : Número a ser verificado
 *
 * Output:
 *   1   - Se válido
 *   0   - Se inválido
 *  -2   - Argumento NULL
 */
int numutil_valida(struct Numero *num);


double numutil_estima_ln(struct Numero *num);

#endif 
