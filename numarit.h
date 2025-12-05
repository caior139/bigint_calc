#ifndef NUMARIT_H
#define NUMARIT_H

#define MAX_ITER 100
#define TOL 1e-14
#include "numero.h"

/*
 * Biblioteca de operações aritméticas para o tipo abstrato Numero.
 * Fornece funções para soma, subtração, multiplicação, divisão, módulo
 * e funções auxiliares envolvendo números grandes armazenados em blocos.
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
 */
int numarit_soma(const struct Numero *A, const struct Numero *B, struct Numero *S);

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
 */
int numarit_subtracao(const struct Numero *A, const struct Numero *B, struct Numero *S);

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
 */
int numarit_multiplicacao(const struct Numero *A, const struct Numero *B, struct Numero *M);

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
 */
int numarit_multiplicacao_por_ull(const struct Numero *A, uint32_t b, struct Numero *M);

/*
 * Divide um número A por um valor unsigned long long divisor.
 *
 * Input:
 *   struct Numero *A : Número a ser dividido
 *   unsigned long long divisor : Divisor
 *   struct Numero *Q : Resultado (quociente)
 *   uint64_t *resto_out : Resto da divisão
 *
 * Output:
 *   Valor retornado: resto da divisão
 *   -1   - Falha de alocação de Q
 *   -2   - Argumento NULL
 *   -3   - Divisor zero
 *
 */
int numarit_divisao_por_ull(const struct Numero *num,
                            uint32_t divisor,
                            struct Numero *quociente,
                            uint32_t *resto_out);

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
 */
int numarit_divisao(const struct Numero *A, const struct Numero *B,
                    struct Numero *Q, struct Numero *R);

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
 */
int numarit_modulo(const struct Numero *A, const struct Numero *B, struct Numero *resto);

/*
 * Calcula a função de Lambert W aproximada.
 *
 * Esta função resolve a equação:
 *
 *       W(x) * e^(W(x)) = x
 *
 * para um número grande armazenado em formato Numero. A aproximação é
 * obtida por iterações de Newton usando MAX_ITER iterações no máximo.
 *
 * Input:
 *   struct Numero *num       : Valor de entrada x
 *   struct Numero *resultado : Saída contendo W(x) aproximado
 *
 * Output:
 *   Retorna um double auxiliar contendo uma aproximação flutuante de W(x),
 *   útil para inicialização de algoritmos externos.
 *   Não usa códigos de erro numéricos (sempre retorna um double).
 */
double numarit_lambert(const struct Numero *num, struct Numero *resultado);

#endif
