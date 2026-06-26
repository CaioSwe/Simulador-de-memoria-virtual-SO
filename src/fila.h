#ifndef _FILA_H
#define _FILA_H

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
 * @return Um ponteiro para a fila recém-criada.
 */
Queue* initQueue();
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
/*######################################################################################################################*/
#endif