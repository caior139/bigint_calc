#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "numarit.h"
#include "numutil.h"

int numarit_soma(const struct Numero *num1, const struct Numero *num2, struct Numero *resultado)
{
    if (num1 == NULL || num2 == NULL || resultado == NULL)
        return -2;

    if (numero_inicializa(resultado, num1->tamanho + 1) != 0)
        return -1;

    if(numutil_compara(num1, num2) == -1)
    {
        const struct Numero *temp = num1;
        num1 = num2;
        num2 = temp;
    }

    uint64_t i = 0, carry = 0;

    for (; i < num2->tamanho; i++)
    {
        uint64_t soma = (uint64_t)num1->blocos_ptr[i] + (uint64_t)num2->blocos_ptr[i] + carry;

        if (soma >= BLOCO_BASE)
        {
            carry = soma / BLOCO_BASE;
            soma %= BLOCO_BASE;
        }
        else
            carry = 0;

        resultado->blocos_ptr[i] = (uint32_t)soma;
    }

    for (; i < num1->tamanho; i++)
    {
        uint64_t soma = (uint64_t)num1->blocos_ptr[i] + carry;

        if (soma >= BLOCO_BASE)
        {
            carry = soma / BLOCO_BASE;
            soma %= BLOCO_BASE;
        }
        else
            carry = 0;

        resultado->blocos_ptr[i] = (uint32_t)soma;
    }

    resultado->blocos_ptr[i] = (uint32_t)carry;

    numutil_normaliza(resultado);
    return 0;
}

int numarit_subtracao(const struct Numero *num1, const struct Numero *num2, struct Numero *resultado)
{
    if (num1 == NULL || num2 == NULL || resultado == NULL)
        return -2;

    if (numero_inicializa(resultado, num1->tamanho) != 0)
        return -1;

    if(numutil_compara(num1, num2) == -1)
    {
        const struct Numero *temp = num1;
        num1 = num2;
        num2 = temp;
        resultado->sinal = -1;
    }

    uint64_t i = 0, emprestimo = 0;

    for (; i < num2->tamanho; i++)
    {
        uint64_t a = num1->blocos_ptr[i] - emprestimo;
        uint64_t b = num2->blocos_ptr[i];

        if (a < b)
        {
            resultado->blocos_ptr[i] = (uint32_t)(a + BLOCO_BASE - b);
            emprestimo = 1;
        }
        else
        {
            resultado->blocos_ptr[i] = (uint32_t)(a - b);
            emprestimo = 0;
        }
    }

    for (; i < num1->tamanho; i++)
    {
        uint64_t a = num1->blocos_ptr[i];

        if (a < emprestimo)
        {
            resultado->blocos_ptr[i] = (uint32_t)(a + BLOCO_BASE - emprestimo);
            emprestimo = 1;
        }
        else
        {
            resultado->blocos_ptr[i] = (uint32_t)(a - emprestimo);
            emprestimo = 0;
        }
    }

    numutil_normaliza(resultado);
    return 0;
}

int numarit_multiplicacao(const struct Numero *num1, const struct Numero *num2, struct Numero *resultado)
{
    if (num1 == NULL || num2 == NULL || resultado == NULL)
        return -2;

    if (numero_inicializa(resultado, num1->tamanho + num2->tamanho + 1) != 0)
        return -1;

    if(numutil_compara(num1, num2) == -1)
    {
        const struct Numero *temp = num1;
        num1 = num2;
        num2 = temp;
    }

    for (uint64_t i = 0; i < num1->tamanho; i++)
    {
        uint64_t carry = 0;
        uint64_t a = num1->blocos_ptr[i];

        for (uint64_t j = 0; j < num2->tamanho; j++)
        {
            uint64_t idx = i + j;

            uint64_t prod = a * (uint64_t)num2->blocos_ptr[j];
            uint64_t soma = (uint64_t)resultado->blocos_ptr[idx] + prod + carry;

            resultado->blocos_ptr[idx] = (uint32_t)(soma % BLOCO_BASE);
            carry = soma / BLOCO_BASE;
        }

        uint64_t k = i + num2->tamanho;

        uint64_t temp = (uint64_t)resultado->blocos_ptr[k] + carry;
        resultado->blocos_ptr[k] = (uint32_t)(temp % BLOCO_BASE);
        carry = temp / BLOCO_BASE;
        k++;

        while (carry > 0 && k < resultado->tamanho)
        {
            temp = (uint64_t)resultado->blocos_ptr[k] + carry;
            resultado->blocos_ptr[k] = (uint32_t)(temp % BLOCO_BASE);
            carry = temp / BLOCO_BASE;
            k++;
        }
    }

    numutil_normaliza(resultado);
    return 0;
}

int numarit_multiplicacao_por_ull(const struct Numero *num, uint32_t valor, struct Numero *resultado)
{
    if (num == NULL || resultado == NULL)
        return -2;
    if (numero_inicializa(resultado, num->tamanho + 1) != 0)
        return -1;

    uint64_t carry = 0;

    for (uint64_t i = 0; i < num->tamanho; i++)
    {
        uint64_t prod = (uint64_t)num->blocos_ptr[i] * valor + carry;
        resultado->blocos_ptr[i] = (uint32_t)(prod % BLOCO_BASE);
        carry = prod / BLOCO_BASE;
    }

    resultado->blocos_ptr[num->tamanho] = (uint32_t)carry;

    numutil_normaliza(resultado);
    return 0;
}

long long numarit_divisao_por_ull(const struct Numero *num, uint32_t divisor, struct Numero *quociente)
{
    if (num == NULL || quociente == NULL)
        return -2;
    if (divisor == 0)
        return -3;
    if (numero_inicializa(quociente, num->tamanho) != 0)
        return -1;

    uint64_t resto = 0;

    for (uint64_t i = num->tamanho; i-- > 0;)
    {
        uint64_t atual = num->blocos_ptr[i];
        uint64_t temp = resto * BLOCO_BASE + atual;

        quociente->blocos_ptr[i] = temp / divisor;
        resto = temp % divisor;
    }

    numutil_normaliza(quociente);

    return resto;
}

int numarit_divisao_knuth(const struct Numero *dividendo, const struct Numero *divisor, struct Numero *quociente, struct Numero *resto)
{
    if (dividendo == NULL || divisor == NULL ||
        quociente == NULL || resto == NULL)
        return -2;

    if (divisor->tamanho == 0 ||
        (divisor->tamanho == 1 && divisor->blocos_ptr[0] == 0))
        return -3;
    
    uint64_t m = dividendo->tamanho;
    uint64_t n = divisor->tamanho;


    if(numutil_compara(dividendo, divisor) == -1)
    {
        numero_set(quociente, 0);
        numero_copia(dividendo, resto);
        return 0;
    }
    uint64_t fator = BLOCO_BASE / (divisor->blocos_ptr[n - 1] + 1ULL);

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

    for (uint64_t j = m - n + 1; j-- > 0;)
    {
        uint64_t i = j;

        uint64_t u2 = (i + n < norm_dividendo.tamanho)
                          ? norm_dividendo.blocos_ptr[i + n]
                          : 0ULL;
        uint64_t u1 = (i + n - 1 < norm_dividendo.tamanho)
                          ? norm_dividendo.blocos_ptr[i + n - 1]
                          : 0ULL;
        uint64_t u0 = (i + n - 2 < norm_dividendo.tamanho)
                          ? norm_dividendo.blocos_ptr[i + n - 2]
                          : 0ULL;

        uint64_t v1 = norm_divisor.blocos_ptr[n - 1];
        uint64_t v2 = (n >= 2) ? norm_divisor.blocos_ptr[n - 2] : 0ULL;

        uint64_t q_hat, r_hat;

        if (u2 == v1)
        {
            q_hat = BLOCO_BASE - 1;
            r_hat = u1;
        }
        else
        {
            uint64_t numerador = u2 * BLOCO_BASE + u1;
            q_hat = numerador / v1;
            r_hat = numerador % v1;
        }

        if (q_hat >= BLOCO_BASE)
            q_hat = BLOCO_BASE - 1;

          if (n >= 2) {
            while ((uint64_t)v2 * q_hat > (uint64_t)r_hat * BLOCO_BASE + u0) {
                q_hat--;
                r_hat += v1;
                if (r_hat >= BLOCO_BASE)
                    break;
            }
        }

        uint64_t emprestimo = 0;

        for (uint64_t k = 0; k < n; k++)
        {
            uint64_t produto =
                (uint64_t)norm_divisor.blocos_ptr[k] * q_hat + emprestimo;

            uint64_t produto_lo = produto % BLOCO_BASE;
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
            uint64_t vai_um = 0;
            for (uint64_t k = 0; k < n; k++)
            {
                uint64_t soma = norm_dividendo.blocos_ptr[i + k] +
                                norm_divisor.blocos_ptr[k] + vai_um;

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
            q_hat--;
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

int numarit_modulo(const struct Numero *num1, const struct Numero *num2, struct Numero *resto)
{
    struct Numero quociente;
    numero_cria_vazio(&quociente);

    int resultado = numarit_divisao_knuth(num1, num2, &quociente, resto);

    numero_libera(&quociente);

    return resultado;
}

double numarit_lambert(const struct Numero *num, struct Numero *resultado)
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

    uint32_t resultado_final = (uint32_t)floor(w);

    numero_set(resultado, resultado_final);

    return w - resultado_final;
}
