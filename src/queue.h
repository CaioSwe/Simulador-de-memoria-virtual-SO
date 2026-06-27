#ifndef _QUEUE_H
#define _QUEUE_H

#include "fileManager.h"

typedef struct node Node;
typedef struct queue Queue;





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
Queue* initQueue(int tam);
/**
 * Esta função insere um novo elemento na fila.
 * @param queue Um ponteiro para a fila onde o elemento será inserido.
 * @param addr  O endereço do elemento a ser inserido na fila.
 */
void insertElem(Queue* queue, int addr);
/**
 * Esta função remove o primeiro elemento da fila.
 * @param queue Um ponteiro para a fila da qual o elemento será removido.
 * @return      O endereço do elemento removido.
 */
int removeElem(Queue* queue);
/**
 * Esta função libera a memória alocada para a fila.
 * @param queue Um ponteiro para a fila a ser liberada.
 */
void freeQueue(Queue* queue);
/*######################################################################################################################*/





/*                                                  FUNÇÕES AUXILIARES                                                  */
/**
 * Esta função retorna o tamanho atual da fila.
 * @param queue Um ponteiro para a fila a ser verificada.
 * @return      O tamanho da fila.
 */
int sizeofQueue(Queue* queue);

/**
 * Esta função retorna um booleano indicando se a fila esta cheia ou nao.
 * @param queue Um ponteiro para a fila a ser verificada.
 * @return      Verdadeiro se fila esta cheia, falso caso contrario.
 */
bool isQueueFull(Queue* queue);

/**
 * Esta função retorna um booleano indicando se o elemento passado esta presente na fila.
 * @param queue Um ponteiro para a fila a ser verificada a presenca do valor.
 * @param compFunc Funcao de comparacao de cada item
 * @param item Item para ser verificado
 * @return      Verdadeiro se o valor esta na fila, falso caso contrario.
 */
bool isInQueue(Queue* queue, compararItens compFunc, void* item);
/*######################################################################################################################*/
#endif