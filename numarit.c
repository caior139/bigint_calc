#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "numarit.h"
#include "numutil.h"

int numarit_soma(struct Numero *num1, struct Numero *num2, struct Numero *resultado)
{
    if (num1 == NULL || num2 == NULL || resultado == NULL)
        return -2;

    if (numero_inicializa(resultado, num1->tamanho + 1) != 0)
        return -1;

    unsigned long long indice1 = 0, indice2 = 0, indice_res = 0;
    unsigned long long vai_um = 0;

    while (indice2 < num2->tamanho)
    {
        unsigned long long bloco1 = num1->blocos_ptr[indice1++];
        unsigned long long bloco2 = num2->blocos_ptr[indice2++];

        unsigned long long soma = bloco1 + bloco2 + vai_um;
        if (soma >= BLOCO_BASE)
        {
            vai_um = soma / BLOCO_BASE;
            soma %= BLOCO_BASE;
        }
        else
        {
            vai_um = 0;
        }

        resultado->blocos_ptr[indice_res++] = soma;
    }

    while (indice1 < num1->tamanho)
    {
        unsigned long long bloco1 = num1->blocos_ptr[indice1++];
        unsigned long long soma = bloco1 + vai_um;

        if (soma >= BLOCO_BASE)
        {
            vai_um = soma / BLOCO_BASE;
            soma %= BLOCO_BASE;
        }
        else
        {
            vai_um = 0;
        }

        resultado->blocos_ptr[indice_res++] = soma;
    }

    resultado->blocos_ptr[indice_res] = vai_um;
    numutil_normaliza(resultado);

    return 0;
}

int numarit_subtracao(struct Numero *num1, struct Numero *num2, struct Numero *resultado)
{
    if (num1 == NULL || num2 == NULL || resultado == NULL)
        return -2;

    if (numero_inicializa(resultado, num1->tamanho) != 0)
        return -1;

    unsigned long long indice1 = 0, indice2 = 0, indice_res = 0;
    unsigned long long emprestimo = 0;

    while (indice2 < num2->tamanho)
    {
        unsigned long long bloco1 = num1->blocos_ptr[indice1++];
        unsigned long long bloco2 = num2->blocos_ptr[indice2++];

        if (bloco1 < bloco2 + emprestimo)
        {
            resultado->blocos_ptr[indice_res++] = bloco1 + BLOCO_BASE - bloco2 - emprestimo;
            emprestimo = 1;
        }
        else
        {
            resultado->blocos_ptr[indice_res++] = bloco1 - bloco2 - emprestimo;
            emprestimo = 0;
        }
    }

    while (indice1 < num1->tamanho)
    {
        unsigned long long bloco1 = num1->blocos_ptr[indice1++];
        if (bloco1 < emprestimo)
        {
            resultado->blocos_ptr[indice_res++] = bloco1 + BLOCO_BASE - emprestimo;
            emprestimo = 1;
        }
        else
        {
            resultado->blocos_ptr[indice_res++] = bloco1 - emprestimo;
            emprestimo = 0;
        }
    }

    resultado->tamanho = num1->tamanho;
    numutil_normaliza(resultado);

    return 0;
}

int numarit_multiplicacao(struct Numero *num1, struct Numero *num2, struct Numero *resultado)
{
    if (num1 == NULL || num2 == NULL || resultado == NULL)
        return -2;

    if (numero_inicializa(resultado, num1->tamanho + num2->tamanho + 1) != 0)
        return -1;

    for (unsigned long long i = 0; i < num1->tamanho; i++)
    {
        unsigned long long vai_um = 0;

        for (unsigned long long j = 0; j < num2->tamanho; j++)
        {
            long long indice_res = i + j;

            unsigned long long prod = num1->blocos_ptr[i] * num2->blocos_ptr[j];
            unsigned long long soma = resultado->blocos_ptr[indice_res] + prod + vai_um;

            resultado->blocos_ptr[indice_res] = soma % BLOCO_BASE;
            vai_um = soma / BLOCO_BASE;
        }

        long long k = i + num2->tamanho;
        while (vai_um > 0 && k < resultado->tamanho)
        {
            unsigned long long temp = resultado->blocos_ptr[k] + vai_um;
            resultado->blocos_ptr[k] = temp % BLOCO_BASE;
            vai_um = temp / BLOCO_BASE;
            k++;
        }
    }

    numutil_normaliza(resultado);

    return 0;
}

int numarit_multiplicacao_por_ull(struct Numero *num, unsigned long long valor, struct Numero *resultado)
{
    if (num == NULL || resultado == NULL)
        return -2;
    if (numero_inicializa(resultado, num->tamanho + 1) != 0)
        return -1;

    unsigned long long vai_um = 0;
    for (unsigned long long i = 0; i < num->tamanho; i++)
    {
        unsigned long long prod = num->blocos_ptr[i] * valor + vai_um;
        resultado->blocos_ptr[i] = prod % BLOCO_BASE;
        vai_um = prod / BLOCO_BASE;
    }

    resultado->blocos_ptr[num->tamanho] = vai_um;
    numutil_normaliza(resultado);

    return 0;
}

long long numarit_divisao_por_ull(struct Numero *num, unsigned long long divisor, struct Numero *quociente)
{
    if (num == NULL || quociente == NULL)
        return -2;
    if (divisor == 0)
        return -3;
    if (numero_inicializa(quociente, num->tamanho) != 0)
        return -1;

    unsigned long long resto = 0;
    for (unsigned long long i = num->tamanho; i-- > 0;)
    {
        unsigned long long atual = num->blocos_ptr[i];
        unsigned long long temp = resto * BLOCO_BASE + atual;

        quociente->blocos_ptr[i] = temp / divisor;
        resto = temp % divisor;
    }

    numutil_normaliza(quociente);

    return resto;
}

int numarit_divisao_knuth(struct Numero *dividendo, struct Numero *divisor, struct Numero *quociente, struct Numero *resto)
{
    if (dividendo == NULL || divisor == NULL || quociente == NULL || resto == NULL)
        return -2;
    if (divisor->tamanho == 0 || (divisor->tamanho == 1 && divisor->blocos_ptr[0] == 0))
        return -3;

    unsigned long long m = dividendo->tamanho;
    unsigned long long n = divisor->tamanho;

    if (m == n)
    {
        if (numutil_compara(dividendo, divisor) == -1)
        {
            numero_set(quociente, 0);
            numero_copia(dividendo, resto);

            return 0;
        }
    }

    unsigned long long fator = BLOCO_BASE / (divisor->blocos_ptr[n - 1] + 1ULL);

    struct Numero norm_dividendo, norm_divisor;
    numero_cria_vazio(&norm_dividendo);
    numero_cria_vazio(&norm_divisor);

    if (numero_inicializa(&norm_dividendo, m + 1) != 0)
        return -1;
    if (numero_inicializa(&norm_divisor, n) != 0)
    {
        numero_libera(&norm_dividendo);
        return -1;
    }

    numarit_multiplicacao_por_ull(dividendo, fator, &norm_dividendo);
    numarit_multiplicacao_por_ull(divisor, fator, &norm_divisor);

    if (numero_inicializa(quociente, m + 1) != 0)
    {
        numero_libera(&norm_dividendo);
        numero_libera(&norm_divisor);
        return -1;
    }

    if (numero_inicializa(resto, n) != 0)
    {
        numero_libera(&norm_dividendo);
        numero_libera(&norm_divisor);
        numero_libera(quociente);
        return -1;
    }

    for (unsigned long long j = m - n + 1; j-- > 0;)
    {
        unsigned long long i = j;

        unsigned long long u2 = (i + n < norm_dividendo.tamanho) ? norm_dividendo.blocos_ptr[i + n] : 0ULL;
        unsigned long long u1 = (i + n - 1 < norm_dividendo.tamanho) ? norm_dividendo.blocos_ptr[i + n - 1] : 0ULL;
        unsigned long long u0 = (i + n - 2 < norm_dividendo.tamanho) ? norm_dividendo.blocos_ptr[i + n - 2] : 0ULL;

        unsigned long long v1 = norm_divisor.blocos_ptr[n - 1];
        unsigned long long v2 = (n >= 2) ? norm_divisor.blocos_ptr[n - 2] : 0ULL;

        unsigned long long q_hat, r_hat;

        if (u2 == v1)
        {
            q_hat = BLOCO_BASE - 1;
            r_hat = u1;
        }
        else
        {
            unsigned long long numerador = u2 * BLOCO_BASE + u1;
            q_hat = numerador / v1;
            r_hat = numerador % v1;
        }

        if (q_hat >= BLOCO_BASE)
            q_hat = BLOCO_BASE - 1;

        while (n >= 2)
        {
            unsigned long long esquerda = v2 * q_hat;
            unsigned long long direita = r_hat * BLOCO_BASE + u0;
            if (esquerda <= direita)
                break;
            q_hat--;
            r_hat += v1;
            if (r_hat >= BLOCO_BASE)
                break;
        }

        unsigned long long emprestimo = 0;
        for (unsigned long long k = 0; k < n; k++)
        {
            unsigned long long produto = norm_divisor.blocos_ptr[k] * q_hat + emprestimo;
            unsigned long long produto_lo = produto % BLOCO_BASE;
            emprestimo = produto / BLOCO_BASE;

            if (norm_dividendo.blocos_ptr[i + k] < produto_lo)
            {
                norm_dividendo.blocos_ptr[i + k] += BLOCO_BASE - produto_lo;
                emprestimo += 1;
            }
            else
            {
                norm_dividendo.blocos_ptr[i + k] -= produto_lo;
            }
        }

        if (norm_dividendo.blocos_ptr[i + n] < emprestimo)
        {
            unsigned long long vai_um = 0;
            for (unsigned long long k = 0; k < n; k++)
            {
                unsigned long long soma = norm_dividendo.blocos_ptr[i + k] + norm_divisor.blocos_ptr[k] + vai_um;
                if (soma >= BLOCO_BASE)
                {
                    norm_dividendo.blocos_ptr[i + k] = soma - BLOCO_BASE;
                    vai_um = 1;
                }
                else
                {
                    norm_dividendo.blocos_ptr[i + k] = soma;
                    vai_um = 0;
                }
            }
            norm_dividendo.blocos_ptr[i + n] += BLOCO_BASE - emprestimo + vai_um;
            q_hat -= 1;
        }
        else
        {
            norm_dividendo.blocos_ptr[i + n] -= emprestimo;
        }

        quociente->blocos_ptr[i] = q_hat;
    }

    numarit_divisao_por_ull(&norm_dividendo, fator, resto);

    numutil_normaliza(quociente);
    numutil_normaliza(resto);

    numero_libera(&norm_dividendo);
    numero_libera(&norm_divisor);

    return 0;
}

int numarit_modulo(struct Numero *num1, struct Numero *num2, struct Numero *resto)
{
    struct Numero quociente;
    numero_cria_vazio(&quociente);

    int resultado = numarit_divisao_knuth(num1, num2, &quociente, resto);

    numero_libera(&quociente);

    return resultado;
}

double numarit_lambert(struct Numero *num, struct Numero *resultado)
{
    if (num->sinal < 0)
        return -1;

    if (num->sinal == 0)
    {
        numero_set(resultado, 0);
        return 0;
    }

    double ln_n = numutil_estima_ln(num);

    double w;
    if (ln_n < 1.0)
        w = ln_n;
    else
        w = ln_n - log(ln_n);

    double tolerancia = 1e-9;

    for (int i = 0; i < MAX_ITER; i++)
    {
        double log_w = log(w);
        double numerador = w - ln_n + log_w;
        double denominador = 1.0 + (1.0 / w);
        double diff = numerador / denominador;
        w = w - diff;
        if (fabs(diff) < tolerancia)
            break;
    }

    unsigned long long resultado_final = (unsigned long long)floor(w);

    numero_set(resultado, resultado_final);

    return w - resultado_final;
}
