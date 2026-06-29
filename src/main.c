#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "fileManager.h"

#include "priorityQueue.h"
#include "queue.h"

#include "addressesManager.h"





/*                                                SUBSTITUIÇÃO DE PÁGINA                                                */
/** ALGORITMO: LRU (Least Recently Used)
 * Esta função implementa o algoritmo de substituição de páginas LRU (Least Recently Used) para gerenciar a memória virtual.
 * Ela é chamada pelo gerenciador de memória (memoryManager.c) quando uma página precisa ser substituída.
 * 
 * Este algoritmo mantém uma fila de prioridade (priorityQueue.c),
 * onde cada página é associada a uma prioridade que representa o tempo desde a última vez que foi acessada.
 * 
 * @param pQueue:   A fila de prioridade que mantém as páginas na memória.
 * @param pageInfo: Informações sobre a página que está sendo acessada.
 * @param insert:   Um booleano que indica se a página deve ser inserida na fila (true) ou apenas verificada (false).
 * @param extra:    Um ponteiro para armazenar informações adicionais, como a página removida, caso uma substituição ocorra.
 * @return:         TRUE se a página já estava na fila (acesso bem-sucedido). FALSE se a página foi inserida na fila (substituição ocorreu) ou se a página não estiver na fila.
 */
static bool LRU(PriorityQueue pQueue, Info pageInfo, bool insert, void* extra){
    // Relógio para rastrear o tempo de acesso das páginas, usado para determinar a prioridade na fila de prioridade.
    static double lruClock = 0.0f;

    /** Verifica o bool insert
     * TRUE: A página deve ser inserida na fila. 
     * Retorna TRUE se a página já estava na fila (prioridade promovida). FALSE se a página foi inserida na fila (substituição ocorreu).
     * 
     * 1. Se a página não estiver na fila de prioridade e a fila estiver cheia:
     * - A página com a menor prioridade (least recently used) é removida.
     * - O ponteiro extra é usado para armazenar informações sobre a página removida, caso seja necessário.
     * - O número do quadro da página removida é obtido usando a função memoryManager_getFrameNumber.
     * - O número do quadro da nova página é atualizado para o mesmo número do quadro da página removida usando a função memoryManager_setFrameNumber.
     * - A nova página é então inserida na fila com a prioridade atual (tempo atual).
     * 
     * 2. Se a página não estiver na fila e a fila não estiver cheia:
     * - A nova página é simplesmente inserida na fila com a prioridade atual (tempo atual).
     * 
     * 3. Se a página já estiver na fila, sua prioridade é atualizada para o tempo atual:
     * - A função promoteElementeInPriorityQueue é chamada para atualizar a prioridade da página existente.
     * 
     * FALSE: A página deve apenas ser verificada.
     * - A função isInPriorityQueue é chamada para verificar se a página está na fila de prioridade.
     * - Retorna TRUE se a página já estiver na fila (acesso bem-sucedido). FALSE se a página não estiver na fila.
     */
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
    else {return isInPriorityQueue(pQueue, pageInfo, memoryManager_comparePagesInfo);}
}

/** ALGORITMO: FIFO (First In, First Out)
 * Esta função implementa o algoritmo de substituição de páginas FIFO (First In, First Out) para gerenciar a memória virtual.
 * Ela é chamada pelo gerenciador de memória (memoryManager.c) quando uma página precisa ser substituída.
 * 
 * Este algoritmo mantém uma fila simples (queue.c) para gerenciar as páginas na memória,
 * onde a página que foi inserida primeiro é a primeira a ser removida quando a memória estiver cheia.
 * 
 * @param queue:    A fila que mantém as páginas na memória.
 * @param pageInfo: Informações sobre a página que está sendo acessada.
 * @param insert:   Um booleano que indica se a página deve ser inserida na fila (true) ou apenas verificada (false).
 * @param extra:    Um ponteiro para armazenar informações adicionais, como a página removida, caso uma substituição ocorra.
 * @return:         TRUE se a página já estava na fila (acesso bem-sucedido). FALSE se a página foi inserida na fila (substituição ocorreu) ou se a página não estiver na fila.
 */
static bool FIFO(Queue queue, Info pageInfo, bool insert, void* extra){
    /** Verifica o bool insert
     * TRUE: A página deve ser inserida na fila.
     * Retorna TRUE se a página já estava na fila (acesso bem-sucedido). FALSE se a página foi inserida na fila (substituição ocorreu).
     * 
     * 1. Se a página não estiver na fila e a fila estiver cheia:
     * - A página que foi inserida primeiro (first in) é removida.
     * - O ponteiro extra é usado para armazenar informações sobre a página removida, caso seja necessário.
     * - O número do quadro da página removida é obtido usando a função memoryManager_getFrameNumber.
     * - O número do quadro da nova página é atualizado para o mesmo número do quadro da página removida usando a função memoryManager_setFrameNumber.
     * - A nova página é então inserida na fila.
     * 
     * 2. Se a página não estiver na fila e a fila não estiver cheia:
     * - A nova página é simplesmente inserida na fila.
     * 
     * 3. Se a página já estiver na fila, não há necessidade de fazer nada, pois o FIFO não atualiza prioridades.
     * 
     * FALSE: A página deve apenas ser verificada.
     * - A função isInQueue é chamada para verificar se a página está na fila.
     * - Retorna TRUE se a página já estiver na fila (acesso bem-sucedido). FALSE se a página não estiver na fila.
     */
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
        else {return true;}
    }
    else {return isInQueue(queue, memoryManager_comparePagesInfo, pageInfo);}
}
/*######################################################################################################################*/





/*                                                        PRINT                                                         */
/** Função auxiliar para imprimir o conteúdo da memória.
 * Esta função é usada para exibir o conteúdo de um item na memória, que é do tipo Item (void*):
 * - item é interpretado como um ponteiro para signed char, e é atribuído o valor do signed char à variável 'c'.
 * - Em seguida, usa-se snprintf para formatar o valor de 'c' como uma string decimal e armazená-lo em um buffer.
 * - O ponteiro 'p' aponta para o buffer contendo a representação em string do valor do signed char.
 * - Retorna-se o ponteiro 'p'.
 * 
 * @param item:     O item a ser impresso, que é do tipo Item (void*).
 * @param extra:    Parâmetro extra (não utilizado nesta função).
 * @return:         Um ponteiro para o buffer contendo a representação em string do valor do signed char.
 */
static const char* printMemoryContent(Item item, void* extra){
    signed char c = *(signed char*)item;

    static char buffer[32];
    /** snprintf
     * Esta função é usada para formatar o valor do signed char 'c' como uma string decimal e armazená-lo em um buffer.
     * 
     * Esta formatação funciona da seguinte maneira:
     * snprintf(char *const _Buffer, const size_t _BufferCount, const char *const _Format, ...)
     * - char *const _Buffer:       Um ponteiro para o buffer onde a string formatada será armazenada.
     * - const size_t _BufferCount: O tamanho do buffer, garantindo que não haja estouro de buffer.
     * - const char *const _Format: A string de formato que especifica como os argumentos subsequentes devem ser formatados.
     * - ...:                       Os argumentos adicionais que serão formatados de acordo com a string de formato.
     * 
     * De forma que:
     * snprintf(buffer, sizeof(buffer), "%d", c);
     * - buffer:            O buffer onde a string formatada será armazenada.
     * - sizeof(buffer):    O tamanho do buffer, garantindo que não haja estouro de buffer.
     * - "%d":              A string de formato que indica que o valor de 'c' deve ser formatado como um número decimal.
     * - c:                 O valor do signed char que será formatado como um número decimal.
     */
    snprintf(buffer, sizeof(buffer), "%d", c);
    const char* p = buffer;

    return p;
}
/*######################################################################################################################*/





/*                                                       TIPOS DO                                                       */
typedef enum TYPES {
    ADDR,
    FRAMES,
    SUBSTYPE
} TYPES;
/*######################################################################################################################*/





int main(int argc, char* argv[]){
    printf("\n\n\n");


    
    /*                                              1: TRATA OS PARÂMETROS                                              */
    // Aloca memória para armazenar os caminhos dos arquivos fornecidos como argumentos de linha de comando.
    char** paths = calloc(3, sizeof(char*));

    // 1.1: Verifica se o número de parâmetros fornecidos é válido (deve ser pelo menos 4: nome do programa + 3 parâmetros).
    if(argc < 4){
        printf(" [ERROR]: Number of parameters is invalid. \n");
        free(paths);
        return 1;
    }

    /** 1.2: Pega todos os nomes dos arquivos base e organiza na variável paths
    * paths[0] = arquivo de endereços
    * paths[1] = quadros
    * paths[2] = tipo de algoritmo de substituição de página
    */
    for(int i = 1; i < argc; i++){
        int param = i - 1;
        free(paths[param]);
        paths[param] = malloc(strlen(argv[i]) + 1);
        strcpy(paths[param], argv[i]);
    }

    // 1.3: Verifica se o algoritmo de substituição de páginas fornecido é válido (deve ser "LRU" ou "FIFO").
    if(strcmp("LRU", paths[SUBSTYPE]) != 0 && strcmp("FIFO", paths[SUBSTYPE]) != 0){
        printf(" [ERROR]\n");
        printf("in main.c(3) Invalid page replacement algorithm. Must be 'LRU' or 'FIFO'.");
        for(int i = 0; i < 3; i++) free(paths[i]);
        free(paths);
        return 0;
    }

    // 1.4: Coloca ".txt" no arquivo de endereços caso não tenha
    char* newPathAddr = (char*)changeExtension(paths[ADDR], ".txt");
    free(paths[ADDR]);
    paths[ADDR] = newPathAddr;

    // 1.5: Lista todos os parâmetros fornecidos
    printf("%-12s: \t\t%s\n", "Addresses file", paths[ADDR]);
    printf("%-12s: \t\t%s\n", "Frames quantity", paths[FRAMES]);
    printf("%-12s: \t%s\n\n", "Page replacement algorithm", paths[SUBSTYPE]);
    /*##################################################################################################################*/
    


    /*                                       2: CRIANDO O GERENCIADOR DE MEMÓRIA                                        */
    /** 2.1: Inicializa o gerenciador de memória
     * 
     * 1. O caminho do arquivo binário (BACKING_STORE.bin) é definido como "./files/BACKING_STORE.bin".
     * 
     * 2. A quantidade de quadros é obtida a partir do parâmetro fornecido na linha de comando (paths[FRAMES])
     * e convertida para um inteiro usando atoi.
     * 
     * 3. O tamanho do quadro de memória é definido como 256 bytes (2^8 bytes).
     * 
     * 4. A função memoryManager_Init é chamada para criar e inicializar o gerenciador de memória,
     * passando os parâmetros definidos acima.
     * 
     * 5. O endereço do gerenciador de memória é impresso na tela para fins de depuração.
     */
    const char* binPath = "./files/BACKING_STORE.bin";
    int frameCount = atoi(paths[FRAMES]); 
    int frameSize  = 256; // Tamanho do quadro de memória (em bytes) = 256 bytes = 2^8 bytes
    MemoryManager memMng = memoryManager_Init(binPath, frameCount, frameSize, printMemoryContent);
    printf("Memory Manager address:\n");
    printf("%p\n\n", memMng);

    // 2.2: Inicializa as estruturas de dados para a TLB e a tabela de páginas,
    // dependendo do algoritmo de substituição de páginas escolhido (LRU ou FIFO).
    PageReplacementAlgorithm strRep = NULL;
    Structure strPageTable          = NULL;
    Structure strTLB                = NULL;
    runThroughItems runFunc         = NULL;
    highFreeFunc fAlg               = NULL;

    // 2.3: Verifica qual algoritmo de substituição de páginas será utilizado e
    // Cria a estrutura de dados correspondente (fila de prioridade para LRU ou fila simples para FIFO)
    if(strcmp(paths[SUBSTYPE], "LRU") == 0){
        strPageTable = criaPriorityQueue(frameCount);
        strTLB       = criaPriorityQueue(16);
        strRep       = LRU;
        runFunc      = runThroughPriorityQueue;
        fAlg         = destroiPriorityQueue;
    }
    else{
        strPageTable = initQueue(frameCount);
        strTLB       = initQueue(16);
        strRep       = FIFO;
        runFunc      = runThroughQueue;
        fAlg         = freeQueue;
    }

    // 2.4: Calcula o tamanho da tabela de páginas com base no tamanho do quadro de memória (frameSize).
    int pageTableSize = 65536 / frameSize;

    // 2.5: Adiciona a estrutura de dados criada ao gerenciador de memória inicializado, 
    // juntamente com o algoritmo de substituição de páginas escolhido.
    memoryManager_addPageTable(memMng, pageTableSize, strRep, strPageTable, runFunc, fAlg);
    memoryManager_addTLB(memMng, strRep, strTLB, runFunc, fAlg);
    /*##################################################################################################################*/


    
    /*                                  3: LENDO O ARQUIVO DE ENDEREÇOS (addresses.txt)                                 */
    // 3.1: fPathAddr = paths[ADDR] + ".txt"   =>   addresses + ".txt"   =>   "addresses.txt"
    const char* fPathAddr = strcatcat(paths[ADDR], "");

    // 3.2: Define o caminho do arquivo de saída (correct.txt) para armazenar os resultados da execução do programa.
    const char* fOutputPathAddr = "./files/correct.txt";

    // 3.3: Abre o caminho do output (correct.txt) em modo de escrita
    FILE* fSaida = fopen(fOutputPathAddr, "w");
    printf("Writing on file: %s\n", fOutputPathAddr);
    fprintf(fSaida, " == BEGINING OF EXECUTION == \n\n\n\n");

    processAddrFile(memMng, fPathAddr, fSaida);
    
    fprintf(fSaida, " == END OF EXECUTION == ");
    fclose(fSaida);
    /*##################################################################################################################*/



    /*                                                     4: FREEs                                                     */
    // 4.1: Libera a memória alocada para o gerenciador de memória e suas estruturas internas.
    memoryManager_free(memMng, NULL);

    // 4.2: Libera a memória alocada para os caminhos dos arquivos fornecidos como argumentos de linha de comando.
    // 4.2.1: Libera a memória alocada para cada caminho de arquivo.
    for(int i = 0; i < 3; i++) free(paths[i]);
    // 4.2.2: Libera a memória alocada para o array de caminhos.
    free(paths);
    /*##################################################################################################################*/



    printf("\n [SUCCESS] Program finalized \n");
    printf("\n\n\n\n");
    return 0;
}