/*
 * Trabalho Pratico de AED - Arvores no Mundo Real (BST e AVL)
 * Aluno: Joao Maerbal Nunes Barros
 * RA: 202510288
 * Video: (link do video aqui)
 *
 * Etapa 1: struct do no, leitura do CSV, insercao na BST,
 * percurso em ordem e liberacao da memoria.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 256
#define TAM_DATA 32
#define TAM_LINHA 512

/* No da arvore: uma jogada do osu!.
 * A chave e o par (pp em milesimos, ID do beatmap).
 * Altura, tamanho, fator e soma serao usados nas proximas etapas. */
typedef struct No {
    int pp;                 /* pp em milesimos: 447.932 vira 447932 */
    int id;                 /* ID do beatmap, desempata pp iguais */
    char nome[TAM_NOME];
    char data[TAM_DATA];
    int altura;
    int tamanho;            /* quantidade de nos na subarvore */
    double fator;           /* 0,95 elevado ao tamanho */
    double soma;            /* soma ponderada da subarvore */
    struct No *esq;
    struct No *dir;
} No;

/* Compara a chave (pp, id) com a chave de um no.
 * Retorna -1 se for menor, 0 se for igual e 1 se for maior.
 * O pp decide; o ID so desempata. */
int comparar_chaves(int pp, int id, No *no)
{
    if (pp < no->pp) return -1;
    if (pp > no->pp) return 1;
    if (id < no->id) return -1;
    if (id > no->id) return 1;
    return 0;
}

/* Aloca um no novo e preenche os dados da jogada.
 * Retorna NULL se o malloc falhar. */
No *criar_no(int pp, int id, const char *nome, const char *data)
{
    No *novo = (No *) malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente ao criar no.\n");
        return NULL;
    }
    novo->pp = pp;
    novo->id = id;
    strncpy(novo->nome, nome, TAM_NOME - 1);
    novo->nome[TAM_NOME - 1] = '\0';
    strncpy(novo->data, data, TAM_DATA - 1);
    novo->data[TAM_DATA - 1] = '\0';
    novo->altura = 1;
    novo->tamanho = 1;
    novo->fator = 0.95;
    novo->soma = pp / 1000.0;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

/* Insere uma jogada na BST e devolve a raiz (possivelmente nova).
 * status recebe 1 se inseriu, 0 se a chave ja existia
 * e -1 se faltou memoria. */
No *inserir_bst(No *raiz, int pp, int id, const char *nome, const char *data, int *status)
{
    int cmp;

    if (raiz == NULL) {
        No *novo = criar_no(pp, id, nome, data);
        *status = (novo != NULL) ? 1 : -1;
        return novo;
    }

    cmp = comparar_chaves(pp, id, raiz);
    if (cmp < 0) {
        raiz->esq = inserir_bst(raiz->esq, pp, id, nome, data, status);
    } else if (cmp > 0) {
        raiz->dir = inserir_bst(raiz->dir, pp, id, nome, data, status);
    } else {
        *status = 0;
    }
    return raiz;
}

/* Converte um texto como "447.932" ou "447,932" para milesimos (447932)
 * sem passar por ponto flutuante, evitando erro de arredondamento.
 * Retorna 1 se conseguiu converter e 0 se o texto for invalido. */
int converter_pp(const char *texto, int *pp)
{
    int inteira = 0, decimal = 0, casas = 0, achou_digito = 0;
    const char *c = texto;

    while (*c == ' ') c++;
    while (*c >= '0' && *c <= '9') {
        inteira = inteira * 10 + (*c - '0');
        achou_digito = 1;
        c++;
    }
    if (*c == '.' || *c == ',') {
        c++;
        while (*c >= '0' && *c <= '9') {
            if (casas < 3) {
                decimal = decimal * 10 + (*c - '0');
                casas++;
            }
            achou_digito = 1;
            c++;
        }
    }
    while (*c == ' ') c++;
    if (!achou_digito || *c != '\0') return 0;

    while (casas < 3) {        /* "408.2" vira 408200 */
        decimal *= 10;
        casas++;
    }
    *pp = inteira * 1000 + decimal;
    return 1;
}

/* Separa uma linha "nome;id;pp;data" nos quatro campos.
 * Os campos sao lidos da direita para a esquerda, entao um nome
 * que contenha ';' nao quebra a leitura.
 * Retorna 1 se a linha e valida e 0 caso contrario. */
int separar_linha(char *linha, char *nome, int *id, int *pp, char *data)
{
    char *sep_data, *sep_pp, *sep_id, *fim;
    long id_lido;

    sep_data = strrchr(linha, ';');
    if (sep_data == NULL) return 0;
    *sep_data = '\0';

    sep_pp = strrchr(linha, ';');
    if (sep_pp == NULL) return 0;
    *sep_pp = '\0';

    sep_id = strrchr(linha, ';');
    if (sep_id == NULL) return 0;
    *sep_id = '\0';

    id_lido = strtol(sep_id + 1, &fim, 10);
    if (fim == sep_id + 1 || *fim != '\0') return 0;
    if (!converter_pp(sep_pp + 1, pp)) return 0;

    *id = (int) id_lido;
    strncpy(nome, linha, TAM_NOME - 1);
    nome[TAM_NOME - 1] = '\0';
    strncpy(data, sep_data + 1, TAM_DATA - 1);
    data[TAM_DATA - 1] = '\0';
    return 1;
}

/* Le o CSV e insere cada jogada na BST, na ordem do arquivo.
 * Linhas invalidas (como um cabecalho) sao ignoradas e contadas. */
No *carregar_bst(const char *arquivo, int *total)
{
    FILE *fp;
    char linha[TAM_LINHA], nome[TAM_NOME], data[TAM_DATA];
    int id, pp, status;
    int ignoradas = 0, repetidas = 0;
    No *raiz = NULL;

    *total = 0;
    fp = fopen(arquivo, "r");
    if (fp == NULL) {
        printf("Erro: nao foi possivel abrir o arquivo %s.\n", arquivo);
        return NULL;
    }

    while (fgets(linha, TAM_LINHA, fp) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';    /* tira a quebra de linha */
        if (linha[0] == '\0') continue;

        if (!separar_linha(linha, nome, &id, &pp, data)) {
            ignoradas++;
            continue;
        }

        status = 0;
        raiz = inserir_bst(raiz, pp, id, nome, data, &status);
        if (status == 1) {
            (*total)++;
        } else if (status == 0) {
            repetidas++;
        } else {
            break;                               /* faltou memoria */
        }
    }
    fclose(fp);

    printf("Jogadas carregadas: %d | linhas ignoradas: %d | chaves repetidas: %d\n",
           *total, ignoradas, repetidas);
    return raiz;
}

/* Mostra uma jogada em uma linha. */
void imprimir_jogada(No *no)
{
    printf("%4d.%03d pp | ID %8d | %s | %s\n",
           no->pp / 1000, no->pp % 1000, no->id, no->data, no->nome);
}

/* Percurso em ordem: esquerda, no, direita (pp crescente). */
void em_ordem(No *raiz)
{
    if (raiz == NULL) return;
    em_ordem(raiz->esq);
    imprimir_jogada(raiz);
    em_ordem(raiz->dir);
}

/* Libera todos os nos em pos-ordem: os filhos antes do pai. */
void liberar_arvore(No *raiz)
{
    if (raiz == NULL) return;
    liberar_arvore(raiz->esq);
    liberar_arvore(raiz->dir);
    free(raiz);
}

int main(void)
{
    int total = 0;
    No *bst = carregar_bst("jogadas_osu.csv", &total);

    printf("\nJogadas em ordem crescente de pp:\n");
    em_ordem(bst);

    liberar_arvore(bst);
    return 0;
}
