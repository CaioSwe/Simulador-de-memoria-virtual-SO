#include <stdio.h>
#include <stdlib.h>

#include "queue.h"





/** ESTRUTURA: Fila
 * Esta estrutura representa uma fila implementada como uma lista simplesmente encadeada.
 * Ela possui ponteiros para o primeiro e último nó da fila, bem como um contador do tamanho atual da fila.
 * Assim como a estrutura de nó, que contém o dado do elemento e um ponteiro para o próximo nó na fila.
 * 
 * QUEUE:
 * @param front Ponteiro para o primeiro nó da fila.
 * @param rear  Ponteiro para o último nó da fila.
 * @param size  Contador do tamanho atual da fila.
 * 
 * NODE:
 * @param addr O dado do elemento armazenado no nó (neste caso, um endereço).
 * @param next Ponteiro para o próximo nó na fila.
 */
typedef struct node{
    int addr;
    struct node* next;
} Node;
typedef struct queue{
    Node* front;
    Node* rear;
    int size;
    int maxSize;
} Queue;





/*                                                  FUNÇÕES PRINCIPAIS                                                  */
Queue* initQueue(int tam){
    // 1: Aloca memória dinâmicamente para a fila
    Queue* queue = (Queue*)malloc(sizeof(Queue));

    // 2: Verifica se a alocação de memória foi bem-sucedida
    if(queue == NULL){
        printf("[ERROR]\n");
        printf("in initQueue (fila.c): Memory allocation failed for queue\n");
        return NULL;
    }

    // 3: Inicializa os ponteiros da fila e o tamanho
    queue->front = NULL;
    queue->rear  = NULL;
    queue->size  = 0;
    queue->maxSize = tam;

    // 4: Retorna o ponteiro para a fila recém-criada
    printf("[SUCCESS]\n");
    printf("in initQueue (fila.c): Queue initialized successfully\n");
    return queue;
}

void insertElem(Queue* queue, int addr){
    // 1: Verifica se a fila é válida (não nula)
    if(queue == NULL){
        printf("[ERROR]\n");
        printf("in insertElem (fila.c): Queue invalid. Pointer is NULL\n");
        return;
    }

    if(queue->maxSize == queue->size){
        printf("[ERROR]\n");
        printf("in insertElem (fila.c): Max size for queue has been reached\n");
        return;
    }

    // 2: Aloca memória para um novo nó
    Node* newNode = (Node*)malloc(sizeof(Node));
    // 2.1: Verifica se a alocação de memória para o novo nó foi bem-sucedida
    if(newNode == NULL){
        printf("[ERROR]\n");
        printf("in insertElem (fila.c): Memory allocation failed for new node\n");
        return;
    }

    // 3: Inicializa o novo nó com o endereço fornecido
    newNode->addr = addr;
    newNode->next = NULL;

    // 4: Insere o novo nó na fila
    // 4.1: Se a fila estiver vazia, o novo nó se torna o primeiro e último nó
    if(queue->rear == NULL){
        queue->front = newNode; // Atualiza o ponteiro front para apontar para o novo nó
        queue->rear  = newNode; // Atualiza o ponteiro rear para apontar para o novo nó
    } 
    // 4.2: Se a fila não estiver vazia, adiciona o novo nó ao final da fila e atualiza o ponteiro rear
    else{
        queue->rear->next = newNode;    // Atualiza o ponteiro next do último nó para apontar para o novo nó
        queue->rear = newNode;          // Atualiza o ponteiro rear para apontar para o novo nó
    }

    // 5: Incrementa o tamanho da fila
    queue->size++;
    printf("[SUCCESS]\n");
    printf("in insertElem (fila.c): Element inserted successfully\n");
}

int removeElem(Queue* queue){
    // 1: Verifica se a fila é válida (não nula)
    if(queue == NULL){
        printf("[ERROR]\n");
        printf("in removeElem (fila.c): Queue invalid. Pointer is NULL\n");
        return -1;
    }

    // 2: Verifica se a fila está vazia
    if(queue->front == NULL){
        printf("[ERROR]\n");
        printf("in removeElem (fila.c): Queue is empty. Cannot remove element\n");
        return -1;
    }

    // 3: Armazena o nó que será removido e o endereço do elemento
    Node* temp = queue->front;  // Nó a ser removido
    int addr   = temp->addr;    // Endereço do elemento a ser retornado

    // 4: Atualiza o ponteiro front para apontar para o próximo nó na fila
    queue->front = queue->front->next;

    // 5: Se a fila ficar vazia após a remoção, atualiza o ponteiro rear para NULL
    if(queue->front == NULL) {queue->rear = NULL;}

    // 6: Libera a memória do nó removido e decrementa o tamanho da fila
    free(temp);
    queue->size--;
    
    // 7: Retorna o endereço do elemento removido
    printf("[SUCCESS]\n");
    printf("in removeElem (fila.c): Element removed successfully\n");    
    return addr;
}

void freeQueue(Queue* queue){
    // 1: Verifica se a fila é válida (não nula)
    if(queue == NULL){
        printf("[ERROR]\n");
        printf("in freeQueue (fila.c): Queue invalid. Pointer is NULL\n");
        return;
    }

    // 2: Inicializa um ponteiro para percorrer a fila e um ponteiro temporário para liberar os nós
    Node* current = queue->front;
    Node* temp = NULL;

    // 3: Percorre a fila e libera cada nó
    while(current != NULL){
        temp = current;             // Armazena o nó atual em uma variável temporária
        current = current->next;    // Move para o próximo nó
        free(temp);                 // Libera a memória do nó atual
    }

    // 4: Libera a memória alocada para a estrutura da fila
    printf("[SUCCESS]\n");
    printf("in freeQueue (fila.c): Queue freed successfully\n");
    free(queue);
}
/*######################################################################################################################*/





/*                                                  FUNÇÕES AUXILIARES                                                  */
int sizeofQueue(Queue* queue){
    // 1: Verifica se a fila é válida (não nula)
    if(queue == NULL){
        printf("[ERROR]\n");
        printf("in sizeofQueue (fila.c): Queue invalid. Pointer is NULL\n");
        return -1;
    }

    // 2: Retorna o tamanho atual da fila
    return queue->size;
}

bool isQueueFull(Queue* queue){
    if(queue == NULL){
        printf("[ERROR]\n");
        printf("in sizeofQueue (fila.c): Queue invalid. Pointer is NULL\n");
        return -1;
    }

    return queue->size == queue->maxSize;
}

bool isInQueue(Queue* queue, compararItens compFunc, void* item){
    if(queue == NULL){
        printf("[ERROR]\n");
        printf("in sizeofQueue (fila.c): Queue invalid. Pointer is NULL\n");
        return -1;
    }

    Node* temp = queue->front;

    while(temp != NULL){
        if(compFunc(temp, item)) return true;
        temp = temp->next;
    }

    return false;
}
/*######################################################################################################################*/