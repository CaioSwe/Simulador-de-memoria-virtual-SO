#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "fileManager.h"

#include "priorityQueue.h"
#include "queue.h"

#include "addressesManager.h"

/////////////////////////////

// SUBSTITUICAO DE PAGINA

static bool LRU(PriorityQueue pQueue, Info pageInfo, bool insert, void* extra){
    static double lruClock = 0.0f;

    // 1: Verifica o bool insert
    // 1.1: Se for true, insere o endereço na fila de prioridade    
    if(insert){
        if(!isInPriorityQueue(pQueue, pageInfo, memoryManager_comparePagesInfo)){
            if(isPriorityQueueCheia(pQueue)){
                Info removedPageInfo = removerMinPriorityQueue(pQueue);
                if(extra != NULL) *(Info*)extra = removedPageInfo;

                int frameNumber = memoryManager_getFrameNumber(removedPageInfo);
                printf("\n %d", frameNumber);
                memoryManager_setFrameNumber(pageInfo, frameNumber);
            }
            inserirPriorityQueue(pQueue, pageInfo, (float)(++lruClock));
            return false;
        }
        else{
            promoteElementeInPriorityQueue(pQueue, pageInfo, memoryManager_comparePagesInfo, (float)(++lruClock));
            return true;
        }
    }
    // 1.2: Se for false, verifica se está na fila de prioridade
    else{
        return isInPriorityQueue(pQueue, pageInfo, memoryManager_comparePagesInfo);
    }
}

static bool FIFO(Queue queue, Info pageInfo, bool insert, void* extra){
    // 1: Verifica o bool insert
    // 1.1: Se for true, insere o endereço na fila
    if(insert){
        if(!isInQueue(queue, memoryManager_comparePagesInfo, pageInfo)){
            if(isQueueFull(queue)){
                Info removedPageInfo = removeElem(queue);
                if(extra != NULL) *(Info*)extra = removedPageInfo;

                int frameNumber = memoryManager_getFrameNumber(removedPageInfo);
                memoryManager_setFrameNumber(pageInfo, frameNumber);
            }
            insertElem(queue, pageInfo);
            return false;
        }
        else{
            return true;
        }
    }
    // 1.2: Se for false, verifica se está na fila
    else{
        return isInQueue(queue, memoryManager_comparePagesInfo, pageInfo);
    }
}

// PRINT

static const char* printMemoryContent(Item item, void* extra){
    signed char c = *(signed char*)item;

    static char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d", c);
    const char* p = buffer;

    return p;
}

//////////////////////////////

// Tipos do 
typedef enum TYPES {
    ADDR,
    FRAMES,
    SUBSTYPE
} TYPES;

//////////////////////////////

int main(int argc, char* argv[]){
    printf("\n");

    char** paths = calloc(3, sizeof(char*));

    if(argc < 4){
        printf(" [ERROR]: Number of parameters is invalid. \n");
        free(paths);
        return 1;
    }

    /*
        Pega todos os nomes dos arquivos base e organiza na variável paths
        paths[0] = arquivo de endereços
        paths[1] = quadros
        paths[2] = tipo de algoritmo de substituição de página
    */
    for(int i = 1; i < argc; i++){
        int param = i - 1;
        free(paths[param]);
        paths[param] = malloc(strlen(argv[i]) + 1);
        strcpy(paths[param], argv[i]);
    }

    if(strcmp("LRU", paths[SUBSTYPE]) != 0 && strcmp("FIFO", paths[SUBSTYPE]) != 0){
        // Erro, algoritmo de substituição não é válido para este trabalho.
        for(int i = 0; i < 3; i++) free(paths[i]);
        free(paths);
        return 0;
    }

    // Coloca ".txt" no arquivo de endereços caso não tenha
    char* newPathAddr = (char*)changeExtension(paths[ADDR], ".txt");
    free(paths[ADDR]);
    paths[ADDR] = newPathAddr;

    // Lista todos os parâmetros fornecidos:
    printf("%-12s: %s\n", "Addresses file", paths[ADDR]);
    printf("%-12s: %s\n", "Frames quantity", paths[FRAMES]);
    printf("%-12s: %s\n", "Page replacement algorithm", paths[SUBSTYPE]);
    
    // (2) Criando o gerenciador de memória
    ////////////////////////////////////////////////

    const char* binPath = "./files/BACKING_STORE.bin";
    int frameCount = atoi(paths[FRAMES]); 
    int frameSize  = 256; // Tamanho do quadro de memória (em bytes) = 256 bytes = 2^8 bytes

    MemoryManager memMng = memoryManager_Init(binPath, frameCount, frameSize, printMemoryContent);

    printf("\n = %p =", memMng);

    PageReplacementAlgorithm strRep = NULL;

    Structure strPageTable = NULL;
    Structure strTLB = NULL;
    
    runThroughItems runFunc = NULL;
    highFreeFunc fAlg = NULL;

    // Verifica qual algoritmo de substituição de páginas será utilizado e
    // Cria a estrutura de dados correspondente (fila de prioridade para LRU ou fila simples para FIFO)
    if(strcmp(paths[SUBSTYPE], "LRU") == 0){
        strPageTable = criaPriorityQueue(frameCount);
        strTLB = criaPriorityQueue(16);
        strRep = LRU;
        runFunc = runThroughPriorityQueue;
        fAlg = destroiPriorityQueue;
    }
    else{
        strPageTable = initQueue(frameCount);
        strTLB = initQueue(16);
        strRep = FIFO;
        runFunc = runThroughQueue;
        fAlg = freeQueue;
    }

    int pageTableSize = 65536 / frameSize;

    // Adiciona a estrutura de dados criada ao gerenciador de memória
    memoryManager_addPageTable(memMng, pageTableSize, strRep, strPageTable, runFunc, fAlg);
    memoryManager_addTLB(memMng, strRep, strTLB, runFunc, fAlg);

    // (3) Lendo o arquivo de endereços (addresses.txt)
    ////////////////////////////////////////////////

    // ADDR (addresses.txt)
    const char* fPathAddr = strcatcat(paths[ADDR], "");

    // SAIDA (correct.txt)
    const char* fOutputPathAddr = "./files/correct.txt";

    // Abre o caminho do output (correct.txt) em modo de escrita
    FILE* fSaida = fopen(fOutputPathAddr, "w");
    printf("\nWriting on file: %s\n", fOutputPathAddr);
    fprintf(fSaida, " == BEGINING OF EXECUTION == \n");

    processAddrFile(memMng, fPathAddr, fSaida);
    
    fprintf(fSaida, " == END OF EXECUTION == ");
    fclose(fSaida);

    ////////////////////////////////////////////
    // Free's

    memoryManager_free(memMng, NULL);

    // Paths
    for(int i = 0; i < 3; i++) free(paths[i]);
    free(paths);
    
    ////////////////////////////////////////////

    printf("\n [SUCCESS] Program finalized \n");

    printf("\n");
    return 0;
}