all:
	cc -g -Wall -pedantic -std=c99 main.c disas.c symbols.c -o lc3disas
install:
	cp ./lc3disas /usr/local/bin
uninstall:
	rm /usr/local/bin/lc3disas
clean:
	rm ./lc3disas
