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
else
    # Comandos para Linux / macOS / WSL
    RM   = rm -f
    EXEC = 
	LIBS = -lm
# -lm:   Biblioteca matemática para funções como sqrt, pow, etc.
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
OBJETOS = main.o fila.o fileManager.o LRU.o priorityQueue.o

main.o: main.c fila.h fileManager.h LRU.h priorityQueue.h

fila.o: fila.c fila.h
fileManager.o: fileManager.c fileManager.h
LRU.o: LRU.c LRU.h
priorityQueue.o: priorityQueue.c priorityQueue.h



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
	$(RM) $(OBJETOS) $(PROJ_NAME)$(EXEC)
	@echo Arquivos objetos e executavel limpos com sucesso!

	@echo.

.PHONY: all
.PHONY: clean
# =============================================================================================== #