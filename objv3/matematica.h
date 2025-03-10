#include "header.h"

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

        int a = tamanho (resp);
        int b = tamanho (args);

        se (a > b) loopEmTexto (resp, c, x)
        M
            printf ("(%c)",c);
        W
        fimDoLoopEmTexto;

        seNao loopEmTexto (args, c, x)
        M 
            printf ("'%c'",c);
        W
        fimDoLoopEmTexto;
    W0
W
funcaoComRetorno; 