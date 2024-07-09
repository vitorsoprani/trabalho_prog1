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

#define MOV_ESQUERDA        'a'
#define MOV_DIREITA         'd'
#define ATIRAR              ' '
#define PASSAR_A_VEZ        's'

#define TAM_JOGADOR         3

#define TAM_INIMIGO         3
#define QTD_FRAMES          3

#define DIREITA             1
#define ESQUERDA            2
#define BAIXO               3





typedef struct {
    int largura;
    int altura;
    int grid[ALTURA_MAX_MAPA][LARGURA_MAX_MAPA];
}tGridInt;

tGridInt InicializaHeatMap(int largura, int altura);

void GeraHeatMap(tGridInt heatMap, char diretorio[]);





typedef struct {
    int largura;
    int altura;
    char grid[ALTURA_MAX_MAPA][LARGURA_MAX_MAPA];
}tGridChar;

/*Cria o desenho do mapa*/
tGridChar InicializaMapa(int largura, int altura, int alturaMaxInimigo);

tGridChar InicializaTela(int largura, int altura);

/*Função que desenha matrizes na tela, sendo o indice [0][0] da matriz localizado no ponto (x, y)*/
tGridChar DesenhaMatrizNaTela(tGridChar tela, int l, int c, char matriz[l][c], int x, int y);

/*Desenha o caracter fornecido no ponto indicado*/
tGridChar DesenhaCaracterNaTela(tGridChar tela, char c, int x, int y);

void ImprimeTela(tGridChar tela);





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

    int direcao;

    int numeroDeDescidas;

    int linhaMorte;
    int iteracaoMorte;
}tInimigo;

/*Inicializa cada inimigo no vetor de inimigos e retorna a quantidade de inimigos*/
int InicializaInimigos(tInimigo inimigos[], FILE* mapa_txt, char diretorio[]);

/*Gera as informações referentes a cada inimgo*/
tInimigo InicializaInimigo(int x, int y, char diretorio[], int fileira, int indice);

/*Le os frames da animação no arquivo "inimigo.txt"*/
tInimigo LeFramesInimigo(tInimigo inimigo);

/*Move todos os inimgos do vetor e muda a direção caso necessário*/
void MoveInimigos(tInimigo inimigos[], int qtdInimigos, tGridChar mapa, FILE* resumo, int iteracao);

/*Percorre o vetor de inimigos e retorna TRUE caso haja alguma colisão com a parede e FALSE caso contrario*/
int ChecaColisaoInimigosParede(tInimigo inimigos[], int qtdInimigos, tGridChar mapa, FILE* resumo, int iteracao);

int NumeroDeDescidas(tInimigo inimigo);

void OrdenaInimigos(tInimigo inimigos[], int qtdInimigos);

/*Retorna true caso o inimigo 1 deva aparecer primeiro que o inimigo 2 no ranking e false caso contrario*/
int AparecePrimeiroQue(tInimigo inimigo1, tInimigo inimigo2);

void GeraRanking(tInimigo inimigos[], int qtdInimigos, char diretorio[]);

/*Retorna o frame atual do inimigo de acordo com a anmimação*/
int FrameAtualInimigo(tInimigo inimigo, int iteracao);





typedef struct {
    int x;
    int y;

    char s; //carcater que representa o jogador;
    char desenho[TAM_JOGADOR][TAM_JOGADOR];

    int numeroDeMovimentos;
}tJogador;

/*Inicializa o jogador (desenho, simbolo e etc) a partir de um arquivo de configuração*/
tJogador InicializaJogador(FILE* config);

/*Retorna a linha do jogador*/
int LinhaJogador(tJogador jogador);

/*Retorna a coluna do jogador*/
int ColunaJogador(tJogador jogador);

tJogador MoveJogador(tGridChar mapa, tJogador jogador, char movimento, FILE* resumo, int iteracao);

int NumeroDeMovimentos(tJogador tJogador);





typedef struct {
    char s;

    int x;
    int y;

    int estaAtivo;

    int numeroDeTiros;
    int numeroDeTirosErrados;
}tTiro;

tTiro InicializaTiro();

tTiro EfetuaTiro(tJogador jogador, tTiro tiro);

tTiro MoveTiro(tTiro tiro);

int NumeroDeTiros(tTiro tiro);

int NumeroDeTirosErrados(tTiro tiro);





typedef struct {
    tGridChar mapa;
    tGridChar tela;
    tGridInt heatMap;

    tJogador jogador;

    int qtdInimigos;
    int alturaMaxInimigos;
    tInimigo inimigos[MAX_INIMIGOS];

    tTiro tiro;

    int iteracao;
    int pontos;
}tJogo;

/*Faz as devidas inicializações e gera o codigo*/
tJogo InicializaJogo(char diretorio[]);

tJogo DesenhaMapaNaTela(tJogo jogo, tGridChar tela, tGridChar mapa);

tJogo AtualizaHeatMap(tJogo jogo, tGridInt heatMap, tTiro tiro, tJogador jogador);

tJogo DesenhaTiroNaTela(tJogo jogo, tGridChar tela, tTiro tiro);

/*Não são checados casos de sobreposição nem colisões.
Considera-se que essas verificações são feitas na função de movimentar o jogador*/
tJogo DesenhaJogadorNaTela(tJogo jogo, tGridChar tela, tJogador jogador);

tJogo DesenhaInimigosNaTela(tJogo jogo, tGridChar tela, tInimigo inimigos[]);

tJogo AtualizaTela(tJogo jogo);

void GeraArquivoInicializacao(tGridChar tela, tJogador jogador, char diretorio[]);

tJogo RealizaJogo(tJogo jogo, char diretorio[]);

tJogo RealizaJogada(tJogo jogo, char jogada, FILE* resumo);

/*Percorre o vetor de inimigos checando se cada um deles colidiu com o tiro
e atualizando os status quando necessário*/
tJogo ColisaoInimigosTiro(tJogo jogo, tInimigo inimigos[], tTiro tiro, FILE* resumo);

/*Checa se um dado inimigo colidiu com um tiro*/
int ColidiuInimigoTiro(tInimigo inimigo, tTiro tiro);

int VerificaVitoria(tJogo jogo, tInimigo inimigos[]);

int VerificaDerrota(tJogo jogo, tInimigo inimigos[]);

void GeraEstatisticas(tJogo jogo, char diretorio[]);





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

    jogo = RealizaJogo(jogo, diretorio);

    GeraEstatisticas(jogo, diretorio);

    GeraHeatMap(jogo.heatMap, diretorio);

    OrdenaInimigos(jogo.inimigos, jogo.qtdInimigos);

    GeraRanking(jogo.inimigos, jogo.qtdInimigos, diretorio);

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

    jogo.jogador = InicializaJogador(mapa_txt);

    jogo.alturaMaxInimigos = LinhaJogador(jogo.jogador) - 2;

    jogo.mapa = InicializaMapa(larguraMapa, alturaMapa, jogo.alturaMaxInimigos);

    jogo.heatMap = InicializaHeatMap(larguraMapa, alturaMapa);

    jogo.tela = InicializaTela(larguraMapa + 2, alturaMapa + 2);


    jogo.qtdInimigos = InicializaInimigos(jogo.inimigos, mapa_txt, diretorio);
    //    jogo.tiro = InicializaTiro();

    jogo.tiro = InicializaTiro();

    jogo = AtualizaTela(jogo);

    GeraArquivoInicializacao(jogo.tela, jogo.jogador, diretorio);

    fclose(mapa_txt);
    return jogo;
}

tJogo DesenhaMapaNaTela(tJogo jogo, tGridChar tela, tGridChar mapa) {

    // for (int i = 0; i < mapa.altura + 2; i++) {
    //     for (int j = 0; j < mapa.largura + 2; j++) {
    //         jogo.tela.grid[i + 0][j + 0] = mapa.grid[i][j];
    //     }
    // }

    // jogo.tela = tela;

    jogo.tela = DesenhaMatrizNaTela(jogo.tela, (mapa.altura + 2), (mapa.largura + 2), mapa.grid, 0, 0);

    // for (int i = 0; i < l; i++) {
    //     for (int j = 0; j < c; j++) {
    //         tela.grid[i + y][j + x] = matriz[i][j];
    //     }
    // }
    // return tela;

    return jogo;
}

tJogo AtualizaHeatMap(tJogo jogo, tGridInt heatMap, tTiro tiro, tJogador jogador) {
    if (tiro.estaAtivo) {
        if (heatMap.grid[tiro.y - 1][tiro.x - 1] < 999) {
            heatMap.grid[tiro.y - 1][tiro.x - 1]++;
            //Tanto o jogador qunto o tiro devem ter uma "defasagem" de -1 nas coordenadas,
            //pois essas são referentes ao mapa (considerando as molduras) e não ao heatmap (que não tem molduras) 
        }
    }

    //Laço que percorre todas as posições do jogador (em relação ao centro)
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (heatMap.grid[jogador.y + i - 1][jogador.x + j - 1] < 999) {
                heatMap.grid[jogador.y + i - 1][jogador.x + j - 1]++;
            }
        }
    }

    jogo.heatMap = heatMap;
    return jogo;
}

tJogo AtualizaTela(tJogo jogo) {
    jogo = DesenhaMapaNaTela(jogo, jogo.tela, jogo.mapa);

    jogo = DesenhaJogadorNaTela(jogo, jogo.tela, jogo.jogador);

    jogo = DesenhaInimigosNaTela(jogo, jogo.tela, jogo.inimigos);

    jogo = DesenhaTiroNaTela(jogo, jogo.tela, jogo.tiro);

    return jogo;
}

tJogo DesenhaTiroNaTela(tJogo jogo, tGridChar tela, tTiro tiro) {
    if (tiro.estaAtivo) {
        jogo.tela = DesenhaCaracterNaTela(jogo.tela, tiro.s, tiro.x, tiro.y);
    }

    return jogo;
}

tJogo DesenhaJogadorNaTela(tJogo jogo, tGridChar tela, tJogador jogador) {
    jogo.tela = DesenhaMatrizNaTela(jogo.tela, TAM_JOGADOR, TAM_JOGADOR, jogador.desenho, jogador.x - 1, jogador.y - 1);

    return jogo;
}

tJogo DesenhaInimigosNaTela(tJogo jogo, tGridChar tela, tInimigo inimigos[]) {
    //laço que percorre todos os inimigos do vetor
    for (int i = 0; i < jogo.qtdInimigos; i++) {
        if (inimigos[i].estaVivo) {
            jogo.tela = DesenhaMatrizNaTela(jogo.tela,
                TAM_INIMIGO,
                TAM_INIMIGO,
                inimigos[i].frames[FrameAtualInimigo(inimigos[i], jogo.iteracao)],
                inimigos[i].x - 1,
                inimigos[i].y - 1);
        }
    }

    return jogo;
}

void GeraArquivoInicializacao(tGridChar tela, tJogador jogador, char diretorio[]) {
    char diretorioSaida[TAM_MAX_CAMINHO];
    FILE* arquivoInicializacao;

    strcpy(diretorioSaida, diretorio);
    strcat(diretorioSaida, "/saida/inicializacao.txt");
    arquivoInicializacao = fopen(diretorioSaida, "w");

    //Desenha a tela em "inicializacao.txt"

    for (int i = 0; i < tela.altura; i++) {
        for (int j = 0; j < tela.largura; j++) {
            fprintf(arquivoInicializacao, "%c", tela.grid[i][j]);
        }
        fprintf(arquivoInicializacao, "\n");
    }

    fprintf(arquivoInicializacao, "A posicao central do jogador iniciara em (%d %d).",
        jogador.x, jogador.y);

    fclose(arquivoInicializacao);
}

tJogo RealizaJogo(tJogo jogo, char diretorio[]) {
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
    char diretorioSaida[TAM_MAX_CAMINHO];
    FILE* arquivoResumo;

    strcpy(diretorioSaida, diretorio);
    strcat(diretorioSaida, "/saida/resumo.txt");
    arquivoResumo = fopen(diretorioSaida, "w");

    char jogada;

    while (TRUE) {
        //system("clear");
        jogo = AtualizaTela(jogo);
        jogo = AtualizaHeatMap(jogo, jogo.heatMap, jogo.tiro, jogo.jogador);
        printf("Pontos: %d | Iteracoes: %d\n", jogo.pontos, jogo.iteracao);
        ImprimeTela(jogo.tela);

        if (VerificaDerrota(jogo, jogo.inimigos)) {
            printf("Você perdeu, tente novamente!");
            break;
        }
        if (VerificaVitoria(jogo, jogo.inimigos)) {
            printf("Parabéns, você ganhou!");
            break;
        }

        jogo = ColisaoInimigosTiro(jogo, jogo.inimigos, jogo.tiro, arquivoResumo);

        MoveInimigos(jogo.inimigos, jogo.qtdInimigos, jogo.mapa, arquivoResumo, jogo.iteracao);

        jogo.tiro = MoveTiro(jogo.tiro);

        jogo.iteracao++;

        scanf("%c", &jogada);
        scanf("%*c");

        jogo = RealizaJogada(jogo, jogada, arquivoResumo);
    }

    fclose(arquivoResumo);
    return jogo;
}

tJogo RealizaJogada(tJogo jogo, char jogada, FILE* resumo) {
    if (jogada == MOV_ESQUERDA || jogada == MOV_DIREITA || jogada == PASSAR_A_VEZ) {
        jogo.jogador = MoveJogador(jogo.mapa, jogo.jogador, jogada, resumo, jogo.iteracao);
    } else if (jogada == ATIRAR) {
        jogo.tiro = EfetuaTiro(jogo.jogador, jogo.tiro);
    } else {
        printf("[ERRO] Jogada nao definida.\n");
        exit(1);
    }

    return jogo;
}

tJogo ColisaoInimigosTiro(tJogo jogo, tInimigo inimigos[], tTiro tiro, FILE* resumo) {
    for (int i = 0; i < jogo.qtdInimigos; i++) {
        if (ColidiuInimigoTiro(inimigos[i], tiro)) {
            tiro.estaAtivo = FALSE;
            inimigos[i].estaVivo = FALSE;
            inimigos[i].iteracaoMorte = jogo.iteracao;
            inimigos[i].linhaMorte = jogo.mapa.altura - tiro.y + 1;
            fprintf(resumo, "[%d] Inimigo de indice %d da fileira %d foi atingido na posicao (%d %d).\n",
                jogo.iteracao, inimigos[i].indice, inimigos[i].fileira, tiro.x, tiro.y);
            jogo.tiro = tiro;
            jogo.inimigos[i] = inimigos[i];
            jogo.pontos += inimigos[i].x * (jogo.mapa.altura - inimigos[i].y); //CONSERTAR ACESSO INDEVIDO 
        }
    }

    return jogo;
}

int ColidiuInimigoTiro(tInimigo inimigo, tTiro tiro) {
    if (tiro.estaAtivo == FALSE) return FALSE;
    if (inimigo.estaVivo == FALSE) return  FALSE;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1;j <= 1; j++) {
            if (inimigo.y + i == tiro.y && inimigo.x + j == tiro.x) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

int VerificaVitoria(tJogo jogo, tInimigo inimigos[]) {
    for (int i = 0; i < jogo.qtdInimigos; i++) {
        if (inimigos[i].estaVivo == TRUE) return FALSE;
    }

    return TRUE;
}

int VerificaDerrota(tJogo jogo, tInimigo inimigos[]) {
    for (int i = 0; i < jogo.qtdInimigos; i++) {
        if (inimigos[i].y >= jogo.alturaMaxInimigos && inimigos[i].estaVivo) return TRUE;
    }

    return FALSE;
}

void GeraEstatisticas(tJogo jogo, char diretorio[]) {
    /*
    Numero total de movimentos (A ou D): 34;
    Numero de tiros efetivos: 3;
    Numero de tiros que nao acertaram: 0;
    Numero de descidas dos inimigos: 2;
    */

    char diretorioSaida[TAM_MAX_CAMINHO];
    FILE* arquivoEstatisticas;

    strcpy(diretorioSaida, diretorio);
    strcat(diretorioSaida, "/saida/estatisticas.txt");
    arquivoEstatisticas = fopen(diretorioSaida, "w");

    fprintf(arquivoEstatisticas, "Numero total de movimentos (A ou D): %d;\n", NumeroDeMovimentos(jogo.jogador));
    fprintf(arquivoEstatisticas, "Numero de tiros efetivos: %d;\n", NumeroDeTiros(jogo.tiro));
    fprintf(arquivoEstatisticas, "Numero de tiros que nao acertaram: %d;\n", NumeroDeTirosErrados(jogo.tiro));
    fprintf(arquivoEstatisticas, "Numero de descidas dos inimigos: %d;\n", NumeroDeDescidas(jogo.inimigos[0]));


    fclose(arquivoEstatisticas);
}





tJogador InicializaJogador(FILE* config) {
    tJogador jogador;

    fscanf(config, "(%d %d)%*c", &jogador.x, &jogador.y);

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


    jogador.numeroDeMovimentos = 0;

    return jogador;
}

int LinhaJogador(tJogador jogador) {
    return jogador.y;
}

int ColunaJogador(tJogador jogador) {
    return jogador.x;
}

tJogador MoveJogador(tGridChar mapa, tJogador jogador, char movimento, FILE* resumo, int iteracao) {
    if (movimento == MOV_ESQUERDA) {
        if (jogador.x - 2 > 0) {
            jogador.x--;
            jogador.numeroDeMovimentos++;
        } else {
            fprintf(resumo, "[%d] Jogador colidiu na lateral esquerda.\n", iteracao);
        }
    } else if (movimento == MOV_DIREITA) {
        if (jogador.x + 2 < mapa.largura + 1) {
            jogador.x++;
            jogador.numeroDeMovimentos++;
        } else {
            fprintf(resumo, "[%d] Jogador colidiu na lateral direita.\n", iteracao);

        }
    } else if (movimento == PASSAR_A_VEZ) {
        //printf("Passou a vez! \n");
    } else {
        printf("[ERRO NAO IDENTIFICADO] Em MoveJogador(). Suposto movimento: %c", movimento);
        exit(1);
    }
    return jogador;
}

int NumeroDeMovimentos(tJogador tJogador) {
    return tJogador.numeroDeMovimentos;
}





tGridChar InicializaMapa(int largura, int altura, int alturaMaxInimigo) {
    tGridChar mapa;

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

tGridChar InicializaTela(int largura, int altura) {
    tGridChar tela;
    tela.altura = altura;
    tela.largura = largura;
    return tela;
}

tGridChar DesenhaMatrizNaTela(tGridChar tela, int l, int c, char matriz[l][c], int x, int y) {
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++) {
            tela.grid[i + y][j + x] = matriz[i][j];
        }
    }
    return tela;
}

tGridChar DesenhaCaracterNaTela(tGridChar tela, char c, int x, int y) {
    tela.grid[y][x] = c;
    return tela;
}

void ImprimeTela(tGridChar tela) {
    for (int i = 0; i < tela.altura; i++) {
        for (int j = 0; j < tela.largura; j++) {
            printf("%c", tela.grid[i][j]);
        }
        printf("\n");
    }
}





tGridInt InicializaHeatMap(int largura, int altura) {
    tGridInt heatMap;
    heatMap.largura = largura;
    heatMap.altura = altura;

    for (int i = 0; i < heatMap.altura; i++) {
        for (int j = 0; j < heatMap.largura; j++) {
            heatMap.grid[i][j] = 0;
        }
    }

    return heatMap;
}

void GeraHeatMap(tGridInt heatMap, char diretorio[]) {

    char diretorioSaida[TAM_MAX_CAMINHO];
    FILE* arquivoHeatMap;

    strcpy(diretorioSaida, diretorio);
    strcat(diretorioSaida, "/saida/heatmap.txt");
    arquivoHeatMap = fopen(diretorioSaida, "w");

    for (int i = 0; i < heatMap.altura; i++) {
        for (int j = 0; j < heatMap.largura; j++) {
            fprintf(arquivoHeatMap, "%3d ", heatMap.grid[i][j]);
        }
        fprintf(arquivoHeatMap, "\n");
    }
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
    inimigo.direcao = DIREITA;
    inimigo.iteracaoMorte = 0;
    inimigo.linhaMorte = 0;

    inimigo.x = x;
    inimigo.y = y;

    inimigo.numeroDeDescidas = 0;

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



    fclose(inimigo.config);
    return inimigo;
}

void MoveInimigos(tInimigo inimigos[], int qtdInimigos, tGridChar mapa, FILE* resumo, int iteracao) {
    for (int i = 0; i < qtdInimigos; i++) {
        if (inimigos[i].direcao == DIREITA) {
            inimigos[i].x++;
        } else if (inimigos[i].direcao == ESQUERDA) {
            inimigos[i].x--;
        }
    }
    if (ChecaColisaoInimigosParede(inimigos, qtdInimigos, mapa, resumo, iteracao) == TRUE) {
        for (int i = 0; i < qtdInimigos; i++) {
            if (inimigos[i].direcao == DIREITA) {
                inimigos[i].direcao = ESQUERDA;
                inimigos[i].x--;
            } else if (inimigos[i].direcao == ESQUERDA) {
                inimigos[i].direcao = DIREITA;
                inimigos[i].x++;
            }
            inimigos[i].y++;
            inimigos[i].numeroDeDescidas++;
        }
    }
}

int ChecaColisaoInimigosParede(tInimigo inimigos[], int qtdInimigos, tGridChar mapa, FILE* resumo, int iteracao) {
    int colisao = FALSE;

    for (int i = 0; i < qtdInimigos; i++) {
        if (inimigos[i].x - 1 == 0 && inimigos[i].estaVivo) {
            fprintf(resumo, "[%d] Inimigo de indice %d da fileira %d colidiu na lateral esquerda.\n",
                iteracao, inimigos[i].indice, inimigos[i].fileira);
            colisao = TRUE;
        } else if (inimigos[i].x + 1 > mapa.largura && inimigos[i].estaVivo) {
            fprintf(resumo, "[%d] Inimigo de indice %d da fileira %d colidiu na lateral direita.\n",
                iteracao, inimigos[i].indice, inimigos[i].fileira);
            colisao = TRUE;
        }
    }
    return colisao;
}

int NumeroDeDescidas(tInimigo inimigo) {
    return inimigo.numeroDeDescidas;
}

void OrdenaInimigos(tInimigo inimigos[], int qtdInimigos) {
    for (int i = 0; i < qtdInimigos - 1; i++) {
        int idxMenorRelativo;//menor valor do vetor depoir de i+1
        idxMenorRelativo = i + 1;

        //encontra o menor valor do vetor (sem contar o i)
        for (int j = i + 2; j < qtdInimigos; j++) {
            if (AparecePrimeiroQue(inimigos[j], inimigos[idxMenorRelativo])) {
                idxMenorRelativo = j;
            }
        }
        if (AparecePrimeiroQue(inimigos[idxMenorRelativo], inimigos[i])) {
            //troca de lugar
            tInimigo aux;
            aux = inimigos[idxMenorRelativo];
            inimigos[idxMenorRelativo] = inimigos[i];
            inimigos[i] = aux;
        }
    }
}

int AparecePrimeiroQue(tInimigo inimigo1, tInimigo inimigo2) {
    if (inimigo1.linhaMorte == inimigo2.linhaMorte) {
        return inimigo1.iteracaoMorte < inimigo2.iteracaoMorte;
    }

    return inimigo1.linhaMorte < inimigo2.linhaMorte;
}

void GeraRanking(tInimigo inimigos[], int qtdInimigos, char diretorio[]) {
    char diretorioSaida[TAM_MAX_CAMINHO];
    FILE* arquivoRanking;

    strcpy(diretorioSaida, diretorio);
    strcat(diretorioSaida, "/saida/ranking.txt");
    arquivoRanking = fopen(diretorioSaida, "w");

    fprintf(arquivoRanking, "indice,fileira,linha,iteracao\n");

    for (int i = 0; i < qtdInimigos; i++) {
        if (inimigos[i].estaVivo == FALSE) {
            fprintf(arquivoRanking, "%d,%d,%d,%d\n", inimigos[i].indice, inimigos[i].fileira, inimigos[i].linhaMorte, inimigos[i].iteracaoMorte);
        }

    }
    fclose(arquivoRanking);
}

int FrameAtualInimigo(tInimigo inimigo, int iteracao) {
    if (inimigo.animado) {
        return iteracao % 3;
    } else {
        return 0;
    }
}




tTiro InicializaTiro() {
    tTiro tiro;
    tiro.s = 'o';
    tiro.estaAtivo = FALSE;
    tiro.numeroDeTiros = 0;
    tiro.numeroDeTirosErrados = 0;

    return tiro;
}

tTiro EfetuaTiro(tJogador jogador, tTiro tiro) {
    if (tiro.estaAtivo == FALSE) {
        tiro.estaAtivo = TRUE;
        tiro.numeroDeTiros++;
        tiro.x = jogador.x;
        tiro.y = jogador.y - 2;
    }
    return tiro;
}

tTiro MoveTiro(tTiro tiro) {
    if (tiro.estaAtivo == TRUE) {
        if (tiro.y - 1 < 1) {
            tiro.estaAtivo = FALSE;
            tiro.numeroDeTirosErrados++;
        } else {
            tiro.y--;
        }
    }
    return tiro;
}

int NumeroDeTiros(tTiro tiro) {
    return tiro.numeroDeTiros;
}

int NumeroDeTirosErrados(tTiro tiro) {
    return tiro.numeroDeTirosErrados;
}
