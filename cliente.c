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
	int sockdescr, perdidos, fora_ordem, total_pacotes;
	unsigned int i, seq_esperada;
	struct sockaddr_in sa;
	struct hostent *hp;
	char *host;
	struct mensagem_t mensagem;
	char msg [TAM_MSG];
	informacoes_t info_casa, info_fora;


	if(argc != 3) {
		puts("Uso correto: <cliente> <nome-servidor> <porta>");
		exit(1);
	}

	host = argv[1];

	if ((hp = gethostbyname (host)) == NULL){
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


	// pede para entrar na lista de transmissao
	constroi_ENTRAR (&mensagem, msg);
	if(sendto(sockdescr, msg, TAM_MSG, 0, (struct sockaddr *) &sa, sizeof sa) != TAM_MSG){
		puts("Nao consegui mandar os dados"); 
		exit(1);
	}


	// confere se entrou na transmissao ou se o servidor esta lotado
	recvfrom (sockdescr, msg, TAM_MSG, 0, (struct sockaddr *) &sa, &i);
	interpreta_mensagem (&mensagem, msg);
	if (mensagem.tipo == LOTADO) {
		printf ("Servidor Lotado\n");
		close (sockdescr);
		return 0;
	}


    // sequencia é inicializada com valor arbitrario para ser realmente inicializada com o primeiro pacote recebido
	seq_esperada = 5555;
	perdidos = 0;
	fora_ordem = 0;
	total_pacotes = 0;
	inicializa_informacoes (&info_casa);
	inicializa_informacoes (&info_fora);

	//escuta a transmissao enquanto nao recebeu mensagem de FIM
	while (mensagem.tipo != FIM) {
		recvfrom (sockdescr, msg, TAM_MSG, 0, (struct sockaddr *) &sa, &i);
		interpreta_mensagem (&mensagem, msg);

		if ((mensagem.tipo == DADOS)) {
			total_pacotes++;
			atualiza_informacoes (&info_casa, &info_fora, mensagem);
			if (confere_seq(mensagem.sequencia, &seq_esperada, &perdidos, &fora_ordem)) {
				printf ("%s", mensagem.dados);
			}
		}
	}

	close (sockdescr);

	printf ("\n\n");
	printf ("Pacotes recebidos: %d\n", total_pacotes);
	printf ("Pacotes fora de ordem: %d\n", fora_ordem);
	printf ("Pacotes perdidos: %d\n", perdidos);
	escreve_informacoes (info_casa, info_fora);

	return 0;
}