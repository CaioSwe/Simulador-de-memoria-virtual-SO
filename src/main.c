#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "fila.h"
#include "fileManager.h"
#include "LRU.h"
#include "priorityQueue.h"

typedef enum TYPES {
    ADDR,
    FRAMES,
    SUBSTYPE
} TYPES;

///////////////

int main(int argc, char* argv[]){
    printf("\n");

    char** paths = calloc(3, sizeof(char*));

    if(argc < 4){
        printf(" [ERRO]: Number of parameters is invalid. \n");
        for(int i = 1; i < 3; i++) free(paths[i]);
        free(paths);
        return 1;
    }

    /*
        Pega todos os nomes dos arquivos base e organiza na variável paths
        paths[0] = endereços
        paths[1] = quadros
        paths[2] = tipo de algoritmo de substituição de página
    */
    for(int i = 1; i < argc; i++){
        int param = i - 1;
        free(paths[param]);
        paths[param] = malloc(strlen(argv[i]) + 1);
        strcpy(paths[param], argv[i]);
    }
    
    // Coloca ".txt" no arquivo de endereços caso não tenha
    char* newPathAddr = (char*)changeExtension(paths[ADDR], ".txt");
    free(paths[ADDR]);
    paths[ADDR] = newPathAddr;

    // Lista todos os parâmetros fornecidos:
    printf("%-12s: %s\n", "Addresses file", paths[ADDR]);
    printf("%-12s: %s\n", "Frames quantity", paths[FRAMES]);
    printf("%-12s: %s\n", "Page replacement algorithm", paths[SUBSTYPE]);
    
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

    // processAddrFile(fPathBin, fPathAddr, fSaida, fOutputPathAddr);
    
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