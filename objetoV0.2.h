#ifndef OBJETO_H
#define OBJETO_H

/**
 * Criador: Thiago Pereira de Oliveira
 * Data: 01/2025
 * Objetivo: Ser a criação e o tratamento de um tipo 'objeto' unificada em propósito. 
*/

/**
 * -- DESCRIÇÃO DA BIBLIOTECA --
 *   Ela é baseada no conceito de objeto. Eles são um tipo de dado genérico que pode segurar todos os outros tipos. O ObjetoComplexo possui uma estrutura semelhante a de árvore em memória, mas de JSON ao imprimir e para o programa.
 *   O único tipo de dado definido para isto foi o tipo "Objeto", que é um struct composto por um boolean, um caractere, um ponteiro para outro objeto, e um union entre outro ponteiro para objeto e um ponteiro para função.
 *   O subtipo baseado em objeto que o usuário poderá criar será construído acima do ObjetoComplexo. Mas para o ObjetoComplexo existir, toda uma hierarquia de dados e subtipos é construida antes.
 * 
 * -- SUBTIPOS DA BIBLIOTECA --
 * 
 * - objeto vazio
 * - caractere
 * - texto
 * - campo
 * - componente
 * - arranjo
 * - função
 * - objeto complexo
 * 
 *   Os subtipos que o usuário terá acesso são os seguintes:
 * 
 * - objeto vazio - pois é o tipo mais básico de objeto.
 * - texto - é o sistema de strings que o objeto pode receber.
 * - arranjo - para o usuário ser capaz de realizar mais ações.
 * - objeto complexo - todos os subtipos criados pelo usuário serão dessa categoria.
 * 
 *   Os subtipos que o usuário não terá acesso são os seguintes:
 * 
 * - caractere - ele terá que usar texto, para garantir que o sistema não precise de funções tratando objetos semelhantes.
 * - campo - guarda um nome e um objeto complexo. Será oculto pois o usuário não precisa ver o funcionamento interno da biblioteca.
 * - componente - emprega o campo para fazer uma pilha de caracteristicas no ObjetoComplexo, o usuário não precisa saber que existe.
 * - função - um dos tipos de argumentos no objeto complexo. Novamente, desnecessário do usuário ter acesso direto.
 */

/////////////////////////////////////////////////////////////////
// BIBLIOTECAS

#include <stdlib.h>
#include <stdarg.h> // para argumentos variados
#include <stdio.h>

/////////////////////////////////////////////////////////////////
// DEFINIÇÕES DE LOOPS

/**
 * Para: Usuários
 * Descrição: Faz um FOR simplificado.
*/
#define loop(var,qnts) \
for (int var = 0, max = qnts, continuar = 1; \
var < max && continuar; var++)

/**
 * Para: Usuários
 * Descrição: Para o LOOP.
*/
#define pararLoop continuar = 0;

/////////////////////////////////////////////////////////////////
// ERRO, DEBUG E FINALIZAÇÃO

////////////////// VARIÁVEIS
long int memoriaTotal = 0; // total de memoria usada
     int lixo         = 0; // lixo do programa não limpo

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
        "\n\n\t%s\n\t%s '%d' %s\n\n\t'%d' %s\n", 
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
 * Para: Usuário
 * Descrição: Uma forma mais enxuta od VERIFY_ERROR, tendo apenas um valor como condição. Se o valor for verdadeiro, a função dispara um erro.
*/  
#define verificarErro(erro) VERIFY_ERROR (erro, __FILE__, __LINE__, __func__);

/**
 * Para: Programador
 * Descrição: Uma linha de DEBUG que ajuda a identificar erros no código. Fácil de ver após debugar e fácil de limpar.
*/ 
#define DEBUGLINE_DEBUGLINE_DEBUGLINE_DEBUGLINE_DEBUGLINE \
printf ("\n\tlinha %d\n", __LINE__);

/////////////////////////////////////////////////////////////////
// TIPO MEMÓRIA

////////////////// DEFINIÇÃO DO TIPO MEMORIA
typedef struct semPtr_memoria semPtr_memoria;
typedef        semPtr_memoria*       memoria;
typedef struct semPtr_memoria
{
    memoria prox; // ponteiro para próxima memória da pilha
    void*  dados; // ponteiro para os dados armazenados
    int    nivel; // guarda a data de validade do tipo de dado
}
semPtr_memoria;

/**
 * Para: Programador
 * Descrição: Reserva memória com uma segurança. Desnecessário considerando o resto da biblioteca, já que outras funções de criação de dados serão mais apetitosas.
*/   
void* mallocarSimples (size_t tam) 
{
    void* tmp = malloc (tam); 

    verificarErro (tmp == NULL); 
    
    return tmp;
}

////////////////// CONSTRUTOR
memoria nova_memoria (void* ptr, int nivel)
{
    memoria tmp = (memoria) mallocarSimples (sizeof (semPtr_memoria));

    tmp->nivel = nivel;
    tmp->prox  =  NULL;
    tmp->dados =   ptr;

    return tmp;
}

////////////////// DESTRUTOR
void limpar_memoria (memoria *mem)
{
    verificarErro (  mem == NULL || *mem == NULL);
    verificarErro ((*mem)->dados         == NULL);

    free ((*mem)->dados);
    free  (*mem);

    lixo --;

    mem = NULL;
}

/////////////////////////////////////////////////////////////////
// TIPO GARBAGE COLLECTOR

////////////////// DEFINIÇÃO DO TIPO GARBAGE COLLECTOR
typedef struct semPtr_ColetorDeLixo semPtr_ColetorDeLixo;
typedef        semPtr_ColetorDeLixo*       ColetorDeLixo;
typedef struct semPtr_ColetorDeLixo
{
    memoria topo; // denota o topo da pilha de memória

    int nivel; // denota a data de validade do tipo
}
semPtr_ColetorDeLixo;
ColetorDeLixo coletor_de_lixo;

////////////////// CONSTRUTOR
ColetorDeLixo novo_ColetorDeLixo ()
{
    ColetorDeLixo tmp = (ColetorDeLixo) mallocarSimples 
    (
        sizeof (semPtr_ColetorDeLixo)
    );

    tmp->nivel = 0;

    // célula cabeça
    tmp->topo = nova_memoria (mallocarSimples (sizeof (char)), 0);

    return tmp;
} 

/**
 * Para: Programador
 * Descrição: Adiciona um ponteiro no coletor
*/  
void adicionarNo_ColetorDeLixo (ColetorDeLixo cdl, void* ptr)
{
    verificarErro (cdl       == NULL);
    verificarErro (cdl->topo == NULL);

    memoria nova_mem = nova_memoria (ptr, cdl->nivel);

    nova_mem->prox = cdl->topo->prox;
    cdl->topo->prox = nova_mem;    
}

/**
 * Para: Programador
 * Descrição: Apaga o primeiro ponteiro
*/  
void limparTopoDo_ColetorDeLixo (ColetorDeLixo cdl)
{
    verificarErro (cdl             == NULL);
    verificarErro (cdl->topo       == NULL);
    verificarErro (cdl->topo->prox == NULL);

    memoria tmp = cdl->topo->prox;

    cdl->topo->prox = cdl->topo->prox->prox;

    limpar_memoria (&tmp);
}

/**
 * Para: Programador
 * Descrição: Pega o nível do topo da pilha
*/  
int nivelDoTopoDo_ColetorDeLixo (ColetorDeLixo cdl)
{
    verificarErro (cdl             == NULL);
    verificarErro (cdl->topo       == NULL);

    int resp = -1;

    if (cdl->topo->prox != NULL) resp = cdl->topo->prox->nivel;

    return resp;
}

/**
 * Para: Programador
 * Descrição: Avisa o CDL que chaves foram abertas.
*/   
void subirNivel_ColetorDeLixo (ColetorDeLixo cdl)
{
    cdl->nivel ++;
}

/**
 * Para: Programador
 * Descrição: Avisa o CDL que chaves foram fechadas.
*/   
void descerNivel_ColetorDeLixo (ColetorDeLixo cdl)
{
    cdl->nivel --;

    while (nivelDoTopoDo_ColetorDeLixo (cdl) > cdl->nivel)
    {
        limparTopoDo_ColetorDeLixo (cdl);
    }
}

////////////////// DESTRUTOR
void limpar_ColetorDeLixo (ColetorDeLixo *cdl)
{
    verificarErro (  cdl == NULL || *cdl == NULL);
    verificarErro ((*cdl)->topo          == NULL);

    memoria tmpProx = (*cdl)->topo->prox;

    limpar_memoria (&(*cdl)->topo);

    while (tmpProx != NULL)
    {
        memoria aux = tmpProx->prox;
        limpar_memoria (&tmpProx);
        tmpProx = aux;
    }

    free (*cdl);
    *cdl = NULL;

    lixo ++; // uso a função de limpar memória no nó cabeça
    // devo somar lixo para equilibrar ele, já que o nó cabeça 
    // não é considerado memória.
}

/**
 * Para: Usuário
 * Descrição: Serve de chaves {}, mas se usadas corretamente, irão agir como garbage collector.
*/   
#define M {subirNivel_ColetorDeLixo (coletor_de_lixo);
#define W descerNivel_ColetorDeLixo (coletor_de_lixo);}

/////////////////////////////////////////////////////////////////
// FUNÇÕES DE RESERVA

/**
 * Para: Programador
 * Descrição: Reserva memória com uma segurança. Desnecessário considerando o resto da biblioteca, já que outras funções de criação de dados serão mais apetitosas.
*/   
void* mallocar (size_t tam) 
{
    void* tmp = malloc (tam); 

    verificarErro (tmp == NULL); 
    memoriaTotal++; 
    lixo++; 

    adicionarNo_ColetorDeLixo (coletor_de_lixo, tmp);
    
    return tmp;
}

/**
 * Para: Programador
 * Descrição: Um encapsulamento da função mallocar para facilitar o Programador. Novamente, existem sistemas melhores para reserva de memória nesta biblioteca. Todavia, são construídos acima destes dois.
*/   
#define reservar(qnts,tipo) \
(tipo*) mallocar (qnts * sizeof (tipo));

/////////////////////////////////////////////////////////////////
// ORDEM DO PROGRAMA

// substitui o main
#define INICIO_DO_PROGRAMA \
int main (void) \
{ \
coletor_de_lixo = novo_ColetorDeLixo (); \
if (1)

// substitui o return 0
#define FIM_DO_PROGRAMA \
limpar_ColetorDeLixo (&coletor_de_lixo); \
return (0); }

// substitui o finalizar 0
#define DEBUGAR_FIM_DO_PROGRAMA \
limpar_ColetorDeLixo (&coletor_de_lixo); \
finalizar (0); }

// faz o mesmo que o anterior, sem limpar a memória (para debug)
#define PARAR_PROGRAMA \
finalizar (0); }

#endif