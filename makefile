all:
	gcc -Wall cliente.c mensagem.c -o cliente
	gcc -Wall servidor.c mensagem.c -o servidor -lpthread