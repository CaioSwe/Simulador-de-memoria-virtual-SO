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
    Item item;
    struct node* next;
} Node;

typedef struct QueueStr{
    Node* front;
    Node* rear;
    
    int size;
    int maxSize;
} QueueStr;

/*                                                  FUNÇÕES PRINCIPAIS                                                  */
Queue initQueue(int tam){
    // 1: Aloca memória dinâmicamente para a fila
    QueueStr* queue = (QueueStr*)malloc(sizeof(QueueStr));

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
    return queue;
}

void insertElem(Queue queue, Item item){
    // 1: Verifica se a fila é válida (não nula)
    if(queue == NULL){
        printf("[ERROR]\n");
        printf("in insertElem (fila.c): Queue invalid. Pointer is NULL\n");
        return;
    }

    QueueStr* q = (QueueStr*)queue;

    if(q->maxSize == q->size){
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
    newNode->item = item;
    newNode->next = NULL;

    // 4: Insere o novo nó na fila
    // 4.1: Se a fila estiver vazia, o novo nó se torna o primeiro e último nó
    if(q->rear == NULL){
        q->front = newNode; // Atualiza o ponteiro front para apontar para o novo nó
        q->rear  = newNode; // Atualiza o ponteiro rear para apontar para o novo nó
    } 
    // 4.2: Se a fila não estiver vazia, adiciona o novo nó ao final da fila e atualiza o ponteiro rear
    else{
        q->rear->next = newNode;    // Atualiza o ponteiro next do último nó para apontar para o novo nó
        q->rear = newNode;          // Atualiza o ponteiro rear para apontar para o novo nó
    }

    // 5: Incrementa o tamanho da fila
    q->size++;
}

Item removeElem(Queue queue){
    // 1: Verifica se a fila é válida (não nula)
    if(queue == NULL){
        printf("[ERROR]\n");
        printf("in removeElem (fila.c): Queue invalid. Pointer is NULL\n");
        return NULL;
    }

    QueueStr* q = (QueueStr*)queue;

    // 2: Verifica se a fila está vazia
    if(q->front == NULL){
        printf("[ERROR]\n");
        printf("in removeElem (fila.c): Queue is empty. Cannot remove element\n");
        return NULL;
    }

    // 3: Armazena o nó que será removido e o endereço do elemento
    Node* temp = q->front;  // Nó a ser removido
    Item item = temp->item;     // Endereço do elemento a ser retornado

    // 4: Atualiza o ponteiro front para apontar para o próximo nó na fila
    q->front = q->front->next;

    // 5: Se a fila ficar vazia após a remoção, atualiza o ponteiro rear para NULL
    if(q->front == NULL) {q->rear = NULL;}

    // 6: Libera a memória do nó removido e decrementa o tamanho da fila
    free(temp);
    q->size--;
    
    // 7: Retorna o endereço do elemento removido
    return item;
}

Item removeItemFila(Structure dataStructure, Info targetItem, compararItens compareFunc){
    // 1: Verifica se a estrutura de dados é válida (não nula)
    if(dataStructure == NULL) return NULL;

    // 2: Converte a estrutura de dados genérica para uma fila específica
    QueueStr* q = (QueueStr*)dataStructure;

    // 3: Verifica se a fila está vazia
    if (q->front == NULL) return NULL;

    // 4: Inicializa ponteiros para percorrer a fila
    Node* current = q->front;
    Node* previous = NULL;

    // 5: Percorre a fila para encontrar o item alvo usando a função de comparação fornecida
    while(current != NULL){
        // 5.1: Se o item atual for igual ao item alvo, remove o nó da fila
        if(compareFunc((Info)current->item, targetItem)){
            if(previous == NULL) {q->front = current->next;} 
            else                 {previous->next = current->next;}
            
            if(current == q->rear) {q->rear = previous;}

            Info i = current->item;
            free(current);
            q->size--;

            return i;
        }
        // 5.2: Se o item atual não for o alvo, avança para o próximo nó na fila    
        previous = current;
        current = current->next;
    }

    return NULL;
}

void freeQueue(Queue queue, freeFunc fFunc, void* extra){
    // 1: Verifica se a fila é válida (não nula)
    if(queue == NULL){
        printf("[ERROR]\n");
        printf("in freeQueue (fila.c): Queue invalid. Pointer is NULL\n");
        return;
    }

    QueueStr* q = (QueueStr*)queue;

    // 2: Inicializa um ponteiro para percorrer a fila e um ponteiro temporário para liberar os nós
    Node* current = q->front;
    Node* temp = NULL;

    // 3: Percorre a fila e libera cada nó
    while(current != NULL){
        temp = current;             // Armazena o nó atual em uma variável temporária
        current = current->next;    // Move para o próximo nó
        fFunc(temp->item, extra);
        free(temp);                 // Libera a memória do nó atual
    }

    // 4: Libera a memória alocada para a estrutura da fila
    free(q);
}
/*######################################################################################################################*/





/*                                                  FUNÇÕES AUXILIARES                                                  */
int sizeofQueue(Queue queue){
    // 1: Verifica se a fila é válida (não nula)
    if(queue == NULL){
        printf("[ERROR]\n");
        printf("in sizeofQueue (fila.c): Queue invalid. Pointer is NULL\n");
        return -1;
    }

    QueueStr* q = (QueueStr*)queue;

    // 2: Retorna o tamanho atual da fila
    return q->size;
}

bool isQueueFull(Queue queue){
    if(queue == NULL){
        printf("[ERROR]\n");
        printf("in sizeofQueue (fila.c): Queue invalid. Pointer is NULL\n");
        return -1;
    }

    QueueStr* q = (QueueStr*)queue;

    return q->size == q->maxSize;
}

bool isInQueue(Queue queue, compararItens compFunc, void* item){
    if(queue == NULL){
        printf("[ERROR]\n");
        printf("in sizeofQueue (fila.c): Queue invalid. Pointer is NULL\n");
        return -1;
    }

    QueueStr* q = (QueueStr*)queue;

    Node* temp = q->front;

    while(temp != NULL){
        if(compFunc(temp->item, item)) return true;
        temp = temp->next;
    }

    return false;
}

void runThroughQueue(Queue queue, itemFunc iFunc, void* extra){
    if(queue == NULL){
        printf("[ERROR]\n");
        printf("in sizeofQueue (fila.c): Queue invalid. Pointer is NULL\n");
        return;
    }

    QueueStr* q = (QueueStr*)queue;

    Node* temp = q->front;

    while(temp != NULL){
        iFunc(temp->item, extra);
        temp = temp->next;
    }
}
/*######################################################################################################################*/