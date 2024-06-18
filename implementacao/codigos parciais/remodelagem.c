#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MODO_DEBUG          0

#define TRUE                1
#define FALSE               0

#define TAM_MAX_CAMINHO     1001    //1000 caracteres + o \0

#define LARGURA_MAX_MAPA    102     //100 + bordas
#define ALTURA_MAX_MAPA     42      //40 + bordas
#define QTD_FILEIRAS        3
#define MAX_INIMIGOS        100     //Qtd maxima de inimigos possiveis, tendo 3 fileiras e largura maxima 100

#define MOV_ESQUERDA        'a'
#define MOV_DIREITA         'd'
#define ATIRAR              ' '
#define PASSAR_A_VEZ        's'

#define TAM_JOGADOR         3

#define TAM_INIMIGO         3
#define QTD_FRAMES          3





typedef struct {
    char s;

    int x;
    int y;

    int estaAtivo;
}tTiro;





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

/*Inicializa cada inimigo no vetor de inimigos e retorna a quantidade de inimigos*/
int InicializaInimigos(tInimigo inimigos[], FILE* mapa_txt, char diretorio[]);

/*Gera as informações referentes a cada inimgo*/
tInimigo InicializaInimigo(int x, int y, char diretorio[], int fileira, int indice);

/*Le os frames da animação no arquivo "inimigo.txt"*/
tInimigo LeFramesInimigo(tInimigo inimigo);





typedef struct {
    int x;
    int y;

    char s; //carcater que representa o jogador;
    char desenho[TAM_JOGADOR][TAM_JOGADOR];
}tJogador;

/*Inicializa o jogador (desenho, simbolo e etc) a partir de um arquivo de configuração*/
tJogador InicializaJogador(FILE* config);

/*Retorna a linha do jogador*/
int LinhaJogador(tJogador jogador);

/*Retorna a coluna do jogador*/
int ColunaJogador(tJogador jogador);

tJogador MoveJogador(int larguraMapa, tJogador jogador, char movimento);




typedef struct {
    int largura;
    int altura;
    char grid[ALTURA_MAX_MAPA][LARGURA_MAX_MAPA];
}tGrid;

/*Cria o desenho do mapa*/
tGrid InicializaMapa(int largura, int altura, int alturaMaxInimigo);

void ImprimeTela(tGrid tela);

/*Não são checados casos de sobreposição nem colisões.
Considera-se que essas verificações são feitas na função de movimentar o jogador*/
tGrid DesenhaJogadorNaTela(tGrid tela, tJogador jogador);

tGrid DesenhaInimigosNaTela(tGrid tela, tInimigo inimigos[], int qtdInimigos, int iteracao);

tGrid DesenhaMapaNaTela(tGrid tela, tGrid mapa);

int Largura(tGrid grid);





typedef struct {
    tGrid mapa;
    tGrid tela;

    tJogador jogador;

    int qtdInimigos;
    int AlturaMaxInimigos;
    tInimigo inimigos[MAX_INIMIGOS];

    tTiro tiro;

    int iteracao;
    int pontos;
}tJogo;

/*Faz as devidas inicializações e gera o codigo*/
tJogo InicializaJogo(char diretorio[]);

tJogo AtualizaTela(tJogo jogo);

void GeraArquivoInicializacao(tGrid tela, tJogador jogador, char diretorio[]);

tJogo RealizaJogo(tJogo jogo);

tJogo RealizaJogada(tJogo jogo, char jogada);



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

    jogo = RealizaJogo(jogo);

    return 0;
}





tJogo InicializaJogo(char diretorio[]) {
    tJogo jogo;

    jogo.iteracao = 0;
    jogo.pontos = 0;

    char caminhoMapa[TAM_MAX_CAMINHO];
    FILE* mapa_txt;

    //cria o caminho até o arquivo de configuração
    strcpy(caminhoMapa, diretorio);
    strcat(caminhoMapa, "/mapa.txt");

    //abrindo o arquivo de configuração do mapa no modo leitura
    mapa_txt = fopen(caminhoMapa, "r");
    //checa se foi possivel abrir o arquivo no diretorio indicado
    if (mapa_txt == NULL) {
        printf("[ERRO] O arquivo \"%s\" nao foi encontrado.\n", caminhoMapa);
        exit(1);
    }


    int larguraMapa;
    int alturaMapa;

    fscanf(mapa_txt, "%d %d\n", &larguraMapa, &alturaMapa);
    if (MODO_DEBUG) printf("->Largua e altura do mapa: %d %d.\n", larguraMapa, alturaMapa);

    jogo.jogador = InicializaJogador(mapa_txt);
    if (MODO_DEBUG) printf("->Jogador inicializado.\n");

    jogo.AlturaMaxInimigos = LinhaJogador(jogo.jogador) - 2;

    jogo.mapa = InicializaMapa(larguraMapa, alturaMapa, jogo.AlturaMaxInimigos);
    if (MODO_DEBUG) printf("->Mapa inicializado.\n");

    //Inicialmente  não tem problema serem iguais. É mais conveniente do que criar outra função
    jogo.tela = jogo.mapa;
    if (MODO_DEBUG) printf("->Tela Inicializada.\n");


    jogo.qtdInimigos = InicializaInimigos(jogo.inimigos, mapa_txt, diretorio);
    if (MODO_DEBUG) printf("->Todos inmigos inicializados.\n");
    //    jogo.tiro = InicializaTiro();

    jogo = AtualizaTela(jogo);
    if (MODO_DEBUG) printf("->Tela Atualizada\n");

    GeraArquivoInicializacao(jogo.tela, jogo.jogador, diretorio);

    fclose(mapa_txt);
    return jogo;
}

tJogo AtualizaTela(tJogo jogo) {
    jogo.tela = DesenhaMapaNaTela(jogo.tela, jogo.mapa);

    jogo.tela = DesenhaJogadorNaTela(jogo.tela, jogo.jogador);

    jogo.tela = DesenhaInimigosNaTela(jogo.tela, jogo.inimigos, jogo.qtdInimigos, jogo.iteracao);

    return jogo;
}

void GeraArquivoInicializacao(tGrid tela, tJogador jogador, char diretorio[]) {
    char diretorioSaida[TAM_MAX_CAMINHO];
    FILE* arquivoInicializacao;

    strcpy(diretorioSaida, diretorio);
    strcat(diretorioSaida, "/saida/inicializacao.txt");
    arquivoInicializacao = fopen(diretorioSaida, "w");

    if (MODO_DEBUG) printf("->inicializacao.txt criado.\n");
    //Desenha a tela em "inicializacao.txt"

    for (int i = 0; i < tela.altura + 2; i++) {
        for (int j = 0; j < tela.largura + 2; j++) {
            fprintf(arquivoInicializacao, "%c", tela.grid[i][j]);
        }
        fprintf(arquivoInicializacao, "\n");
    }

    fprintf(arquivoInicializacao, "A posicao central do jogador iniciara em (%d %d).",
        jogador.x, jogador.y);

    fclose(arquivoInicializacao);
}

tJogo RealizaJogo(tJogo jogo) {
    /*
        #Verifica condiçõesa de vitoria/derrota;
        #Verifica colisao do tiro com o inimigo;
        #Move inimigos;
        #Move tiro (caso atinja a borda, desabilita-lo);
        #incrementar contador de iterações (inicia em 0);
        #Ler joagda do usuário, (w a s d ou ' '), se o jogador for ultrapassar a borda ele não deve ser movido
            e só deve ser efetuado um novo disparo caso não haja tiros ativos no mapa.
        #imprimir na saída padrão os pontos, as iterações e o mapa;
    */
    char jogada;

    while (TRUE) {
        system("clear");
        jogo = AtualizaTela(jogo);
        printf("Pontos: %d | Iteracoes: %d\n", jogo.pontos, jogo.iteracao);
        ImprimeTela(jogo.tela);

        //verifica vitoria/derrota

        //verifica morte de inimigos

        //move inimigos

        //move tiro

        jogo.iteracao++;

        scanf("%c", &jogada);
        scanf("%*c");

        jogo = RealizaJogada(jogo, jogada);
    }

    return jogo;
}

tJogo RealizaJogada(tJogo jogo, char jogada) {
    if (jogada == MOV_ESQUERDA || jogada == MOV_DIREITA || jogada == PASSAR_A_VEZ) {
        jogo.jogador = MoveJogador(Largura(jogo.mapa), jogo.jogador, jogada);
    } else if (jogada == ATIRAR) {
        //jogo.tiro = EfetuaTiro(jogo.jogador, jogo.tiro);
    } else {
        printf("[ERRO] Jogada nao definida. Suposta jogada: '%c'.\n", jogada);
        exit(1);
    }

    return jogo;
}




tJogador InicializaJogador(FILE* config) {
    tJogador jogador;

    fscanf(config, "(%d %d)\n", &jogador.x, &jogador.y);
    if (MODO_DEBUG) printf("->Posicao incial do jogador: (%d %d)\n", jogador.x, jogador.y);

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

    if (MODO_DEBUG) {
        printf("->Desenho do Jogador:\n");

        for (int i = 0; i < TAM_JOGADOR; i++) {
            printf("\t");
            for (int j = 0; j < TAM_JOGADOR; j++) {
                printf("%c", jogador.desenho[i][j]);
            }
            printf("\n");
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

tJogador MoveJogador(int larguraMapa, tJogador jogador, char movimento) {
    if (movimento == MOV_ESQUERDA) {
        if (jogador.x - 2 > 0) {
            jogador.x--;
        } else {
        }
    } else if (movimento == MOV_DIREITA) {
        if (jogador.x + 2 < larguraMapa) {
            jogador.x++;
        } else {
        }
    } else if (movimento == PASSAR_A_VEZ) {
        printf("Passou a vez! \n");
    } else {
        printf("[ERRO NAO IDENTIFICADO] Em MoveJogador(). Suposto movimento: %c", movimento);
        exit(1);
    }
    return jogador;
}





tGrid InicializaMapa(int largura, int altura, int alturaMaxInimigo) {
    tGrid mapa;

    mapa.largura = largura;
    mapa.altura = altura;

    for (int i = 0; i < mapa.altura + 2; i++) {
        for (int j = 0; j < mapa.largura + 2; j++) {
            if ((i == 0 || i == mapa.altura + 1) && (j == 0 || j == mapa.largura + 1)) {
                mapa.grid[i][j] = '+';
            } else if (i == 0 || i == mapa.altura + 1) {
                mapa.grid[i][j] = '-';
            } else if (j == 0 || j == mapa.largura + 1) {
                if (i == alturaMaxInimigo) {
                    mapa.grid[i][j] = '-';
                } else {
                    mapa.grid[i][j] = '|';
                }
            } else {
                mapa.grid[i][j] = ' ';
            }
        }
    }

    return mapa;
}

tGrid DesenhaJogadorNaTela(tGrid tela, tJogador jogador) {
    //laço que percorre todas as posições do desenho do jogador
    int k = -1;
    for (int i = 0; i < TAM_JOGADOR; i++) {
        int l = -1;
        for (int j = 0; j < TAM_JOGADOR; j++) {
            tela.grid[jogador.y + k][jogador.x + l] = jogador.desenho[i][j];
            l++;
        }
        k++;
    }

    if (MODO_DEBUG) printf("->Jogador desenhado no grid da tela.\n");

    return tela;
}

void ImprimeTela(tGrid tela) {
    for (int i = 0; i < tela.altura + 2; i++) {
        for (int j = 0; j < tela.largura + 2; j++) {
            printf("%c", tela.grid[i][j]);
        }
        printf("\n");
    }
}

tGrid DesenhaInimigosNaTela(tGrid tela, tInimigo inimigos[], int qtdInimigos, int iteracao) {
    //laço que percorre todos os inimigos do vetor
    for (int x = 0; x < qtdInimigos; x++) {
        if (inimigos[x].estaVivo) {
            //laço que percorre todas as posições do desenho do inimigo
            int k = -1;
            int frame;

            if (inimigos[x].animado) {
                frame = iteracao % QTD_FRAMES;
            } else {
                frame = 0;
            }

            for (int i = 0; i < TAM_INIMIGO; i++) {
                int l = -1;
                for (int j = 0; j < TAM_INIMIGO; j++) {
                    tela.grid[inimigos[x].y + k][inimigos[x].x + l] = inimigos[x].frames[frame][i][j];
                    l++;
                }
                k++;
            }
            if (MODO_DEBUG) {
                printf("->Inimigo %d desenhado no grid da tela:\n", x);
                ImprimeTela(tela);
            }
        }
    }

    return tela;
}

tGrid DesenhaMapaNaTela(tGrid tela, tGrid mapa) {

    for (int i = 0; i < tela.altura + 2; i++) {
        for (int j = 0; j < tela.largura + 2; j++) {
            tela.grid[i][j] = mapa.grid[i][j];
        }
    }

    return tela;
}

int Largura(tGrid grid) {
    return grid.largura;
}





int InicializaInimigos(tInimigo inimigos[], FILE* mapa_txt, char diretorio[]) {
    int inimigoAtual = 0;
    int qtdInimigos = 0;

    for (int i = 0; i < QTD_FILEIRAS; i++) {
        char lixo;
        int indice = 1;

        while (TRUE) {
            fscanf(mapa_txt, "%c", &lixo);
            if (lixo == ' ') {
                //vai ler o proximo inimigo na fileira
                continue;
            } else if (lixo == '(') {
                int x, y;
                fscanf(mapa_txt, "%d %d)", &x, &y);

                inimigos[inimigoAtual] = InicializaInimigo(x, y, diretorio, i + 1, indice);
                indice++;
                qtdInimigos++;
                inimigoAtual++;
            } else {
                //terminou a fileira
                break;
            }
        }
    }

    return qtdInimigos;
}

tInimigo InicializaInimigo(int x, int y, char diretorio[], int fileira, int indice) {
    tInimigo inimigo;

    strcpy(inimigo.arquivo, diretorio);
    strcat(inimigo.arquivo, "/inimigo.txt");

    inimigo.estaVivo = TRUE;

    inimigo.x = x;
    inimigo.y = y;

    if (MODO_DEBUG) printf("Lendo frames do inimigo.\n");
    inimigo = LeFramesInimigo(inimigo);

    inimigo.fileira = fileira;
    inimigo.indice = indice;

    if (MODO_DEBUG) {
        printf("->Inimigo inicializado:\n");
        printf("\tFileira: %d\n", inimigo.fileira);
        printf("\tIndice: %d\n", inimigo.indice);
        printf("\tPosicao: (%d %d)\n", inimigo.x, inimigo.y);
    }

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


    if (MODO_DEBUG) {
        printf("  Frame 0:\n");
        for (int i = 0; i < TAM_INIMIGO; i++) {
            printf("\t");
            for (int j = 0; j < TAM_INIMIGO; j++) {
                printf("%c", inimigo.frames[0][i][j]);
            }
            printf("\n");
        }
    }

    fclose(inimigo.config);
    return inimigo;
}