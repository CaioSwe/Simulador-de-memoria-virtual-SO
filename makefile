# Makefile para o projeto TSO - Trabalho de Sistemas Operacionais
# Autores: 
# - André Felipe Ijiri Ribeiro
# - Caio Sweiver de Carvalho

PROJ_NAME = tso
all: $(PROJ_NAME)


# =========================  DETECÇÃO AUTOMÁTICA DO SISTEMA OPERACIONAL ========================= #
ifeq ($(OS),Windows_NT)
    # Comandos para Windows (CMD/PowerShell)
    RM   = del /Q /F
    EXEC = .exe
	LIBS = -lm -lssp
# -lm:   Biblioteca matemática para funções como sqrt, pow, etc.
# -lssp: Biblioteca para proteção contra estouro de pilha (stack protector)
    OBJETOS_DEL = $(subst /,\\,$(OBJETOS))
# Converte as barras / em \ para o comando 'del' do Windows não quebrar
else
    # Comandos para Linux / macOS / WSL
    RM   = rm -f
    EXEC = 
	LIBS = -lm
# -lm:   Biblioteca matemática para funções como sqrt, pow, etc.
    OBJETOS_DEL = $(OBJETOS)
# No Linux, mantém as barras normais
endif
# =============================================================================================== #





# ===================================== COMPILADOR E FLAGS ====================================== #
# Compilador
CC = gcc
# Flags
CFLAGS  = -g -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration -Wall
LDFLAGS = 
# =============================================================================================== #





# ================================= COMPILAÇÃO PROJ. PRINCIPAL ================================== #
# Arquivos-fonte e objetos
PATHS = src/
OBJETOS = $(PATHS)main.o $(PATHS)addressesManager.o $(PATHS)fileManager.o $(PATHS)memoryManager.o $(PATHS)priorityQueue.o $(PATHS)queue.o $(PATHS)types.o

main.o: $(PATHS)main.c $(PATHS)addressesManager.h $(PATHS)fileManager.h $(PATHS)memoryManager.h $(PATHS)priorityQueue.h $(PATHS)queue.h $(PATHS)types.h

addressesManager.h:	$(PATHS)addressesManager.c $(PATHS)addressesManager.h
fileManager.o: 		$(PATHS)fileManager.c 	   $(PATHS)fileManager.h
memoryManager.o: 	$(PATHS)memoryManager.c    $(PATHS)memoryManager.h
priorityQueue.o: 	$(PATHS)priorityQueue.c    $(PATHS)priorityQueue.h
queue.o: 			$(PATHS)queue.c 		   $(PATHS)queue.h
types.o: 			$(PATHS)types.c 		   $(PATHS)types.h



# Projeto principal
$(PROJ_NAME): $(OBJETOS)
	@echo.
	@echo.
	@echo.
	@echo.

	@echo "Compilando o projeto principal $(PROJ_NAME): ..."
	$(CC) -o $(PROJ_NAME) $(OBJETOS) $(LIBS) $(LDFLAGS)
	@echo "Compilacao finalizada com sucesso."

	@echo.

# Arquivos .c em .o
%.o : %.c
	@echo.
	@echo "Compilando $< ..."
	$(CC) -c $(CFLAGS) $< -o $@
# =============================================================================================== #





# =======================================  CLEAN & PHONY ======================================== #
# Regra para limpar os arquivos objetos e o executável
clean:
	@echo.

	@echo Limpando os arquivos objetos e o executavel...
	$(RM) ".\files\correct.txt"

	@echo.

	$(RM) $(OBJETOS_DEL) $(PROJ_NAME)$(EXEC)
	@echo Arquivos objetos e executavel limpos com sucesso!

	@echo.

.PHONY: all
.PHONY: clean
# =============================================================================================== #