#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "mensagem.h"



// monta a mesagem setando todos campos
void constroi_mensagem (struct mensagem_t mensagem, char msg[TAM_MSG]) {
    //limpar o vetor colocando tudo 0
    memset (msg, 0, TAM_MSG);

    msg[0] = mensagem.tipo;
    msg[1] = mensagem.sequencia;

    for (int i = 0; i < TAM_DADOS; i++) {
        msg[4+i] = mensagem.dados[i];
    }

    return;
}

// separa os dados dos restos dos campos
void interpreta_mensagem (struct mensagem_t *mensagem, char msg[TAM_MSG]) {
	mensagem->tipo = msg[0];
	mensagem->sequencia = msg[1];

    // ARRUMAR NO CONSTROI
    char teste[4];
    teste[0] = msg[1];
    teste[1] = msg[2];
    teste[2] = msg[3];
    teste[3] = msg[4];
    // sprintf(teste, "%c%c%c%c", msg[1], msg[2], msg[3], msg[4]);
    printf ("%d\n",atoi(teste));

    //limpar o vetor colocando tudo 0
    memset (mensagem->dados, 0, TAM_DADOS);
    for (int i = 0; i < TAM_DADOS; i++) {
        mensagem->dados[i] = msg[4+i];
    }

    return;
}


// monta mensagem para o cliente conectar ao servidor
void constroi_ENTRAR (struct mensagem_t *mensagem, char msg[TAM_MSG]) {
    mensagem->tipo = ENTRAR;
    mensagem->sequencia = 0;
    memset (mensagem->dados, 0, TAM_DADOS);
    constroi_mensagem (*mensagem, msg);

    return;
}

// monsta mensagem para informar que o cliente foi adicionado
void constroi_ADICIONADO (struct mensagem_t *mensagem, char msg[TAM_MSG]) {
    mensagem->tipo = ADICIONADO;
    mensagem->sequencia = 0;
    memset (mensagem->dados, 0, TAM_DADOS);
    constroi_mensagem (*mensagem, msg);

    return;
}

// monsta mensagem para informar que o servidor ja esta lotado
void constroi_LOTADO (struct mensagem_t *mensagem, char msg[TAM_MSG]) {
    mensagem->tipo = LOTADO;
    mensagem->sequencia = 0;
    memset (mensagem->dados, 0, TAM_DADOS);
    constroi_mensagem (*mensagem, msg);

    return;
}

// monta mensagem para o servidor informar o fim da transmissao
void constroi_FIM (struct mensagem_t *mensagem, char msg[TAM_MSG], int seq) {
    mensagem->tipo = FIM;
    mensagem->sequencia = seq;
    memset (mensagem->dados, 0, TAM_DADOS);
    constroi_mensagem (*mensagem, msg);

    return;
}

// confere se a sequencia é valida e atualiza a variavel
int seq_valida (unsigned int seq, unsigned int *seq_esperada) {
    if (seq > *seq_esperada) {
        *seq_esperada = seq;
        return 1;
    }

    if (seq == *seq_esperada) {
        *seq_esperada = *seq_esperada + 1;
        return 1;
    }

    return 0;
}
