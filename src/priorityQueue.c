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

void promoteElementeInPriorityQueue(PriorityQueue priorityQueue, PriorityItem item, compararItens compareFunc, double newPriority){
    if(priorityQueue == NULL){
        printf("\n - promoteElementeInPriorityQueue() -> Fila de prioridade nula passada. -");
        return;
    }
    
    PriorityQueueStr* pq = (PriorityQueueStr*)priorityQueue;

    // Procura o valor no heap.
    for(int i = 0; i < pq->qPreenchida; i++){
        if(compareFunc(pq->itens[i].pItem, item)){
            // Caso ache, muda a prioridade e ordena o heap.
            double prevPrio = pq->itens[i].prioridade;
            pq->itens[i].prioridade += newPriority;
            
            if(newPriority < prevPrio) checkPriorityUp(pq, i);
            else checkPriorityDown(pq, i);
            
            return;
        }
    }

    printf("\n - promoteElementeInPriorityQueue() -> Elemento nao encontrado na fila de prioridade. -");
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

void removeItemPriorityQueue(Structure dataStructure, Info targetItem, bool (*compareFunc)(Info, Info)){
    // 1: Verifica se a estrutura de dados é nula
    if(dataStructure == NULL) return;

    // 2: Converte a estrutura de dados genérica para a estrutura específica da fila de prioridade
    PriorityQueueStr* pq = (PriorityQueueStr*)dataStructure;

    // 3: Verifica se a fila de prioridade está vazia
    if(pq->qPreenchida <= 0) return;

    // 4: Percorre o vetor heap da fila de prioridade para encontrar o item alvo usando a função de comparação fornecida
    for(int i = 0; i < pq->qPreenchida; i++){
        // Se o item alvo for encontrado, remove-o da fila de prioridade
        if(compareFunc((Info)pq->itens[i].pItem, targetItem)){
            // 4.1: Se o item tiver sido alocado dinamicamente, libera a memória associada a ele
            if(pq->itens[i].pItem != NULL) {free(pq->itens[i].pItem);}

            // 4.2: Se o item for único ou estiver no final do Heap, apenas decrementa a quantidade preenchida
            if(i == pq->qPreenchida - 1) {pq->qPreenchida--;}
            // 4.3: Se o item estiver no meio do Heap, substitui-o pelo último elemento e ajusta a posição do Heap
            else{
                // Obtém a prioridade antiga do item removido e a prioridade do último item do Heap
                double oldPriority = pq->itens[i].prioridade;
                double newPriority = pq->itens[pq->qPreenchida - 1].prioridade;

                // Move o último elemento do Heap para a posição que ficou vaga e decrementa a quantidade preenchida
                pq->itens[i] = pq->itens[pq->qPreenchida - 1];
                pq->qPreenchida--;

                // Compara a nova prioridade com a antiga para saber se o item deve subir ou descer
                if(newPriority < oldPriority) {checkPriorityUp(pq, i);}
                else                          {checkPriorityDown(pq, i);}
            }
            // 4.4: Sai do loop após remover o item, pois não há necessidade de continuar procurando
            return;
        }
    }
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

void runThroughPriorityQueue(PriorityQueue priorityQueue, itemFunc iFunc, void* extra){
    if(priorityQueue == NULL){
        printf("\n - isInPriorityQueue() -> Fila de prioridade nula passada. -");
        return;
    }

    PriorityQueueStr* pq = (PriorityQueueStr*)priorityQueue;

    for(int i = 0; i < pq->qPreenchida; i++){
        iFunc(pq->itens[i].pItem, extra);
    }
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