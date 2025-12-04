#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>

#include "numio.h"
#include "numero.h"
#include "numarit.h"
#include "numutil.h"

int numio_gera_numero(struct Numero *num, uint64_t num_blocos, int seed)
{
    if (num_blocos == 0)
        return -1;

    if (num == NULL)
        return -2;

    if (seed == 0)
        srand((unsigned int)time(NULL));
    else
        srand((unsigned int)seed);

    if (numero_inicializa(num, num_blocos) != 0)
        return -1;

    for (uint64_t i = 0; i < num_blocos - 1; i++)
        num->blocos_ptr[i] = (uint32_t)(rand() % BLOCO_BASE);

    num->blocos_ptr[num_blocos - 1] = (uint32_t)((rand() % (BLOCO_BASE - 1)) + 1);
    return 0;
}

int numio_le_numero_txt(const char *caminho, struct Numero *num)
{
    if (caminho == NULL || num == NULL)
        return -2;

    FILE *fptr = fopen(caminho, "r");
    if (fptr == NULL)
        return -2;

    uint64_t numero_digitos = 0;
    int c;

    while ((c = fgetc(fptr)) != EOF)
        if (isdigit(c))
            numero_digitos++;

    if (numero_digitos == 0)
    {
        fclose(fptr);
        numero_set(num, 0);
        return 0;
    }

    rewind(fptr);

    uint64_t num_blocos = (numero_digitos + DIGITOS_BLOCO - 1) / DIGITOS_BLOCO;

    if (numero_inicializa(num, num_blocos) != 0)
    {
        fclose(fptr);
        return -1;
    }

    char *digitos = (char *)malloc(numero_digitos + 1);
    if (digitos == NULL)
    {
        fclose(fptr);
        return -1;
    }

    uint64_t idx = 0;
    while ((c = fgetc(fptr)) != EOF && idx < numero_digitos)
        if (isdigit(c))
            digitos[idx++] = c;

    digitos[numero_digitos] = '\0';
    fclose(fptr);

    uint64_t pos = numero_digitos;

    for (uint64_t bloco = 0; bloco < num_blocos; bloco++)
    {
        uint32_t tamanho_bloco = DIGITOS_BLOCO;
        if (pos < DIGITOS_BLOCO)
            tamanho_bloco = pos;

        pos -= tamanho_bloco;

        char bloco_str[DIGITOS_BLOCO + 1];
        for (uint32_t i = 0; i < tamanho_bloco; i++)
            bloco_str[i] = digitos[pos + i];

        bloco_str[tamanho_bloco] = '\0';
        num->blocos_ptr[bloco] = numio_str_para_numero(bloco_str, tamanho_bloco);
    }

    free(digitos);
    numutil_normaliza(num);

    num->sinal =
        (num->tamanho == 1 && num->blocos_ptr[0] == 0) ? 0 : 1;

    return 0;
}

uint64_t numio_str_para_numero(const char *str, uint64_t tam_str)
{
    uint64_t valor = 0;

    for (uint64_t i = 0; i < tam_str; i++)
    {
        if (isdigit(str[i]))
            valor = valor * 10 + (str[i] - '0');
    }

    return valor;
}

int numio_print_numero(const struct Numero *num)
{
    if (num == NULL)
        return -2;

    if (num->tamanho == 0 || (num->tamanho == 1 && num->blocos_ptr[0] == 0))
    {
        printf("0\n");
        return 0;
    }

    if (num->sinal == -1)
        printf("-");

    printf("%" PRIu32, num->blocos_ptr[num->tamanho - 1]);

    for (long long i = (long long)num->tamanho - 2; i >= 0; i--)
        printf("%0*u", DIGITOS_BLOCO, num->blocos_ptr[i]);

    printf("\n");
    return 0;
}

int numio_print_numero_compacto(const struct Numero *num)
{
    if (num == NULL)
        return -2;

    if (num->tamanho == 0 || (num->tamanho == 1 && num->blocos_ptr[0] == 0))
    {
        printf("0\n");
        return 0;
    }

    uint64_t tam = num->tamanho;
    int mostrar_blocos = NUMIO_LIMITE_PRINT; 
    int blocos_inicio = mostrar_blocos / 2;
    int blocos_fim = mostrar_blocos - blocos_inicio;

    if (blocos_inicio > (long long)tam)
        blocos_inicio = (long long)tam;
    if (blocos_fim > (long long)tam - blocos_inicio)
        blocos_fim = (long long)tam - blocos_inicio;

    if (num->sinal == -1)
        printf("-");


    for (long long i = tam - 1; i >= (long long)(tam - blocos_inicio); i--)
    {
        if (i == (long long)(tam - 1))
            printf("%" PRIu32, num->blocos_ptr[i]); 
        else
            printf("%0*u", DIGITOS_BLOCO, num->blocos_ptr[i]);
    }

    if (tam > (uint64_t)mostrar_blocos)
        printf("...");

    for (long long i = (long long)blocos_fim - 1; i >= 0; i--)
    {
        printf("%0*u", DIGITOS_BLOCO, num->blocos_ptr[i]);
    }

    printf("\n");
    return 0;
}


void numio_limpa_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
void numio_interface()
{
    struct Numero num1, num2, resultado, resto;

    numero_cria_vazio(&num1);
    numero_cria_vazio(&num2);
    numero_cria_vazio(&resultado);
    numero_cria_vazio(&resto);

    printf("========== CALCULADORA BIG INT ==========\n");
    while (1)
    {
        int escolha;
        double w = 0;

        printf("\nMENU PRINCIPAL:\n");
        printf("1 - Inserir/ler novos numeros\n");
        printf("2 - Realizar operacao com numeros atuais\n");
        printf("3 - Sair\n");
        printf("Opcao: ");

        scanf("%d", &escolha);
        numio_limpa_buffer();
        printf("\n");

        if (escolha == 1)
        {

            numero_libera(&num1);
            numero_libera(&num2);

            int opcao_entrada;

            printf("Escolha a forma de entrada:\n");
            printf("1 - Digitar manualmente\n");
            printf("2 - Ler de arquivos txt\n");
            printf("3 - Gerar aleatoriamente\n");
            printf("Opcao: ");

            scanf("%d", &opcao_entrada);
            numio_limpa_buffer();

            if (opcao_entrada == 1)
            {
                uint32_t v1, v2;

                printf("Digite o primeiro numero (uint32): ");
                scanf("%" SCNu32, &v1);

                printf("Digite o segundo numero (uint32): ");
                scanf("%" SCNu32, &v2);

                numio_limpa_buffer();

                numero_set(&num1, v1);
                numero_set(&num2, v2);

                num1.sinal = 1;
                num2.sinal = 1;
            }
            else if (opcao_entrada == 2)
            {
                char caminho1[512], caminho2[512];

                printf("Digite o caminho do primeiro arquivo: ");
                fgets(caminho1, sizeof(caminho1), stdin);
                caminho1[strcspn(caminho1, "\n")] = 0;

                printf("Digite o caminho do segundo arquivo: ");
                fgets(caminho2, sizeof(caminho2), stdin);
                caminho2[strcspn(caminho2, "\n")] = 0;

                if (numio_le_numero_txt(caminho1, &num1) != 0 ||
                    numio_le_numero_txt(caminho2, &num2) != 0)
                {
                    printf("Erro ao ler arquivos!\n");
                    continue;
                }
            }
            else if (opcao_entrada == 3)
            {
                uint64_t bloc1, bloc2;

                printf("Quantos blocos para o primeiro numero? ");
                scanf("%" SCNu64, &bloc1);

                printf("Quantos blocos para o segundo numero? ");
                scanf("%" SCNu64, &bloc2);

                numio_limpa_buffer();
                unsigned int seed = (unsigned int)time(NULL);

                numio_gera_numero(&num1, bloc1, seed);
                numio_gera_numero(&num2, bloc2, seed + 12345);

                num1.sinal = 1;
                num2.sinal = 1;
            }
            else
            {
                printf("Opcao invalida!\n");
                continue;
            }

            numutil_normaliza(&num1);
            numutil_normaliza(&num2);

            printf("\nNumero 1: ");
            if (num1.tamanho > NUMIO_LIMITE_PRINT)
                numio_print_numero_compacto(&num1);
            else
                numio_print_numero(&num1);

            printf("Numero 2: ");
            if (num2.tamanho > NUMIO_LIMITE_PRINT)
                numio_print_numero_compacto(&num2);
            else
                numio_print_numero(&num2);
        }
        if (escolha == 2)
        {
            printf("\nNumero 1: ");
            if (num1.tamanho > NUMIO_LIMITE_PRINT)
                numio_print_numero_compacto(&num1);
            else
                numio_print_numero(&num1);

            printf("Numero 2: ");
            if (num2.tamanho > NUMIO_LIMITE_PRINT)
                numio_print_numero_compacto(&num2);
            else
                numio_print_numero(&num2);
        }
        if (escolha == 3)
        {
            printf("Saindo do programa...\n");
            break;
        }

        int opcao_operacao;

        printf("\nEscolha a operacao:\n");
        printf("1 - Soma\n");
        printf("2 - Subtracao\n");
        printf("3 - Multiplicacao\n");
        printf("4 - Divisao inteira\n");
        printf("5 - Modulo\n");
        printf("6 - Lambert W\n");
        printf("Opcao: ");

        scanf("%d", &opcao_operacao);
        numio_limpa_buffer();
        printf("\n");
        
        switch (opcao_operacao)
        {
        case 1:
            numarit_soma(&num1, &num2, &resultado);
            resultado.sinal = 1;
            break;

        case 2:
            numarit_subtracao(&num1, &num2, &resultado);
            
            break;

        case 3:
            numarit_multiplicacao(&num1, &num2, &resultado);
            resultado.sinal = (num1.sinal == num2.sinal) ? 1 : -1;
            break;

        case 4:
            numarit_divisao_knuth(&num1, &num2, &resultado, &resto);
            resultado.sinal = (num1.sinal == num2.sinal) ? 1 : -1;
            numero_libera(&resto);
            break;

        case 5:
            numarit_modulo(&num1, &num2, &resultado);
            resultado.sinal = 1;
            break;
        case 6:
            w = numarit_lambert(&num1, &resultado);
            break;

        default:
            printf("Operacao invalida!\n");
            continue;
        }

        printf("\n========== RESULTADO ==========\n");
        if (opcao_operacao == 6)
        {
            printf("Parte inteira: ");
            numio_print_numero_compacto(&resultado);
            printf("Parte decimal: %lf\n", w);
        }
        else
        {
            if (resultado.tamanho > NUMIO_LIMITE_PRINT)
                numio_print_numero_compacto(&resultado);
            else
                numio_print_numero(&resultado);
            printf("===============================\n");
        }

        char salvar;
        printf("Deseja salvar como test case? (s/n): ");
        scanf(" %c", &salvar);
        numio_limpa_buffer();

        if (salvar == 's' || salvar == 'S')
        {

            printf("Digite o caminho do test case: ");

            char caminho_completo[1024];

            fgets(caminho_completo, sizeof(caminho_completo), stdin);
            caminho_completo[strcspn(caminho_completo, "\n")] = 0;


            FILE *fptr = fopen(caminho_completo, "r");

            if (fptr != NULL)
            {

                fclose(fptr);
                struct Numero esperado;
                numero_cria_vazio(&esperado);
                if (numio_le_numero_txt(caminho_completo, &esperado) == 0)
                {
                    if (numutil_compara(&resultado, &esperado) == 0)
                        printf("Resultado corresponde ao test case.\n");
                    else
                        printf("Resultado diferente do test case!\n");
                }
                numero_libera(&esperado);
            }
            else
            {

                fptr = fopen(caminho_completo, "w");
                if (fptr == NULL)
                {
                    printf("Erro ao criar test case!\n");
                }
                else
                {
                    fprintf(fptr, "%" PRIu32, resultado.blocos_ptr[resultado.tamanho - 1]);
                    for (long long i = (long long)resultado.tamanho - 2; i >= 0; i--)
                        fprintf(fptr, "%0*u", DIGITOS_BLOCO, resultado.blocos_ptr[i]);
                    fprintf(fptr, "\n");
                    fclose(fptr);
                    printf("Test case criado!\n");
                }
            }
        }

        numero_libera(&resultado);
    }

    numero_libera(&num1);
    numero_libera(&num2);
}