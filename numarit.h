#ifndef NUMARIT_H
#define NUMARIT_H

#include "numero.h"

/*
 * Biblioteca de operações aritméticas para o tipo abstrato Numero.
 * Fornece funções para soma, subtração, multiplicação, divisão e módulo
 * de números grandes representados em blocos.
 */

/*
 * Soma dois números A + B e armazena o resultado em S.
 *
 * Input:
 *   struct Numero *A : Primeiro operando
 *   struct Numero *B : Segundo operando
 *   struct Numero *S : Resultado da soma
 *
 * Output:
 *   0   - Sucesso
 *  -1   - Falha de alocação de S
 *  -2   - Argumento NULL
 *
 * Observação:
 *   O resultado é automaticamente normalizado para remover blocos
 *   desnecessários.
 */
int numarit_soma(struct Numero *A, struct Numero *B, struct Numero *S);

/*
 * Subtrai dois números A - B e armazena o resultado em S.
 *
 * Input:
 *   struct Numero *A : Minuendo
 *   struct Numero *B : Subtraendo
 *   struct Numero *S : Resultado da subtração
 *
 * Output:
 *   0   - Sucesso
 *  -2   - Argumento NULL
 *
 * Observação:
 *   Assume que A >= B; o resultado é normalizado para remover
 *   blocos desnecessários.
 */
int numarit_subtracao(struct Numero *A, struct Numero *B, struct Numero *S);

/*
 * Multiplica dois números A * B e armazena o resultado em M.
 *
 * Input:
 *   struct Numero *A : Primeiro operando
 *   struct Numero *B : Segundo operando
 *   struct Numero *M : Resultado da multiplicação
 *
 * Output:
 *   0   - Sucesso
 *  -1   - Falha de alocação de M
 *  -2   - Argumento NULL
 *
 * Observação:
 *   O resultado é normalizado para remover blocos desnecessários.
 */
int numarit_multiplicacao(struct Numero *A, struct Numero *B, struct Numero *M);

/*
 * Multiplica um número A por um valor unsigned long long b e armazena
 * o resultado em M.
 *
 * Input:
 *   struct Numero *A : Número a ser multiplicado
 *   unsigned long long b : Multiplicador
 *   struct Numero *M : Resultado da multiplicação
 *
 * Output:
 *   0   - Sucesso
 *  -1   - Falha de alocação de M
 *  -2   - Argumento NULL
 *
 * Observação:
 *   Útil para multiplicações por constantes ou fatores de normalização.
 */
int numarit_multiplicacao_por_ull(struct Numero *A, unsigned long long b, struct Numero *M);

/*
 * Divide um número A por um valor unsigned long long div.
 *
 * Input:
 *   struct Numero *A : Número a ser dividido
 *   unsigned long long div : Divisor
 *   struct Numero *Q : Resultado (quociente)
 *
 * Output:
 *   Valor retornado: resto da divisão
 *   -1   - Falha de alocação de Q
 *   -2   - Argumento NULL
 *   -3   - Divisor zero
 *
 * Observação:
 *   O quociente é normalizado. O resto é retornado como long long.
 */
long long numarit_divisao_por_ull(struct Numero *A, unsigned long long div, struct Numero *Q);

/*
 * Divide A por B usando o algoritmo de Knuth (divisão longa).
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
 *
 * Observação:
 *   Quociente e resto são normalizados.
 */
int numarit_divisao_knuth(struct Numero *A, struct Numero *B, struct Numero *Q, struct Numero *R);

/*
 * Calcula o módulo A % B e armazena o resultado em resto.
 *
 * Input:
 *   struct Numero *A : Número a ser reduzido
 *   struct Numero *B : Divisor
 *   struct Numero *resto : Resultado (resto)
 *
 * Output:
 *   0   - Sucesso
 *  -1   - Falha de alocação
 *  -2   - Argumento NULL
 *  -3   - Divisor zero
 *
 * Observação:
 *   Internamente utiliza a função numarit_divisao_knuth.
 */
int numarit_modulo(struct Numero *A, struct Numero *B, struct Numero *resto);

#endif
