#ifndef STATIC_LIST_H
#define STATIC_LIST_H

#include <stdbool.h>
#include <stdlib.h>

typedef void *StList;

// Cria uma lista estática
//     size: tamanho da lista
// Retorno: lista alocada ou NULL se a criação falhou
StList StList_Create(int size);
// Muda o tamanho de uma lista estática
//    staticList: ponteiro para a lista estática
//    size: novo tamanho
void StList_Resize(StList staticList, int size);
// Destrói uma lista estática
//     staticList: ponteiro para a lista estática
//     destroyElement: função que destrói um elemento da lista
void StList_Destroy(StList staticList, void destroyElement(void*));
// Adiciona um elemento a uma lista estática
//     staticList: ponteiro para a lista estática
//     element: elemento a ser adicionado
bool StList_Add(StList staticList, void *element);
// Executa a função f, mandando de parâmetros o elemento e 'parameter', destrói e remove da lista
// todos os elementos que satisfizerem a condição da função compFunc, que recebe o elemento e 'comparingField'.
//     staticList: ponteiro para a lista estática
//     f: função que será executada em cada elemento
//     parameter: parâmetro extra que será passado para a função
//     compFunc: função que aceita um elemento da lista, um parametro e retorna true ou false
//     comparingField: elemento que será passado como segundo parâmetro da função compFunc
//     destroyElement: função que destrói um elemento da lista
void StList_ExecAndDestroy(StList staticList, void f(void*, void*), void *parameter, 
                           bool compFunc(void*, void*), void *comparingField, void destroyElement(void*));
// Remove o primeiro elemento em que a função compFunc retornar true
//     staticList: ponteiro para a lista estática
//     compFunc: função que aceita um elemento da lista, um parametro e retorna true ou false
//     comparingField: elemento que será passado como segundo parâmetro da função compFunc
// Retorno: NULL se o elemento não for encontrado, um elemento caso seja
void *StList_Remove(StList staticList, bool compFunc(void*, void*), void *comparingField);
// Remove o último elemento da lista estática
//     staticList: ponteiro para a lista estática
// Retorno: elemento removido
void *StList_RemoveLast(StList staticList);
// Remove o elemento da lista estática na posição fornecida
//     staticList: ponteiro para a lista estática
//     pos: posição do elemento (fornecida pela própria lista)
// Retorno: elemento removido
void *StList_RemoveAt(StList staticList, int pos);
// Verifica se a lista está vazia
//     staticList: ponteiro para a lista estática
// Retorno: true se estiver vazia, false caso contrário
bool StList_IsEmpty(StList staticList);
// Verifica se a lista está cheia
//     staticList: ponteiro para a lista estática
// Retorno: true se estiver cheia, false caso contrário
bool StList_IsFull(StList staticList);
// Seleciona o primeiro elemento da lista
//     staticList: ponteiro para a lista estática
// Retorno: NULL se a lista estiver vazia, um elemento caso contrário
void *StList_GetFirst(StList staticList);
// Seleciona o primeiro elemento em que a função compFunc retornar true
//     staticList: ponteiro para a lista estática
//     compFunc: função que aceita um elemento da lista, um parametro e retorna true ou false
//     comparingField: elemento que será passado como segundo parâmetro da função compFunc
// Retorno: NULL se o elemento não for encontrado, um elemento caso seja
void *StList_Find(StList staticList, bool compFunc(void*, void*), void *comparingField);
// Retorna a posição (interna da lista) do primeiro elemento da lista em que a função compFunc retornar true
//     staticList: ponteiro para a lista estática
//     compFunc: função que aceita um elemento da lista, um parametro e retorna true ou false
//     comparingField: elemento que será passado como segundo parâmetro da função compFunc
// Retorno: -1 se o elemento não for encontrado, uma posição caso seja
int StList_FindPos(StList staticListVoid, bool compFunc(void*, void*), void *comparingField);
// Retorna o elemento da posição fornecida
//     staticList: ponteiro para a lista estática
//     pos: posição do elemento (fornecida pela própria lista)
// Retorno: elemento da posição ou NULL se a posiçao for inválida
void *StList_Get(StList staticListVoid, int pos);
// Executa a função f em todos os elementos presentes na lista
//     staticList: ponteiro para a lista estática
//     f: função que será executada em cada elemento
//     parameter: parâmetro extra que será passado para a função
void StList_Execute(StList staticListVoid, void f(void*, void*), void *parameter);

//void oi(StList sv); // REMOVER

#endif