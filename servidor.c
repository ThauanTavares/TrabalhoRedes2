#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


#define TAMFILA      5
#define MAXHOSTNAME 30
#define BUF_SIZE 1000
#define MAX_CLIENTES 50

struct sockaddr_in clientes[MAX_CLIENTES];
unsigned int quant_clientes = 0;




void adiciona_cliente(int s, struct sockaddr_in *cliente) {

	if (quant_clientes < MAX_CLIENTES) {
		clientes[quant_clientes] = *cliente;
		quant_clientes++;
	}
	// else {
    //   sleep(3);

	// 	sendto (s, "Servidor lotado\n", BUF_SIZE, 0, (struct sockaddr *) cliente, sizeof(*cliente));
	// 	fprintf (stderr, "lotado\n");
	// }
	return;
}

void *escuta_clientes(void *arg) {
	unsigned int i;
	char buf [BUF_SIZE + 1];
	struct sockaddr_in cliente;
	int *s = (int*)arg;

    while (1) {
        recvfrom (*s, buf, 1, 0, (struct sockaddr *) &cliente, &i);
		adiciona_cliente (*s, &cliente);
    }
}

void envia_dados (int s, char buf [BUF_SIZE + 1]) {
	
	unsigned int tam; 
	for (int i = 0; i < quant_clientes; i++) {
		tam = sizeof(clientes[i]);
		sendto (s, buf, BUF_SIZE, 0, (struct sockaddr *) &clientes[i], tam);
	}

	return;
}

int main ( int argc, char *argv[] )
  {
	int s;
	unsigned int i;
	char buf [BUF_SIZE + 1];
	struct sockaddr_in sa;  /* sa: servidor, isa: cliente */
	struct hostent *hp;
	char localhost [MAXHOSTNAME];
	int intervalo;


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
	
	sa.sin_port = htons(atoi(argv[1]));

	bcopy ((char *) hp->h_addr, (char *) &sa.sin_addr, hp->h_length);

	sa.sin_family = hp->h_addrtype;		


	if ((s = socket(hp->h_addrtype,SOCK_DGRAM,0)) < 0){
           puts ( "Nao consegui abrir o socket" );
		exit ( 1 );
	}	

	if (bind(s, (struct sockaddr *) &sa,sizeof(sa)) < 0){
		puts ( "Nao consegui fazer o bind" );
		exit ( 1 );
	}		
 

	FILE *arq;
	arq = fopen("./FLAXSAN.txt","r");

	struct sockaddr_in teste;
	recvfrom (s, buf, 1, 0, (struct sockaddr *) &teste, &i);


	adiciona_cliente (s, &teste);

	pthread_t escuta_clientes_thread;
	pthread_create(&escuta_clientes_thread, NULL, escuta_clientes, &s);

	while (!feof (arq)) {
		fgets(buf, BUF_SIZE, arq);
		envia_dados (s, buf);
		sleep (intervalo);
	}
	fclose (arq);

	

	// struct sockaddr_in teste;
	// recvfrom (s, buf, 1, 0, (struct sockaddr *) &teste, &i);


	// adiciona_cliente (s, &teste);

	// pthread_t escuta_clientes_thread;
	// pthread_create(&escuta_clientes_thread, NULL, escuta_clientes, &s);

	// int cont = 0;
	// while (1) {
		
	// 	sprintf (buf, "%d", cont);
	// 	strcat (buf, "\n");
	// 	envia_dados (s, buf);
	// 	cont++;

	// 	sleep (1);
	// }

    return 0;
}