// gcc -o main main.c objv3/obj.c && main

#include "objv3/matematica.h"

/////////////////////////////////////////////////////////////////
// MAIN

INICIO_DO_PROGRAMA;
M
    /*objeto a = novo_numeroNatural    (O);
    objeto b = novo_numeroInteiro    (O);
    objeto c = novo_numeroRacional   (O);
    objeto d = novo_numeroReal       (O);
    objeto f = novo_numeroImaginario (O);
    objeto g = novo_numeroReal 
    (nova_listagem 
        (
            "sinal"     , novo_texto ("-"),
            "numero"    , novo_texto ("123456789"),
            "decimal"   , novo_texto ("4444"),
            "irracional", novo_texto ("pi"),
        O),         
    O);

    //imprimir (a, b, c, d, f, g, O);

    //imprimirNumero (g, O);

    objeto teste = nova_funcao (imprimirNumero);

    objeto test2 = novo_objeto ();
    
    //executar (g, teste);

    adicionar (test2, "imprimir", teste);

    //imprimir (pegar (test2, "imprimir"));
    //imprimir (BIBOBJ_PICKBETA (pegar (test2, "imprimir")));
    //imprimir (BIBOBJ_PICKBETA (BIBOBJ_PICKBETA (pegar (test2, "imprimir"))));
    //imprimir (BIBOBJ_PICKALFA (BIBOBJ_PICKBETA (BIBOBJ_PICKBETA (pegar (test2, "imprimir")))));
    
    objeto fun = BIBOBJ_PICKBETA (BIBOBJ_PICKBETA (pegar (test2, "imprimir")));

    //imprimir (fun, O);

    //executar (g, fun);

    alterar (g, "tipo", novo_texto ("teste"));

    //imprimir (g, O);

    adicionar (g, "funcao de imprimir", teste);
    adicionar (g, "funcao de imprimir atoa", teste);

    imprimir (g, O);

    objeto dup = duplicar (g);

    printf ("uiui");*/

    /*objeto um = novo_objeto ();

    adicionar (um, "bacate", novo_texto ("verde"));
    adicionar (um, "teste", nova_funcao (imprimir));
    adicionar (um, "teste2", nova_funcao (imprimirNumero));
    adicionar (um, "macas", novo_texto ("vermelhas"));

    objeto dois = duplicar (um);

    imprimir (um, dois, O);*/

    objeto g = novo_numeroRacional
    (nova_listagem 
        (
            "sinal"     , novo_texto ("-"),
            "numero"    , novo_texto ("123456789"),
            "decimal"   , novo_texto ("4444"),
            // "irracional", novo_texto ("pi"),
        O),         
    O);

    objeto dup = duplicar (g);

    //sohImprimirComponentes ();

    //adicionar (g, "gambiarra", dup);

    executar (dup, nova_funcao (imprimir));

    objeto nL = novo_texto ("\n");

    imprimir (nL , g, O);

    

    objeto resp = somar (g, g, O);

    imprimir (resp, O);
    //executar (g, pegar (test2, "imprimir"));

W
DEBUGAR_FIM_DO_PROGRAMA;
