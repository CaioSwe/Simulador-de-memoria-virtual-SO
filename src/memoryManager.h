#ifndef _MEMORYMANAGER_H_
#define _MEMORYMANAGER_H_

#include <stdbool.h>

// Um esqueleto bem basico para memoryManager

typedef void* MemoryManager;
typedef void* TLB;

// Declaração de escopo padrão para um algoritmo de troca de paginas.
typedef int (*PageReplacementAlgorithm) (void* tableStructure, int addressToInsert, void* extra);

// Inicializa uma TLB
TLB memoryManager_InitTLB(PageReplacementAlgorithm fPageReplacementAlg);

// Inicializa uma memoryMng, com arquivo .bin para memória, quantQuadros e booleano se deseja usar TLB
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
TLB memoryManager_free(MemoryManager memMng);

// libera a TLB passada
void memoryManager_freeTLB(TLB tlb);

#endif