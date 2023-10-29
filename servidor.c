#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mensagem.h"


struct sockaddr_in clientes[MAX_CLIENTES];
unsigned int quant_clientes = 0;

void adiciona_cliente(int s, struct sockaddr_in *cliente, struct mensagem_t mensagem, char msg[TAM_MSG]) {

	if (quant_clientes < MAX_CLIENTES) {
		clientes[quant_clientes] = *cliente;
		quant_clientes++;

		constroi_ADICIONADO (&mensagem, msg);
		unsigned int tam = sizeof(*cliente);
		sendto (s, msg, TAM_MSG, MSG_CONFIRM, (struct sockaddr *) cliente, tam);
	}
	// else {
    //   sleep(3);

	// 	sendto (s, "Servidor lotado\n", TAM_MSG, 0, (struct sockaddr *) cliente, sizeof(*cliente));
	// 	fprintf (stderr, "lotado\n");
	// }
	return;
}

void envia_dados (int s, char msg [TAM_MSG]) {
	
	unsigned int tam; 
	for (int i = 0; i < quant_clientes; i++) {
		tam = sizeof(clientes[i]);
		sendto (s, msg, TAM_MSG, MSG_DONTWAIT, (struct sockaddr *) &clientes[i], tam);
		printf ("adicionado\n");
	}

	return;
}

int main ( int argc, char *argv[] ) {
	int s, intervalo;
	unsigned int i, seq;
	struct sockaddr_in servidor, cliente;
	struct hostent *hp;
	char localhost [MAXHOSTNAME];
	struct mensagem_t mensagem;
	char msg[TAM_MSG];


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

	recvfrom (s, msg, TAM_MSG, 0, (struct sockaddr *) &cliente, &i);
	adiciona_cliente (s, &cliente, mensagem, msg);

	seq = 0;
	while (!feof (arq)) {
		if (recvfrom (s, msg, TAM_MSG, MSG_DONTWAIT, (struct sockaddr *) &cliente, &i) > 0) {
			adiciona_cliente (s, &cliente, mensagem, msg);
		}

		fgets (mensagem.dados, TAM_DADOS, arq);
		mensagem.tipo = DADOS;
		mensagem.sequencia = seq;
		constroi_mensagem (mensagem, msg);
		
		envia_dados (s, msg);

		seq++;
		if (seq == 16){
			seq = 0;
		}
		
		sleep (intervalo);
	}
	fclose (arq);

	constroi_FIM (&mensagem, msg, seq);
	envia_dados (s,msg);

    return 0;
}