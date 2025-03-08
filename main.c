// gcc -o main main.c objv3/obj.c && main

#include "objv3/matematica.h"

/////////////////////////////////////////////////////////////////
// MAIN

INICIO_DO_PROGRAMA;
M
    objeto a = novo_numeroNatural    (O);
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

    imprimir (a, b, c, d, f, g, O);

    imprimirNumero (g, O);
W
DEBUGAR_FIM_DO_PROGRAMA;
