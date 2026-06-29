// Implementa o FIFO ou LRU
#include "memoryManager.h"
#include "priorityQueue.h"

#include "queue.h"

#include <stdlib.h>
#include <math.h>

// Estruturas do gerenciador de memoria
////////////////////////////////////////

// Para cada elemento da tabela de páginas / TLB
typedef struct PageInfoStr{
    int pageNumber;
    int frameNumber;
    bool ValidatingBit;
} PageInfoStr;

// Retorno da consulta retorna um objeto desta struct
typedef struct InfoStr{
    PageInfoStr information;
    AccessResult aResult;
} InfoStr;

// Estrutura da TLB
typedef struct TLBStr{
    PageReplacementAlgorithm fPageReplacementAlg;
    runThroughItems runFunc;
    highFreeFunc fFunc;
    
    Structure dataStructure;
} TLBStr;

// Estrutura da tabela de páginas
typedef struct PageTableStr{
    PageReplacementAlgorithm fPageReplacementAlg;
    runThroughItems runFunc;
    highFreeFunc fFunc;

    Structure dataStructure;
    
    PageInfoStr* entries;
} PageTableStr;

// Estrutura do gerenciador de memória
typedef struct memoryManagerStr{
    FILE* binFile;
    
    int currentAmountOfFrames;
    int frameCount;
    int frameSize;

    char* physicalMemory;

    printFunc pFunc;

    PageTableStr* pageTable;
    TLBStr* TLB;
} memoryManagerStr;

////////////////////////////////////////

// Inicializa uma memoryMng, com arquivo .bin para memória, quantQuadros e uma estrutura de TLB (se for nula, cria um memoryManager sem TLB)
MemoryManager memoryManager_Init(const char* binPath, int frameCount, int frameSize, printFunc pFunc){
    memoryManagerStr* mMng = (memoryManagerStr*)malloc(sizeof(memoryManagerStr));

    if(checkAllocation(mMng, "msg de erro")) return NULL;

    FILE* binFile = fopen(binPath, "rb");

    if(binFile == NULL){
        printf("msg de erro");
        free(mMng);
        return NULL;
    }

    mMng->binFile = binFile;

    mMng->physicalMemory = (char*)calloc(frameCount * frameSize, sizeof(char));

    mMng->currentAmountOfFrames = 0;
    mMng->frameCount = frameCount;
    mMng->frameSize = frameSize;

    mMng->pFunc = pFunc;

    mMng->pageTable = NULL;
    mMng->TLB = NULL;

    return mMng;
}

static PageTableStr* memoryManager_InitPageTable(int size, PageReplacementAlgorithm fPageReplacementAlg, Structure pageTableStructure, runThroughItems runFunc, highFreeFunc fFunc){
    PageTableStr* pTable = (PageTableStr*)malloc(sizeof(PageTableStr));

    if(checkAllocation(pTable, "msg de erro")) return NULL;

    pTable->fPageReplacementAlg = fPageReplacementAlg;
    pTable->dataStructure = pageTableStructure;
    pTable->runFunc = runFunc;
    pTable->fFunc = fFunc;
    pTable->entries = (PageInfoStr*)malloc(sizeof(PageInfoStr) * size);

    for(int i = 0; i < size; i++){
        pTable->entries[i].pageNumber = i;
        pTable->entries[i].ValidatingBit = 0;
        pTable->entries[i].frameNumber = -1;
    }

    return pTable;
}

static TLBStr* memoryManager_InitTLB(PageReplacementAlgorithm fPageReplacementAlg, Structure tlbStructure, runThroughItems runFunc, highFreeFunc fFunc){
    TLBStr* tlb = (TLBStr*)malloc(sizeof(TLBStr));

    if(checkAllocation(tlb, "msg de erro")) return NULL;

    tlb->fPageReplacementAlg = fPageReplacementAlg;
    tlb->dataStructure = tlbStructure;
    tlb->runFunc = runFunc;
    tlb->fFunc = fFunc;

    return tlb;
}

void memoryManager_addPageTable(MemoryManager memMng, int size, PageReplacementAlgorithm fPageReplacementAlg, Structure pageTableStructure, runThroughItems runFunc, highFreeFunc fFunc){
    memoryManagerStr* mMng = (memoryManagerStr*)memMng;

    if(mMng == NULL){
        printf("msg de erro");
        return;
    }

    mMng->pageTable = memoryManager_InitPageTable(size, fPageReplacementAlg, pageTableStructure, runFunc, fFunc);
}

static void memoryManager_invalidateTLBEntry(Info item, void* extra){
    int removedPage = *(int*)extra;
    
    PageInfoStr* pInfo = (PageInfoStr*)item;
    
    if(pInfo->pageNumber == removedPage){
        pInfo->ValidatingBit = 0;
        pInfo->frameNumber = -1;
    }
}

void memoryManager_addTLB(MemoryManager memMng, PageReplacementAlgorithm fPageReplacementAlg, Structure tlbStructure, runThroughItems runFunc, highFreeFunc fFunc){
    memoryManagerStr* mMng = (memoryManagerStr*)memMng;

    if(mMng == NULL){
        printf("msg de erro");
        return;
    }

    mMng->TLB = memoryManager_InitTLB(fPageReplacementAlg, tlbStructure, runFunc, fFunc);
}

static void memoryManager_printAddressTableInfo(Info item, FILE* fOutput){
    PageInfoStr pInfo = ((InfoStr*)item)->information;

    fprintf(fOutput, "%d - %d - %d\n", pInfo.pageNumber, pInfo.frameNumber, pInfo.ValidatingBit);
}

static void memoryManager_printAddressTableInfoVoid(Info item, void* extra){
    memoryManager_printAddressTableInfo(item, (FILE*)extra);
}

void memoryManager_printPageTable(MemoryManager memMng, FILE* fOutput){
    memoryManagerStr* mMng = (memoryManagerStr*)memMng;

    runThroughItems runFunc = mMng->pageTable->runFunc;

    runFunc(mMng->pageTable->dataStructure, memoryManager_printAddressTableInfoVoid, fOutput);
}

void memoryManager_printTLB(MemoryManager memMng, FILE* fOutput){
    memoryManagerStr* mMng = (memoryManagerStr*)memMng;

    runThroughItems runFunc = mMng->TLB->runFunc;

    runFunc(mMng->TLB->dataStructure, memoryManager_printAddressTableInfoVoid, fOutput);
}

void memoryManager_printAddressInfo(MemoryManager memMng, Info item, int address, FILE* fOutput){
    memoryManagerStr* mMng = (memoryManagerStr*)memMng;
    PageInfoStr pInfo = ((InfoStr*)item)->information;
    
    printFunc pFunc = mMng->pFunc;
    int frameNumber = pInfo.frameNumber;
    
    int offset = address & (mMng->frameSize - 1);
    
    char pageContent = mMng->physicalMemory[(frameNumber * mMng->frameSize) + offset];

    int physicalAddress = (pInfo.frameNumber * mMng->frameSize) + offset;
    const char* value = pFunc(&pageContent, NULL);

    fprintf(fOutput, "Endereço virtual: %5d | Endereço físico: %4d | Conteúdo: %s\n", address, physicalAddress, value);
}

bool memoryManager_isInTLB(MemoryManager memMng, int address){
    memoryManagerStr* mMng = (memoryManagerStr*)memMng;

    if(mMng->TLB == NULL) return false;

    PageReplacementAlgorithm repTLB = mMng->TLB->fPageReplacementAlg;
    Structure tlbStructure = mMng->TLB->dataStructure;

    PageInfoStr* info = (PageInfoStr*)malloc(sizeof(PageInfoStr));
    int pageNumber = address >> (int)log2(mMng->frameSize);

    info->pageNumber = pageNumber;

    bool isInTlb = repTLB(tlbStructure, info, false, NULL);

    free(info);

    return isInTlb;
}

static void memoryManager_insertPageInMemory(memoryManagerStr* mMng, int pageNumber, int frameNumber){
    int physicalFrameId = frameNumber * mMng->frameSize;

    fseek(mMng->binFile, pageNumber * mMng->frameSize, SEEK_SET);
    fread(mMng->physicalMemory + physicalFrameId, mMng->frameSize, 1, mMng->binFile);
}

Info memoryManager_accessAddress(MemoryManager memMng, int address){
    // 1: Faz um cast do ponteiro genérico para a estrutura específica do gerenciador de memória e verifica se é nulo
    memoryManagerStr* mMng = (memoryManagerStr*)memMng;
    if(mMng == NULL) return NULL;

    // 2: Aloca memória para armazenar as informações da página acessada e o resultado do acesso e verifica se a alocação foi bem-sucedida
    InfoStr* info = (InfoStr*)malloc(sizeof(InfoStr));
    if(checkAllocation(info, "msg de erro")) return NULL;

    // 3: Calcula o número da página a partir do endereço virtual fornecido e o armazena na estrutura de informações da página
    // O cálculo é feito usando o tamanho do quadro de memória (frameSize) e armazena no campo correspondente da estrutura InfoStr
    // pageNumber = address >> log2(frameSize) = address >> 8 (se frameSize = 256 bytes)
    int pageNumber = address >> (int)log2(mMng->frameSize);
    info->information.pageNumber = pageNumber;

    // 4: Verifica se a TLB está presente no gerenciador de memória e armazena o resultado em uma variável booleana
    bool hasTLB = (mMng->TLB != NULL);

    // 5: Se a TLB estiver presente, verifica se a página está na TLB usando o algoritmo de substituição de páginas da TLB
    if(hasTLB){
        // 5.1: Obtém o algoritmo de substituição de páginas da TLB e a estrutura de dados correspondente
        PageReplacementAlgorithm repTLB = mMng->TLB->fPageReplacementAlg;
        Structure tlbStructure = mMng->TLB->dataStructure;

        // 5.2: Verifica se a página está presente na TLB usando o algoritmo de substituição de páginas
        if(repTLB(tlbStructure, &info->information, false, NULL)){
            // Se a página estiver na TLB, promove a página na TLB (atualiza a prioridade) 
            repTLB(tlbStructure, &info->information, true, NULL);

            // Atualiza as informações da estrutura InfoStr
            // Com o resultado do acesso (ACCESS_TLB_HIT), o número do quadro correspondente e o bit de validação
            info->aResult = ACCESS_TLB_HIT;
            info->information.frameNumber = mMng->pageTable->entries[pageNumber].frameNumber;
            info->information.ValidatingBit = 1;

            // Retorna a estrutura InfoStr contendo as informações da página acessada e o resultado do acesso
            return info;
        }
    }

    // 6: Se não foi um TLB hit, consulta a tabela de páginas
    // 6.1: Obtém o algoritmo de substituição de páginas da tabela de páginas e a estrutura de dados correspondente
    PageReplacementAlgorithm repPT = mMng->pageTable->fPageReplacementAlg;
    Structure ptStructure = mMng->pageTable->dataStructure;
    // 6.2: Obtém as informações da página correspondente ao número da página calculado anteriormente
    PageInfoStr entrada = mMng->pageTable->entries[pageNumber]; 
    // 6.3: Obtem o bit de validação da entrada da tabela de páginas para verificar se a página está na memória
    bool isInMemory = entrada.ValidatingBit;

    // 7: Verifica se a página está na memória
    if(isInMemory){
        // 7.1: Atualiza as informações da estrutura InfoStr
        // Com o resultado do acesso (ACCESS_PAGE_TABLE_HIT), o número do quadro correspondente e o bit de validação
        info->aResult = ACCESS_PAGE_TABLE_HIT;
        info->information.frameNumber = entrada.frameNumber;
        info->information.ValidatingBit = 1;

        // 7.3: Retorna a estrutura InfoStr contendo as informações da página acessada e o resultado do acesso
        return info;
    }

    // 8: Se a página não está na memória, trata-se de uma page fault,
    // inicializa o número do quadro como -1, indicando que ainda não foi atribuído um quadro para a página
    int frameNumber = -1;
    // 8.1: Verifica se a memória não está cheia
    if(mMng->currentAmountOfFrames < mMng->frameCount){
        // A memória não está cheia, então atribui o próximo quadro disponível à página
        frameNumber = mMng->currentAmountOfFrames;

        // Atualiza as informações da estrutura InfoStr
        // Com o número do quadro atribuído e o bit de validação definido como 1 (válido)
        info->information.frameNumber = frameNumber;
        info->information.ValidatingBit = 1;

        // Insere a página na memória usando o algoritmo de substituição de páginas da tabela de páginas
        repPT(ptStructure, &info->information, true, NULL);

        // Incrementa a quantidade atual de quadros na memória, indicando que um novo quadro foi ocupado
        mMng->currentAmountOfFrames += 1;
    }
    // 8.2: A memória está cheia, então é necessário substituir uma página existente na memória
    else{
        // Inicializa um ponteiro para armazenar as informações da página removida (substituída) da memória
        Info removedInfo = NULL;

        // Substitui a página na memória usando o algoritmo de substituição de páginas da tabela de páginas
        repPT(ptStructure, &info->information, true, &removedInfo);

        // Obtém o número do quadro da página removida a partir das informações da página removida
        frameNumber = ((PageInfoStr*)removedInfo)->frameNumber;
        printf(" -> %d", frameNumber);

        // Atualiza a entrada da tabela de páginas para a página removida, 
        // definindo o número do quadro como -1 e o bit de validação como 0 (inválido)
        int removedPage = ((PageInfoStr*)removedInfo)->pageNumber;
        mMng->pageTable->entries[removedPage].frameNumber = -1;
        mMng->pageTable->entries[removedPage].ValidatingBit = 0;

        // Se a TLB estiver presente, invalida a entrada correspondente à página removida na TLB
        if(hasTLB){
            runThroughItems runFunc = mMng->TLB->runFunc;
            runFunc(mMng->TLB->dataStructure, memoryManager_invalidateTLBEntry, &removedPage);
        }
    }

    // 9: Insere a página na memória física (physicalMemory) do gerenciador de memória
    memoryManager_insertPageInMemory(mMng, pageNumber, frameNumber);

    // 10: Atualiza a entrada da tabela de páginas para a página acessada, 
    // definindo o número do quadro atribuído e o bit de validação como 1 (válido)
    mMng->pageTable->entries[pageNumber].frameNumber = frameNumber;
    mMng->pageTable->entries[pageNumber].ValidatingBit = 1;

    // 11: Se a TLB estiver presente, 
    // insere a página acessada na TLB usando o algoritmo de substituição de páginas da TLB
    if(hasTLB){
        // 11.1: Obtém o algoritmo de substituição de páginas da TLB e a estrutura de dados correspondente
        PageReplacementAlgorithm repTLB = mMng->TLB->fPageReplacementAlg;
        Structure tlbStructure = mMng->TLB->dataStructure;

        // 11.2: Insere a página acessada na TLB usando o algoritmo de substituição de páginas da TLB
        repTLB(tlbStructure, &info->information, true, NULL);
    }

    // 12: Atualiza as informações da estrutura InfoStr com o resultado do acesso (ACCESS_PAGE_FAULT)
    info->aResult = ACCESS_PAGE_FAULT;

    // 13: Retorna a estrutura InfoStr contendo as informações da página acessada e o resultado do acesso
    return info;
}

int memoryManager_getFrameNumber(Info info){
    PageInfoStr* pInfo = (PageInfoStr*)info;

    return pInfo->frameNumber;
}

AccessResult memoryManager_getInfoResult(Info info){
    InfoStr* pInfo = (InfoStr*)info;

    return pInfo->aResult;
}

void memoryManager_setPageNumber(Info info, int pageNumber){
    PageInfoStr* pInfo = (PageInfoStr*)info;

    pInfo->pageNumber = pageNumber;
}

void memoryManager_setFrameNumber(Info info, int frameNumber){
    PageInfoStr* pInfo = (PageInfoStr*)info;

    pInfo->frameNumber = frameNumber;
}

bool memoryManager_comparePagesInfo(Info info1, Info info2){
    PageInfoStr* i1 = (PageInfoStr*)info1;
    PageInfoStr* i2 = (PageInfoStr*)info2;
    
    return i1->pageNumber == i2->pageNumber;
}

static void memoryManager_freePageInfo(Info pInfo, void* extra){
    PageInfoStr* pInfoStr = (PageInfoStr*)pInfo;

    free(pInfoStr);
}

static void memoryManager_freePageTable(PageTableStr* pageTable, void* extra){
    if(pageTable == NULL) return;

    pageTable->fFunc(pageTable->dataStructure, memoryManager_freePageInfo, extra);
    free(pageTable->entries);
    free(pageTable);
}

static void memoryManager_freeTLB(TLBStr* tlb, void* extra){
    if(tlb == NULL) return;

    tlb->fFunc(tlb->dataStructure, memoryManager_freePageInfo, extra);
    free(tlb);
}

void memoryManager_free(MemoryManager memMng, void* extra){
    memoryManagerStr* mMng = (memoryManagerStr*)memMng;

    fclose(mMng->binFile);
    
    memoryManager_freePageTable(mMng->pageTable, extra);
    memoryManager_freeTLB(mMng->TLB, extra);

    free(mMng->physicalMemory);

    free(mMng);
}