all:
	gcc -Wall cliente.c -o cliente
	gcc -Wall servidor.c -o servidor -lpthread