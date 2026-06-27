#ifndef _MEMORYMANAGER_H_
#define _MEMORYMANAGER_H_

#include <stdbool.h>

/**
 * Cabecalho especializado para a implementacao de um gerenciador de memoria virtual.
 * O modulo simula o comportamento de uma MMU, controlando uma tabela de paginas,
 * uma memoria cache (TLB) e o mapeamento de um arquivo binario de backup.
 */

typedef void* MemoryManager;

typedef void* Structure;
typedef void* Info;
typedef const char* PageContent;

// Declaracao de escopo padrao para um algoritmo de troca de paginas.
typedef int (*PageReplacementAlgorithm) (Structure tableStructure, int addressToInsert, bool replace, void* extra);

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
 * @param fPageReplacementAlg Funcao do algoritmo de substituicao de paginas para a tabela.
 * @param pageTableStructure A estrutura de dados escolhida para representar a tabela.
 * @return Nao ha retorno de algum valor.
 */
void memoryManager_addPageTable(MemoryManager memMng, PageReplacementAlgorithm fPageReplacementAlg, Structure pageTableStructure);

/**
 * @brief Vincula uma estrutura de cache TLB ao gerenciador de memoria.
 * @param memMng O objeto gerenciador de memoria a ser modificado.
 * @param fPageReplacementAlg Funcao do algoritmo de substituicao de entradas na TLB.
 * @param tlbStructure A estrutura de dados escolhida para representar a TLB.
 * @return Nao ha retorno de algum valor.
 */
void memoryManager_addTLB(MemoryManager memMng, PageReplacementAlgorithm fPageReplacementAlg, Structure tlbStructure);

/**
 * @brief Exibe no console o estado de ocupacao e os mapeamentos presentes na tabela de paginas.
 * @param memMng O objeto gerenciador de memoria de referencia.
 * @return Nao ha retorno de algum valor.
 */
void memoryManager_printPageTable(MemoryManager memMng);

/**
 * @brief Exibe no console o estado de ocupacao e os mapeamentos presentes na TLB.
 * @param memMng O objeto gerenciador de memoria de referencia.
 * @return Nao ha retorno de algum valor.
 */
void memoryManager_printTLB(MemoryManager memMng);

// ...
void memoryManager_printAddressInfo(MemoryManager memMng, FILE* fOutput, Info item);

/**
 * @brief Verifica se o mapeamento de um determinado endereco logico esta no cache TLB.
 * @param memMng O objeto gerenciador de memoria de referencia.
 * @param address O endereco logico a ser consultado na TLB.
 * @return Retorna true se houver um TLB Hit, false caso ocorra um TLB Miss.
 */
bool memoryManager_isInTLB(MemoryManager memMng, int address);

/**
 * @brief Move manualmente o mapeamento de um endereco consultado para dentro da TLB.
 * @param memMng O objeto gerenciador de memoria a ser modificado.
 * @param address O endereco logico que sera espelhado no cache.
 * @return Nao ha retorno de algum valor.
 */
void memoryManager_moveAddressIntoTLB(MemoryManager memMng, int address);

/**
 * @brief Move manualmente o mapeamento de um endereco do disco para a tabela de paginas.
 * @param memMng O objeto gerenciador de memoria a ser modificado.
 * @param address O endereco logico carregado do arquivo auxiliar apos um Page Fault.
 * @return Nao ha retorno de algum valor.
 */
void memoryManager_moveAddressIntoPageTable(MemoryManager memMng, int address);

/**
 * @brief Obtem o numero do frame fisico associado a um determinado endereco logico.
 * @param memMng O objeto gerenciador de memoria de referencia.
 * @param address O endereco logico cuja correspondencia fisica sera buscada.
 * @return Retorna o indice numerico do frame correspondente na memoria fisica.
 */
int memoryManager_getFrameNumber(MemoryManager memMng, int address);

/**
 * @brief Traduz o endereco logico e retorna o ponteiro para o conteudo de dados da pagina.
 * @note Esta funcao encapsula a validacao automatica na TLB e a copia do binario em caso de Page Fault.
 * @param memMng O objeto gerenciador de memoria ativo.
 * @param address O endereco logico cujo conteudo textual sera lido.
 * @return Retorna um ponteiro para o array de caracteres que representa os dados da pagina.
 */
PageContent memoryManager_getPageContentAt(MemoryManager memMng, int address);

/**
 * @brief Fecha os arquivos de suporte e libera a memoria alocada para o gerenciador.
 * @param memMng O objeto gerenciador de memoria a ser desalocado.
 * @return Nao ha retorno de algum valor.
 */
void memoryManager_free(MemoryManager memMng);

#endif