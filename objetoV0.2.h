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



#endif