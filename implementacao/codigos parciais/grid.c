#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX_CAMINHO 1001 //1000 caracteres + o \0

#define LARGURA_MAX_MAPA 102 //100 + bordas
#define ALTURA_MAX_MAPA 42 //40 + bordas



typedef struct {
    char arquivoMapa[TAM_MAX_CAMINHO];
    FILE* config;

    int largura;
    int altura;

    char grid[ALTURA_MAX_MAPA][LARGURA_MAX_MAPA];
    char gridVazio[ALTURA_MAX_MAPA][LARGURA_MAX_MAPA];
}tMapa;

tMapa InicializaMapa(char diretorio[]);
/*Le as informações dos arquivos de configuração e inicializa os valores*/

tMapa InicializaGridMapa(tMapa mapa);
/*Inicializa os valores do gridVazio (somente as bordas e espaços em branco)*/

tMapa AtualizaMapa(tMapa mapa);
/*Desenha os "objetos" no mapa seguindo a ordem de prioridade*/

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
    printf("%d %d\n", mapa.largura, mapa.altura);

    mapa = InicializaGridMapa(mapa);

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

tMapa AtualizaMapa(tMapa mapa) {
    for (int i = 0; i < mapa.altura + 2; i++) {
        for (int j = 0; j < mapa.largura + 2; j++) {
            mapa.grid[i][j] = mapa.gridVazio[i][j];
        }
    }

    return mapa;
}