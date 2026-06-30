#include "addressesManager.h"

#include <string.h>
#include <stdio.h>

#include "memoryManager.h"

/**
 * Usando o gerenciador de memoria em memMng
 * le o arquivo em fPathAddr
 * escreve o output na saida fSaida
 */
void processAddrFile(MemoryManager memMng, const char* fPathAddr, FILE* fSaida){
    // memMng -> .h
    // abrir arquivo bin para ler 'Conteúdo'
    
    // 1: abrir fPathaAddr (arquivo)
    FILE* addrFile = fopen(fPathAddr, "r");
    // 1.1: verificar se o arquivo foi aberto corretamente
    if(addrFile == NULL){
        printf("[ERROR] Error opening addresses file.\n");
        return;
    }

    // 2: Processar endereços do addresses.txt usando memMng
    // TO-DO
    char line[256];
    int virtualAddress = 0;

    int totalAccesses = 0;
    int hit = 0;
    int miss = 0;
    int fault = 0;
    while(fgets(line, sizeof(line), addrFile) != NULL){
        // 2.1: Lê a linha do arquivo e verifica se é diferente de um inteiro (endereço virtual)
        if(sscanf(line, "%d", &virtualAddress) != 1){
            // Caso não seja um inteiro:
            
            // Remove o caracter de nova linha
            line[strcspn(line, "\n")] = 0;
            
            // Verifica se a linha é um comando especial (PageTable ou TLB) ou se é um simples endereço virtual. 
            // Se for um comando especial, chama a função correspondente do gerenciador de memória. 
            if(strcmp(line, "PageTable") == 0){
                memoryManager_printPageTable(memMng, fSaida);
            }else if(strcmp(line, "TLB") == 0){
                memoryManager_printTLB(memMng, fSaida);
            }
        }
        // Caso seja um inteiro, processa o endereço virtual normalmente.
        else{
            Info pInfo = memoryManager_accessAddress(memMng, virtualAddress);
            memoryManager_printAddressInfo(memMng, pInfo, virtualAddress, fSaida);

            AccessResult result = memoryManager_getInfoResult(pInfo);
        
            switch(result){
                case ACCESS_TLB_HIT:
                    hit += 1;
                    break;
                case ACCESS_PAGE_TABLE_HIT:
                    miss += 1;
                    break;
                case ACCESS_PAGE_FAULT:
                    fault += 1;
                    break;
            default:
                break;
            }

            totalAccesses += 1;
        }
    }

    ////////////////////////////
    // Estatisticas
    ////////////////////////////

    float avgMiss = (float)totalAccesses / (float)miss;
    float avgFault = (float)totalAccesses / (float)fault;

    fprintf(fSaida, "\n=== STATISTICS OF EXECUTION ===\n\n");
    fprintf(fSaida, "%s - %s - %s\n", "TLB hits", "TLB misses", "Page faults");
    fprintf(fSaida, "%8d - %10d - %11d\n\n", hit, miss, fault);
    fprintf(fSaida, "%-30s: %3.1f\n", "Avg accesses per TLB miss", avgMiss);
    fprintf(fSaida, "%-30s: %3.1f\n", "Avg accesses per page fault", avgFault);
    fprintf(fSaida, "\n===============================\n\n");

    fclose(addrFile);
}