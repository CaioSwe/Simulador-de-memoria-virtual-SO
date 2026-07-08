# AndreIjiriEDIIT2
Repositório para o trabalho da matéria de Sistemas Operacionais

Alunos: 
- André Felipe Ijiri Ribeiro
- Caio Sweiver de Carvalho



# Compilação
C:\ ... \Simulador-de-memoria-virtual-SO
mingw32-make
./tso files/addresses.txt (número de quadros) (algoritmo de sibstituição)



# Clean
mingw32-make clean



# Valgrind
Abrir Powershell
Digitar "wsl"
Digitar "cd OneDrive/Documentos/GitHub/Simulador-de-memoria-virtual-SO"
Compilar com o Makefile: "make"
Executar "valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./tso files/addresses.txt (número de quadros) (algoritmo de sibstituição)"