#ifndef OBJETO_H
#define OBJETO_H

/**
 * Criador: Thiago Pereira de Oliveira
 * Data: 01/2025
 * Objetivo: Ser a criação e o tratamento de um tipo 'objeto' unificada em propósito. 
 * 
 * O tipo Objeto tem várias construções antes de se tornar um objeto complexo:
 * - objeto simples
 * - caractere
 * - texto 
 * - campo
 * - objeto complexo
 * 
 * O objeto complexo vai ser apenas uma lista renomeada.
 * 
 * O objeto complexo vai ser uma lista de campos.
 * 
 * Campos vão ser feitos com 'alfa' texto para o nome e 'beta' com ou um objeto complexo, ou um texto
 * 
 * Texto será uma lista duplamente encadeada de caracteres
 * 
 * Os outros tipos anteriormente planejados (lista e componentes) são irrelevantes. 
 * Uma lista pode ser feita dentro de um objeto complexo, e campos já são componentes.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

// PARTE DA BIBLIOTECA NUCLEAR NECESSÁRIA PARA OBJETO EXISTIR
/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////
// LOOPS (tem que estar aqui aparentemente)

// for menor 
#define loop(qnts,vari) for (int vari = 0, max = qnts; vari < max; vari++)

/////////////////////////////////////////////////////////////////////////////
// ERRO, DEBUG E FINALIZAÇÃO

// variáveis
long int memT = 0; // total de memoria usada
int lixo = 0;

// finaliza o programa
void finalizar (int val)
{
     if (val)
        printf ("\n\n\tPROGRAMA FINALZADO COM ERRO CODIGO '%d'\n\n", -val); 
     
    printf ("\n\n\tFIM DO PROGRAMA\n\tFinalizado com um total de '%d' lixo\n\n\t'%d' memoria usada\n", lixo, memT);

    printf ("\n");

    exit (val);
}

// mata programa se erro
void VERIFY_ERROR (int erro, char* arquivo, int linha, const char* funcao)
{
    if (erro)
    {
        printf (
            "\n\n\tERRO NO ARQUIVO: %s\n\tDA FUNCAO: %s\n\tNA LINHA : %d\n\n", 
            arquivo,
            funcao,
            linha
            );

        finalizar (erro);
    }
}
#define verificar_erro(erro) VERIFY_ERROR (erro, __FILE__, __LINE__, __func__);

// linha de debug
#define DEBUGLINE_DEBUGLINE_DEBUGLINE_DEBUGLINE_DEBUGLINE printf ("\n\tlinha %d\n", __LINE__);

/////////////////////////////////////////////////////////////////////////////
// RESERVAR E LIMPAR

// funções
void* mallocar (size_t tam) {void* tmp = malloc (tam); verificar_erro (tmp == NULL); lixo++; memT++; return tmp;}

// definição de reservar (remover depois, a função só será realmente usada uma vez)
#define reservar(qnts,tipo) (semPtr_##tipo*) mallocar (qnts * sizeof (semPtr_##tipo));

//   O usuário não deveria ter acesso a isso, já que objetos não são algo que ele deveria
// se preocupar em reservar. Ele tem uma estrutura sem fim por um motivo!

/////////////////////////////////////////////////////////////////////////////
// FUNÇÃO NECESSÁRIA PARA O TEXTO

// pega o tamanho (não para uso do usuário)
int pegar_tamanho_da_string (char* entrada)
{
    verificar_erro (entrada == NULL);
    bool continuar = true;
    int tamanho = 0;

    while (continuar)
    {
        if (entrada [tamanho++] == '\0') continuar = false;
    }

    return tamanho - 1;
}

// reservar de texto (não para o usuário)
char* reservar_string (size_t tam)
{ 
    char* tmp = (char*) mallocar (tam * sizeof (char));

    loop (tam, x) tmp [x] = ' ';

    tmp [tam] = '\0';

    return tmp;
}

// TIPO OBJETO EM SI
/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////
// TIPO OBJETO

// protótipo da struct
typedef struct semPtr_objeto semPtr_objeto;
typedef        semPtr_objeto*       objeto;

// definição do tipo objeto
typedef struct semPtr_objeto
{
    bool tipo; // se o tipo for false, não há necessidade de limpar os objetos apontados

   char dado;
   /*
          Menor tipo de dado possível do objeto, só acessível quando o objeto é simples.
        
          Para objetos especiais do sistema (como texto) será usado em vez de um nome, para
        identificar o tipo.
   */

    objeto alfa; 
    objeto beta;
    /*
          Para montar todo tipo de arquitetura
    */
}
semPtr_objeto;

/////////////////////////////////////////////////////////////////////////////
// CONSTRUTORES

// protótipos usados aqui
objeto adicionarLetraAoFinalDo_texto (objeto texto, char letra);
objeto adicionar (objeto obj, char* nome, objeto cmp);
int tamanho (objeto obj);

// retorna uma versão sem definição ainda
objeto novo_objeto ()
{
    objeto tmp = reservar (1, objeto);

    tmp->tipo   = false;
    tmp->dado   =  '\0';

    tmp->alfa =  NULL;
    tmp->beta =  NULL;

    return tmp;
}

// retorna um objeto de caractere
objeto novo_caractere (char* c)
{
    verificar_erro (c == NULL);

    objeto tmp = novo_objeto ();

    tmp->dado = c [0];

    return tmp;
}

// retorna um objeto de texto
objeto novo_textoVazio ()
{
    objeto resposta = novo_objeto ();

    resposta->dado =  't'; // código do sistema para componente de texto
    resposta->tipo = true; // para afirmar que ele é complexo

    // células cabeça e cauda
    resposta->alfa = novo_caractere ("\0");
    resposta->beta = novo_caractere ("\0");

    resposta->alfa->tipo = true; // para fazer ambos serem limpos
    resposta->beta->tipo = true;

    resposta->beta->alfa = resposta->alfa;
    resposta->alfa->beta = resposta->beta;

    resposta->alfa->alfa = NULL;
    resposta->beta->beta = NULL;  

    return resposta;
}

// retorna um objeto de texto
objeto novo_texto (char* texto)
{
    objeto resposta = novo_textoVazio ();

    if (texto != NULL)
    {
        int tamanho = pegar_tamanho_da_string (texto);

        loop (tamanho, x)
        {
            adicionarLetraAoFinalDo_texto (resposta, texto [x]);
        }
    }

    return resposta;
}

// retorna um campo (não ao uso do usuário)
objeto novo_campoSemComponente (char* nome, objeto conteudo)
{
    verificar_erro (conteudo == NULL);
    verificar_erro (!conteudo-> tipo);
    verificar_erro (conteudo->dado != 't' && conteudo->dado != 'o'); // t é o código para texto | o é o código para complexo

    objeto tmp = novo_objeto ();
    tmp->tipo = true;
    tmp->dado =  'c';
    
    tmp->alfa = novo_texto (nome);

    tmp->beta = conteudo;

    return tmp;
}

// retorna um componente (não ao uso do usuário)
objeto novo_componenteSemCampo (objeto campo)
{
    verificar_erro (campo == NULL);
    verificar_erro (!campo-> tipo);
    verificar_erro (campo->dado != 'c');

    objeto tmp = novo_objeto ();
    tmp->tipo  = true;
    tmp->dado  =  'k';

    tmp->beta = campo;

    return tmp;
}

// ao uso do usuário talvez, pois cria o campo de forma correta pra ele
objeto novo_componente (char* nome, objeto obj)
{
    return novo_componenteSemCampo (novo_campoSemComponente (nome, obj));
}

/*   
    Retorna um objeto complexo (o usuário não deve ter acesso a isso, o objeto comum deve 
   ser convertido em complexo de acordo com a necessidade).
*/
objeto novo_objetoComplexo ()
{
    objeto tmp = novo_objeto ();
    tmp->tipo  = true;
    tmp->dado  =  'o';

    tmp->alfa  = novo_componente ("cabeca", novo_texto ("vazio"));
    tmp->beta  = tmp->alfa;

    return tmp;
}

// converte texto em chars (melhor se o usuário não usar)
char* novo_chars (objeto texto)
{
    verificar_erro (texto == NULL);
    verificar_erro (texto->tipo != true);
    verificar_erro (texto->dado != 't');

    char* resp = reservar_string (tamanho (texto) + 1);
    int ptr = 0;

    objeto index = texto->alfa->beta;

    while (index != texto->beta && index != NULL)
    {
        resp [ptr++] = index->dado;

        index = index->beta;
    }

    resp [ptr] = '\0';

    return resp;
}

// adiciona vários componentes no objeto (ideal para argumentos de função)
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

/////////////////////////////////////////////////////////////////////////////
// FUNÇÕES DE SAVE e LOAD

// protótipos necessários
void CLEAR_OBJECT (objeto *obj);

// salva byte
void SAVE_BYTE (int val, FILE* arquivo)
{
    unsigned char byte = val; 
    fwrite (&byte, sizeof (unsigned char), 1, arquivo);
}
// salva o objeto
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
                    verificar_erro (obj->alfa == NULL); // dado inexistente

                    objeto componente = obj->alfa->alfa;

                    while 
                    (
                        componente             != NULL && // acessa os componentes
                        componente->beta       != NULL && 
                        componente->beta->alfa != NULL &&
                        componente->beta->beta != NULL
                    ) 
                    {
                        SAVE_OBJECT (componente->beta->alfa, arquivo); // nome do componente
                        SAVE_OBJECT (componente->beta->beta, arquivo); // o objeto no componente

                        componente = componente->alfa;
                    }

                    SAVE_BYTE (0x00, arquivo); // faz um \0 para finalizar
                break;

                case 't':
                    verificar_erro (obj->alfa == NULL || obj->beta == NULL); // dado inexistente

                    objeto letra = obj->alfa->beta;

                    while (letra != NULL && letra != obj->beta)
                    {
                        SAVE_BYTE (letra->dado, arquivo); // salva os caracteres

                        letra = letra->beta;
                    }

                    SAVE_BYTE (0x00, arquivo); // faz um \0 para finalizar
                break;

                default:
                    verificar_erro (3001); // salvando (30) dado sem existencia (01)
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
// versão user_friendly
void salvar (objeto obj, char* caminho)
{
    FILE* arquivo = fopen (caminho, "ab");

    SAVE_OBJECT (obj, arquivo);

    fclose (arquivo);
}

// pega o byte
int READ_BYTE (FILE* arquivo)
{
    unsigned char byte;

    size_t lidos = fread (&byte, sizeof (unsigned char), 1, arquivo); // Lê 1 byte

    verificar_erro (lidos != 1);

    return byte;
}
// compila o objeto
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
                    CLEAR_OBJECT (&obj);

                    obj = READ_OBJECT (arquivo);

                    adicionar (resp, txt, obj);

                    free (txt); lixo --;
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
                    verificar_erro (4001); // lendo (40) dado sem existencia (01)
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
// versão user_friendly
objeto ler (char* caminho)
{
    FILE* arquivo = fopen (caminho, "rb");

    objeto resp = READ_OBJECT (arquivo);

    fclose (arquivo);
    return resp;
}

/////////////////////////////////////////////////////////////////////////////
// FUNÇÕES DE CRESCIMENTO DE OBJETO

// funções necessárias
short int comparar (objeto a, objeto b);
objeto pegar (objeto obj, char* nome);
void CLEAR_OBJECT (objeto *obj);
void PRINT_OBJECT (objeto obj);

// adiciona uma letra ao final do texto (não para uso de usuário)
objeto adicionarLetraAoFinalDo_texto (objeto texto, char letra)
{
    verificar_erro (texto == NULL || texto->alfa == NULL || texto->beta == NULL);

    char a [2] =   " ";
         a [0] = letra;
    objeto   obj_letra = novo_caractere (a);
             obj_letra->tipo = true; // para mostrar que os ponteiros merecem limpeza
    
    obj_letra->alfa = texto->beta->alfa;
    obj_letra->beta = texto->beta;

    texto->beta->alfa->beta = obj_letra;
    texto->beta->alfa       = obj_letra;

    // não tem como verificar se constante. Textos não tem componentes

    return texto;
}

// adiciona um componente no objeto
objeto adicionar (objeto obj, char* nome, objeto cmp)
{
    verificar_erro (obj       == NULL);
    verificar_erro (cmp       == NULL);

    if (obj->tipo == false && obj->dado == '\0')
    { 
        obj->tipo = true;
        obj->dado =  'o'; // tranformar em tipo complexo antes
        obj->alfa  = novo_componente ("cabeca", novo_texto ("vazio"));
        obj->beta  = obj->alfa;

        objeto componente = novo_componente (nome, cmp);

        obj->beta->alfa = componente; 
        obj->beta       = componente;
    }
    else if (obj->tipo == true && obj->dado == 'o')
    {
        objeto componente = novo_componente (nome, cmp);

        obj->beta->alfa = componente; 
        obj->beta       = componente;
    }
    else verificar_erro (1);

    // verificar se o valor não é constante
    objeto mod = novo_texto ("constante");
    objeto inv = pegar (obj, "modificador"); 
    if (inv != NULL && inv->beta != NULL) 
        verificar_erro (comparar (mod, inv->beta->beta) == 0.0);
    CLEAR_OBJECT (&mod);

    return obj;
}
objeto adicionarTexto (objeto obj, char* a, char* b)
{
    return adicionar (obj, a, novo_texto (b));
}

// concatena o texto
objeto concatenar (objeto a, objeto b)
{
    verificar_erro (a == NULL);
    verificar_erro (b == NULL);
    verificar_erro (a->tipo != true && a->dado != 't');
    verificar_erro (b->tipo != true && b->dado != 't');
    verificar_erro (b->alfa == NULL);

    char* txt = novo_chars (a);

    objeto texto = novo_texto (txt);

    objeto index = b->alfa->beta;

    while (index != b->beta && index != NULL)
    {
        adicionarLetraAoFinalDo_texto (texto, index->dado);

        index = index->beta;
    }

    free (txt); lixo --;

    // não tem como verificar se constante. Textos não tem componentes

    return texto;
}

/////////////////////////////////////////////////////////////////////////////
// FUNÇÃO DE IMPRIMIR

// variável global que imprimir usa
int TAB_NO_OBJ = 0; // controla a impressao de objeto complexo. Defina em 0 para que não dê tabulação
void TABNAR_COD (int dif) {if (TAB_NO_OBJ != -1) {loop (TAB_NO_OBJ, x) {printf ("\t");}TAB_NO_OBJ += dif;}}

// imprimir todo o dado
void PRINT_OBJECT (objeto obj)
{
    if (obj == NULL) printf ("#nulo");
    else 
    {
        if (obj->tipo) // objetos complexos
        {
            switch (obj->dado) // para definir um tratamento para cada tipo
            {
                case 'c':
                    verificar_erro (obj->alfa == NULL || obj->beta == NULL);

                    PRINT_OBJECT (obj->alfa); // imprimir o nome
                    printf (": ");

                    if (obj->beta->dado == 't') printf ("\""); else if (TAB_NO_OBJ == 0) printf ("{");
                    PRINT_OBJECT (obj->beta);
                    if (obj->beta->dado == 't') printf ("\""); else if (TAB_NO_OBJ == 0) printf ("}");                        
                break;

                case 'k':
                    printf ("{");
                    PRINT_OBJECT (obj->beta);
                    printf ("}");
                break;

                case 'o':
                    verificar_erro (obj->alfa == NULL || obj->beta == NULL);
                    
                    if (TAB_NO_OBJ != -1) TAB_NO_OBJ ++; printf ("{"); if (TAB_NO_OBJ != -1) printf ("\n");

                    objeto index = obj->alfa->alfa;

                    while (index != NULL)
                    {
                        verificar_erro (index->beta == NULL);

                        TABNAR_COD (0); PRINT_OBJECT (index->beta);

                        index = index->alfa;

                        if (index != NULL) printf (","); if (TAB_NO_OBJ != -1) printf ("\n");
                    }

                    if (TAB_NO_OBJ != -1) TAB_NO_OBJ --;

                    TABNAR_COD (0); printf ("}");
                break;

                case 't': // se for um texto
                    verificar_erro (obj->alfa == NULL || obj->beta == NULL);

                    if (obj->alfa->beta == obj->beta) printf ("#textoVazio");
                    else // se realmente tiver um texto
                    {
                        objeto index = obj->alfa->beta;

                        while (index != obj->beta && index != NULL)
                        {
                            printf ("%c", index->dado);

                            index = index->beta;
                        }
                    }
                break;

                default:
                    printf ("\n\tTIPO DESCONHECIDO '%c'", obj->dado);
                    verificar_erro (1001); // printando (10) dado sem existencia (01)
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
// poliprintar
void PRINT_ALL (objeto obj, ...)
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
}
// modificação com DEFINE para ter infinitos argumentos
#define imprimir(...) PRINT_ALL (__VA_ARGS__, NULL);

/////////////////////////////////////////////////////////////////////////////
// FUNÇÃO DE DUPLICAR

// protótipos necessários
void CLEAR_INDIVIDUAL (objeto *obj);

// retorna uma cópia do objeto
objeto duplicar (objeto obj)
{
    objeto resp = NULL;

    if (obj != NULL) // se for NULL, retorna NULL
    {
        if (obj->dado == 't') // tratamento especial, caso seja um texto
        {
            verificar_erro (obj->alfa == NULL);

            resp = novo_textoVazio ();

            objeto index = obj->alfa->beta;

            while (index != obj->beta && index != NULL)
            {
                adicionarLetraAoFinalDo_texto (resp, index->dado);

                index = index->beta;
            }
        }
        else if (obj->dado == 'o') 
        {
            verificar_erro (obj->alfa == NULL);

            resp = novo_objetoComplexo ();

            objeto index = obj->alfa->alfa; 

            while (index != NULL)
            {
                verificar_erro (index->beta == NULL);

                char * a = novo_chars (index->beta->alfa);
                adicionar (resp, a, duplicar (index->beta->beta));

                free (a); lixo --;

                index = index->alfa;
            }
        }
        else // tratamento comum
        {        
            resp =  novo_objeto (); 
            resp->tipo = obj->tipo;
            resp->dado = obj->dado;

            resp->alfa = duplicar (obj->alfa);
            resp->beta = duplicar (obj->beta);
        }
    }

    return resp;
}

/////////////////////////////////////////////////////////////////////////////
// FUNÇÃO DE COMPARAR DOIS OBJETOS

// Retorna 0 quando iguais, 1 quando b é 'maior', -1 quando a é 'maior'.
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

                            indexa = indexa->beta;
                            indexb = indexb->beta;
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
                        resposta = comparar (a->beta, b->beta);
                    }
                }
            }
        }
    } 

    return resposta; // retorna null quando são iguais. Retorna o componente que os diferencia se o achar.
}
// adicionar um comparar com N argumentos no futuro

/////////////////////////////////////////////////////////////////////////////
// FUNÇÕES DE PEGAR DADOS DO OBJETO

// protótipos necessários
objeto pegar (objeto obj, char* nome);
void CLEAR_OBJECT (objeto *obj);

// retorna o tipo do objeto
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
                    else if (t->beta != NULL) resp = duplicar (t->beta->beta);
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

// retorna um dos componentes do objeto complexo
objeto pegar (objeto obj, char* nome)
{
    verificar_erro (obj  == NULL);
    verificar_erro (nome == NULL);
    verificar_erro (obj->alfa == NULL);
    verificar_erro (obj->tipo != true);
    verificar_erro (obj->dado !=  'o');

    objeto texto = novo_texto (nome);

    objeto resposta = NULL;

    objeto index = obj->alfa->alfa;

    while (index != NULL && resposta == NULL)
        if (index->beta != NULL && index->beta->alfa != NULL && comparar (index->beta->alfa, texto) == 0)
            resposta = index;
        else index = index->alfa;

    CLEAR_OBJECT (&texto);

    return resposta; // se não, ele retorna NULL
}

// retorna o tamanho do texto no objeto
int tamanho (objeto obj)
{
    verificar_erro (obj == NULL);
    verificar_erro (obj->tipo != true);
    verificar_erro (obj->dado != 't');

    int resposta = -1;
    objeto index = obj->alfa;

    while (index != obj->beta && index != NULL) 
    { 
        resposta++;

        index = index->beta;
    }

    return resposta;
}

/////////////////////////////////////////////////////////////////////////////
// DESTRUTORES

// limpar objetos de acordo com o que eles são
void CLEAR_INDIVIDUAL (objeto *obj)
{
    free (*obj);
    *obj = NULL;
    lixo     --;
}
void CLEAR_OBJECT (objeto *obj)
{  
    if ((*obj) != NULL)
    {
        // printf ("<%c>", (*obj)->dado); // debug para saber como ele está limpando
        
        if ((*obj)->tipo)
        {
            switch ((*obj)->dado)
            {
                case 'c':
                    verificar_erro ((*obj)->alfa == NULL || (*obj)->beta == NULL);
                    
                    CLEAR_OBJECT (&(*obj)->alfa);
                    CLEAR_OBJECT (&(*obj)->beta);
                break;

                case 'k':
                    verificar_erro ((*obj)->beta == NULL); // apenas o beta TEM que ter conteúdo

                    CLEAR_OBJECT (&(*obj)->beta);                    
                break;

                case 'o':
                    verificar_erro ((*obj)->alfa == NULL || (*obj)->beta == NULL);

                    objeto index0 = (*obj)->alfa;

                    while (index0 != NULL)
                    {
                        objeto prox = index0->alfa;
                        CLEAR_OBJECT (&index0);
                        index0 = prox;
                    }
                break;

                case 't':
                    verificar_erro ((*obj)->alfa == NULL || (*obj)->beta == NULL);

                    objeto index = (*obj)->alfa->beta;
                    
                    CLEAR_INDIVIDUAL (&(*obj)->alfa); // remove o primeiro da lista

                    while (index != NULL)
                    {
                        objeto prox = index->beta;
                        CLEAR_INDIVIDUAL (&index);
                        index = prox;
                    }
                break;

                default: 
                    printf ("\n\tTIPO DESCONHECIDO '%c'", (*obj)->dado);
                    verificar_erro (2001); // limpando (20) dado sem existencia (01)
                break;
            }
        }
        // se não for complexo, o clear individual é o suficiente para apagar
        // então objetos vazios e caracteres acabam aqui

        CLEAR_INDIVIDUAL (&(*obj));
    }
}
// polimpar 
void CLEAR_ALL (objeto* obj, ...)
{
    va_list args;
    va_start (args, obj);

    objeto* b = obj; // Corrigido para um ponteiro

    while (b != NULL)
    {
        CLEAR_OBJECT (b); // Chama a função de limpeza corretamente

        b = va_arg (args, objeto*);
    }

    va_end (args);
}
// Macro que chama CLEAR_ALL passando os endereços
#define limpar(...)  CLEAR_ALL(ARGS_ADRESS1(__VA_ARGS__), NULL)
#define ARGS_ADRESS1(a,...) &a __VA_OPT__(, ARGS_ADRESS2(__VA_ARGS__))
#define ARGS_ADRESS2(a,...) &a __VA_OPT__(, ARGS_ADRESS3(__VA_ARGS__))
#define ARGS_ADRESS3(a,...) &a __VA_OPT__(, ARGS_ADRESS4(__VA_ARGS__))
#define ARGS_ADRESS4(a,...) &a __VA_OPT__(, ARGS_ADRESS5(__VA_ARGS__))
#define ARGS_ADRESS5(a,...) &a __VA_OPT__(, ARGS_ADRESS6(__VA_ARGS__))
#define ARGS_ADRESS6(a,...) &a __VA_OPT__(, ARGS_ADRESS7(__VA_ARGS__))
#define ARGS_ADRESS7(a,...) &a __VA_OPT__(, ARGS_ADRESS8(__VA_ARGS__))
#define ARGS_ADRESS8(a,...) &a __VA_OPT__(, ARGS_ADRESS9(__VA_ARGS__))
#define ARGS_ADRESS9(a,...) &a __VA_OPT__(, ARGS_ADRESS0(__VA_ARGS__))
#define ARGS_ADRESS0(...)   /* isso é para garantir a possibilidade de até 10 argumentos no limpar */ 

// SEMÂNTICA
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ORDEM DO PROGRAMA

// substitui o main
#define INICIO_DO_PROGRAMA int main (void) {if (1)

// substitui o return 0
#define FIM_DO_PROGRAMA return (0); }

// substitui o finalizar 0
#define DEBUGAR_FIM_DO_PROGRAMA finalizar (0); }

/////////////////////////////////////////////////////////////////////////////
// LÓGICA

// comum 
#define usoUnico "modificador", novo_texto ("uso unico")
#define fimDaListagem NULL
#define naoExiste == NULL
#define existe != NULL
#define vazio NULL 
#define seNao else 
#define e  &&
#define se if
#define ou ||
#define nao ! 

// para funções objetificadas
#define seTiverArgumento(var, arg) objeto var = ((temArgumentos) && (pegar(argumentos, arg) != NULL) && (pegar(argumentos, arg)->beta != NULL))? pegar(argumentos, arg)->beta->beta:NULL; if (var != NULL)
#define adicionarTextoNaResposta(nome,txt) adicionar (resposta, nome, novo_texto (txt)); // dá pra fazer função
#define adicionarNaResposta(nome,comp) adicionar (resposta, nome, comp); // da pra fazer função
#define apagarResposta if (resposta != NULL) limpar (resposta);
#define pegarArgumento(arg) pegar (argumentos, arg)
#define temArgumentos (argumentos!=NULL)

// tratamento em texto
#define fimDoLoopEmTexto } free (_texto_); lixo --;
#define loopEmTexto(txt,carac,var) \
char* _texto_ = novo_chars (txt);  \
loop (tamanho (txt),var)           \
{ char carac = _texto_ [var];      \
    if (1)

/////////////////////////////////////////////////////////////////////////////
// CONSTRUTORES

// Irei usar um padrão de colocar o nome na parte 'inicio' do objeto.
#define novoConstrutor(o_a, que) \
objeto nov##o_a##_##que (objeto argumentos) \
{ \
objeto resposta = novo_objeto ();    \
adicionar (resposta, "tipo", novo_texto (#que)); \
if (1)

/////////////////////////////////////////////////////////////////////////////
// FUNÇÕES

// cria um objeto sem tipo para retorno. Talvez no futuro, ter o tipo 'resposta'.
#define novaFuncao(nome) \
objeto nome (objeto argumentos) \
{ \
    objeto resposta = novo_objeto (); \
    if (1)

/*
// cria um objeto sem tipo para retorno. Esta versão aceita vários argumentos
#define novaFuncaoComArgs(nome) \
objeto nome (objeto primeiro, ...) \
{ \
    objeto resposta = novo_objeto (); \
    va_list args; \
    va_start (args, primeiro);\
    int quantiaDeArgumentos = 1; \
    objeto *obj;\
    while ((obj = va_arg (args, objeto *)) != NULL){quantiaDeArgumentos++;}\
    va_end (args);\
    objeto *argumentos = (objeto*) malloc (quantiaDeArgumentos * sizeof (semPtr_objeto));\
    va_start (args, primeiro); argumentos[0] = primeiro;\
    for (int i = 1; i < quantiaDeArgumentos; i++){argumentos[i] = *va_arg (args, objeto *);}\
    va_end (args);\
    if (1)
*/

// limpa o retorno e finaliza função
    #define funcaoSemRetorno \
    seTiverArgumento (config, "modificador") { objeto txt = novo_texto ("uso unico"); if (comparar (config, txt) == 0) limpar (argumentos); limpar (txt);}\
    limpar (resposta); \
    return NULL; \
}

// retorna a resposta
#define funcaoComRetorno \
    seTiverArgumento (config, "modificador") { objeto txt = novo_texto ("uso unico"); if (comparar (config, txt) == 0) limpar (argumentos); limpar (txt);}\
 return resposta; }

/* Isso deveria ser em todas
#define funcaoComArgsSemRetorno \
    loop (quantiaDeArgumentos, x) {free (&argumentos [x]);} free (&argumentos);\
    limpar (resposta); \
    return NULL; \
}
*/
/*
// retorna a resposta
#define funcaoComArgsERetorno return resposta; \
loop (quantiaDeArgumentos, x) {free (&argumentos [x]);} free (&argumentos);}*/

/////////////////////////////////////////////////////////////////////////////
// TIPOS

/* COMO O PROGRAMA IDENTIFICA TIPOS

    SUBTIPOS QUE O PROGRAMA IRÁ USAR

    * - objeto vazio
    * - caractere
    * - texto 
    * - campo
    * - objeto complexo
    
    DEFINIÇÕES

    -- objeto vazio
    caractere = '\0'
    tipo = simples
    descrição = ele é apenas o objeto completamente vazio.

    -- caractere
    caractere = qualquer um menos '\0'
    tipo = simples
    descrição = ele é um objeto de caractere. Ele e o anterior são os únicos de tipo simples.

    -- texto
    caractere = 't'
    tipo = complexo
    descrição = lista duplamente encadeada com cabeça (alfa) e cauda (beta) de caracteres

    -- campo
    caractere = 'c'
    tipo = complexo
    descrição = alfa segura um texto e beta segura, ou um texto, ou um objeto complexo.

    -- componente
    caractere = 'k'
    tipo = complexo
    descrição = seu alfa aponta para outro componente complexo (para fazer uma pilha). Ele aponta para
    um campo em seu beta. 

    -- objeto complexo
    caractere = 'o'
    tipo = complexo
    descrição = seu alfa aponta para uma pilha simplesmente encadeada de componentes. Seu beta aponta para
    o final da pilha. Ele é o objeto do sistema final. Com eles, infinitos outros semitipos de objetos são
    criados.
*/

/* MODIFICADORES

    MODIFICADORES QUE O PROGRAMA IRÁ DETECTAR EM TIPOS COMPLEXOS

    * - constante
    * - uso unico
    
    -- constante
    descrição: objetos complexos com este modificador não poderão ser capazes de serem alterados usando
    funções do sistema.

    -- uso unico
    descrição: objetos complexos com este modificador serão apagados pela primeira função que o detectar.

*/

#endif 
