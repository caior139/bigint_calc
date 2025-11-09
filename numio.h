#ifndef NUMIO_H
#define NUMIO_H

#include "numero.h"

/*
 * Biblioteca de entrada/saída para o tipo abstrato Número
 */

/*
 * Gera um número aleatório com num_blocos blocos
 *
 * Input:
 *   struct Numero *num           : Número a ser gerado
 *   unsigned long long num_blocos: Quantidade de blocos
 *   int seed                     : Semente para rand(); se 0 usa tempo atual
 *
 * Output:
 *   0   - Sucesso
 *  -1   - Falha de alocação de num
 *  -2   - Argumento NULL
 */
int numio_gera_numero(struct Numero *num, unsigned long long num_blocos, int seed);

/*
 * Imprime o número no stdout
 *
 * Input:
 *   struct Numero *num: Número a ser impresso
 *
 * Output:
 *   0   - Sucesso
 *  -2   - Argumento NULL
 */
int numio_print_numero(struct Numero *num);

/*
 * Converte uma string de dígitos para número unsigned long long
 *
 * Input:
 *   const char *str           : String contendo apenas dígitos
 *   unsigned long long tam_str: Tamanho da string
 *
 * Output:
 *   Valor convertido
 */
unsigned long long numio_str_para_numero(const char *str, unsigned long long tam_str);

/*
 * Lê um número de um arquivo de texto
 *
 * Input:
 *   const char *caminho: Caminho do arquivo de texto
 *   struct Numero *num  : Número que receberá o valor
 *
 * Output:
 *   0   - Sucesso
 *  -1   - Falha de alocação de num
 *  -2   - Argumento NULL ou erro ao abrir arquivo
 */
int numio_le_numero_txt(const char *caminho, struct Numero *num);

#endif 
