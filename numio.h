#ifndef NUMIO_H
#define NUMIO_H

#include "numero.h"
#define NUMIO_LIMITE_PRINT 10

/*
 * Biblioteca de entrada/saída para o tipo abstrato Numero.
 * Fornece funções para gerar números aleatórios, ler de arquivos,
 * converter strings para números e imprimir números.
 */

/*
 * Gera um número aleatório com um determinado número de blocos.
 *
 * Input:
 *   struct Numero *num           : Número a ser gerado
 *   unsigned long long num_blocos: Quantidade de blocos do número
 *   int seed                     : Semente para rand()
 *                                   0 -> usa tempo atual
 *
 * Output:
 *   0   - Sucesso
 *  -1   - Falha de alocação do número
 *  -2   - Argumento NULL
 */
int numio_gera_numero(struct Numero *num, uint64_t num_blocos, int seed);
/*
 * Imprime um número no stdout.
 *
 * Input:
 *   struct Numero *num: Número a ser impresso
 *
 * Output:
 *   0   - Sucesso
 *  -1   - Número de Blocos 0
 *  -2   - Argumento NULL
 *
 * Observação:
 *   Imprime o número completo, respeitando o sinal e preenchendo blocos
 *   com zeros à esquerda quando necessário.
 */
int numio_print_numero(const struct Numero *num);

/*
 * Converte uma string de dígitos para um valor unsigned long long.
 *
 * Input:
 *   const char *str           : String contendo apenas dígitos
 *   unsigned long long tam_str: Tamanho da string
 *
 * Output:
 *   Valor convertido da string
 *
 * Observação:
 *   Ignora caracteres que não são dígitos.
 */
uint64_t numio_str_para_numero(const char *str, uint64_t tam_str);

/*
 * Lê um número de um arquivo de texto e inicializa um struct Numero.
 *
 * Input:
 *   const char *caminho: Caminho para o arquivo de texto
 *   struct Numero *num  : Número que receberá o valor lido
 *
 * Output:
 *   0   - Sucesso
 *  -1   - Falha de alocação do número
 *  -2   - Argumento NULL ou erro ao abrir arquivo
 *
 * Observação:
 *   O arquivo pode conter caracteres não numéricos, que serão ignorados.
 *   Se nenhum dígito for encontrado, o número será definido como zero.
 */
int numio_le_numero_txt(const char *caminho, struct Numero *num);

/*
 * Interface interativa para operações com números grandes.
 *
 * Permite:
 *   - Inserir números manualmente, ler de arquivos ou gerar aleatoriamente
 *   - Realizar operações: soma, subtração, multiplicação, divisão inteira e módulo
 *   - Visualizar os resultados
 *
 * Não retorna valor.
 */
void numio_interface();

#endif
