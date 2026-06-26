#ifndef _PRIORITYQUEUE_h_
#define _PRIORITYQUEUE_h_

#include <stdbool.h>
#include "fileManager.h"

/**
 * Mo'dulo especializado para a implementacao da fila de prioridade (Priority Queue).
 * 
 * Uma fila de prioridades e' uma colecao de itens aos quais e' associada uma
 * prioridade, de forma que os elementos de maior prioridade sao “servidos”
 * antes que os de menor prioridade. A prioridade e' um nu'mero inteiro de
 * forma que, quanto maior o nu'mero, maior a prioridade. O crite'rio de
 * desempate para itens de mesma prioridade nao e' definido. Cada tipo item
 * prove um mecanismo para a obtencao do valor de sua chave, aqui
 * representado pelo campo 'ch' usando a notacao comum a registros.
 * Uma fila de prioridades pode ter uma capacidade ma'xima (i.e., o nu'mero
 * ma'ximo de itens que pode armazenar) definida no momento de sua
 * criacao.
 */

typedef void* PriorityQueue;
typedef void* PriorityItem;

// Ponteiro de funcao freeFunc esta' no cabecalho "fileManager.h".

/**
 * @brief Cria uma PriorityQueue (fila de prioridade).
 * @param tam Tamanho da 'a ser criada.
 * @return Retorna um ponteiro para a fila de prioridade criada.
 */
PriorityQueue criaPriorityQueue(int tam);

/**
 * @brief Insere uma nova informacao na PriorityQueue especificada.
 * @param PriorityQueue PriorityQueue a ser inserida um valor novo.
 * @param valor Ponteiro para um valor a ser inserido na fila.
 * @param prioridade Prioridade do item a ser inserido.
 * @return Nao ha' retorno de algum valor.
 */
void inserirPriorityQueue(PriorityQueue priorityQueue, PriorityItem valor, double prioridade);

/**
 * @brief Muda a prioridade de um elemento da PriorityQueue, caso o elemento nao exista na fila, insere com a nova prioridade.
 * @param priorityQueue PriorityQueue a ter a prioridade alterada.
 * @param valor Ponteiro para um valor a ser alterado na fila.
 * @param prioridade Nova prioridade do item.
 * @param compFunc Ponteiro para uma funcao externa de comparacao dos itens dentro da fila.
 * @return Retorna true (Verdadeiro) caso o valor exista na fila, false (Falso) caso cntra'rio.
 */
bool changePriorityQueue(PriorityQueue priorityQueue, PriorityItem valor, double prioridade, compararItens compFunc);

/**
 * @brief Remove o elemento de menor prioridade da PriorityQueue.
 * @param priorityQueue PriorityQueue a ser retidada o valor.
 * @return Retorna um ponteiro para o elemento retirado.
 */
PriorityItem removerMinPriorityQueue(PriorityQueue priorityQueue);

/**
 * @brief Pega o elemento de menor prioridade da PriorityQueue sem retirar da fila.
 * @param priorityQueue PriorityQueue a ser pega o valor.
 * @return Retorna um ponteiro para o elemento retirado.
 */
PriorityItem getMinPriorityQueue(PriorityQueue priorityQueue);

/**
 * @brief Destroi a PriorityQueue especificada.
 * @param priorityQueue Fila de prioridade a ser destrui'da.
 * @param fFunc Ponteiro para um funcao de liberacao para os valores inseridos na fila.
 * @param extra Ponteiro para um item auxiliar a ser passada para a funcao freeFunc caso necessa'rio.
 * @return Nao ha' retorno de algum valor.
 */
void destroiPriorityQueue(PriorityQueue priorityQueue, freeFunc fFunc, void* extra);

/**
 * @brief Verifica se a fila de prioridade conte'm o item passado.
 * @param priorityQueue Fila de prioridade para verificacao.
 * @param item Item a ser buscado na fila.
 * @param compFunc Ponteiro para uma funcao externa de comparacao dos itens dentro da fila.
 * @return Retorna Verdadeiro (True) caso a fila contenha o item, Falso (False) caso contrário.
 */
bool isInPriorityQueue(PriorityQueue priorityQueue, PriorityItem item, compararItens compFunc);

/**
 * @brief Verifica se a fila de prioridade especificada esta' vazia.
 * @param priorityQueue Fila de prioridade para verificacao.
 * @return Retorna Verdadeiro (True) caso a fila esteja vazia, Falso (False) caso contrário.
 */
bool isPriorityQueueVazia(PriorityQueue priorityQueue);

/**
 * @brief Verifica se a fila de prioridade especificada esta' cheia.
 * @param PriorityQueue Fila de prioridade para verificacao.
 * @return Retorna Verdadeiro (True) caso a fila esteja cheia, Falso (False) caso contrário.
 */
bool isPriorityQueueCheia(PriorityQueue priorityQueue);

#endif