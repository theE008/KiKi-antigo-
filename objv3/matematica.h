#include "header.h"
#include <string.h>

/////////////////////////////////////////////////////////////////
// IMPRIMIR

novaFuncao (imprimirNumero)
M
    seTiverArgumento (sin, "sinal")
        se (nao comparar (sin, novo_texto ("-"))) imprimir (sin, O);

    seTiverArgumento (num, "numero")
        imprimir (num, O);
    seNao verificarErro (1234); // precisa ter um numero aqui né
    
    seTiverArgumento (dec, "decimal")
        imprimir (novo_texto (","), dec, O);

    seTiverArgumento (irr, "irracional")
        imprimir (novo_texto ("*"), irr, O);

    // vou fingir que números laterais não existem :)
W
funcaoSemRetorno;

/////////////////////////////////////////////////////////////////
// VALORES

novoConstrutor (o, numeroNatural)
M
    pegarParaResposta ("numero", novo_texto ("0"));

    M0
        adicionarNaResposta ("imprimir", nova_funcao (imprimirNumero));
    W0
W
funcaoComRetorno;

novoConstrutor (o, numeroInteiro)
M
    usarDeBase (novo_numeroNatural);
    pegarParaResposta ("sinal", novo_texto ("+"));
W
funcaoComRetorno;

novoConstrutor (o, numeroRacional)
M
    usarDeBase (novo_numeroInteiro);
    pegarParaResposta ("decimal", novo_texto ("0"));
W
funcaoComRetorno;

novoConstrutor (o, numeroReal)
M 
    usarDeBase (novo_numeroRacional);
    pegarParaResposta ("irracional", novo_texto ("p"));
W
funcaoComRetorno;

novoConstrutor (o, numeroImaginario)
M
    usarDeBase (novo_numeroReal);
    pegarParaResposta ("lateral", novo_texto ("0"));
W 
funcaoComRetorno;

novoConstrutor (o, numero)
M 
    usarDeBase (novo_numeroRacional);
W
funcaoComRetorno;

/////////////////////////////////////////////////////////////////
// MÉTODOS

novaFuncao (somar)
M
    se (forPrimeiroArgumento)
    M0
        seTiverArgumento (num, "numero") adicionar (resposta, "numero", num);
    W0
    ouSe (temArgumentos e pegar (argumentos, "numero") existir) 
    M0
        objeto resp = pegar (resposta,   "numero");
        objeto args = pegar (argumentos, "numero");

        int a = tamanho   (resp);
        int b = tamanho   (args);
        int m = (a>b)? a : b + 1;

        char *resultado = (char*) malloc ((m + 1) * sizeof (char));
        se (nao resultado) verificarErro (1);
        resultado[m] = '\0';
        int carry = 0, i = a - 1, j = b - 1, k = m - 1;

        char* ca = novo_chars (resp);
        char* cb = novo_chars (args);

        while (i >= 0 || j >= 0 || carry)
        {
            int digitoA = (i >= 0) ? ca[i] - '0' : 0;
            int digitoB = (j >= 0) ? cb[j] - '0' : 0;
            int soma = digitoA + digitoB + carry;

            resultado[k] = (soma % 10) + '0';
            carry = soma / 10;

            i--;
            j--;
            k--;
        }

        char r0 = resultado[0];
        if (nao ('0' <= r0 e r0 <= '9'))
        {
            memmove (resultado, resultado + 1, m); // Remove o zero extra no início
        }

        alterar (resposta, "numero", novo_texto (resultado));

        free (resultado);
    W0
W
funcaoComRetorno;