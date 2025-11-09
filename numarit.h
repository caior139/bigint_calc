#ifndef NUMARIT_H
#define NUMARIT_H

#include "numero.h"

/*
 * Biblioteca de operações aritméticas para o tipo abstrato Numero
 */

/*
 * Soma A + B e armazena o resultado em S
 *
 * Input:
 *   struct Numero *A : Primeiro operando
 *   struct Numero *B : Segundo operando
 *   struct Numero *S : Resultado da soma
 *
 * Output:
 *   0   - Sucesso
 *  -1   - Falha de alocação
 *  -2   - Argumento NULL
 */u
int numarit_soma(struct Numero *A, struct Numero *B, struct Numero *S);

/*
 * Subtrai A - B e armazena o resultado em S
 *
 * Input:
 *   struct Numero *A : Minuendo
 *   struct Numero *B : Subtraendo
 *   struct Numero *S : Resultado da subtração
 *
 * Output:
 *   0   - Sucesso
 *  -2   - Argumento NULL
 */
int numarit_subtracao(struct Numero *A, struct Numero *B, struct Numero *S);

/*
 * Multiplica A * B e armazena o resultado em M
 *
 * Input:
 *   struct Numero *A : Primeiro operando
 *   struct Numero *B : Segundo operando
 *   struct Numero *M : Resultado da multiplicação
 *
 * Output:
 *   0   - Sucesso
 *  -1   - Falha de alocação
 *  -2   - Argumento NULL
 */
int numarit_multiplicacao(struct Numero *A, struct Numero *B, struct Numero *M);

/*
 * Multiplica A por um valor unsigned long long b e armazena o resultado em M
 *
 * Input:
 *   struct Numero *A : Número a ser multiplicado
 *   unsigned long long b : Multiplicador
 *   struct Numero *M : Resultado da multiplicação
 *
 * Output:
 *   0   - Sucesso
 *  -1   - Falha de alocação
 *  -2   - Argumento NULL
 */
int numarit_multiplicacao_por_ull(struct Numero *A, unsigned long long b, struct Numero *M);

/*
 * Divide A por um valor unsigned long long div e armazena o quociente em Q
 *
 * Input:
 *   struct Numero *A : Número a ser dividido
 *   unsigned long long div : Divisor
 *   struct Numero *Q : Resultado (quociente)
 *
 * Output:
 *   0   - Sucesso
 *  -1   - Falha de alocação
 *  -2   - Argumento NULL
 *  -3   - Divisor zero
 */
long long numarit_divisao_por_ull(struct Numero *A, unsigned long long div, struct Numero *Q);

/*
 * Divide A por B usando o algoritmo de Knuth (divisão longa)
 *
 * Input:
 *   struct Numero *A : Dividendo
 *   struct Numero *B : Divisor
 *   struct Numero *Q : Quociente resultante
 *   struct Numero *R : Resto resultante
 *
 * Output:
 *   0   - Sucesso
 *  -1   - Falha de alocação
 *  -2   - Argumento NULL
 *  -3   - Divisor zero
 */
int numarit_divisao_knuth(struct Numero *A, struct Numero *B, struct Numero *Q, struct Numero *R);

#endif
