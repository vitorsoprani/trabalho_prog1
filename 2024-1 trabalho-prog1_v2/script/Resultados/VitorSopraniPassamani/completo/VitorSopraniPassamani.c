#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE                1
#define FALSE               0

#define TAM_MAX_CAMINHO     1001    //1000 caracteres + o \0

#define LARGURA_MAX_MAPA    102     //100 + bordas
#define ALTURA_MAX_MAPA     42      //40 + bordas
#define QTD_FILEIRAS        3
#define MAX_INIMIGOS        100     //Qtd maxima de inimigos possiveis, tendo 3 fileiras e largura maxima 100

#define TAM_JOGADOR         3

#define TAM_INIMIGO         3
#define QTD_FRAMES          3


typedef struct {
    char arquivo[TAM_MAX_CAMINHO];
    FILE* config;

    int x;
    int y;

    int animado;
    int estaVivo;

    char frames[QTD_FRAMES][TAM_JOGADOR][TAM_JOGADOR];

    int fileira;
    int indice;
}tInimigo;

tInimigo InicializaInimigo(FILE* configMapa, char diretorio[], int fileira, int indice);
/*Inicializa o inimigo (posição, animação e etc) a partir de um arquivo de configuração*/

tInimigo LeFramesInimigo(tInimigo inimigo);
/*Acessa o arquivo de configuração do inimigo e armazena os desenhos*/


typedef struct {
    int x;
    int y;

    char s; //carcater que representa o jogador;
    char desenho[TAM_JOGADOR][TAM_JOGADOR];
}tJogador;

tJogador InicializaJogador(FILE* config);
/*Inicializa o jogador (desenho, simbolo e etc) a partir de um arquivo de configuração*/

int LinhaJogador(tJogador jogador);
/*Retorna a linha do jogador*/

int ColunaJogador(tJogador jogador);
/*Retorna a coluna do jogador*/



typedef struct {
    char arquivo[TAM_MAX_CAMINHO];
    FILE* config;

    int largura;
    int altura;

    char grid[ALTURA_MAX_MAPA][LARGURA_MAX_MAPA];
    char gridVazio[ALTURA_MAX_MAPA][LARGURA_MAX_MAPA];

    tJogador jogador;

    int qtdInimigos;
    tInimigo inimigos[MAX_INIMIGOS];
    int alturaMaximaInimigo;
}tMapa;

tMapa InicializaMapa(char diretorio[]);
/*Le as informações dos arquivos de configuração e inicializa os valores*/

void GeraArquivoInicializacao(tMapa mapa, char diretorio[]);

tMapa InicializaGridMapa(tMapa mapa);
/*Inicializa os valores do gridVazio (somente as bordas e espaços em branco)*/

tMapa AtualizaMapa(tMapa mapa);
/*Desenha os "objetos" no mapa seguindo a ordem de prioridade*/

tMapa DesenhaGridVazioNoMapa(tMapa mapa);
/*Imprime no grid do mapa o grid vazio (apaga tudo)*/

tMapa DesenhaJogadorNoMapa(tMapa mapa, tJogador jogador);
/*Imprime o jogador no mapa
(considera-se que o jogador fornecido nunca ultrapassará as bordas,
pois essa condição ja é checada na função que movimenta o jogador)*/

tMapa DesenhaInimigosNoMapa(tMapa mapa);
/*Percorre o vetor de inimigos desenhando cada um deles no mapa
(considera-se que o jogador fornecido nunca ultrapassará as bordas,
pois essa condição ja é checada na função que movimenta o jogador)*/

tMapa DesenhaInimigoNoMapa(tMapa mapa, tInimigo inimigo);
/*Desenha um inimigo no mapa indiviodualmente*/

tMapa InicializaInimigosNoMapa(tMapa mapa, char diretorio[]);
/*Percorre o vetor inimigos inicializanodo cada um deles*/

void ImprimeMapa(tMapa mapa);
/*Imprime o mapa*/



typedef struct {
    tMapa mapa;
}tJogo;

tJogo InicializaJogo(char diretorio[]);



tJogo jogo;

int main(int argc, char* argv[]) {
    //Checa se algum diretorio foi passado
    if (argc < 2) {
        printf("[ERRO] Informe o diretorio com os arquivos de configuracao.\n");
        exit(1);
    }

    //"lendo" o diretorio
    char diretorio[TAM_MAX_CAMINHO];
    strcpy(diretorio, argv[1]);

    jogo = InicializaJogo(diretorio);

    ImprimeMapa(jogo.mapa);

    return 0;
}



tJogo InicializaJogo(char diretorio[]) {
    tJogo jogo;
    jogo.mapa = InicializaMapa(diretorio);

    return jogo;
}



tMapa InicializaMapa(char diretorio[]) {
    tMapa mapa;

    //cria o caminho até o arquivo de configuração
    strcpy(mapa.arquivo, diretorio);
    strcat(mapa.arquivo, "/mapa.txt");

    //abrindo o arquivo de configuração do mapa no modo leitura
    mapa.config = fopen(mapa.arquivo, "r");
    //checa se foi possivel abrir o arquivo no diretorio indicado
    if (mapa.config == NULL) {
        printf("[ERRO] O arquivo \"%s\" nao foi encontrado.\n", mapa.arquivo);
        exit(1);
    }

    fscanf(mapa.config, "%d %d\n", &mapa.largura, &mapa.altura);

    mapa.jogador = InicializaJogador(mapa.config);

    mapa.alturaMaximaInimigo = LinhaJogador(mapa.jogador) - 2;

    mapa = InicializaGridMapa(mapa);

    mapa = InicializaInimigosNoMapa(mapa, diretorio);

    mapa = AtualizaMapa(mapa);

    GeraArquivoInicializacao(mapa, diretorio);

    //fechando o arquivo de configuração do mapa
    fclose(mapa.config);
    return mapa;
}

void GeraArquivoInicializacao(tMapa mapa, char diretorio[]) {
    char diretorioSaida[TAM_MAX_CAMINHO];
    FILE* arquivoInicializacao;

    strcpy(diretorioSaida, diretorio);
    strcat(diretorioSaida, "/saida/inicializacao.txt");
    arquivoInicializacao = fopen(diretorioSaida, "w");

    //Desenha o mapa em "inicializacao.txt"

    for (int i = 0; i < mapa.altura + 2; i++) {
        for (int j = 0; j < mapa.largura + 2; j++) {
            fprintf(arquivoInicializacao, "%c", mapa.grid[i][j]);
        }
        fprintf(arquivoInicializacao, "\n");
    }

    fprintf(arquivoInicializacao, "A posicao central do jogador iniciara em (%d %d).",
        ColunaJogador(mapa.jogador), LinhaJogador(mapa.jogador));

    fclose(arquivoInicializacao);
}

tMapa InicializaGridMapa(tMapa mapa) {
    for (int i = 0; i < mapa.altura + 2; i++) {
        for (int j = 0; j < mapa.largura + 2; j++) {
            if ((i == 0 || i > mapa.altura) && (j == 0 || j > mapa.largura)) {
                mapa.gridVazio[i][j] = '+';
            } else if (i == 0 || i > mapa.altura) {
                mapa.gridVazio[i][j] = '-';
            } else if (j == 0 || j > mapa.largura) {
                if (i == mapa.alturaMaximaInimigo) {
                    mapa.gridVazio[i][j] = '-';
                } else {
                    mapa.gridVazio[i][j] = '|';
                }
            } else {
                mapa.gridVazio[i][j] = ' ';
            }
        }
    }
    return mapa;
}

void ImprimeMapa(tMapa mapa) {
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

tMapa DesenhaJogadorNoMapa(tMapa mapa, tJogador jogador) {

    //laço que percorre todas as posições do desenho do jogador
    int k = -1;
    for (int i = 0; i < TAM_JOGADOR; i++) {
        int l = -1;
        for (int j = 0; j < TAM_JOGADOR; j++) {
            mapa.grid[jogador.y + k][jogador.x + l] = jogador.desenho[i][j];
            l++;
        }
        k++;
    }
    return mapa;
}

tMapa DesenhaInimigosNoMapa(tMapa mapa) {
    for (int i = 0; i < mapa.qtdInimigos; i++) {
        mapa = DesenhaInimigoNoMapa(mapa, mapa.inimigos[i]);
    }
    return mapa;
}

tMapa DesenhaInimigoNoMapa(tMapa mapa, tInimigo inimigo) {

    //laço que percorre todas as posições do desenho do inimigo
    int k = -1;
    int frame = 0;
    for (int i = 0; i < TAM_JOGADOR; i++) {
        int l = -1;
        for (int j = 0; j < TAM_JOGADOR; j++) {
            mapa.grid[inimigo.y + k][inimigo.x + l] = inimigo.frames[frame][i][j];
            l++;
        }
        k++;
    }
    return mapa;
}

tMapa AtualizaMapa(tMapa mapa) {
    mapa = DesenhaGridVazioNoMapa(mapa);

    mapa = DesenhaJogadorNoMapa(mapa, mapa.jogador);

    mapa = DesenhaInimigosNoMapa(mapa);

    return mapa;
}

tMapa InicializaInimigosNoMapa(tMapa mapa, char diretorio[]) {
    int inimigoAtual = 0;
    mapa.qtdInimigos = 0;

    for (int i = 0; i < QTD_FILEIRAS; i++) {
        char lixo;
        int indice = 1;

        while (TRUE) {
            fscanf(mapa.config, "%c", &lixo);
            if (lixo == ' ') {
                //vai ler o proximo inimigo na fileira
                continue;
            } else if (lixo == '(') {
                mapa.inimigos[inimigoAtual] = InicializaInimigo(mapa.config, diretorio, i + 1, indice);
                fscanf(mapa.config, ")");
                indice++;
                mapa.qtdInimigos++;
                inimigoAtual++;
            } else {
                //terminou a fileira
                break;
            }
        }
    }

    return mapa;
}



tJogador InicializaJogador(FILE* config) {
    tJogador jogador;

    fscanf(config, "(%d %d)\n", &jogador.x, &jogador.y);

    jogador.s = 'M';

    //laço que cria o desenho do jogador
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

int LinhaJogador(tJogador jogador) {
    return jogador.y;
}

int ColunaJogador(tJogador jogador) {
    return jogador.x;
}



tInimigo InicializaInimigo(FILE* configMapa, char diretorio[], int fileira, int indice) {
    tInimigo inimigo;

    strcpy(inimigo.arquivo, diretorio);
    strcat(inimigo.arquivo, "/inimigo.txt");

    inimigo.estaVivo = TRUE;

    fscanf(configMapa, "%d %d", &inimigo.x, &inimigo.y);

    inimigo = LeFramesInimigo(inimigo);

    inimigo.fileira = fileira;
    inimigo.indice = indice;

    return inimigo;
}

tInimigo LeFramesInimigo(tInimigo inimigo) {
    inimigo.config = fopen(inimigo.arquivo, "r");
    if (inimigo.config == NULL) {
        printf("[ERRO] O arquivo \"%s\" nao foi encontrado.\n", inimigo.arquivo);
        exit(1);
    }

    fscanf(inimigo.config, "%d", &inimigo.animado);
    fscanf(inimigo.config, "%*c");//apaga o \n

    for (int i = 0; i < QTD_FRAMES; i++) {
        for (int j = 0; j < TAM_INIMIGO; j++) {
            for (int k = 0; k < TAM_INIMIGO; k++) {
                fscanf(inimigo.config, "%c", &inimigo.frames[i][j][k]);
            }
            fscanf(inimigo.config, "%*c"); //"apaga" o \n
        }
    }

    // for (int i = 0; i < TAM_INIMIGO; i++) {
    //     for (int j = 0; j < TAM_INIMIGO; j++) {
    //         printf("'%c'", inimigo.frames[0][i][j]);
    //     }
    //     printf("\n");
    // }

    fclose(inimigo.config);
    return inimigo;
}
