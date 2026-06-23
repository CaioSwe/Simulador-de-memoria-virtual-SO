#include "fileManager.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void freeReg(void* item, void* extra){
    free(item);
}

const char* changeExtension(const char* path, const char* ext){
    if (path == NULL || ext == NULL){
        printf("\n - changeExtension() -> Um ou mais valores nulos passados como parametro. -");
        return NULL;
    }

    // Procura a primeira ocorrência do caractere '.' na string path.
    char* d = strrchr(path, '.');

    // Aloca espaço necessário para ambos os casos de '.' estar presente ou não.
    char* newStr = (char*)malloc((d ? (d - path) : strlen(path)) + strlen(ext) + 1);
    if(checkAllocation(newStr, "String de mudanca de entensao.")) return NULL;

    // Caso d seja um caractere não nulo (achado), copia path para newStr até o caractere antes do '.'.
    if(d != NULL){
        strncpy(newStr, path, d - path);
        newStr[d - path] = '\0';
    }
    // Caso d seja nulo, copia todo o path para newStr.
    else strcpy(newStr, path);

    // Retorna newStr concatenado com a nova entensão ext.
    return (const char*)strcat(newStr, ext);
}

const char* trimString(const char* string, int n){
    if(string == NULL){
        printf("\n - trimString() -> String e' nula.");
        return NULL;
    }

    // Adiquire o tamanho da string original.
    int stringSize = (int)strlen(string);

    // Caso o inteiro N for maior que o tamanho da string fornecida, imprime mensagem de aviso e retorna NULO.
    if(n > stringSize){
        printf("\n- trimString() -> Valor dado e' maior que a string dada: (%d) > (%d) -\n", stringSize, n);
        return NULL;
    }
    
    // Calcula o tamanho necessário para a nova string truncada.
    int sizeofResult = (stringSize - n + 1);

    // Dinâmicamente aloca espaço para a string.
    char* resultTrim = (char*)malloc(sizeof(char) * sizeofResult);
    if(checkAllocation(resultTrim, "String de corte.")) return NULL;

    // Copia o espaço de memória ocupado pela string original para a string truncada com o tamanho calculado.
    memcpy(resultTrim, string, sizeofResult - 1);
    // Substitui o último caratere pelo terminador '\0'.
    resultTrim[sizeofResult - 1] = '\0';

    // Retorna a string truncada.
    return (const char*)resultTrim;
}

const char* strcatcat(const char* cat1, const char* cat2){
    if(cat1 == NULL || cat2 == NULL){
        printf("\n - strcatcat() -> Um ou mais parametros nulos passados. -");
        return NULL;
    }

    // Dinâmicamente aloca um espaço do tamanho da string1 + string2 + caractere nulo
    char* resultCat = (char*)malloc(sizeof(char) * (strlen(cat1) + strlen(cat2) + 1));
    if(checkAllocation(resultCat, "String de resultado da concatenacao.")) return NULL;

    // Copia e concatena a string1 e string2 para resultCat.
    strcpy(resultCat, cat1);
    strcat(resultCat, cat2);

    // Retorna a string concatenada.
    return (const char*)resultCat;
}

bool checkAllocation(void* var, const char* text){
    // Caso a varia'vel passada for nula, lanca uma mensagem de erro.
    if(var == NULL){
        printf("\n[!] ERRO DE ALOCACAO: %s", text);
        return true;
    }
    return false;
}

void copyFile(FILE* to, const char* from){
    FILE* fromF = fopen(from, "r");

    char linha[1028];
    while(fgets(linha, 256, fromF) != NULL) fprintf(to, "%s", linha);
    
    fclose(fromF);
}