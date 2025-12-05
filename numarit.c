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

int numarit_divisao_por_ull(const struct Numero *num, uint32_t divisor, struct Numero *quociente, uint32_t *resto_out)
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

int numarit_divisao(const struct Numero *dividendo, const struct Numero *divisor,
                    struct Numero *quociente, struct Numero *resto)
{
    if (dividendo == NULL || divisor == NULL || quociente == NULL || resto == NULL)
        return -2;

    if (divisor->tamanho == 0 || (divisor->tamanho == 1 && divisor->blocos_ptr[0] == 0))
        return -3;

    if (numutil_compara(dividendo, divisor) < 0)
    {
        numero_set(quociente, 0);
        numero_copia(dividendo, resto);
        return 0;
    }

    uint64_t m = dividendo->tamanho;
    uint64_t n = divisor->tamanho;

    if (n == 1)
    {
        uint32_t rem32;
        numarit_divisao_por_ull(dividendo, divisor->blocos_ptr[0], quociente, &rem32);
        numero_inicializa(resto, 1);
        resto->blocos_ptr[0] = (uint32_t)rem32;
        numutil_normaliza(quociente);
        numutil_normaliza(resto);
        return 0;
    }

    uint64_t fator;
    uint64_t vn_1 = divisor->blocos_ptr[n - 1];
    
    if (vn_1 < BLOCO_BASE / 2)
        fator = BLOCO_BASE / (vn_1 + 1);
    
    else
        fator = 1; 
    


    struct Numero u_norm, v_norm;
    numero_cria_vazio(&u_norm);
    numero_cria_vazio(&v_norm);
    
    numero_inicializa(&u_norm, m + 1);
    numero_inicializa(&v_norm, n);
    

    numarit_multiplicacao_por_ull(dividendo, fator, &u_norm);
    numarit_multiplicacao_por_ull(divisor, fator, &v_norm);
    
    uint32_t *u = u_norm.blocos_ptr;
    uint32_t *v = v_norm.blocos_ptr;
    

    numero_inicializa(quociente, m - n + 1);
    uint32_t *q = quociente->blocos_ptr;
    

    for (int64_t j = m - n; j >= 0; j--)
    {

        uint64_t numerador = (uint64_t)u[j + n] * BLOCO_BASE + u[j + n - 1];
        uint64_t q_hat = numerador / v[n - 1];
        uint64_t r_hat = numerador % v[n - 1];
        
        
        if (q_hat >= BLOCO_BASE)
        {
            q_hat = BLOCO_BASE - 1;
            r_hat = numerador - q_hat * v[n - 1];
        }
        
    
        while (r_hat < BLOCO_BASE)
        {
            uint64_t esq = q_hat * v[n - 2];
            uint64_t dir = r_hat * BLOCO_BASE + u[j + n - 2];
            
            if (esq > dir)
            {
                q_hat--;
                r_hat += v[n - 1];
            }
            else
                break;
            
        }
        
       
        uint64_t emprestimo = 0;
        for (uint64_t i = 0; i < n; i++)
        {
            uint64_t prod = q_hat * v[i] + emprestimo;
            uint64_t prod_lo = prod % BLOCO_BASE;
            uint64_t prod_hi = prod / BLOCO_BASE;
            
    
            int64_t dif = (int64_t)u[i + j] - (int64_t)prod_lo;
            
            if (dif < 0)
            {
                dif += BLOCO_BASE;
                prod_hi++;  
            }
            
            u[i + j] = (uint32_t)dif;
            emprestimo = prod_hi; 
        }
        

        int64_t dif_final = (int64_t)u[j + n] - (int64_t)emprestimo;
        
 
        q[j] = (uint32_t)q_hat;
        
     
        if (dif_final < 0)
        {
            q[j]--;
            
            uint64_t carry = 0;
            for (uint64_t i = 0; i < n; i++)
            {
                uint64_t soma = (uint64_t)u[i + j] + (uint64_t)v[i] + carry;
                u[i + j] = (uint32_t)(soma % BLOCO_BASE);
                carry = soma / BLOCO_BASE;
            }
            
            u[j + n] += (uint32_t)carry;
        }
        else
            u[j + n] = (uint32_t)dif_final;
        
    }
    

    numero_inicializa(resto, n);

    for (uint64_t i = 0; i < n; i++)
        resto->blocos_ptr[i] = u[i];
    
    
    uint32_t temp_rem32;
    numarit_divisao_por_ull(resto, fator, resto, &temp_rem32);
    
    numutil_normaliza(quociente);
    numutil_normaliza(resto);
    numero_libera(&u_norm);
    numero_libera(&v_norm);
    
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
    if (num == NULL || resultado == NULL)
        return -1;

    if (num->sinal < 0)
        return -1;

    if (num->sinal == 0)
    {
        numero_set(resultado, 0);
        return 0.0;
    }

    double ln_n = numutil_estima_ln(num);
    double ln_ln = log(ln_n);
    double w = ln_n - ln_ln + ln_ln / ln_n;

    for (int i = 0; i < MAX_ITER; i++)
    {
        double log_w = log(w);
        double numerador = w - ln_n + log_w;
        double denominador = 1.0 + 1.0 / w;
        double dif = numerador / denominador;
        w = w - dif;
        if (fabs(dif) < TOL)
            break;
    }

    uint32_t w_int = (uint32_t)floor(w);
    numero_set(resultado, w_int);
    return w - w_int;
}
