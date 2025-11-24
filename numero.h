#ifndef NUMERO_H
#define NUMERO_H

/*
 * Biblioteca do Tipo Abstrato Numero
 */

#define BLOCO_BASE 100000000ULL  /* 10^8 */
#define DIGITOS_BLOCO 8
#define LIMITE_COMPACTA 2
/*
 * Struct Numero que representa um número com blocos de tamanho BLOCO_BASE
 *
 * blocos_ptr : Ponteiro para blocos de unsigned long long
 * tamanho     : Quantidade de blocos efetivamente utilizados
 * alocado     : Quantidade real de blocos alocados na memória
 * sinal       : Indica o sinal do número (0 = zero, 1 = positivo, -1 = negativo)
 */
struct Numero {
    unsigned long long *blocos_ptr;
    unsigned long long tamanho;
    unsigned long long alocado;
    int sinal;
};

/*
 * Inicializa o tipo Numero com 'tam' blocos
 *
 * Input:
 *   struct Numero *num : Número a ser inicializado
 *   unsigned long long tam : Quantidade de blocos a alocar
 *
 * Output:
 *   0   - Sucesso
 *  -1   - Falha de alocação
 *  -2   - Argumento NULL
 */
int numero_inicializa(struct Numero *num, unsigned long long tam);

/*
 * Libera memória associada ao número
 *
 * Input:
 *   struct Numero *num : Número a ser liberado
 *
 * Output:
 *   VOID
 */
void numero_libera(struct Numero *num);

/*
 * Realoca o número para seu tamanho atual
 *
 * Input:
 *   struct Numero *num : Número a ser realocado
 *
 * Output:
 *   0   - Sucesso
 *  -1   - Falha de alocação
 *  -2   - Argumento NULL
 */
int numero_resize(struct Numero *num);

/*
 * Compacta o número liberando blocos extras
 *
 * Input:
 *   struct Numero *num : Número a ser compactado
 *
 * Output:
 *   0   - Sucesso
 *  -2   - Argumento NULL
 */
int numero_compacta(struct Numero *num);

/*
 * Inicializa um número vazio (blocos_ptr = NULL, tamanho = 0)
 *
 * Input:
 *   struct Numero *num : Número a ser inicializado
 *
 * Output:
 *   VOID
 */
void numero_cria_vazio(struct Numero *num);

/*
 * Define o número com um valor unsigned long long
 *
 * Input:
 *   struct Numero *num : Número a ser definido
 *   unsigned long long valor : Valor a atribuir
 *
 * Output:
 *   0   - Sucesso
 *  -1   - Argumento NULL
 *  -2   - Falha de alocação
 */
int numero_set(struct Numero *num, unsigned long long valor);

/*
 * Copia o número O para C
 *
 * Input:
 *   const struct Numero *O : Número origem
 *   struct Numero *C       : Número destino
 *
 * Output:
 *   0   - Sucesso
 *  -1   - Argumento NULL
 *  -2   - Falha de alocação
 */
int numero_copia(const struct Numero *O, struct Numero *C);

#endif 
