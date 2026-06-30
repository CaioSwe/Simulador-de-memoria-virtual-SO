#ifndef _MEMORYMANAGER_H_
#define _MEMORYMANAGER_H_

#include <stdbool.h>
#include <stdio.h>

#include "types.h"

/**
 * Cabecalho especializado para a implementacao de um gerenciador de memoria virtual.
 * O modulo simula o comportamento de uma MMU, controlando uma tabela de paginas,
 * uma memoria cache (TLB) e o mapeamento de um arquivo binario de backup.
 */

typedef void* MemoryManager;

typedef void* Structure;
typedef void* Info;
typedef char PageContent;

// Declaracao de escopo padrao para um algoritmo de troca de paginas.
typedef bool (*PageReplacementAlgorithm) (Structure tableStructure, Info pageInfo, bool replace, void* extra);
/////////////////////////////////////////////////////////////////////
// Declaracao de escopo padrao para um algoritmo de remocao de itens.
typedef void (*removeListItemFunc)(Structure dataStruct, Info item, bool (*compare)(Info, Info));
/////////////////////////////////////////////////////////////////////

// Resultado de uma consulta (accessAddress)
typedef enum {
    ACCESS_TLB_HIT,
    ACCESS_PAGE_TABLE_HIT,
    ACCESS_PAGE_FAULT
} AccessResult;

/////////////////////////////////////////////////////////////////////

/**
 * @brief Cria e inicializa uma instancia de um gerenciador de memoria.
 * @param binPath O caminho de arquivo do .bin para memoria auxiliar (Backing Store).
 * @param frameCount A quantidade de frames disponiveis na memoria fisica simulada.
 * @param frameSize O tamanho de cada quadro/pagina em bytes.
 * @return Retorna um ponteiro para o gerenciador de memoria criado.
 */
MemoryManager memoryManager_Init(const char* binPath, int frameCount, int frameSize);

/**
 * @brief Vincula uma estrutura de tabela de paginas ao gerenciador de memoria.
 * @param memMng O objeto gerenciador de memoria a ser modificado.
 * @param size Tamanho da tabela de paginas.
 * @param fPageReplacementAlg Funcao do algoritmo de substituicao de paginas para a tabela.
 * @param pageTableStructure A estrutura de dados escolhida para representar a tabela.
 * @param runFunc Um ponteiro para a funcao de percorrimento da estrutura associada.
 * @param fFunc Um ponteiro para uma funcao de liberacao da estrutura.
 * @return Nao ha retorno de algum valor.
 */
void memoryManager_addPageTable(MemoryManager memMng, int size, PageReplacementAlgorithm fPageReplacementAlg, Structure pageTableStructure, runThroughItems runFunc, highFreeFunc fFunc);

Info memoryManager_createPageInfoCopy(Info pageInfo);

/**
 * @brief Vincula uma estrutura de cache TLB ao gerenciador de memoria.
 * @param memMng O objeto gerenciador de memoria a ser modificado.
 * @param fPageReplacementAlg Funcao do algoritmo de substituicao de entradas na TLB.
 * @param tlbStructure A estrutura de dados escolhida para representar a TLB.
 * @param runFunc Um ponteiro para a funcao de percorrimento da estrutura associada.
 * @param fFunc Um ponteiro para uma funcao de liberacao da estrutura.
 * @param removeFunc Um ponteiro para uma funcao de remocao de itens da estrutura.
 * @return Nao ha retorno de algum valor.
 */
void memoryManager_addTLB(MemoryManager memMng, PageReplacementAlgorithm fPageReplacementAlg, Structure tlbStructure, runThroughItems runFunc, highFreeFunc fFunc, removeListItemFunc removeFunc);

/**
 * @brief Exibe o estado de ocupacao e os mapeamentos presentes na tabela de paginas.
 * @param memMng O objeto gerenciador de memoria de referencia.
 * @param fOutput Um ponteiro para o arquivo a ser escrito.
 * @return Nao ha retorno de algum valor.
 */
void memoryManager_printPageTable(MemoryManager memMng, FILE* fOutput);

/**
 * @brief Exibe o estado de ocupacao e os mapeamentos presentes na TLB.
 * @param memMng O objeto gerenciador de memoria de referencia.
 * @param fOutput Um ponteiro para o arquivo a ser escrito.
 * @return Nao ha retorno de algum valor.
 */
void memoryManager_printTLB(MemoryManager memMng, FILE* fOutput);

/**
 * @brief Exibe as informacoes associadas a informacao passada de uma consulta.
 * @param memMng O objeto gerenciador de memoria de referencia.
 * @param item A informação associada ao memoryManager a ser impressa no arquivo.
 * @param address Endereco a ser impresso (utilizado para offset).
 * @param fOutput Um ponteiro para o arquivo a ser escrito.
 * @return Nao ha retorno de algum valor.
 */
void memoryManager_printAddressInfo(MemoryManager memMng, Info item, int address, FILE* fOutput);

/**
 * @brief Verifica se o mapeamento de um determinado endereco logico esta no cache TLB.
 * @param memMng O objeto gerenciador de memoria de referencia.
 * @param address O endereco logico a ser consultado na TLB.
 * @return Retorna true se houver um TLB Hit, false caso ocorra um TLB Miss.
 */
bool memoryManager_isInTLB(MemoryManager memMng, int address);

/**
 * @brief Obtem a informação de uma página na memória.
 * @param memMng O objeto gerenciador de memoria de referencia.
 * @param address Endereço virtual correspondente ao acesso.
 * @return Retorna um ponteiro de informação relacionada à consulta.
 */
Info memoryManager_accessAddress(MemoryManager memMng, int address);

/**
 * @brief Obtem o numero do frame fisico associado a uma consulta feita.
 * @param info Informação de uma consulta à memória usando accessAddress.
 * @return Retorna o indice numerico do frame correspondente na memoria fisica.
 */
int memoryManager_getFrameNumber(Info info);

/**
 * @brief Obtem o resultado de uma consulta feita. Podendo ser HIT, MISS ou FAULT.
 * @param info Informação de uma consulta à memória usando accessAddress.
 * @return Retorna o resultado da consulta.
 */
AccessResult memoryManager_getInfoResult(Info info);

/**
 * @brief Seta o numero de pagina (virtual) associado a uma consulta.
 * @param info Informação de uma consulta à memória usando accessAddress.
 * @param pageNumber Numero da pagina a ser associada.
 * @return Nao ha' retorno de algum valor.
 */
void memoryManager_setPageNumber(Info info, int pageNumber);

/**
 * @brief Seta o numero do frame fisico associado a uma consulta.
 * @param info Informação de uma consulta à memória usando accessAddress.
 * @param frameNumber Numero do frame a ser associado.
 * @return Nao ha' retorno de algum valor.
 */
void memoryManager_setFrameNumber(Info info, int frameNumber);

/**
 * @brief Compara duas informacoes de pa'ginas.
 * @param info1 Ponteiro para uma informacao de pa'gina.
 * @param info2 Ponteiro para uma informacao de pa'gina.
 * @return Retorna verdadeiro caso as pa'ginas sejam verdadeiras, falso caso contra'rio.
 */
bool memoryManager_comparePagesInfo(Info info1, Info info2);

/**
 * @brief Fecha os arquivos de suporte e libera a memoria alocada para o gerenciador.
 * @param memMng O objeto gerenciador de memoria a ser desalocado.
 * @param extra Ponteiro extra para algum conteúdo adicional
 * @return Nao ha retorno de algum valor.
 */
void memoryManager_free(MemoryManager memMng, void* extra);

#endif