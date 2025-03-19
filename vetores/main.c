//////////////////////////////////////////////////
// EXPLICAÇÃO DO BYTE DE CONFIGURAÇÃO

/** -- BYTE DE CONFIGURAÇÃO
 * 
 *   Antes de iniciar os dados, os valores irão ter um
 * byte de configuração.
 * 
 *   O byte irá ser dividido em dois, a primeira metade
 * falando propriedades do valor, e a segunda metade 
 * falando o sbutipo do valor. 
 * 
 *  -- PROPRIEDADES
 *  
 *  0 - valor morto
 *  0 - constante \_ Quando ambos estão 1, modificador rigido
 *  0 - maleavel  /  Quando ambos estão 0?
 *  0 - FALTA DECIDIR
 * 
 *  -- SUBTIPO
 * 
 *   Subtipos vão ser os selecionados ao sistema, para garantir
 * que tenha os essenciais, sem passar do limite de bits.
 *   
 *   Para a representação, teremos quatro bits.
 * 
 *  0 - 16
 *  0 - 8
 *  0 - 4
 *  0 - 2
 * 
 *   Somando todos, temos um total de 31 tipos do sistema possíveis.
 * 
 *   06 - Texto
 *   05 - Double
 *   04 - Memória
 *   03 - Ponteiro
 *   02 - Int
 *   01 - Manipulador
 *   00 - Vazio
 * 
 */

//////////////////////////////////////////////////
// BIBLIOTECAS

#include <stdbool.h>
#include  <stdlib.h>
#include  <stdarg.h> // para argumentos variados
#include   <stdio.h>

#include  <string.h> // strlen

//////////////////////////////////////////////////
// ERRO, DEBUG E FINALIZAÇÃO

////////////////// VARIÁVEIS
static long int memoriaTotal = 0; // total de memoria usada
static     int lixo          = 0; // lixo do programa não limpo

/**
 * Para: Programador
 * Descrição: Finaliza o programa com um código.
*/       
void finalizar (int val)
{
     if (val)
        printf ("\n\n\tPROGRAMA FINALZADO COM ERRO CODIGO '%d'\n\n", -val); 
     
    printf 
    (
        "\n\n\t%s\n\t%s '%d' %s\n\n\t'%ld' %s\n", 
        "FIM DO PROGRAMA", "Finalizado com um total de", 
        lixo, "lixo", memoriaTotal, "memoria usada"
    );

    printf ("\n");

    exit (val);
}

/**
 * Para: Programador
 * Descrição: Finaliza o programa se um erro for percebido.
*/   
void VERIFY_ERROR (int erro, char* arquivo, int linha, const char* funcao)
{
    if (erro)
    {
        printf 
        (
            "\n\n\tERRO NO ARQUIVO: %s\n\tDA FUNCAO: %s\n\tNA LINHA : %d\n\n", 
            arquivo,
            funcao,
            linha
        );

        finalizar (erro);
    }
}

/**
 * Para: Programador
 * Descrição: Uma linha de DEBUG que ajuda a identificar erros no código. 
 * Fácil de ver após debugar e fácil de limpar.
*/ 
#define DEBUGLINE_DEBUGLINE_DEBUGLINE_DEBUGLINE_DEBUGLINE \
printf ("\n\tlinha %d\n", __LINE__);

/**
 * Para: Usuário
 * Descrição: Uma forma mais enxuta od VERIFY_ERROR, tendo apenas um valor 
 * como condição. Se o valor for verdadeiro, a função dispara um erro.
*/  
#define verificarErro(erro) VERIFY_ERROR (erro, __FILE__, __LINE__, __func__);

//////////////////////////////////////////////////
// DEFINIÇÃO DO TIPO VALOR

// Definições de tipo
typedef unsigned char byte        ;
typedef               byte * valor;
typedef               void *   ptr;

// Protótipos
void registrarNaMemoria (ptr pon);

// Tamanhos comuns
int tamanhoDeDouble = sizeof (double);
int tamanhoDeByte = sizeof (byte );
int tamanhodeChar = sizeof (char );
int tamanhoDePtr  = sizeof (ptr  );
int tamanhoDeInt  = sizeof (int  );

/**
 * Para: Usuário
 * Descrição: Imprime o valor bit por bit.
*/  
void imprimirPorBit (valor val, int tamanho)
{
    verificarErro (val == NULL);
    verificarErro (tamanho <= 0);

    for (int i = tamanho - 1; i >= 0; i--)
    {
        byte b = val[i];

        for (int j = 7; j >= 0; j--)
        {
            printf ("%d", (b >> j) & 1);
        }

        printf (" "); // Separação visual entre bytes
    }

    printf ("\n");
}

/**
 * Para: Usuário
 * Descrição: Imprime endereços no mesmo sistema usado acima.
*/ 
void imprimirEndereco (void* ptr) 
{
    unsigned char * bytes = (unsigned char *) & ptr;
    for (int i = sizeof (ptr) - 1; i >= 0; i --) 
    {
        for (int j = 7; j >= 0; j --) 
        {
            printf ("%d", (bytes [i] >> j) & 1);
        }
        printf (" ");
    }
    printf ("\n");
}


//////////////////////////////////////////////////
// DEFINIÇÃO DO SUBTIPO MANIPULADOR

/**
 * Para: Usuário
 * Descrição: Constrói algo semelhante a um FILE*, só que para construção 
 * e leitura de valores.
*/  
bool usadoPelaMemoria = false; // se for usado pela memória, não salva na pilha de memória
valor novo_manipulador (valor val)
{
    verificarErro (val == NULL);

    valor tmp = malloc (tamanhoDeInt + tamanhoDePtr); // Index e o Valor propriamente dito

    verificarErro (tmp == NULL);

    // Escreve o índice (inicialmente 0) na primeira parte
    *((int *) tmp) = 0;

    // Escreve o ponteiro 'val' na segunda parte
    *((valor *) (tmp + tamanhoDeInt)) = val;

    if (!usadoPelaMemoria) 
    registrarNaMemoria (tmp);
    return tmp;
}

/**
 * Para: Usuário
 * Descrição: Retorna o índice do manipulador.
*/  
int pegar_indice (valor manipulador)
{
    verificarErro (manipulador == NULL);

    return *((int *) manipulador);
}

/**
 * Para: Usuário
 * Descrição: Atualiza o índice do manipulador.
*/  
void atualizar_indice (valor manipulador, int indice)
{
    verificarErro (manipulador == NULL);
    verificarErro (indice < 0);

    *((int *) manipulador) = indice;
}

/**
 * Para: Usuário
 * Descrição: Retorna o ponteiro do valor apontado.
*/  
valor pegar_valor (valor manipulador)
{
    verificarErro (manipulador == NULL);

    return *((valor *) (manipulador + tamanhoDeInt));
}

/**
 * Para: Usuário
 * Descrição: Retorna o próximo valor byte registrado no valor.
*/  
int pegarProximo_byte (valor manipulador)
{
    verificarErro (manipulador == NULL);

    int x = pegar_indice (manipulador);
    atualizar_indice (manipulador, x + tamanhoDeByte);

    valor val = pegar_valor (manipulador);

    return *((byte *) (val + x));
}

/**
 * Para: Usuário
 * Descrição: Retorna o próximo valor inteiro registrado no valor.
*/  
int pegarProximo_inteiro (valor manipulador)
{
    verificarErro (manipulador == NULL);

    int x = pegar_indice (manipulador);
    atualizar_indice (manipulador, x + tamanhoDeInt);

    valor val = pegar_valor (manipulador);

    return *((int *) (val + x));
}

/**
 * Para: Usuário
 * Descrição: Retorna o próximo valor double registrado no valor.
*/  
double pegarProximo_double (valor manipulador)
{
    verificarErro (manipulador == NULL);

    int x = pegar_indice (manipulador);
    atualizar_indice (manipulador, x + tamanhoDeDouble);

    valor val = pegar_valor (manipulador);

    return *((double *) (val + x));
}

/**
 * Para: Usuário
 * Descrição: Retorna o próximo valor string registrado no valor.
*/  
char * pegarProximo_string (valor manipulador)
{
    verificarErro (manipulador == NULL);

    int x = pegar_indice (manipulador);

    int tam = pegarProximo_inteiro (manipulador);
    
    atualizar_indice (manipulador, x + tamanhoDeInt + tam * tamanhodeChar);

    char * texto = malloc (tam + 1);

    verificarErro (texto == NULL);
    registrarNaMemoria (texto);

    valor val = pegar_valor (manipulador);

    int y = 0;

    for (; y < tam; y ++)
    {
        texto [y] = * ((char *) (val + x + tamanhoDeInt + y * tamanhodeChar));
    }

    texto [y] = '\0';

    return texto;
}

/**
 * Para: Usuário
 * Descrição: Escreve um byte na posição atual do valor.
*/  
void anotar_byte (valor manipulador, byte y)
{
    verificarErro (manipulador == NULL);

    int x = pegar_indice (manipulador);
    atualizar_indice (manipulador, x + tamanhoDeByte);

    valor val = pegar_valor (manipulador);

    *((byte *) (val + x)) = y;
}
void anotar_char (valor manipulador, char y) 
{anotar_byte (manipulador, y);}

/**
 * Para: Usuário
 * Descrição: Escreve um int na posição atual do valor.
*/  
void anotar_int (valor manipulador, int y)
{
    verificarErro (manipulador == NULL);

    int x = pegar_indice (manipulador);
    atualizar_indice (manipulador, x + tamanhoDeInt);

    valor val = pegar_valor (manipulador);

    *((int *) (val + x)) = y;
}

/**
 * Para: Usuário
 * Descrição: Escreve um double na posição atual do valor.
*/  
void anotar_double (valor manipulador, double y)
{
    verificarErro (manipulador == NULL);

    int x = pegar_indice (manipulador);
    atualizar_indice (manipulador, x + tamanhoDeDouble);

    valor val = pegar_valor (manipulador);

    *((double *) (val + x)) = y;
}

/**
 * Para: Usuário
 * Descrição: Escreve um string na posição atual do valor.
*/  
void anotar_string (valor manipulador, char* y)
{
    verificarErro (manipulador == NULL);

    int x = pegar_indice (manipulador);

    valor val = pegar_valor (manipulador);
    int tam = strlen (y);

    for (int z = 0; z < tam; z++)
    {
        
       *((char *) (val + x + z * tamanhodeChar)) = y [z];
    }

    atualizar_indice (manipulador, x + tam * tamanhodeChar);
}

/**
 * Para: Usuário
 * Descrição: Escreve um ptr na posição atual do valor.
*/  
void anotar_ptr (valor manipulador, ptr y)
{
    verificarErro (manipulador == NULL); // Não verifica se y é NULL, pois isto é usado para limpar trechos

    int x = pegar_indice (manipulador);
    atualizar_indice (manipulador, x + tamanhoDePtr);

    valor val = pegar_valor (manipulador);

    *((ptr *) (val + x)) = y;
}

//////////////////////////////////////////////////
// DEFINIÇÃO DO SUBTIPO INT

/**
 * Para: Usuário
 * Descrição: Retorna um valor int.
 * 
 * Tamanho: (5 bytes)
 *  - 1 byte  de configuração
 *  - 4 bytes de int
*/  
valor novo_int (int val)
{
    valor tmp = malloc (tamanhoDeByte + tamanhoDeInt);

    verificarErro (tmp == NULL);

    valor mnp = novo_manipulador (tmp);

    // Configuração
    anotar_byte (mnp, 96 + 2); // 96 = código rígido. 2 = código int

    // Dado
    anotar_int (mnp, val);

    registrarNaMemoria (tmp);
    return tmp;
}

//////////////////////////////////////////////////
// DEFINIÇÃO DO SUBTIPO PTR

/**
 * Para: Usuário
 * Descrição: Retorna um valor pointer.
 * 
 * Tamanho: (1 + PTR bytes)
 *  - 1 byte  de configuração
 *  - PTR bytes de pointer
*/  
valor novo_ptr (ptr val)
{
    valor tmp = malloc (tamanhoDeByte + tamanhoDePtr);

    verificarErro (tmp == NULL);

    valor mnp = novo_manipulador (tmp);

    // Configuração
    anotar_byte (mnp, 96 + 3); // 96 = código rígido. 3 = código ptr

    // Dado
    anotar_ptr (mnp, val);

    registrarNaMemoria (tmp);
    return tmp;
}

//////////////////////////////////////////////////
// DEFINIÇÃO DO SUBTIPO DOUBLE

/**
 * Para: Usuário
 * Descrição: Retorna um valor double.
 * 
 * Tamanho: (1 + PTR bytes)
 *  - 1 byte  de configuração
 *  - PTR bytes de pointer
*/  
valor novo_double (double val)
{
    valor tmp = malloc (tamanhoDeByte + tamanhoDeDouble);

    verificarErro (tmp == NULL);

    valor mnp = novo_manipulador (tmp);

    // Configuração
    anotar_byte (mnp, 96 + 5); // 96 = código rígido. 5 = código double

    // Dado
    anotar_double (mnp, val);

    registrarNaMemoria (tmp);
    return tmp;
}

//////////////////////////////////////////////////
// DEFINIÇÃO DO SUBTIPO MEMÓRIA

/**
 * Para: Usuário
 * Descrição: Retorna uma memória, que segura o nível em que foi criada,
 * o ponteiro para a próxima do stack, e o ponteiro para algo alocado 
 * dinâmicamente.
 * 
 * Tamanho: (5 + 2 * PTR bytes)
 *  - 1 byte  de configuração
 *  - 4 bytes de int (para o nível)
 *  - PTR bytes para o valor alocado
 *  - PTR bytes para o próximo do stack
*/  
valor nova_memoria (ptr val, ptr prox, int nivel)
{   
    valor tmp = malloc (tamanhoDeByte + tamanhoDeInt + 2 * tamanhoDePtr);

    verificarErro (tmp == NULL);
    verificarErro (val == NULL);

    valor mnp = novo_manipulador (tmp); // <- remover isso para garantir que não aloque recursivamente

    // Configuração
    anotar_byte (mnp, 96 + 4); // 96 = código rígido. 4 = código memória

    // Nível
    anotar_int (mnp, nivel);

    // Dado
    anotar_ptr (mnp, val);

    // Prox
    anotar_ptr (mnp, prox); // pensei em deixar NULL, mas é melhor aproveitar a chamada da função

    if (!usadoPelaMemoria) registrarNaMemoria (tmp);
    else free (mnp);
    return tmp ;
}

// Função auxiliar: retorna o próximo nó na pilha de memória
valor obterProximo (valor val) 
{
    return *(valor *)(val + tamanhoDeByte + tamanhoDeInt + tamanhoDePtr);
}

//////////////////////////////////////////////////
// SUBTIPO TEXTO

/**
 * Para: Usuário
 * Descrição: Retorna um valor texto.
 * 
 * Tamanho: (5 + TAM bytes)
 *  - 1 byte  de configuração
 *  - 4 bytes (int) para tamanho do texto
 *  - TAM bytes (char) para o texto em si
*/  
valor novo_texto (char * val)
{
    verificarErro (val == NULL);

    int tam = strlen (val);

    valor tmp = malloc (tamanhoDeByte + tamanhoDeInt + tam * tamanhodeChar);

    verificarErro (tmp == NULL);

    valor mnp = novo_manipulador (tmp);

    // Configuração
    anotar_byte (mnp, 96 + 6); // 96 = código rígido. 6 = código texto

    // Tamanho
    anotar_int (mnp, tam);

    // Dado
    anotar_string (mnp, val);

    registrarNaMemoria (tmp);
    return tmp;
}

//////////////////////////////////////////////////
// COLETOR DE LIXO (pilha de memória)

// Definição da pilha
valor pilhaDeMemoria;

// Amarração da pilha
__attribute__((constructor)) void construtor_pilhaDeMemoria () 
{
    ptr dummy = malloc (1);
    
    verificarErro (dummy == NULL);

    pilhaDeMemoria = nova_memoria (dummy, NULL, -1);
}

int nivel = 0;
// Adiciona um ponteiro na pilha de memória
void registrarNaMemoria (ptr pon)
{
    verificarErro (pon == NULL);

    usadoPelaMemoria = true;

    valor tmp = nova_memoria (pon, pilhaDeMemoria, nivel); // <= garanta que irá pegar o termo seguinte da pilha, não a cabeça

    pilhaDeMemoria = tmp;

    usadoPelaMemoria = false;
}

void imprimirPilha () 
{
    valor atual = pilhaDeMemoria;

    printf("=== PILHA DE MEMORIA ===\n");
    while (atual != NULL) {
        int nvl = *(int *)(atual + tamanhoDeByte);
        ptr dado = *(ptr *)(atual + tamanhoDeByte + tamanhoDeInt);
        printf("[Nivel: %d, Ponteiro: %p]\n", nvl, dado);

        atual = obterProximo (atual);
    }
    printf("========================\n");
}

// Remove da pilha os elementos com nível superior ao nível global
void limparPilha () 
{
    // Ponteiro para o nó atual e o próximo nó
    valor atual = pilhaDeMemoria;
    valor anterior = NULL;

    // Percorre a pilha até encontrar um nível menor ou igual ao global
    while (atual != NULL) 
    {
        // Pega o nível do nó atual
        int nivelNo = *(int *)(atual + tamanhoDeByte);

        // Se o nível do nó for menor ou igual ao nível global, para
        if (nivelNo <= nivel) 
        {
            break;
        }

        // Atualiza a cabeça da pilha para o próximo nó
        valor proximo = *(valor *)(atual + tamanhoDeByte + tamanhoDeInt + tamanhoDePtr);
        
        // Libera o nó atual (memória alocada)
        free(*(ptr *)(atual + tamanhoDeByte + tamanhoDeInt)); // Libera o valor armazenado
        free(atual);                           // Libera o nó

        atual = proximo; // Avança para o próximo
    }

    // Atualiza a cabeça da pilha para o último nó válido
    pilhaDeMemoria = atual;
}

void subirNaPilha ()
{
    nivel ++;
}

void descerNaPilha ()
{
    nivel --;
    limparPilha ();
}

#define M {subirNaPilha ();
#define W descerNaPilha ();}

//////////////////////////////////////////////////
// IMPRIMIR

byte pegarConfiguracao (valor val)
{
    verificarErro (val == NULL);

    return *(byte *) val;
}

void PRINT_VALUE (valor val)
{
    //imprimirPorBit (val, tamanhoDeDouble + tamanhoDeByte);

    verificarErro (val == NULL);

    valor mnp = novo_manipulador (val);

    int config = pegarProximo_byte (mnp) - 96;

    // printf ("<%d>", config);

    switch (config)
    {
        case 5:
            printf ("%lf", pegarProximo_double (mnp));
        break;

        case 6:
            printf ("%s", pegarProximo_string (mnp));
        break;

        default:    
            printf ("erro!!!!!!!!!!!!!!!");
        break;
    }
}

/**
 * Para: Usuário
 * Descrição: Imprimir definitivo, simplesmente o ideal.
*/ 
valor imprimir (valor val, ...)
{
    va_list args;
    va_start (args, val);

    valor b = val; // Corrigido para um ponteiro

    while (b != NULL)
    {
        PRINT_VALUE (b); // Chama a função de imprimir corretamente

        b = va_arg (args, valor);
    }

    va_end (args);

    return NULL;
}

#define O NULL

//////////////////////////////////////////////////
// MAIN

void main (void) 
M
    valor a = novo_double (0), b = novo_double (10);

    imprimir (novo_texto ("Integral da funcao 1 entre '"), a, novo_texto ("' e '"), b, novo_texto ("': "), O);
W

//imprimirPorBit (c, 5 * tamanhodeChar + 1 * tamanhoDeByte + 1 * tamanhoDeInt);

/*

void main (void) 
M
    M
        valor a = novo_int (44);
        valor b = novo_double (43.32);  

        printf("Antes de limpar:\n");
        imprimirPilha();    
    W

    printf("\nDepois de limpar:\n");
    imprimirPilha();

W

*/
