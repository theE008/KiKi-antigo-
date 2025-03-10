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

#include  "header.h"

/////////////////////////////////////////////////////////////////
// PROTÓTIPOS

objeto adicionarLetraAoFinalDo_texto (objeto texto, char letra);
objeto adicionar (objeto obj, char* nome, objeto cmp);
short int comparar (objeto a, objeto b);
objeto pegar (objeto obj, char* nome);
char* novo_chars (objeto texto);
int tamanho (objeto obj);

/////////////////////////////////////////////////////////////////
// ERRO, DEBUG E FINALIZAÇÃO

////////////////// VARIÁVEIS
static long int memoriaTotal = 0; // total de memoria usada
static     int lixo         = 0; // lixo do programa não limpo

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

    //printf ("<");

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
void BIBOBJ_SNCDL () {subirNivel_ColetorDeLixo (coletor_de_lixo);}

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
void BIBOBJ_DNCDL () {descerNivel_ColetorDeLixo (coletor_de_lixo);}
void BIBOBJ_DMCDL () {coletor_de_lixo->nivel --;}

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
void BIBOBJ_LCDL () {limpar_ColetorDeLixo (&coletor_de_lixo);}

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
    //printf (">");
    
    return tmp;
}

/**
 * Para: Programador
 * Descrição: Um encapsulamento da função mallocar para facilitar o Programador. Novamente, existem sistemas melhores para reserva de memória nesta biblioteca. Todavia, são construídos acima destes dois.
*/   
#define reservar(qnts,tipo) \
(tipo*) mallocar(qnts * sizeof (tipo));

/////////////////////////////////////////////////////////////////
// FUNÇÕES NECESSÁRIAS PARA O TEXTO

/**
 * Para: Programador
 * Descrição: Retorna o tamanho de uma string 
 * (necessário para mim, desnecessário pro usuário)
*/   
int pegar_tamanho_da_string (char* entrada)
{
    verificarErro (entrada == NULL);
    bool continuar = true;
    int tamanho = 0;

    while (continuar)
    {
        if (entrada [tamanho++] == '\0') continuar = false;
    }

    return tamanho - 1;
}

/**
 * Para: Programador
 * Descrição: Retorna um malloc de texto
 * (necessário para mim, desnecessário pro usuário)
*/   
char* reservar_string (size_t tam)
{ 
    char* tmp = (char*) mallocar (tam * sizeof (char));

    loop (x, tam) tmp [x] = ' ';

    tmp [tam] = '\0';

    return tmp;
}

/////////////////////////////////////////////////////////////////
// TIPO OBJETO

////////////////// DEFINIÇÃO DO TIPO OBJETO
typedef struct semPtr_objeto semPtr_objeto ;
typedef        semPtr_objeto*       objeto ;
typedef struct semPtr_objeto
{
    bool tipo; // complexo ou simples
    char dado; // informação que esse objeto carrega

    objeto alfa; // ponteiro pra outro objeto

    union 
    {
        funcao func; // caso eu queira salvar uma função no objeto
        objeto obj;  // o comum, salvar um objeto no objeto
    }
    beta;
}
semPtr_objeto;

/**
 * Para: Programador
 * Descrição: Garantir que ele tenha acesso aos ponteiros.
*/ 
objeto BIBOBJ_PICKALFA (objeto obj) {return obj->alfa;}
objeto BIBOBJ_PICKBETA (objeto obj) {return obj->beta.obj;}

/////////////////////////////////////////////////////////////////
// CONSTRUTORES

/**
 * Para: Usuário
 * Descrição: Retorna uma nova instância de objeto vazio.
*/ 
objeto novo_objeto ()
{
    objeto tmp = (objeto) reservar (1, semPtr_objeto);

    tmp->tipo   = false;
    tmp->dado   =  '\0';

    tmp->beta.obj = NULL;
    tmp->alfa     = NULL;

    return tmp;
}

/**
 * Para: Programador
 * Descrição: Retorna um caractere.
*/ 
objeto novo_caractere (char* c)
{
    verificarErro (c == NULL);

    objeto tmp = novo_objeto ();

    tmp->dado = c [0];

    return tmp;
}

/**
 * Para: Programador
 * Descrição: Retorna um texto configurado, mas vazio.
*/ 
objeto novo_textoVazio ()
{
    objeto resposta = novo_objeto ();

    resposta->dado =  't'; // código do sistema para componente de texto
    resposta->tipo = true; // para afirmar que ele é complexo

    // células cabeça e cauda
    resposta->alfa     = novo_caractere ("\0");
    resposta->beta.obj = novo_caractere ("\0");

    resposta->alfa->tipo     = true; // para fazer ambos serem limpos
    resposta->beta.obj->tipo = true;

    resposta->beta.obj->alfa = resposta->alfa;
    resposta->alfa->beta = resposta->beta;

    resposta->alfa->alfa         = NULL;
    resposta->beta.obj->beta.obj = NULL;  

    return resposta;
}

/**
 * Para: Usuário
 * Descrição: Retorna um texto.
*/ 
objeto novo_texto (char* texto)
{
    objeto resposta = novo_textoVazio ();

    if (texto != NULL)
    {
        int tamanho = pegar_tamanho_da_string (texto);

        loop (x, tamanho)
        {
            adicionarLetraAoFinalDo_texto (resposta, texto [x]);
        }
    }

    return resposta;
}

/**
 * Para: Programador
 * Descrição: Retorna um campo, não totalmente estruturado.
*/
objeto novo_campoSemComponente (char* nome, objeto conteudo)
{
    verificarErro (nome     == NULL);
    verificarErro (conteudo == NULL);
    verificarErro (!conteudo-> tipo);
    verificarErro 
    (
        conteudo->dado != 't' && 
        conteudo->dado != 'o' && 
        conteudo->dado != 'f'
    ); // t é o código para texto | o é o código para complexo | f para função

    objeto tmp = novo_objeto ();

    tmp->tipo = true;
    tmp->dado =  'c';
    
    tmp->alfa = novo_texto (nome);

    tmp->beta.obj = conteudo;

    return tmp;
}

/**
 * Para: Programador
 * Descrição: Retorna um compnente, não totalmente estruturado.
*/
objeto novo_componenteSemCampo (objeto campo)
{
    verificarErro (campo == NULL);
    verificarErro (!campo-> tipo);
    verificarErro (campo->dado != 'c');

    objeto tmp = novo_objeto ();
    tmp->tipo  = true;
    tmp->dado  =  'k';

    tmp->beta.obj = campo;

    return tmp;
}

/**
 * Para: Programador
 * Descrição: Retorna um compnente, o usuário não deveria acessar, para reduzir a complexidade do que ele deve acessar.
*/
objeto novo_componente (char* nome, objeto obj)
{
    return novo_componenteSemCampo (novo_campoSemComponente (nome, obj));
}

/**
 * Para: Programador
 * Descrição: O usuário não deveria precisar dizer que o objeto é complexo.
*/
objeto novo_objetoComplexo ()
{
    objeto tmp = novo_objeto ();
    tmp->tipo  = true;
    tmp->dado  =  'o';

    tmp->alfa  = novo_componente ("cabeca", novo_texto ("vazio"));
    tmp->beta.obj  = tmp->alfa;

    return tmp;
}

/**
 * Para: Programador
 * Descrição: O usuário não deveria precisar converter objeto de volta a texto.
*/
char* novo_chars (objeto texto)
{
    verificarErro (texto == NULL);
    verificarErro (texto->tipo != true);
    verificarErro (texto->dado != 't');

    char* resp = reservar_string (tamanho (texto) + 1);
    int ptr = 0;

    objeto index = texto->alfa->beta.obj;

    while (index != texto->beta.obj && index != NULL)
    {
        resp [ptr++] = index->dado;

        index = index->beta.obj;
    }

    resp [ptr] = '\0';

    return resp;
}

/**
 * Para: Usuário
 * Descrição: Permite a definição de um objeto complexo estruturadamente. 
 * Usado para passar vários argumentos como um objeto em uma função.
*/
objeto nova_listagem (char* nome, objeto comp, ...)
{
    objeto tmp = novo_objeto ();

    va_list args;
    va_start (args, comp);

    char* n = nome;
    objeto t = comp;

    while (t != NULL)
    {
        adicionar (tmp, n, t);
        
        n = va_arg (args, char*);
        if (n == NULL) break; // Para evitar acessar argumento inválido

        t = va_arg (args, objeto);
        if (t == NULL) break;
    }

    va_end (args);

    return tmp;
}

/**
 * Para: Usuário
 * Descrição: Tranforma a função em um objeto.
*/
objeto nova_funcao (funcao func)
{
    objeto tmp = novo_objeto ();
    tmp->tipo  = true;
    tmp->dado  =  'f';
    
    // tmp->alfa = novo_texto (nome); infelizmente, melhor assim
    tmp->beta.func = func;

    return tmp;
}

/**
 * Para: Usuário
 * Descrição: Executa um objeto de função passando outros objetos dentro.
*/
objeto executar (objeto obj, objeto func)
{
    verificarErro (obj == NULL || func == NULL || func->beta.func == NULL);
    verificarErro (func->dado != 'f');

    return func->beta.func (obj, NULL);
}

/////////////////////////////////////////////////////////////////
// ADDERS

/**
 * Para: Programador
 * Descrição: Não tem como fazer um nome mais auto explicativo.
*/ 
objeto adicionarLetraAoFinalDo_texto (objeto texto, char letra)
{
    verificarErro (texto == NULL || texto->alfa == NULL || texto->beta.obj == NULL);

    char a [2] =   " ";
         a [0] = letra;
    objeto   obj_letra = novo_caractere (a);
             obj_letra->tipo = true; // para mostrar que os ponteiros merecem limpeza
    
    obj_letra->alfa = texto->beta.obj->alfa;
    obj_letra->beta.obj   = texto->beta.obj;

    texto->beta.obj->alfa->beta.obj = obj_letra;
    texto->beta.obj->alfa           = obj_letra;

    // não tem como verificar se constante. Textos não tem componentes

    return texto;
}

/**
 * Para: Usuário
 * Descrição: Adiciona um componente no objeto.
*/ 
objeto adicionar (objeto obj, char* nome, objeto cmp)
{
    verificarErro (obj       == NULL);
    verificarErro (cmp       == NULL);

    if (obj->tipo == false && obj->dado == '\0')
    { 
        obj->tipo = true;
        obj->dado =  'o'; // tranformar em tipo complexo antes
        obj->alfa  = novo_componente ("cabeca", novo_texto ("vazio"));
        obj->beta.obj  = obj->alfa;

        objeto componente = novo_componente (nome, cmp);

        obj->beta.obj->alfa = componente; 
        obj->beta.obj       = componente;
    }
    else if (obj->tipo == true && obj->dado == 'o')
    {
        objeto componente = novo_componente (nome, cmp);

        obj->beta.obj->alfa = componente; 
        obj->beta.obj       = componente;
    }
    else verificarErro (1);

    // verificar se o valor não é constante
    objeto mod = novo_texto ("constante");
    objeto inv = pegar (obj, "modificador");
    if (inv != NULL && inv->beta.obj != NULL) 
        verificarErro (comparar (mod, inv->beta.obj->beta.obj) == 0.0);
    //CLEAR_OBJECT (&mod);

    return obj;
}

/**
 * Para: Usuário
 * Descrição: Adiciona um componente de texto no objeto.
*/ 
objeto adicionarTexto (objeto obj, char* a, char* b)
{
    return adicionar (obj, a, novo_texto (b));
}

/**
 * Para: Usuário
 * Descrição: Concatena o texto.
*/ 
objeto concatenar (objeto a, objeto b)
{
    verificarErro (a == NULL);
    verificarErro (b == NULL);
    verificarErro (a->tipo != true && a->dado != 't');
    verificarErro (b->tipo != true && b->dado != 't');
    verificarErro (b->alfa == NULL);

    char* txt = novo_chars (a);

    objeto texto = novo_texto (txt);

    objeto index = b->alfa->beta.obj;

    while (index != b->beta.obj && index != NULL)
    {
        adicionarLetraAoFinalDo_texto (texto, index->dado);

        index = index->beta.obj;
    }

    // não tem como verificar se constante. Textos não tem componentes

    return texto;
}

/**
 * Para: Usuário
 * Descrição: Altera o conteúdo do objeto
*/ 
objeto alterar (objeto obj, char* nome, objeto cmp)
{
    verificarErro (obj  == NULL);
    verificarErro (nome == NULL);
    verificarErro (obj->alfa == NULL);
    verificarErro (obj->tipo != true);
    verificarErro (obj->dado !=  'o');

    objeto tmp = pegar (obj, nome);

    tmp->beta.obj->beta.obj = cmp;

    return obj;
}

/////////////////////////////////////////////////////////////////
// GETTERS

/**
 * Para: Usuário
 * Descrição: Retorna um dos componentes do objeto. 
 * O componente é retornado como si, e não como componente.
*/ 
objeto pegar (objeto obj, char* nome)   
{
    verificarErro (obj  == NULL);
    verificarErro (nome == NULL);
    verificarErro (obj->alfa == NULL);
    verificarErro (obj->tipo != true);
    verificarErro (obj->dado !=  'o');

    objeto texto = novo_texto (nome);

    objeto resposta = NULL;

    objeto index = obj->alfa->alfa;

    while (index != NULL && resposta == NULL)
        if (index->beta.obj != NULL && index->beta.obj->alfa != NULL && comparar (index->beta.obj->alfa, texto) == 0)
            resposta = index;
        else index = index->alfa;

    return resposta; // se não, ele retorna NULL
}

/**
 * Para: Usuário
 * Descrição: Pega o tamanho de objetos de texto.
*/ 
int tamanho (objeto obj)
{
    verificarErro (obj       == NULL);
    verificarErro (obj->tipo != true);
    verificarErro (obj->dado !=  't');

    int resposta = -1;
    objeto index = obj->alfa;

    while (index != obj->beta.obj && index != NULL) 
    { 
        resposta++;

        index = index->beta.obj;
    }

    return resposta;
}

/**
 * Para: Usuário
 * Descrição: Pega o tamanho de objetos de texto.
*/ 
objeto duplicar (objeto obj)
{
    objeto resp = NULL;

    if (obj != NULL) // se for NULL, retorna NULL
    {
        if (obj->dado == 't') // tratamento especial, caso seja um texto
        {
            verificarErro (obj->alfa == NULL);

            resp = novo_textoVazio ();

            objeto index = obj->alfa->beta.obj;

            while (index != obj->beta.obj && index != NULL)
            {
                adicionarLetraAoFinalDo_texto (resp, index->dado);

                index = index->beta.obj;
            }
        }
        else if (obj->dado == 'o') 
        {
            verificarErro (obj->alfa == NULL);

            resp = novo_objetoComplexo ();

            objeto index = obj->alfa->alfa; 

            while (index != NULL)
            {
                verificarErro (index->beta.obj == NULL);

                char * a = novo_chars (index->beta.obj->alfa);
                //printf ("<%c>", index->beta.obj->beta.obj->dado);
                adicionar (resp, a, duplicar (index->beta.obj->beta.obj));
                
                //free (a); lixo --;
                
                index = index->alfa;
            }
        }
        else if (obj->dado == 'f')
        {
            resp = nova_funcao (obj->beta.func);        
        }
        else // tratamento comum
        {        
            resp =  novo_objeto (); 
            resp->tipo = obj->tipo;
            resp->dado = obj->dado;

            resp->alfa = duplicar (obj->alfa);
            resp->beta.obj = duplicar (obj->beta.obj);
        }
    }

    return resp;
}

/**
 * Para: Usuário
 * Descrição: Retorna o tipo do objeto.
*/ 
objeto tipo (objeto obj)
{
    objeto resp;
    objeto t;

    if (obj == NULL) resp = novo_texto ("nulo");
    else 
    {
        if (obj->tipo == true)
            switch (obj->dado)
            {
                case 't':
                    resp = novo_texto ("texto");
                break;

                case 'c':
                    resp = novo_texto ("campo");
                break;

                case 'k':
                    resp = novo_texto ("componente");
                break;

                case 'o':
                    t = pegar (obj, "tipo");
                    if (t == NULL) resp = novo_texto ("desconhecido");
                    else if (t->beta.obj != NULL) resp = duplicar (t->beta.obj->beta.obj);
                    else resp = novo_texto ("bomba"); // pois quebraria
                break;
                
                default:
                    resp = novo_texto ("desconhecido");
                break;
            }
        else // só pode ser objeto vazio e caractere
            if (obj->dado == '\0') resp = novo_texto ("vazio");
            else resp = novo_texto ("caractere");
    }

    return resp;
}

/////////////////////////////////////////////////////////////////
// COMPARADORES

/**
 * Para: Usuário
 * Descrição: Retorna um inteiro da diferença entre os dois objetos.
 * Zero significa iguais.
 * Um significa o segundo elemento é maior. 
 * Menos um significa o primeiro elemento é maior.
*/ 
short int comparar (objeto a, objeto b)
{
    short int resposta = 0;

    if (a == NULL)
    {
        if (b == NULL) resposta = 0; // se ambos forem NULL
        else // se A for NULL e B não for
        {
            resposta = 1;
        }
    }
    else // se A não for NULL
    {
        if (b == NULL) // se B for, retorna A
        {
            resposta = -1;
        }
        else // se ambos existirem
        {
            if (a->tipo != b->tipo) resposta = (a->tipo > b->tipo)? -1 : 1;
            else
            {
                if (a->dado != b->dado) resposta = (a->dado)? -1 : 1; // se os dados forem diferentes também
                else // se não houver diferenças na estrutura de si mesmos
                {
                    if (a->dado == 't') // se for texto, tem que verificar um a um
                    {
                        objeto indexa = a->alfa;
                        objeto indexb = b->alfa;

                        while (indexa != NULL && indexb != NULL && !resposta)
                        {
                            resposta = (indexa->dado != indexb->dado)?(indexa->dado > indexb->dado)?-1:1:0;

                            indexa = indexa->beta.obj;
                            indexb = indexb->beta.obj;
                        }

                        if (!resposta)
                        {
                            if (indexa == NULL) resposta += 1;
                            if (indexb == NULL) resposta -= 1;
                        }
                    }
                    else resposta = comparar(a->alfa, b->alfa);

                    if (!resposta)
                    if (a->dado != 't')
                    {
                        resposta = comparar (a->beta.obj, b->beta.obj);
                    }
                }
            }
        }
    } 

    return resposta; // retorna null quando são iguais. Retorna o componente que os diferencia se o achar.
}

/////////////////////////////////////////////////////////////////
// IMPRIMIR

/**
 * Para: Programador
 * Descrição: Dá tab no printar.
*/ 
void TABNAR_COD (int dif, int TAB_NO_OBJ) {if (TAB_NO_OBJ != -1) {loop (x, TAB_NO_OBJ) {printf ("\t");}TAB_NO_OBJ += dif;}}

/**
 * Para: Programador
 * Descrição: Imprime um objeto.
 * Designado ao programador, já que uma alternativa que printa vários está logo abaixo.
*/ 
bool imprimirComponentes = false;
void sohImprimirComponentes () {imprimirComponentes = true ;}
void naoImprimirComponentes () {imprimirComponentes = false;}
void PRINT_OBJECT (objeto obj)
{
    static int TAB_NO_OBJ = 0;

    if (obj == NULL) printf ("#nulo");
    else 
    {
        if (obj->tipo) // objetos complexos
        {
            switch (obj->dado) // para definir um tratamento para cada tipo
            {
                case 'c':
                    verificarErro (obj->alfa == NULL || obj->beta.obj == NULL);

                    PRINT_OBJECT (obj->alfa); // imprimir o nome
                    printf (": ");

                    if (obj->beta.obj->dado == 't') printf ("\""); else if (TAB_NO_OBJ == 0) printf ("{");
                    PRINT_OBJECT (obj->beta.obj);
                    if (obj->beta.obj->dado == 't') printf ("\""); else if (TAB_NO_OBJ == 0) printf ("}");                        
                break;

                case 'k':
                    printf ("{");
                    PRINT_OBJECT (obj->beta.obj);
                    printf ("}");
                break;

                case 'o':
                    verificarErro (obj->alfa == NULL || obj->beta.obj == NULL);

                    objeto func = pegar (obj, "imprimir");
                    if (func != NULL && !imprimirComponentes)
                    {
                        executar (obj, func->beta.obj->beta.obj);
                    }
                    else 
                    {
                        if (TAB_NO_OBJ != -1) TAB_NO_OBJ ++; printf ("{"); if (TAB_NO_OBJ != -1) printf ("\n");

                        objeto index = obj->alfa->alfa;
    
                        while (index != NULL)
                        {
                            verificarErro (index->beta.obj == NULL);
    
                            TABNAR_COD (0, TAB_NO_OBJ); PRINT_OBJECT (index->beta.obj);
    
                            index = index->alfa;
    
                            if (index != NULL) printf (","); if (TAB_NO_OBJ != -1) printf ("\n");
                        }
    
                        if (TAB_NO_OBJ != -1) TAB_NO_OBJ --;
    
                        TABNAR_COD (0, TAB_NO_OBJ); printf ("}");
                    }
                break;

                case 't': // se for um texto
                    verificarErro (obj->alfa == NULL || obj->beta.obj == NULL);

                    if (obj->alfa->beta.obj == obj->beta.obj) printf ("#textoVazio");
                    else // se realmente tiver um texto
                    {
                        objeto index = obj->alfa->beta.obj;

                        while (index != obj->beta.obj && index != NULL)
                        {
                            printf ("%c", index->dado);

                            index = index->beta.obj;
                        }
                    }
                break;

                case 'f':
                    printf ("<funcao>");
                break;

                default:
                    printf ("\n\tTIPO DESCONHECIDO '%c'", obj->dado);
                    verificarErro (1001); // printando (10) dado sem existencia (01)
                break;
            }
        }
        else // objetos simples (só existem 2)
        {
            if (obj->dado == '\0') // se for objeto vazio
            {
                printf ("#vazio");
            }
            else // se for caractere
            {
                printf ("%c", obj->dado);   
            }
        }
    }
}

/**
 * Para: Usuário
 * Descrição: Imprimir definitivo, simplesmente o ideal.
*/ 
objeto imprimir (objeto obj, ...)
{
    va_list args;
    va_start (args, obj);

    objeto b = obj; // Corrigido para um ponteiro

    while (b != NULL)
    {
        PRINT_OBJECT (b); // Chama a função de imprimir corretamente

        b = va_arg (args, objeto);
    }

    va_end (args);

    return NULL;
}

/////////////////////////////////////////////////////////////////
// SALVAR E CARREGAR

/**
 * Para: Programador
 * Descrição: Salva um byte por vez.
*/ 
void SAVE_BYTE (int val, FILE* arquivo)
{
    unsigned char byte = val; 
    fwrite (&byte, sizeof (unsigned char), 1, arquivo);
}

/**
 * Para: Programador
 * Descrição: Salva um objeto em um arquivo.
*/ 
void SAVE_OBJECT (objeto obj, FILE* arquivo)
{
    if (obj == NULL)
    {
        // escreve zero para falar de objeto vazio
        SAVE_BYTE (0x00, arquivo);
    }
    else 
    {
        // escreve um para falar de objeto com dado
        SAVE_BYTE (0x01, arquivo);

        if (obj->tipo)
        {
            // escreve um para falar de objeto complexo
            SAVE_BYTE (0x01, arquivo);

            // salva o dado
            SAVE_BYTE (obj->dado, arquivo);

            switch (obj->dado)
            {
                case 'o':
                    verificarErro (obj->alfa == NULL); // dado inexistente

                    objeto componente = obj->alfa->alfa;

                    while 
                    (
                        componente             != NULL && // acessa os componentes
                        componente->beta.obj       != NULL && 
                        componente->beta.obj->alfa != NULL &&
                        componente->beta.obj->beta.obj != NULL
                    ) 
                    {
                        SAVE_OBJECT (componente->beta.obj->alfa, arquivo); // nome do componente
                        SAVE_OBJECT (componente->beta.obj->beta.obj, arquivo); // o objeto no componente

                        componente = componente->alfa;
                    }

                    SAVE_BYTE (0x00, arquivo); // faz um \0 para finalizar
                break;

                case 't':
                    verificarErro (obj->alfa == NULL || obj->beta.obj == NULL); // dado inexistente

                    objeto letra = obj->alfa->beta.obj;

                    while (letra != NULL && letra != obj->beta.obj)
                    {
                        SAVE_BYTE (letra->dado, arquivo); // salva os caracteres

                        letra = letra->beta.obj;
                    }

                    SAVE_BYTE (0x00, arquivo); // faz um \0 para finalizar
                break;

                default:
                    verificarErro (3001); // salvando (30) dado sem existencia (01)
                break;
            }
        }
        else
        {
            // escreve um para falar de objeto simples
            SAVE_BYTE (0x00, arquivo);

            // salva o dado por fim
            SAVE_BYTE (obj->dado, arquivo);
        }
    }
}

/**
 * Para: Usuário
 * Descrição: Versão amigável. Só se passa o caminho e objeto.
*/ 
void salvar (objeto obj, char* caminho)
{
    FILE* arquivo = fopen (caminho, "ab");

    SAVE_OBJECT (obj, arquivo);

    fclose (arquivo);
}

/**
 * Para: Programador
 * Descrição: Lê um byte.
*/ 
int READ_BYTE (FILE* arquivo)
{
    unsigned char byte;

    size_t lidos = fread (&byte, sizeof (unsigned char), 1, arquivo); // Lê 1 byte

    verificarErro (lidos != 1);

    return byte;
}

/**
 * Para: Programador
 * Descrição: Transforma os dados do arquivo em um objeto.
*/ 
objeto READ_OBJECT (FILE* arquivo)
{
    objeto resp = NULL;

    int atual = READ_BYTE (arquivo); // existe?

    if (atual == 0x00) resp = NULL; // não existe
    else 
    {
        bool tipo = READ_BYTE (arquivo); // tipo
        char dado = READ_BYTE (arquivo); // dado

        if (tipo) // tipo complexo
        {
            switch (dado)
            {
                case 'o':
                    resp = novo_objeto ();

                    objeto obj = READ_OBJECT (arquivo); // pega o nome
                    char*  txt = novo_chars (obj);
                    //CLEAR_OBJECT (&obj);

                    obj = READ_OBJECT (arquivo);

                    adicionar (resp, txt, obj);

                    //free (txt); lixo --;
                break;

                case 't':
                    resp = novo_textoVazio ();

                    char letra = READ_BYTE (arquivo);

                    while (letra != 0x00)
                    {
                        adicionarLetraAoFinalDo_texto (resp, (char) letra);

                        letra = READ_BYTE (arquivo);
                    }
                break;

                default:
                    verificarErro (4001); // lendo (40) dado sem existencia (01)
                break;
            }
        }
        else // tipo simples
        {
            if (dado == '\0') // se for objeto vazio
                resp = novo_objeto ();
            else // se for caractere
            {
                char tmp [2] =   "";
                     tmp [0] = dado;

                resp = novo_caractere (tmp);
            }
        }
    }

    return resp;
}

/**
 * Para: Usuário
 * Descrição: Versão amigável.
*/ 
objeto ler (char* caminho)
{
    FILE* arquivo = fopen (caminho, "rb");

    objeto resp = READ_OBJECT (arquivo);

    fclose (arquivo);
    return resp;
}

/////////////////////////////////////////////////////////////////
// CONSTRUTOR DA BIBLIOTECA

/**
 * Para: Programador
 * Descrição: Inicia algumas definições importantes do programa.
*/ 
static void iniciar_biblioteca (void) __attribute__ ((constructor));
static void iniciar_biblioteca (void) 
{
    coletor_de_lixo = novo_ColetorDeLixo ();
}