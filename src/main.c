#include "fileManager.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "lista.h"

typedef enum TYPES {
    ADDR,
    FRAMES,
    SUBSTYPE
} TYPES;

///////////////

int main(int argc, char* argv[]){
    char** paths = calloc(3, sizeof(char*));

    if(argc < 4){
        printf(" [ERRO] Número de parâmetros fornecidos não corresponde com o programa. ");
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
    printf("%-12s: %s\n", "Arquivo de endereços", paths[ADDR]);
    printf("%-12s: %s\n", "Quantidade de quadros", paths[FRAMES]);
    printf("%-12s: %s\n", "Tipo de algoritmo de substituição de página", paths[SUBSTYPE]);
    
    // (3) Lendo o arquivo de endereços (addresses.txt)
    ////////////////////////////////////////////////

    // BIN (BACKING STORE.bin)
    const char* fPathBin = "BACKING STORE.bin";

    // ADDR (addresses.txt)
    const char* fPathAddr = strcatcat(paths[ADDR], "");

    // SAIDA (correct.txt)
    const char* fOutputPathAddr = "correct.txt";

    // Abre o caminho do output (correct.txt) em modo de escrita
    FILE* fSaida = fopen(fOutputPathAddr, "w");
    printf("\nEscrevendo no arquivo: %s\n", fOutputPathAddr);
    fprintf(fSaida, " == INICIO DA EXECUÇÃO == \n");

    processAddrFile(fPathBin, fPathAddr, fSaida, fOutputPathAddr);
    
    fprintf(fSaida, " == FIM DA EXECUÇÃO == ");
    fclose(fSaida);

    //
    // Estatisticas ...
    //

    // Paths
    for(int i = 1; i < 4; i++) free(paths[i]);
    free(paths);
    
    printf("\n [SUCESSO] Programa finalizado. \n");

    return 0;
}