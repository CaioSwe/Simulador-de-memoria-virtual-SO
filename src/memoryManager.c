// Implementa o FIFO ou LRU
#include "memoryManager.h"
#include "priorityQueue.h"
#include "queue.h"

typedef struct TLBStr{
    PageReplacementAlgorithm fPageReplacementAlg;
    void* tableStructure;
} TLBStr;

typedef struct PageTableStr{
    PageReplacementAlgorithm fPageReplacementAlg;
    void* tableStructure;
} PageTableStr;

typedef struct memoryManagerStr{
    FILE* binFile;
    
    TLBStr* TLB;
    PageTableStr* pageTable;
} memoryManagerStr;

TLB memoryManager_InitTLB(PageReplacementAlgorithm fPageReplacementAlg);

// Inicializa uma memoryMng, com arquivo .bin para memória, quantQuadros e uma estrutura de TLB (se for nula, cria um memoryManager sem TLB)
MemoryManager memoryManager_Init(const char* binPath, int frameCount, PageReplacementAlgorithm fPageReplacementAlg, TLB tlb);

// Retorna o estado da TLB (se houve inicialização)
void memoryManager_printTLB(MemoryManager memMng);

bool memoryManager_isInTLB(MemoryManager memMng, int address);

// Move o endereço da tabela de páginas para a TLB manualmente
void memoryManager_moveAddressIntoTLB(MemoryManager memMng, int address);

// Já implementa a função de isInTLB
// Se não estiver na TLB, já move para a mesma
const char* memoryManager_getValueAt(MemoryManager memMng, int address);

// Fecha o arquivo bin e retorna a TLB (caso houver)
TLB memoryManager_free(MemoryManager memMng){

}

// libera a TLB passada
void memoryManager_freeTLB(TLB tlb){

}