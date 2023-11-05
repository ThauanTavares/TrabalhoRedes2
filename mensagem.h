#define MAXHOSTNAME 30
#define TAM_MSG 505
#define TAM_DADOS 500
#define MAX_CLIENTES 50

// Tipos da mensagem
#define ENTRAR 1
#define ADICIONADO 2
#define LOTADO 3
#define DADOS 4
#define FIM 7

// Tipo da informação do jogo
#define GOL 'G'
#define CARTAO_VERMELHO 'V'
#define CARTAO_AMARELO 'A'
#define SUBSTITUICAO 'S'
#define INTERVALO 'I'
#define PENALTI 'P'
#define FALTA_PERIGOSA 'T'  // Tiro livre
#define INICIO 'C'          // Comeco
#define FIM_TEMPO 'F'        // Fim
#define VITORIA 'W'         // Win
#define COMENTARIO 'X'      // comentario qualquer

#define TIME_CASA 'A'
#define TIME_FORA 'B'
#define PRIMEIRO_TEMPO '1'

// estrutura da mensagem
struct mensagem_t {
    unsigned int tipo : 3;
    unsigned int sequencia : 8;
    char info;
    char equipe;
    char metade;
    char dados[TAM_DADOS];
} __attribute__((packed));

typedef struct informacoes_t {
    int gols1;            // quantidade de gols no 1 tempo
    int gols2;
    int cartaoA1;         // quantidade de cartões amarelos no 1 tempo
    int cartaoA2;
    int cartaoV1;         // quantidade de cartões vermelhos no 1 tempo
    int cartaoV2;
    int subs1;            // quantidade de substituições no 1 tempo
    int subs2;
} informacoes_t;


void constroi_mensagem (struct mensagem_t mensagem, char msg[TAM_MSG]);
void interpreta_mensagem (struct mensagem_t *mensagem, char msg[TAM_MSG]);
void constroi_ENTRAR (struct mensagem_t *mensagem, char msg[TAM_MSG]);
void constroi_ADICIONADO (struct mensagem_t *mensagem, char msg[TAM_MSG]);
void constroi_LOTADO (struct mensagem_t *mensagem, char msg[TAM_MSG]);
void constroi_FIM (struct mensagem_t *mensagem, char msg[TAM_MSG], int seq);
int confere_seq (unsigned int seq, unsigned int *seq_esperada, int *perdidos, int *fora_ordem);
void inicializa_informacoes (informacoes_t *info);
void atualiza_informacoes (informacoes_t *info_casa, informacoes_t *info_fora, struct mensagem_t mensagem);
void escreve_informacoes (informacoes_t info_casa, informacoes_t info_fora);