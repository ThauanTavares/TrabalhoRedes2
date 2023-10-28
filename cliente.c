#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1000

int main(int argc, char *argv[]) 

 {  int sockdescr;
    struct sockaddr_in sa;
    struct hostent *hp;
    char buf[BUF_SIZE+1];
    char *host;

    unsigned int i;

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

    if(sendto(sockdescr, "a", 1, 0, (struct sockaddr *) &sa, sizeof sa) != 1){
      puts("Nao consegui mandar os dados"); 
      exit(1);
    }

    while (1) {
      recvfrom(sockdescr, buf, BUF_SIZE, 0, (struct sockaddr *) &sa, &i);

      printf("%s", buf);
    }
   
    close(sockdescr);
    return 0;
}