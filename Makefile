all:
	gcc -g -Wall -pedantic -std=c99 main.c disas.c symbols.c -o lc3disas
