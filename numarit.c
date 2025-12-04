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

    if (numutil_compara(num1, num2) == -1)
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

    if (numutil_compara(num1, num2) == -1)
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

    if (numutil_compara(num1, num2) == -1)
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
        uint64_t prod = (uint64_t)num->blocos_ptr[i] * (uint64_t)valor + carry;
        resultado->blocos_ptr[i] = (uint32_t)(prod % BLOCO_BASE);
        carry = prod / BLOCO_BASE;
    }

    resultado->blocos_ptr[num->tamanho] = (uint32_t)carry;

    numutil_normaliza(resultado);
    return 0;
}

int numarit_divisao_por_ull(const struct Numero *num, uint32_t divisor, struct Numero *quociente, uint64_t *resto_out)
{
    if (num == NULL || quociente == NULL || resto_out == NULL)
        return -2;
    if (divisor == 0)
        return -3;
    if (numero_inicializa(quociente, num->tamanho) != 0)
        return -1;

    uint64_t resto = 0;

    for (uint64_t i = num->tamanho; i-- > 0;)
    {
        uint64_t atual = (uint64_t)num->blocos_ptr[i];
        uint64_t temp = resto * BLOCO_BASE + atual;

        quociente->blocos_ptr[i] = (uint32_t)(temp / divisor);
        resto = temp % divisor;
    }

    numutil_normaliza(quociente);
    *resto_out = resto;
    return 0;
}

int numarit_divisao(const struct Numero *dividendo, const struct Numero *divisor, struct Numero *quociente, struct Numero *resto)
{
    if (dividendo == NULL || divisor == NULL || quociente == NULL || resto == NULL)
        return -2;

    if (divisor->tamanho == 0 ||
        (divisor->tamanho == 1 && divisor->blocos_ptr[0] == 0))
        return -3;

    uint64_t m = dividendo->tamanho;
    uint64_t n = divisor->tamanho;

    if (numutil_compara(dividendo, divisor) == -1)
    {
        numero_set(quociente, 0);
        numero_copia(dividendo, resto);
        numutil_normaliza(quociente);
        numutil_normaliza(resto);
        return 0;
    }

    if (n == 1)
    {
        uint64_t resto64;
        int e = numarit_divisao_por_ull(dividendo, divisor->blocos_ptr[0], quociente, &resto64);
        if (e != 0)
            return e;

        if (numero_inicializa(resto, 1) != 0)
            return -1;
        resto->blocos_ptr[0] = (uint32_t)resto64;
        numutil_normaliza(quociente);
        numutil_normaliza(resto);
        return 0;
    }

    uint64_t v_m = divisor->blocos_ptr[n - 1];
    uint64_t fator = BLOCO_BASE / (v_m + 1ULL);
    if (fator == 0)
        fator = 1;

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

    if (numarit_multiplicacao_por_ull(dividendo, fator, &norm_dividendo) != 0)
    {
        numero_libera(&norm_dividendo);
        numero_libera(&norm_divisor);
        return -1;
    }
    if (numarit_multiplicacao_por_ull(divisor, fator, &norm_divisor) != 0)
    {
        numero_libera(&norm_dividendo);
        numero_libera(&norm_divisor);
        return -1;
    }

    if (norm_dividendo.tamanho < m + 1)
    {
        struct Numero temp;
        numero_cria_vazio(&temp);
        if (numero_inicializa(&temp, m + 1) != 0)
        {
            numero_libera(&norm_dividendo);
            numero_libera(&norm_divisor);
            return -1;
        }
        for (uint64_t i = 0; i < norm_dividendo.tamanho; ++i)
            temp.blocos_ptr[i] = norm_dividendo.blocos_ptr[i];
        numero_libera(&norm_dividendo);
        norm_dividendo = temp;
    }

    uint64_t q_size = (m >= n) ? (m - n + 1) : 1;
    if (numero_inicializa(quociente, q_size) != 0)
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

    uint32_t *u = norm_dividendo.blocos_ptr;
    uint32_t *v = norm_divisor.blocos_ptr;
    uint32_t *q = quociente->blocos_ptr;

    for (uint64_t i = 0; i < q_size; ++i)
        q[i] = 0;

    for (int64_t j = (int64_t)(m - n); j >= 0; --j)
    {
        uint64_t u_jn = (uint64_t)u[j + n];
        uint64_t u_jn1 = (uint64_t)u[j + n - 1];
        uint64_t v_n1 = (uint64_t)v[n - 1];
        uint64_t v_n2 = (uint64_t)v[n - 2];

        uint64_t numerador = u_jn * BLOCO_BASE + u_jn1;
        uint64_t q_hat = numerador / v_n1;
        uint64_t r_hat = numerador % v_n1;

        uint64_t u_jn2 = (uint64_t)u[j + n - 2];

        while (q_hat >= BLOCO_BASE || q_hat * v_n2 > BLOCO_BASE * r_hat + u_jn2)
        {
            q_hat--;
            r_hat += v_n1;
            if (r_hat >= BLOCO_BASE)
                break;
        }

        int64_t emprestimo = 0;
        for (uint64_t i = 0; i < n; ++i)
        {
            uint64_t p = q_hat * (uint64_t)v[i];
            uint64_t p_lo = p % BLOCO_BASE;
            uint64_t p_hi = p / BLOCO_BASE;

            int64_t cur = (int64_t)u[j + i] - (int64_t)p_lo - emprestimo;
            if (cur < 0)
            {
                cur += (int64_t)BLOCO_BASE;
                emprestimo = (int64_t)p_hi + 1;
            }
            else
            {
                emprestimo = (int64_t)p_hi;
            }
            u[j + i] = (uint32_t)cur;
        }

        int64_t t = (int64_t)u[j + n] - emprestimo;
        u[j + n] = (uint32_t)((t < 0) ? (t + (int64_t)BLOCO_BASE) : t);

        q[j] = (uint32_t)q_hat;

        if (t < 0)
        {
            q[j] = q[j] - 1;
            uint64_t carry = 0;
            for (uint64_t i = 0; i < n; ++i)
            {
                uint64_t soma = (uint64_t)u[j + i] + (uint64_t)v[i] + carry;
                u[j + i] = (uint32_t)(soma % BLOCO_BASE);
                carry = soma / BLOCO_BASE;
            }
            uint64_t somatopo = (uint64_t)u[j + n] + emprestimo;
            u[j + n] = (uint32_t)(somatopo % BLOCO_BASE);
        }
    }

    uint64_t rem64;
    if (numarit_divisao_por_ull(&norm_dividendo, fator, resto, &rem64) != 0)
    {
        numero_libera(&norm_dividendo);
        numero_libera(&norm_divisor);
        return -1;
    }

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

    int resultado = numarit_divisao(num1, num2, &quociente, resto);

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
