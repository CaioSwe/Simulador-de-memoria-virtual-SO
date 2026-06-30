#ifndef _QUEUE_H
#define _QUEUE_H

#include "fileManager.h"
#include "memoryManager.h"

typedef void* Queue;

/** MODELO: Fila
 * Este modelo de fila é implementado como uma lista simplemente encadeada, 
 * onde cada elemento da fila é representado por um nó (struct No)
 * que contém as informações do elemento e um ponteiro para o próximo nó na fila. 
 * 
 * A fila em si é representada por uma struct Fila, 
 * que mantém ponteiros para o primeiro e último nó da fila, bem como o tamanho atual da fila.
 */

/*                                                  FUNÇÕES PRINCIPAIS                                                  */
/**
 * Esta função inicializa uma nova fila vazia.
 * @param tam O tamanho máximo da fila.
 * @return    Um ponteiro para a fila recém-criada.
 */
Queue initQueue(int tam);
/**
 * Esta função insere um novo elemento na fila.
 * @param queue Um ponteiro para a fila onde o elemento será inserido.
 * @param item  Um ponteiro para o item a ser inserido na fila generica.
 */
void insertElem(Queue queue, Item item);
/**
 * Esta função remove o primeiro elemento da fila.
 * @param queue Um ponteiro para a fila da qual o elemento será removido.
 * @return      Um ponteiro para o elemento removido.
 */
Item removeElem(Queue queue);
/**
 * Esta função remove um item específico da fila, se ele estiver presente.
 * @param dataStructure Um ponteiro para a estrutura de dados da fila.
 * @param targetItem    Um ponteiro para o item a ser removido da fila.
 * @param compareFunc   Uma função de comparação que determina se dois itens são iguais.
 * @return              Não há retorno de algum valor.
 */
void removeItemFila(Structure dataStructure, Info targetItem, bool (*compareFunc)(Info, Info));
/**
 * Esta função libera a memória alocada para a fila.
 * @param queue Um ponteiro para a fila a ser liberada.
 */
void freeQueue(Queue queue, freeFunc fFunc, void* extra);
/*######################################################################################################################*/





/*                                                  FUNÇÕES AUXILIARES                                                  */
/**
 * Esta função retorna o tamanho atual da fila.
 * @param queue Um ponteiro para a fila a ser verificada.
 * @return      O tamanho da fila.
 */
int sizeofQueue(Queue queue);

/**
 * Esta função retorna um booleano indicando se a fila esta cheia ou nao.
 * @param queue Um ponteiro para a fila a ser verificada.
 * @return      Verdadeiro se fila esta cheia, falso caso contrario.
 */
bool isQueueFull(Queue queue);

/**
 * Esta função retorna um booleano indicando se o elemento passado esta presente na fila.
 * @param queue Um ponteiro para a fila a ser verificada a presenca do valor.
 * @param compFunc Funcao de comparacao de cada item.
 * @param item Item para ser verificado.
 * @return      Verdadeiro se o valor esta na fila, falso caso contrario.
 */
bool isInQueue(Queue queue, compararItens compFunc, void* item);

/**
 * Esta função percorre sob todos os itens da fila.
 * @param queue Um ponteiro da fila a ser percorrida.
 * @param iFunc Funcao a ser chamada sob cada item da fila
 * @param extra Ponteiro extra para algum conteúdo adicional
 * @return      Nao ha retorno de algum valor.
 */
void runThroughQueue(Queue queue, itemFunc iFunc, void* extra);
/*######################################################################################################################*/

#endif