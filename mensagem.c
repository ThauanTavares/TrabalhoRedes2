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
    msg[2] = mensagem.info;
    msg[3] = mensagem.equipe;
    msg[4] = mensagem.metade;

    for (int i = 0; i < TAM_DADOS; i++) {
        msg[5+i] = mensagem.dados[i];
    }

    return;
}

// separa os dados dos restos dos campos
void interpreta_mensagem (struct mensagem_t *mensagem, char msg[TAM_MSG]) {
	mensagem->tipo = msg[0];
	mensagem->sequencia = msg[1];
	mensagem->info = msg[2];
	mensagem->equipe = msg[3];
	mensagem->metade = msg[4];

    //limpar o vetor colocando tudo 0
    memset (mensagem->dados, 0, TAM_DADOS);
    for (int i = 0; i < TAM_DADOS; i++) {
        mensagem->dados[i] = msg[5+i];
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

void inicializa_informacoes (informacoes_t *info) {
    info->gols1 = 0;
    info->gols2 = 0;
    info->cartaoA1 = 0;
    info->cartaoA2 = 0;
    info->cartaoV1 = 0;
    info->cartaoV2 = 0;
    info->subs1 = 0;
    info->subs2 = 0;


    return;
}

void atualiza_informacoes (informacoes_t *info_casa, informacoes_t *info_fora, struct mensagem_t mensagem) {
    if (mensagem.equipe == TIME_CASA) {
        if (mensagem.info == GOL) {
            if (mensagem.metade == '1')
                info_casa->gols1++;
            else
                info_casa->gols2++;
        }
        else if (mensagem.info == CARTAO_AMARELO) {
            if (mensagem.metade == '1')
                info_casa->cartaoA1++;
            else
                info_casa->cartaoA2++;
        }
        else if (mensagem.info == CARTAO_VERMELHO) {
            if (mensagem.metade == '1')
                info_casa->cartaoV1++;
            else
                info_casa->cartaoV2++;
        }
        else if (mensagem.info == SUBSTITUICAO) {
            if (mensagem.metade == '1')
                info_casa->subs1++;
            else
                info_casa->subs2++;
        }
    }
    else {
        if (mensagem.info == GOL) {
            if (mensagem.metade == '1')
                info_fora->gols1++;
            else
                info_fora->gols2++;
        }
        else if (mensagem.info == CARTAO_AMARELO) {
            if (mensagem.metade == '1')
                info_fora->cartaoA1++;
            else
                info_fora->cartaoA2++;
        }
        else if (mensagem.info == CARTAO_VERMELHO) {
            if (mensagem.metade == '1')
                info_fora->cartaoV1++;
            else
                info_fora->cartaoV2++;
        }
        else if (mensagem.info == SUBSTITUICAO) {
            if (mensagem.metade == '1')
                info_fora->subs1++;
            else
                info_fora->subs2++;
        }
    }

    return;
}

void escreve_informacoes (informacoes_t info_casa, informacoes_t info_fora) {
    printf ("Gols:\n");
    printf ("Primeiro tempo: %dx%d\n", info_casa.gols1, info_fora.gols1);
    printf ("Segundo tempo: %dx%d\n", info_casa.gols2, info_fora.gols2);
    printf ("Total: %dx%d\n", info_casa.gols1 + info_casa.gols2, info_fora.gols1 + info_fora.gols2);

    printf ("\n");

    printf ("Cartões:\n");
    printf ("Primeiro tempo: amarelos-%dx%d  vermelhos-%dx%d\n", info_casa.cartaoA1, info_fora.cartaoA1, info_casa.cartaoV1, info_fora.cartaoV1);
    printf ("Seundo tempo: amarelos-%dx%d  vermelhos-%dx%d\n", info_casa.cartaoA2, info_fora.cartaoA2, info_casa.cartaoV2, info_fora.cartaoV2);
    printf ("Total: amarelos-%dx%d  vermelhos-%dx%d\n", info_casa.cartaoA1 + info_casa.cartaoA2, info_fora.cartaoA1 + info_fora.cartaoA2, info_casa.cartaoV1 + info_casa.cartaoV2, info_fora.cartaoV1 + info_fora.cartaoV2);

    printf ("\n");

    printf ("Substituições:\n");
    printf ("Primeiro tempo: %dx%d\n", info_casa.subs1, info_fora.subs1);
    printf ("Segundo tempo: %dx%d\n", info_casa.subs2, info_fora.subs2);
    printf ("Total: %dx%d\n", info_casa.subs1 + info_casa.subs2, info_fora.subs1 + info_fora.subs2);

    return;
}