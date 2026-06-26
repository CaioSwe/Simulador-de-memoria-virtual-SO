#include "priorityQueue.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct PriorityItemStr{
    PriorityItem pItem;
    double prioridade;
} PriorityItemStr;

typedef struct PriorityQueueStr{
    int tam;
    int qPreenchida;
    PriorityItemStr* itens;
} PriorityQueueStr;

PriorityQueue criaPriorityQueue(int tam){
    PriorityQueueStr* pq = (PriorityQueueStr*)malloc(sizeof(PriorityQueueStr));
    if(checkAllocation(pq, "Fila de prioridade.")) return NULL;

    pq->tam = tam;
    pq->itens = (PriorityItemStr*)malloc(sizeof(PriorityItemStr) * tam);
    if(checkAllocation(pq->itens, "Vetor da fila de prioridade.")){
        free(pq);
        return NULL;
    }
    pq->qPreenchida = 0;

    return pq;
}

static void swapItems(PriorityItemStr* item1, PriorityItemStr* item2){
    PriorityItemStr tmp = *item1;
    *item1 = *item2;
    *item2 = tmp;
}

static void checkPriorityUp(PriorityQueueStr* pq, int index) {
    while(index > 0){
        int acima = (index - 1) / 2;

        if(pq->itens[index].prioridade >= pq->itens[acima].prioridade) break;

        swapItems(&pq->itens[index], &pq->itens[acima]);

        index = acima;
    }
}

static void checkPriorityDown(PriorityQueueStr* pq, int index) {
    int indexEsq = (2 * index) + 1;
    int indexDir = (2 * index) + 2;

    int indexMenor = index;

    if(indexEsq < pq->qPreenchida && pq->itens[indexEsq].prioridade < pq->itens[indexMenor].prioridade) indexMenor = indexEsq;
    if(indexDir < pq->qPreenchida && pq->itens[indexDir].prioridade < pq->itens[indexMenor].prioridade) indexMenor = indexDir;
    
    if(indexMenor != index){
        swapItems(&pq->itens[index], &pq->itens[indexMenor]);
        checkPriorityDown(pq, indexMenor);
    }
}

void inserirPriorityQueue(PriorityQueue priorityQueue, PriorityItem valor, double prioridade){
    if(priorityQueue == NULL){
        printf("\n - inserirPriorityQueue() -> Fila de prioridade nula passada. -");
        return;
    }

    PriorityQueueStr* pq = (PriorityQueueStr*)priorityQueue;

    if(pq->qPreenchida >= pq->tam){
        printf("\n - inserirPriorityQueue() -> Fila de prioridade cheia. -");
        return;
    }

    pq->itens[pq->qPreenchida].pItem = valor;
    pq->itens[pq->qPreenchida].prioridade = prioridade;

    checkPriorityUp(pq, pq->qPreenchida);

    pq->qPreenchida += 1;
}

bool changePriorityQueue(PriorityQueue priorityQueue, PriorityItem valor, double prioridade, compararItens compFunc){
    if(priorityQueue == NULL){
        printf("\n - changePriorityQueue() -> Fila de prioridade nula passada. -");
        return false;
    }
    
    PriorityQueueStr* pq = (PriorityQueueStr*)priorityQueue;

    // Procura o valor no heap.
    for(int i = 0; i < pq->qPreenchida; i++){
        if(compFunc(pq->itens[i].pItem, valor)){
            // Caso ache, muda a prioridade e ordena o heap.
            double prevPrio = pq->itens[i].prioridade;
            pq->itens[i].prioridade = prioridade;
            
            if(prioridade < prevPrio) checkPriorityUp(pq, i);
            else checkPriorityDown(pq, i);
            
            return true;
        }
    }

    // Insere normalmente caso nao ache o valor.
    inserirPriorityQueue(priorityQueue, valor, prioridade);
    return false;
}

PriorityItem removerMinPriorityQueue(PriorityQueue priorityQueue){
    if(priorityQueue == NULL){
        printf("\n - removerPriorityQueue() -> Fila de prioridade nula passada. -");
        return NULL;
    }

    PriorityQueueStr* pq = (PriorityQueueStr*)priorityQueue;

    if(pq->qPreenchida <= 0){
        printf("\n - removerPriorityQueue() -> Fila de prioridade vazia. -");
        return NULL;
    }

    PriorityItem pMin = pq->itens[0].pItem;

    pq->itens[0] = pq->itens[pq->qPreenchida - 1];
    pq->qPreenchida -= 1;

    checkPriorityDown(pq, 0);

    return pMin;
}

PriorityItem getMinPriorityQueue(PriorityQueue priorityQueue){
    if(priorityQueue == NULL){
        printf("\n - getMinPriorityQueue() -> Fila de prioridade nula passada. -");
        return NULL;
    }

    PriorityQueueStr* pq = (PriorityQueueStr*)priorityQueue;

    if(pq->qPreenchida <= 0){
        printf("\n - getMinPriorityQueue() -> Fila de prioridade vazia. -");
        return NULL;
    }

    return pq->itens[0].pItem;
}

void destroiPriorityQueue(PriorityQueue priorityQueue, freeFunc fFunc, void* extra){
    if(priorityQueue == NULL){
        printf("\n - destroiPriorityQueue() -> Fila de prioridade nula passada. -");
        return;
    }

    PriorityQueueStr* pq = (PriorityQueueStr*)priorityQueue;

    if(fFunc != NULL){
        for(int i = 0; i < pq->qPreenchida; i++){
            fFunc(pq->itens[i].pItem, extra);
        }
    }

    free(pq->itens);
    free(pq);
}

bool isInPriorityQueue(PriorityQueue priorityQueue, PriorityItem item, compararItens compFunc){
    if(priorityQueue == NULL){
        printf("\n - isInPriorityQueue() -> Fila de prioridade nula passada. -");
        return false;
    }

    PriorityQueueStr* pq = (PriorityQueueStr*)priorityQueue;

    for(int i = 0; i < pq->qPreenchida; i++){
        if(compFunc(pq->itens[i].pItem, item)) return true;
    }

    return false;
}

bool isPriorityQueueVazia(PriorityQueue priorityQueue){
    if(priorityQueue == NULL){
        printf("\n - isPriorityQueueVazia() -> Fila de prioridade nula passada. -");
        return false;
    }

    return ((PriorityQueueStr*)priorityQueue)->qPreenchida == 0;
}

bool isPriorityQueueCheia(PriorityQueue priorityQueue){
    if(priorityQueue == NULL){
        printf("\n - isPriorityQueueCheia() -> Fila de prioridade nula passada. -");
        return true;
    }

    PriorityQueueStr* pq = (PriorityQueueStr*)priorityQueue;

    return pq->qPreenchida == pq->tam;
}