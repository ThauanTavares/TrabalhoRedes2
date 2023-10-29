#define MAXHOSTNAME 30
#define TAM_MSG 503
#define TAM_DADOS 500
#define MAX_CLIENTES 50

// Tipos da mensagem
#define ENTRAR 1
#define ADICIONADO 2
#define DADOS 4
#define FIM 7

// estrutura da mensagem
struct mensagem_t {
    unsigned int tipo : 3;
    unsigned int sequencia : 4;
    unsigned int tam : 6;
    char dados[TAM_DADOS];
} __attribute__((packed));


void constroi_mensagem (struct mensagem_t mensagem, char msg[TAM_MSG]);
void interpreta_mensagem (struct mensagem_t *mensagem, char msg[TAM_MSG]);
void constroi_ENTRAR (struct mensagem_t *mensagem, char msg[TAM_MSG]);
void constroi_ADICIONADO (struct mensagem_t *mensagem, char msg[TAM_MSG]);
void constroi_FIM (struct mensagem_t *mensagem, char msg[TAM_MSG], int seq);