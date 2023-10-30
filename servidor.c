#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mensagem.h"



void adiciona_cliente(int s, struct sockaddr_in *cliente, struct sockaddr_in clientes[MAX_CLIENTES], unsigned int *quant_clientes) {
	struct mensagem_t mensagem;
	char msg[TAM_MSG];
	int res;
	unsigned int tam = sizeof (*cliente);

	if (*quant_clientes < MAX_CLIENTES) {
		constroi_ADICIONADO (&mensagem, msg);

		// se conseguir enviar mensagem para o cliente adiciona ele
		res = sendto (s, msg, TAM_MSG, 0, (struct sockaddr *) cliente, tam);
		if (res > 0) {
			clientes[*quant_clientes] = *cliente;
			*quant_clientes = *quant_clientes + 1;
		}
	}
	else {
		constroi_LOTADO (&mensagem, msg);
		sendto (s, msg, TAM_MSG, 0, (struct sockaddr *) cliente, tam);
	}

	return;
}

void envia_dados (int s, char msg [TAM_MSG], struct sockaddr_in clientes[MAX_CLIENTES], unsigned int quant_clientes) {
	unsigned int tam;

	for (int i = 0; i < quant_clientes; i++) {
		tam = sizeof(clientes[i]);
		sendto (s, msg, TAM_MSG, MSG_CONFIRM, (struct sockaddr *) &clientes[i], tam);
	}

	return;
}

int main ( int argc, char *argv[] ) {
	int s, intervalo;
	unsigned int i, seq, quant_clientes;
	char localhost [MAXHOSTNAME];
	char msg[TAM_MSG];
	
	struct sockaddr_in servidor, cliente;
	struct sockaddr_in clientes[MAX_CLIENTES];
	struct hostent *hp;
	struct mensagem_t mensagem;


	if (argc != 3) {
		puts("Uso correto: servidor <porta> <intervalo entre envio (segundos)>");
		exit(1);
	}

	intervalo = atoi(argv[2]);

	gethostname (localhost, MAXHOSTNAME);

	if ((hp = gethostbyname( localhost)) == NULL){
		puts ("Nao consegui meu proprio IP");
		exit (1);
	}

	fprintf (stderr, "Meu nome: %s\n", hp->h_name);
	
	servidor.sin_port = htons(atoi(argv[1]));

	bcopy ((char *) hp->h_addr, (char *) &servidor.sin_addr, hp->h_length);

	servidor.sin_family = hp->h_addrtype;		


	if ((s = socket(hp->h_addrtype,SOCK_DGRAM,0)) < 0){
           puts ( "Nao consegui abrir o socket" );
		exit ( 1 );
	}	

	if (bind(s, (struct sockaddr *) &servidor,sizeof(servidor)) < 0){
		puts ( "Nao consegui fazer o bind" );
		exit ( 1 );
	}		
 

	FILE *arq;
	arq = fopen ("./FLAXSAN.txt","r");

	// roda uma vez o servidor para iniciar melhor
	sendto (s, msg, TAM_MSG, 0, (struct sockaddr *) &servidor, sizeof(cliente));
	recvfrom (s, msg, TAM_MSG, 0, (struct sockaddr *) &cliente, &i);


	// espera primeiro cliente para começar a transmissao
	quant_clientes = 0;
	recvfrom (s, msg, TAM_MSG, 0, (struct sockaddr *) &cliente, &i);
	adiciona_cliente (s, &cliente, clientes, &quant_clientes);

	sleep (3);

	seq = 1555;
	while (!feof (arq)) {
		if (recvfrom (s, msg, TAM_MSG, MSG_DONTWAIT, (struct sockaddr *) &cliente, &i) > 0) {
			adiciona_cliente (s, &cliente, clientes, &quant_clientes);
		}

		fgets (mensagem.dados, TAM_DADOS, arq);
		mensagem.tipo = DADOS;
		mensagem.sequencia = seq;
		constroi_mensagem (mensagem, msg);
		
		envia_dados (s, msg, clientes, quant_clientes);

		// seq++;
		
		sleep (intervalo);
	}
	fclose (arq);

	constroi_FIM (&mensagem, msg, seq);
	envia_dados (s, msg, clientes, quant_clientes);

    return 0;
}