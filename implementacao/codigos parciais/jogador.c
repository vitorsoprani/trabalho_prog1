#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX_CAMINHO 1001 //1000 caracteres + o \0

#define LARGURA_MAX_MAPA 102 //100 + bordas
#define ALTURA_MAX_MAPA 42 //40 + bordas

#define TAM_JOGADOR 3

typedef struct {
    int x;
    int y;
}tPonto;

tPonto LePonto(FILE* arquivo);
/*Le o um ponto no arquivo indicado (idealmente no arquiivo de configuração do mapa)*/



typedef struct {
    tPonto posicao;

    char s; //carcater que representa o jogador;
    char desenho[TAM_JOGADOR][TAM_JOGADOR];
}tJogador;

tJogador InicializaJogador(tPonto pontoInicial);
/*Inicializa o jogador (desneho, simbolo e etc) a partir de um ponto inicial */



typedef struct {
    char arquivoMapa[TAM_MAX_CAMINHO];
    FILE* config;

    int largura;
    int altura;

    char grid[ALTURA_MAX_MAPA][LARGURA_MAX_MAPA];
    char gridVazio[ALTURA_MAX_MAPA][LARGURA_MAX_MAPA];

    tJogador jogador;
}tMapa;

tMapa InicializaMapa(char diretorio[]);
/*Le as informações dos arquivos de configuração e inicializa os valores*/

tMapa InicializaGridMapa(tMapa mapa);
/*Inicializa os valores do gridVazio (somente as bordas e espaços em branco)*/

tMapa AtualizaMapa(tMapa mapa);
/*Desenha os "objetos" no mapa seguindo a ordem de prioridade*/

tMapa DesenhaGridVazioNoMapa(tMapa mapa);
/*Imprime no grid do mapa o grid vazio (apaga tudo)*/

tMapa DesenheaJogadorNoMapa(tMapa mapa, tJogador jogador);
/*Imprime o jogador no mapa
(considera-se que o jogador fornecido nunca ultrapassará as bordas,
pois essa condição ja é checada na função que movimenta o jogador)*/

void ImprimeMapa(tMapa mapa);
/*Imprime o mapa*/



int main(int argc, char* argv[]) {
    //Checa se algum diretorio foi passado
    if (argc < 2) {
        printf("[ERRO] Informe o diretorio com os arquivos de configuracao.\n");
        exit(1);
    }

    //"lendo" o diretorio
    char diretorio[TAM_MAX_CAMINHO];
    sprintf(diretorio, "%s", argv[1]);

    tMapa mapa = InicializaMapa(diretorio);

    ImprimeMapa(mapa);

    return 0;
}



tMapa InicializaMapa(char diretorio[]) {
    tMapa mapa;
    tPonto posicaoJogador;

    //cria o caminho até o arquivo de configuração
    strcpy(mapa.arquivoMapa, diretorio);
    strcat(mapa.arquivoMapa, "mapa.txt");

    //abrindo o arquivo de configuração do mapa no modo leitura
    mapa.config = fopen(mapa.arquivoMapa, "r");
    //checa se foi possivel abrir o arquivo no diretorio indicado
    if (mapa.config == NULL) {
        printf("[ERRO] O arquivo \"%s\" nao foi encontrado.\n", mapa.arquivoMapa);
        exit(1);
    }

    fscanf(mapa.config, "%d %d\n", &mapa.largura, &mapa.altura);

    mapa = InicializaGridMapa(mapa);

    posicaoJogador = LePonto(mapa.config);

    mapa.jogador = InicializaJogador(posicaoJogador);

    mapa = AtualizaMapa(mapa);

    //fechando o arquivo de configuração do mapa
    fclose(mapa.config);
    return mapa;
}



tMapa InicializaGridMapa(tMapa mapa) {
    //inicializando o mapa
    for (int i = 0; i < mapa.altura + 2; i++) {
        for (int j = 0; j < mapa.largura + 2; j++) {
            if ((i == 0 || i > mapa.altura) && (j == 0 || j > mapa.largura)) {
                mapa.gridVazio[i][j] = '+';
            } else {
                if (i == 0 || i > mapa.altura) {
                    mapa.gridVazio[i][j] = '-';
                } else if (j == 0 || j > mapa.largura) {
                    mapa.gridVazio[i][j] = '|';
                } else {
                    mapa.gridVazio[i][j] = ' ';
                }
            }
        }
    }
    return mapa;
}

void ImprimeMapa(tMapa mapa) {
    //imprimindo o mapa
    for (int i = 0; i < mapa.altura + 2; i++) {
        for (int j = 0; j < mapa.largura + 2; j++) {
            printf("%c", mapa.grid[i][j]);
        }
        printf("\n");
    }
}

tMapa DesenhaGridVazioNoMapa(tMapa mapa) {
    for (int i = 0; i < mapa.altura + 2; i++) {
        for (int j = 0; j < mapa.largura + 2; j++) {
            mapa.grid[i][j] = mapa.gridVazio[i][j];
        }
    }

    return mapa;
}

tMapa DesenheaJogadorNoMapa(tMapa mapa, tJogador jogador) {
    tPonto posicao = jogador.posicao;

    //laço que percorre todas as posições do desenho do jogador
    int k = -1;
    for (int i = 0; i < TAM_JOGADOR; i++) {
        int l = -1;
        for (int j = 0; j < TAM_JOGADOR; j++) {
            mapa.grid[posicao.y + k][posicao.x + l] = jogador.desenho[i][j];
            l++;
        }
        k++;
    }
    return mapa;
}

tMapa AtualizaMapa(tMapa mapa) {
    mapa = DesenhaGridVazioNoMapa(mapa);

    mapa = DesenheaJogadorNoMapa(mapa, mapa.jogador);

    return mapa;
}



tJogador InicializaJogador(tPonto pontoInicial) {
    tJogador jogador;

    jogador.posicao = pontoInicial;

    jogador.s = 'M';

    for (int i = 0; i < TAM_JOGADOR; i++) {
        for (int j = 0; j < TAM_JOGADOR; j++) {
            if (i == 0 && j == 1) {
                jogador.desenho[i][j] = ' ';
            } else {
                jogador.desenho[i][j] = jogador.s;
            }
        }
    }

    return jogador;
}



tPonto LePonto(FILE* arquivo) {
    tPonto ponto;
    fscanf(arquivo, "(%d %d)", &ponto.x, &ponto.y);
    return ponto;
}

