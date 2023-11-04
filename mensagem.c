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
        msg[2+i] = mensagem.dados[i];
    }

    return;
}

// separa os dados dos restos dos campos
void interpreta_mensagem (struct mensagem_t *mensagem, char msg[TAM_MSG]) {
	mensagem->tipo = msg[0];
	mensagem->sequencia = msg[1];

    //limpar o vetor colocando tudo 0
    memset (mensagem->dados, 0, TAM_DADOS);
    for (int i = 0; i < TAM_DADOS; i++) {
        mensagem->dados[i] = msg[2+i];
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
int confere_seq (unsigned int seq, unsigned int *seq_esperada, int *perdidos, int *fora_ordem) {
    if (*seq_esperada == 5555) {
        *seq_esperada = seq + 1;

        return 1;
    }
    
    if (seq > *seq_esperada) {
        *perdidos = *perdidos + (seq - *seq_esperada);

        *seq_esperada = seq + 1;
        if (*seq_esperada == 256)
            *seq_esperada = 0;

        return 1;
    }

    if (seq == *seq_esperada) {
        *seq_esperada = *seq_esperada + 1;
        if (*seq_esperada == 256)
            *seq_esperada = 0;
        
        return 1;
    }

    if ((*seq_esperada >= 250) && (seq <= 5)) {
        int diferenca = 256 - *seq_esperada;
        *perdidos = *perdidos + (diferenca + seq);

        *seq_esperada = seq + 1;

        return 1;
    }

    *fora_ordem = *fora_ordem + 1;
    *perdidos = *perdidos - 1;

    printf ("SEQ ERRADA\n");

    return 0;
}
