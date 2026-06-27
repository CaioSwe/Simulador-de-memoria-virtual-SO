#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "fileManager.h"

#include "priorityQueue.h"
#include "queue.h"
#include "lista.h"

#include "addressesManager.h"

/////////////////////////////

bool compareAddresses(void* ad1, void* ad2){
    return *(int*)ad1 == *(int*)ad2;
}

// Funcoes de print

static void printAddressMemMng(PriorityItem item, void* extra){
    MemoryManager memMng = (MemoryManager)extra;
    memoryManager_printAddressInfo(memMng, stdin, item);
}

static void pLRU(PriorityQueue pQueue, void* extra){
    runThorughPriorityQueue(pQueue, printAddressMemMng, extra);
}

static void pFIFO(Queue* queue, void* extra){
    runThroughQueue(queue, printAddressMemMng, extra);
}

// Algoritmos LRU e FIFO para substituicao de paginas na TLB e Tabela de Paginasdo MemoryManager

// Se a fila de prioridade estiver cheia, e removido o elemento de menor prioridade (quem apareceu menos), e e' substituido pelo endereco do parametro
static bool LRU(PriorityQueue pQueue, int address, bool insert, void* extra){
    // 1: Verifica o bool insert
    // 1.1: Se for true, insere o endereço na fila de prioridade
    if(insert){
        if(!isInPriorityQueue(pQueue, address, compareAddresses)){
            if(isPriorityQueueCheia(pQueue)){int addressRemoved = *(int*)removerMinPriorityQueue(pQueue);}
            inserirPriorityQueue(pQueue, address, 1.0f);
        }
        else{
            promoteElementeInPriorityQueue(pQueue, address, compareAddresses, 1.0f);
            return true;
        }
    }
    // 1.2: Se for false, verifica se está na fila de prioridade
    else{
        return isInPriorityQueue(pQueue, address, compareAddresses);
    }
}

static bool FIFO(Queue* queue, int address, bool insert, void* extra){
    // 1: Verifica o bool insert
    // 1.1: Se for true, insere o endereço na fila
    if(insert){
        if(!isInQueue(queue, compareAddresses, address)){
            if(isQueueFull(queue)) {int removedAddress = removeElem(queue);}
            insertElem(queue, address);
        }
        else {return true;}
    }
    // 1.2: Se for false, verifica se está na fila
    else{
        return isInQueue(queue, compareAddresses, address);
    }

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
        for(int i = 1; i < 3; i++) free(paths[i]);
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

    if(strcmp("LRU", paths[SUBSTYPE]) != 0 || strcmp("FIFO", paths[SUBSTYPE]) != 0){
        // Erro, algoritmo de substituição não é válido para este trabalho.
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

    const char* binPath = "../files/BACKING_STORE.bin";
    int frameCount = paths[FRAMES]; 
    int frameSize  = 256; // Tamanho do quadro de memória (em bytes) = 256 bytes = 2^8 bytes

    MemoryManager memMng = memoryManager_Init(binPath, frameCount, frameSize);

    PageReplacementAlgorithm strRep = NULL;
    Structure strPageTable = NULL;
    Structure strTLB = NULL;

    // Verifica qual algoritmo de substituição de páginas será utilizado e
    // Cria a estrutura de dados correspondente (fila de prioridade para LRU ou fila simples para FIFO)
    if(strcmp(paths[SUBSTYPE], "LRU") == 0){
        strPageTable = createPriorityQueue(frameCount);
        strTLB = createPriorityQueue(frameCount);
        strRep = LRU;
    }
    else{
        strPageTable = initQueue(frameCount);
        strTLB = initQueue(frameCount);
        strRep = FIFO;
    }

    // Adiciona a estrutura de dados criada ao gerenciador de memória
    memoryManager_addPageTable(memMng, strRep, strPageTable);
    memoryManager_addTLB(memMng, strRep, strTLB);

    // (3) Lendo o arquivo de endereços (addresses.txt)
    ////////////////////////////////////////////////

    // BIN (BACKING STORE.bin)
    const char* fPathBin = "BACKING_STORE.bin";

    // ADDR (addresses.txt)
    const char* fPathAddr = strcatcat(paths[ADDR], "");

    // SAIDA (correct.txt)
    const char* fOutputPathAddr = "../files/correct.txt";

    // Abre o caminho do output (correct.txt) em modo de escrita
    FILE* fSaida = fopen(fOutputPathAddr, "w");
    printf("\nWriting on file: %s\n", fOutputPathAddr);
    fprintf(fSaida, " == BEGINING OF EXECUTION == \n");

    processAddrFile(memMng, fPathAddr, fSaida);
    
    fprintf(fSaida, " == END OF EXECUTION == ");
    fclose(fSaida);

    //
    // Estatisticas ...
    //

    // Paths
    for(int i = 1; i < 4; i++) free(paths[i]);
    free(paths);
    
    printf("\n [SUCCESS] Program finalized \n");

    printf("\n");
    return 0;
}