# Compila todos os arquivos e gera o executável
all: create_dir main

# Cria pasta para armazenar os arquivos objetos
create_dir:
	mkdir -p bin

# Compila o arquivo utils.c e gera o arquivo objeto utils.o
bin/utils.o: src/utils.c include/utils.h
	gcc src/utils.c -Iinclude -Wall -c -o bin/utils.o -lm

# Compila o arquivo main.c e gera o arquivo objeto main.o
bin/main.o: src/main.c bin/utils.o
	gcc src/main.c -Iinclude -Wall -pthread -c -o bin/main.o -lm

# Cria o arquivo executável
main: bin/utils.o bin/main.o
	g++ bin/*.o -Iinclude -O0 -g -Wall -pthread -o programa -lm

# Apaga a pasta de arquivos objetos e o executável
clean:
	rm -rf bin programa