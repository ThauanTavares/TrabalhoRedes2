#include <string.h>
#include "mensagem.h"



// monta a mesagem setando todos campos
void constroi_mensagem (struct mensagem_t mensagem, char msg[TAM_MSG]) {
    //limpar o vetor colocando tudo 0
    memset (msg, 0, TAM_MSG);

    msg[0] = mensagem.tipo;
    msg[1] = mensagem.sequencia;
    msg[2] = mensagem.tam;

    for (int i = 0; i < TAM_DADOS; i++) {
        msg[3+i] = mensagem.dados[i];
    }

    return;
}

// separa os dados dos restos dos campos
void interpreta_mensagem (struct mensagem_t *mensagem, char msg[TAM_MSG]) {
	mensagem->tipo = msg[0];
	mensagem->sequencia = msg[1];
	mensagem->tam = msg[2];

    //limpar o vetor colocando tudo 0
    memset (mensagem->dados, 0, TAM_DADOS);
    for (int i = 0; i < TAM_DADOS; i++) {
        mensagem->dados[i] = msg[3+i];
    }

    return;
}


// monta mensagem para o cliente conectar ao servidor
void constroi_ENTRAR (struct mensagem_t *mensagem, char msg[TAM_MSG]) {
    mensagem->tipo = ENTRAR;
    mensagem->tam = 0;
    mensagem->sequencia = 0;
    memset (mensagem->dados, 0, TAM_DADOS);
    constroi_mensagem (*mensagem, msg);

    return;
}

// monsta mensagem para informar que o cliente foi adicionado
void constroi_ADICIONADO (struct mensagem_t *mensagem, char msg[TAM_MSG]) {
    mensagem->tipo = ADICIONADO;
    mensagem->tam = 0;
    mensagem->sequencia = 0;
    memset (mensagem->dados, 0, TAM_DADOS);
    constroi_mensagem (*mensagem, msg);

    return;
}


// monta mensagem para o servidor informar o fim da transmissao
void constroi_FIM (struct mensagem_t *mensagem, char msg[TAM_MSG], int seq) {
    mensagem->tipo = FIM;
    mensagem->tam = 0;
    mensagem->sequencia = seq;
    memset (mensagem->dados, 0, TAM_DADOS);
    constroi_mensagem (*mensagem, msg);

    return;
}