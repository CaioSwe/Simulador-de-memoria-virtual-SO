// Implementa o FIFO ou LRU
#include "memoryManager.h"
#include "priorityQueue.h"
#include "lista.h"

#include "queue.h"

#include <stdlib.h>

// Estruturas do gerenciador de memoria
////////////////////////////////////////

typedef struct InfoStr{
    int page;
    int frame;
    bool ValidatingBit;
} InfoStr;

typedef struct TLBStr{
    PageReplacementAlgorithm fPageReplacementAlg;
    printFunc pFunc;
    runThroughItems runFunc;
    freeFunc fFunc;
    
    Structure dataStructure;
} TLBStr;

typedef struct PageTableStr{
    PageReplacementAlgorithm fPageReplacementAlg;
    printFunc pFunc;
    runThroughItems runFunc;
    freeFunc fFunc;

    Structure dataStructure;
} PageTableStr;

typedef struct memoryManagerStr{
    FILE* binFile;
    
    int frameCount;
    int frameSize;

    Lista physicalMemory;

    PageTableStr* pageTable;
    TLBStr* TLB;
} memoryManagerStr;

////////////////////////////////////////

// Inicializa uma memoryMng, com arquivo .bin para memória, quantQuadros e uma estrutura de TLB (se for nula, cria um memoryManager sem TLB)
MemoryManager memoryManager_Init(const char* binPath, int frameCount, int frameSize){
    memoryManagerStr* mMng = (memoryManagerStr*)malloc(sizeof(memoryManagerStr));

    if(checkAllocation(mMng, "msg de erro")) return NULL;

    FILE* binFile = fopen(binPath, "rb");

    if(binFile == NULL){
        printf("msg de erro");
        free(mMng);
        return NULL;
    }

    mMng->binFile = binFile;

    mMng->physicalMemory = criaLista();

    mMng->frameCount = frameCount;
    mMng->frameSize = frameSize;

    // // Ler binPath para popular a lista (physicalMemory)
    // Lista lista = mMng->physicalMemory;
    // while(true){
    //     inserirFim(lista, ); // ? ! a b
    // }

    mMng->pageTable = NULL;
    mMng->TLB = NULL;

    fclose(binFile);
    return mMng;
}

static TLBStr* memoryManager_InitPageTable(PageReplacementAlgorithm fPageReplacementAlg, Structure pageTableStructure){
    PageTableStr* pTable = (PageTableStr*)malloc(sizeof(PageTableStr));

    if(checkAllocation(pTable, "msg de erro")) return NULL;

    pTable->fPageReplacementAlg = fPageReplacementAlg;
    pTable->dataStructure = pageTableStructure;

    return pTable;
}

static TLBStr* memoryManager_InitTLB(PageReplacementAlgorithm fPageReplacementAlg, Structure tlbStructure){
    TLBStr* tlb = (TLBStr*)malloc(sizeof(TLBStr));

    if(checkAllocation(tlb, "msg de erro")) return NULL;

    tlb->fPageReplacementAlg = fPageReplacementAlg;
    tlb->dataStructure = tlbStructure;

    return tlb;
}

void memoryManager_addPageTable(MemoryManager memMng, PageReplacementAlgorithm fPageReplacementAlg, Structure pageTableStructure){
    memoryManagerStr* mMng = (memoryManagerStr*)memMng;

    if(mMng == NULL){
        printf("msg de erro");
        return;
    }

    mMng->pageTable = memoryManager_InitPageTable(fPageReplacementAlg, pageTableStructure);
}

void memoryManager_addTLB(MemoryManager memMng, PageReplacementAlgorithm fPageReplacementAlg, Structure tlbStructure){
    memoryManagerStr* mMng = (memoryManagerStr*)memMng;

    if(mMng == NULL){
        printf("msg de erro");
        return;
    }

    mMng->TLB = memoryManager_InitTLB(fPageReplacementAlg, tlbStructure);
}

void memoryManager_printPageTable(MemoryManager memMng){

}

// Retorna o estado da TLB (se houve inicialização)
void memoryManager_printTLB(MemoryManager memMng){
    memoryManagerStr* mMng = (memoryManagerStr*)memMng;

    runThroughItems runFunc = mMng->TLB->runFunc;
    printFunc pFunc = mMng->TLB->pFunc;

    // pFunc();
}

///////////////
/*
    3) Incluir dois comandos no arquivo address.txt:
        a) Comando PageTable: escreve a tabela de páginas da seguinte forma:
        ###########
        Página - Quadro - Bit Validade [e outras informações que vc armazenou na tabela de páginas]
        .... uma linha para cada página do processo ...
        ###########
        b) Comando TLB: escreve todas as entradas da TLB
        ************
        Página - Quadro - Bit Validade [e outras informações que vc armazenou na tabela de páginas]
        [ uma linha para cada página da TLB ]
        ************
*/

void memoryManager_printAddressInfo(MemoryManager memMng, FILE* fOutput, Info item){
    memoryManagerStr* mMng = (memoryManagerStr*)memMng;
    InfoStr* info = (InfoStr*)item;

    fprintf(fOutput, "%d - %d - %d\n", info->page, info->frame, info->ValidatingBit);
}

bool memoryManager_isInTLB(MemoryManager memMng, int address){
    memoryManagerStr* mMng = (memoryManagerStr*)memMng;

    PageReplacementAlgorithm repTLB = mMng->TLB->fPageReplacementAlg;
    Structure tlbStructure = mMng->TLB->dataStructure;

    return repTLB(tlbStructure, address, false, NULL);
}

// Move o endereço da tabela de páginas para a TLB manualmente
void memoryManager_moveAddressIntoTLB(MemoryManager memMng, int address){
    memoryManagerStr* mMng = (memoryManagerStr*)memMng;

    PageReplacementAlgorithm repTLB = mMng->TLB->fPageReplacementAlg;
    Structure tlbStructure = mMng->TLB->dataStructure;

    repTLB(tlbStructure, address, true, NULL);
}

void memoryManager_moveAddressIntoPageTable(MemoryManager memMng, int address){
    memoryManagerStr* mMng = (memoryManagerStr*)memMng;

    PageReplacementAlgorithm repPageTable = mMng->pageTable->fPageReplacementAlg;
    Structure pTableStructure = mMng->pageTable->dataStructure;

    repPageTable(pTableStructure, address, true, NULL);
}

PageContent memoryManager_getPageContentAt(MemoryManager memMng, int address){

}

int memoryManager_getFrameNumber(MemoryManager memmng, int address){

}

void memoryManager_free(MemoryManager memMng){
    memoryManagerStr* mMng = (memoryManagerStr*)memMng;

    fclose(mMng->binFile);
    
    memoryManager_freePageTable(mMng->pageTable);
    memoryManager_freeTLB(mMng->TLB);

    limparLista(mMng->physicalMemory, NULL, NULL);

    free(mMng);
}

static void memoryManager_freePageTable(PageTableStr* pageTable){
    pageTable->fFunc(runExtra, pageTable->dataStructure);
    free(pageTable);
}

static void memoryManager_freeTLB(TLBStr* tlb){
    tlb->fFunc(runExtra, tlb->dataStructure);
    free(tlb);
}