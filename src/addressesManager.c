#include "addressesManager.h"

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
        printf("SUA MENSAGEM DE ERRO AQUI");
        return;
    }

    // 2: Processar endereços do addresses.txt usando memMng
    // TO-DO
    char line[256];
    int virtualAddress;
    int physicalAddress;
    char content;
    while(fgets(line, sizeof(line), addrFile) != NULL){
        // 2.1: Lê a linha do arquivo
        sscanf(line, "%s", virtualAddress);

        // Verifica se a linha é um comando especial (PageTable ou TLB) ou se é um simples endereço virtual. 
        // Se for um comando especial, chama a função correspondente do gerenciador de memória. 
        if(strcmp(line, "PageTable") == 0){
            memoryManager_printPageTable(memMng);
        }else if(strcmp(line, "TLB") == 0){
            memoryManager_printTLB(memMng);
        }
        // Caso contrário, processa o endereço virtual normalmente.
        else{
            // 2.2: Converte a linha lida em um endereço virtual (int)
            virtualAddress = atoi(line);

            // 2.3: Separa o endereço virtual em número da página e offset
            int pageNumber = virtualAddress / 100;  // Ex: 16916 / 100 = 169
            int offset     = virtualAddress % 100;  // Ex: 16916 % 100 = 16

            // 2.4: Obter o número do frame correspondente ao número da página usando memMng
            int frameNumber = memoryManager_getFrameNumber(memMng, pageNumber);

            // 2.5: Transformar o número do frame e o offset em endereço físico
            physicalAddress = (frameNumber * 100) + offset; // Ex: (20 * 100) + 16 = 2016

            // 2.6: ler conteúdo do endereço físico
            content = memoryManager_getPageContentAt(memMng, physicalAddress);

            // 2.7: escrever em fSaida
            // Template de output:
            // Endereço Virtual: 16916  Endereço Físico: 20 Conteúdo: '?'
            fprintf(fSaida, "Endereço Virtual: %d  Endereço Físico: %d Conteúdo: '%c'\n", virtualAddress, physicalAddress, content);
        }
    }

    fclose(addrFile);
}