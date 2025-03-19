#ifndef OBJETO_H
#define OBJETO_H

/////////////////////////////////////////////////////////////////
// BIBLIOTECAS

#include <stdbool.h>
#include  <stdlib.h>
#include  <stdarg.h> // para argumentos variados
#include   <stdio.h>

/////////////////////////////////////////////////////////////////
// PROTÓTIPOS

typedef struct semPtr_objeto semPtr_objeto ;
typedef        semPtr_objeto*       objeto ;
typedef           objeto (*funcao) (objeto, ...); // tipo função

objeto adicionar (objeto obj, char* nome, objeto cmp);
objeto alterar   (objeto obj, char* nome, objeto cmp);
objeto executar (objeto obj, objeto func);
objeto pegar     (objeto obj, char* nome);
short int comparar (objeto a, objeto b);
objeto imprimir  (objeto obj, ...);
objeto nova_funcao (funcao func);
objeto duplicar  (objeto obj);

void sohImprimirComponentes ();
void naoImprimirComponentes ();

/////////////////////////////////////////////////////////////////
// PROTÓTIPOS DO SISTEMA

/**
 * Para: Programador
 * Descrição: Chamadas de funções que o usuário não deveria ter acesso,
 * mas que matam a biblioteca se não tiver.
*/
void VERIFY_ERROR (int erro, char* arquivo, int linha, const char* funcao);
objeto BIBOBJ_PICKALFA (objeto obj);
objeto BIBOBJ_PICKBETA (objeto obj);
void BIBOBJ_SNCDL ();
void BIBOBJ_DNCDL ();
void BIBOBJ_DMCDL ();
void BIBOBJ_LCDL  ();

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

/**
 * Para: Usuário
 * Descrição: Uma forma mais enxuta od VERIFY_ERROR, tendo apenas um valor como condição. Se o valor for verdadeiro, a função dispara um erro.
*/  
#define verificarErro(erro) VERIFY_ERROR (erro, __FILE__, __LINE__, __func__);

/**
 * Para: Usuário
 * Descrição: Finaliza o programa com um código.
*/  
void finalizar (int val);

/////////////////////////////////////////////////////////////////
// TIPO GARBAGE COLLECTOR

/**
 * Para: Usuário
 * Descrição: Serve de chaves {}, mas se usadas corretamente, irão agir como garbage collector.
*/   
#define M {BIBOBJ_SNCDL ();
#define W  BIBOBJ_DNCDL ();}

#define M0 {BIBOBJ_DMCDL ();
#define W0  BIBOBJ_SNCDL ();}

/////////////////////////////////////////////////////////////////
// ORDEM DO PROGRAMA

/**
 * Para: Usuário
 * Descrição: Substitui o main
*/ 
#define INICIO_DO_PROGRAMA \
int main (void) \
{ \
if (1)

/**
 * Para: Usuário
 * Descrição: Substitui o return 0
*/ 
#define FIM_DO_PROGRAMA \
BIBOBJ_LCDL (); \
return (0); }

/**
 * Para: Usuário
 * Descrição: Substitui o finalizar 0
*/ 
#define DEBUGAR_FIM_DO_PROGRAMA \
BIBOBJ_LCDL (); \
finalizar (0); }

/**
 * Para: Programador
 * Descrição: Finaliza o programa sem apagar a memória
 * (usado para debug)
*/ 
#define PARAR_PROGRAMA \
finalizar (0);

/////////////////////////////////////////////////////////////////
// CONSTRUTORES

objeto novo_objeto ();
objeto novo_texto (char* texto);
objeto nova_listagem (char* nome, objeto comp, ...);

/////////////////////////////////////////////////////////////////
// SEMÂNTICA

////////////////// GERAL
#define naoExistir == NULL 
#define forDiferente !=
#define existir != NULL
#define ouSe else if 
#define forIgual == 
#define seNao else 
#define nao !
#define se if
#define ou ||
#define e &&

////////////////// PARA FUNÇÕES OBJETIFICADAS
#define seTiverArgumento(var, arg) objeto var = ((temArgumentos) && (pegar(argumentos, arg) != NULL) && (BIBOBJ_PICKBETA(pegar(argumentos, arg)) != NULL))? BIBOBJ_PICKBETA(BIBOBJ_PICKBETA(pegar(argumentos, arg))):NULL; if (var != NULL)
#define copiarArgumentoNaResposta(var,arg) seTiverArgumento(var, arg) adicionar (resposta, arg, duplicar (var))
#define adicionarTextoNaResposta(nome,txt) adicionar (resposta, nome, novo_texto (txt)); // dá pra fazer função
#define adicionarNaResposta(nome,comp) adicionar (resposta, nome, comp); // da pra fazer função
#define apagarResposta if (resposta != NULL) limpar (resposta);
#define pegarArgumento(arg) pegar (argumentos, arg)
#define temArgumentos (argumentos!=NULL)
#define O NULL
#define pegarParaResposta(arg,holder) M0 seTiverArgumento(var##__LINE__, arg) adicionar (resposta, arg, duplicar (var##__LINE__));else{adicionarNaResposta(arg, holder);} W0

////////////////// TRATAMENTO EM TEXTO
#define loopEmTexto(txt,carac,var) \
char* _texto_ = novo_chars (txt);  \
loop (tamanho (txt),var)           \
{ char carac = _texto_ [var]; if (1)
#define fimDoLoopEmTexto }

////////////////// CONSTRUTORES

// Irei usar um padrão de colocar o nome na parte 'inicio' do objeto.
#define novoConstrutor(o_a, que) \
objeto nov##o_a##_##que (objeto argumentos, ...) \
{ \
    objeto resposta = novo_objeto ();    \
    va_list args; \
    va_start (args, argumentos); \
    adicionar (resposta, "tipo", novo_texto (#que)); \
    do \
    {

// altera o protótipo usado pelo construtor
#define usarDeBase(construtor) \
objeto BIBOBJ_TIPO = BIBOBJ_PICKBETA (BIBOBJ_PICKBETA (pegar (resposta, "tipo"))); \
resposta = construtor (argumentos, O); \
alterar (resposta, "tipo", duplicar (BIBOBJ_TIPO));

////////////////// FUNÇÕES

// cria um objeto sem tipo para retorno. Talvez no futuro, ter o tipo 'resposta'.
#define novaFuncao(nome) \
objeto nome (objeto argumentos, ...) \
{ \
    va_list args; \
    va_start (args, argumentos); \
    objeto resposta = novo_objeto (); \
    do \
    {

// limpa o retorno e finaliza função
    #define funcaoSemRetorno \
    if (argumentos != NULL) argumentos = va_arg (args, objeto);} while (argumentos != NULL); \
    return NULL; \
}

// retorna a resposta
#define funcaoComRetorno  if (argumentos != NULL) argumentos = va_arg (args, objeto);} while (argumentos != NULL); return resposta;}



/*\ FIM DA DEFINIÇÃO DA BIBLIOTECA OBJETO \*/ #endif
