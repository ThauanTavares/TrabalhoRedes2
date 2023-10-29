#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mensagem.h"


int main(int argc, char *argv[]) {  
	int sockdescr;
	unsigned int i, seq_esperada;
	struct sockaddr_in sa;
	struct hostent *hp;
	char *host;
	struct mensagem_t mensagem;
	char msg [TAM_MSG];


	if(argc != 3) {
		puts("Uso correto: <cliente> <nome-servidor> <porta>");
		exit(1);
	}

	host = argv[1];

	if((hp = gethostbyname(host)) == NULL){
		puts("Nao consegui obter endereco IP do servidor.");
		exit(1);
	}

	bcopy((char *)hp->h_addr, (char *)&sa.sin_addr, hp->h_length);
	sa.sin_family = hp->h_addrtype;

	sa.sin_port = htons(atoi(argv[2]));

	if((sockdescr=socket(hp->h_addrtype, SOCK_DGRAM, 0)) < 0) {
		puts("Nao consegui abrir o socket.");
		exit(1);
	}


	constroi_ENTRAR (&mensagem, msg);
	if(sendto(sockdescr, msg, TAM_MSG, MSG_DONTWAIT, (struct sockaddr *) &sa, sizeof sa) != TAM_MSG){
		puts("Nao consegui mandar os dados"); 
		exit(1);
	}

	while (mensagem.tipo != ADICIONADO) {
		if(recvfrom (sockdescr, msg, TAM_MSG, MSG_DONTWAIT, (struct sockaddr *) &sa, &i) < 0){
			printf ("erro\n");
			constroi_ENTRAR (&mensagem, msg);
			sendto(sockdescr, msg, TAM_MSG, MSG_DONTWAIT, (struct sockaddr *) &sa, sizeof sa);
			sleep (1);
		}
		else {
			interpreta_mensagem (&mensagem, msg);
		}
	}

	seq_esperada = 0;
	while (mensagem.tipo != FIM) {
		recvfrom (sockdescr, msg, TAM_MSG, 0, (struct sockaddr *) &sa, &i);
		
		interpreta_mensagem (&mensagem, msg);
		//completar a seq_valida
		if ((mensagem.tipo == DADOS) && seq_valida( mensagem.sequencia, &seq_esperada)) {
			fprintf (stderr, "%s", mensagem.dados);
		}
		
	}

	close (sockdescr);
	return 0;
}