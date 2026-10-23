/*
 * Trabalho Pratico de AED - Arvores no Mundo Real (BST e AVL)
 * Aluno: Joao Maerbal Nunes Barros
 * RA: 202510288
 * Video: https://www.youtube.com/watch?v=bza74MyH1AQ
 *
 * As 200 melhores jogadas do perfil do osu! organizadas em uma BST e
 * em uma AVL, com o ID do beatmap como chave. O menu permite inserir,
 * buscar e remover jogadas, ver os percursos e a altura, simular a
 * posicao de uma jogada nova e rodar o experimento comparativo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 256
#define TAM_DATA 32
#define TAM_LINHA 512

/* No da arvore: uma jogada do osu!.
 * A chave e o ID do beatmap; nome, pp e data sao dados da jogada.
 * A altura sera usada pela AVL. */
typedef struct No {
    int id;                 /* CHAVE: o unico campo que a arvore compara */
    char nome[TAM_NOME];
    double pp;              /* usado so na simulacao */
    char data[TAM_DATA];
    int altura;
    struct No *esq;         /* IDs menores */
    struct No *dir;         /* IDs maiores */
} No;

/* Uma linha do CSV, guardada num vetor antes de ir para as arvores.
 * O experimento usa o vetor para montar arvores novas. */
typedef struct {
    int id;
    char nome[TAM_NOME];
    double pp;
    char data[TAM_DATA];
} Jogada;

/* Compara um ID com o ID de um no.
 * Retorna -1 se for menor, 0 se for igual e 1 se for maior. */
int comparar_chaves(int id, No *no)
{
    if (id < no->id) return -1;
    if (id > no->id) return 1;
    return 0;
}

/* Aloca um no novo e preenche os dados da jogada.
 * Retorna NULL se o malloc falhar. */
No *criar_no(int id, const char *nome, double pp, const char *data)
{
    No *novo = (No *) malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente ao criar no.\n");
        return NULL;
    }
    novo->id = id;
    strncpy(novo->nome, nome, TAM_NOME - 1);
    novo->nome[TAM_NOME - 1] = '\0';
    novo->pp = pp;
    strncpy(novo->data, data, TAM_DATA - 1);
    novo->data[TAM_DATA - 1] = '\0';
    novo->altura = 1;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

/* Insere uma jogada na BST e devolve a raiz (possivelmente nova).
 * status recebe 1 se inseriu, 0 se a chave ja existia
 * e -1 se faltou memoria. */
No *inserir_bst(No *raiz, int id, const char *nome, double pp, const char *data, int *status)
{
    int cmp;

    if (raiz == NULL) {
        No *novo = criar_no(id, nome, pp, data);
        *status = (novo != NULL) ? 1 : -1;
        return novo;
    }

    cmp = comparar_chaves(id, raiz);
    if (cmp < 0) {
        raiz->esq = inserir_bst(raiz->esq, id, nome, pp, data, status);
    } else if (cmp > 0) {
        raiz->dir = inserir_bst(raiz->dir, id, nome, pp, data, status);
    } else {
        *status = 0;
    }
    return raiz;
}

/* Separa uma linha "nome;id;pp;data" nos quatro campos.
 * Os campos sao lidos da direita para a esquerda, entao um nome
 * que contenha ';' nao quebra a leitura.
 * Retorna 1 se a linha e valida e 0 caso contrario. */
int separar_linha(char *linha, char *nome, int *id, double *pp, char *data)
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

    *pp = strtod(sep_pp + 1, &fim);
    if (fim == sep_pp + 1 || *fim != '\0') return 0;

    *id = (int) id_lido;
    strncpy(nome, linha, TAM_NOME - 1);
    nome[TAM_NOME - 1] = '\0';
    strncpy(data, sep_data + 1, TAM_DATA - 1);
    data[TAM_DATA - 1] = '\0';
    return 1;
}

/* Le o CSV inteiro para um vetor de jogadas, na ordem do arquivo.
 * Linhas invalidas (como o cabecalho) sao ignoradas e contadas.
 * Devolve o vetor, alocado com malloc, e a quantidade em *n. */
Jogada *ler_jogadas(const char *arquivo, int *n)
{
    FILE *fp;
    char linha[TAM_LINHA];
    int linhas = 0, ignoradas = 0;
    Jogada *v;

    *n = 0;
    fp = fopen(arquivo, "r");
    if (fp == NULL) {
        printf("Erro: nao foi possivel abrir o arquivo %s.\n", arquivo);
        return NULL;
    }

    while (fgets(linha, TAM_LINHA, fp) != NULL)    /* 1a passada: conta as linhas */
        linhas++;

    v = (Jogada *) malloc(linhas * sizeof(Jogada));
    if (v == NULL) {
        printf("Erro: memoria insuficiente ao ler o arquivo.\n");
        fclose(fp);
        return NULL;
    }

    rewind(fp);                                    /* volta ao inicio do arquivo */
    while (fgets(linha, TAM_LINHA, fp) != NULL) {  /* 2a passada: guarda as jogadas */
        linha[strcspn(linha, "\r\n")] = '\0';     /* tira a quebra de linha */
        if (linha[0] == '\0') continue;
        if (separar_linha(linha, v[*n].nome, &v[*n].id, &v[*n].pp, v[*n].data))
            (*n)++;
        else
            ignoradas++;
    }
    fclose(fp);

    printf("Jogadas lidas: %d | linhas ignoradas: %d\n", *n, ignoradas);
    return v;
}

/* Mostra uma jogada em uma linha. */
void imprimir_jogada(No *no)
{
    printf("ID %8d | %8.3f pp | %s | %s\n",
           no->id, no->pp, no->data, no->nome);
}

/* Percurso em ordem: esquerda, no, direita (ID crescente). */
void em_ordem(No *raiz)
{
    if (raiz == NULL) return;
    em_ordem(raiz->esq);
    imprimir_jogada(raiz);
    em_ordem(raiz->dir);
}

/* Pre-ordem: no, esquerda, direita. */
void pre_ordem(No *raiz)
{
    if (raiz == NULL) return;
    imprimir_jogada(raiz);
    pre_ordem(raiz->esq);
    pre_ordem(raiz->dir);
}

/* Pos-ordem: esquerda, direita, no. */
void pos_ordem(No *raiz)
{
    if (raiz == NULL) return;
    pos_ordem(raiz->esq);
    pos_ordem(raiz->dir);
    imprimir_jogada(raiz);
}

/* Altura: quantos nos tem o caminho mais longo da raiz ate uma folha.
 * Arvore vazia tem altura 0. */
int calcular_altura(No *raiz)
{
    int he, hd;

    if (raiz == NULL) return 0;
    he = calcular_altura(raiz->esq);
    hd = calcular_altura(raiz->dir);
    return 1 + (he > hd ? he : hd);
}

/* Altura guardada no no; arvore vazia tem altura 0. */
int altura_no(No *no)
{
    if (no == NULL) return 0;
    return no->altura;
}

/* Recalcula a altura de um no a partir das alturas dos filhos. */
void atualizar_altura(No *no)
{
    int he = altura_no(no->esq);
    int hd = altura_no(no->dir);
    no->altura = 1 + (he > hd ? he : hd);
}

/* Fator de balanco: altura da esquerda menos altura da direita.
 * Na AVL, precisa ficar entre -1 e 1. */
int fator_balanco(No *no)
{
    return altura_no(no->esq) - altura_no(no->dir);
}

/* Rotacao a direita: o filho da esquerda sobe. */
No *rotacao_direita(No *raiz)
{
    No *nova = raiz->esq;      /* o filho da esquerda vai subir */
    raiz->esq = nova->dir;     /* o filho direito dele troca de pai */
    nova->dir = raiz;          /* a raiz antiga desce para a direita */
    atualizar_altura(raiz);    /* primeiro o que desce */
    atualizar_altura(nova);    /* depois o que sobe */
    return nova;               /* nova raiz deste pedaco */
}

/* Rotacao a esquerda: o espelho da direita. */
No *rotacao_esquerda(No *raiz)
{
    No *nova = raiz->dir;
    raiz->dir = nova->esq;
    nova->esq = raiz;
    atualizar_altura(raiz);
    atualizar_altura(nova);
    return nova;
}

/* Atualiza a altura de um no, confere o fator de balanco e faz
 * a rotacao necessaria. Devolve a raiz deste pedaco. */
No *balancear(No *raiz)
{
    int fb;

    atualizar_altura(raiz);
    fb = fator_balanco(raiz);

    if (fb > 1) {                                /* pesado a esquerda */
        if (fator_balanco(raiz->esq) < 0)        /* caso 3: dupla */
            raiz->esq = rotacao_esquerda(raiz->esq);
        return rotacao_direita(raiz);            /* casos 1 e 3 */
    }
    if (fb < -1) {                               /* pesado a direita */
        if (fator_balanco(raiz->dir) > 0)        /* caso 4: dupla */
            raiz->dir = rotacao_direita(raiz->dir);
        return rotacao_esquerda(raiz);           /* casos 2 e 4 */
    }
    return raiz;                                 /* ja equilibrado */
}

/* Insercao na AVL: igual a da BST, mas cada no do caminho
 * se balanceia na volta da recursao. */
No *inserir_avl(No *raiz, int id, const char *nome, double pp, const char *data, int *status)
{
    int cmp;

    if (raiz == NULL) {
        No *novo = criar_no(id, nome, pp, data);
        *status = (novo != NULL) ? 1 : -1;
        return novo;
    }

    cmp = comparar_chaves(id, raiz);
    if (cmp < 0) {
        raiz->esq = inserir_avl(raiz->esq, id, nome, pp, data, status);
    } else if (cmp > 0) {
        raiz->dir = inserir_avl(raiz->dir, id, nome, pp, data, status);
    } else {
        *status = 0;
        return raiz;
    }
    return balancear(raiz);
}

/* Busca uma jogada pelo ID e devolve o no, ou NULL se nao achar.
 * Cada comparacao com um no soma 1 no contador. */
No *buscar(No *raiz, int id, int *comparacoes)
{
    int cmp;

    while (raiz != NULL) {
        cmp = comparar_chaves(id, raiz);
        (*comparacoes)++;
        if (cmp == 0) return raiz;
        if (cmp < 0) raiz = raiz->esq;
        else raiz = raiz->dir;
    }
    return NULL;
}

/* Percorre a arvore inteira e conta as jogadas com pp >= x.
 * A arvore esta ordenada pelo ID, nao pelo pp, entao
 * todo no precisa ser visitado. */
void contar_acima(No *raiz, double x, int *acima)
{
    if (raiz == NULL) return;
    if (raiz->pp >= x) (*acima)++;
    contar_acima(raiz->esq, x, acima);
    contar_acima(raiz->dir, x, acima);
}

/* Mostra em que posicao uma jogada de x pp entraria no perfil
 * e quanto ela valeria com o peso da posicao: x * 0,95^(n-1). */
void simular(No *raiz, double x)
{
    int acima = 0, n, i;
    double peso = 1.0;

    contar_acima(raiz, x, &acima);
    n = acima + 1;

    for (i = 1; i < n; i++)          /* 0,95^(n-1) sem pow() */
        peso = peso * 0.95;

    printf("Jogada de %.3f pp: posicao %d | valor com o peso da posicao: %.3f pp\n",
           x, n, x * peso);
}

/* Devolve o no de menor ID de uma subarvore: o mais a esquerda. */
No *menor_no(No *raiz)
{
    while (raiz->esq != NULL)
        raiz = raiz->esq;
    return raiz;
}

/* Remove a jogada com o ID dado e devolve a raiz (possivelmente nova).
 * removido recebe 1 se achou e removeu, 0 se o ID nao existe. */
No *remover_bst(No *raiz, int id, int *removido)
{
    int cmp;
    No *filho, *suc;

    if (raiz == NULL) return NULL;          /* o ID nao existe */

    cmp = comparar_chaves(id, raiz);
    if (cmp < 0) {
        raiz->esq = remover_bst(raiz->esq, id, removido);
    } else if (cmp > 0) {
        raiz->dir = remover_bst(raiz->dir, id, removido);
    } else if (raiz->esq == NULL && raiz->dir == NULL) {
        /* caso 1: folha */
        free(raiz);
        *removido = 1;
        return NULL;
    } else if (raiz->esq == NULL || raiz->dir == NULL) {
        /* caso 2: um filho, que sobe para o lugar do no */
        filho = (raiz->esq != NULL) ? raiz->esq : raiz->dir;
        free(raiz);
        *removido = 1;
        return filho;
    } else {
        /* caso 3: dois filhos. Copia TODOS os dados do sucessor
         * e remove o sucessor da subarvore direita. */
        suc = menor_no(raiz->dir);
        raiz->id = suc->id;
        strcpy(raiz->nome, suc->nome);
        raiz->pp = suc->pp;
        strcpy(raiz->data, suc->data);
        raiz->dir = remover_bst(raiz->dir, suc->id, removido);
    }
    return raiz;
}

/* Remocao na AVL: igual a da BST, mas cada no do caminho
 * se balanceia na volta da recursao. */
No *remover_avl(No *raiz, int id, int *removido)
{
    int cmp;
    No *filho, *suc;

    if (raiz == NULL) return NULL;

    cmp = comparar_chaves(id, raiz);
    if (cmp < 0) {
        raiz->esq = remover_avl(raiz->esq, id, removido);
    } else if (cmp > 0) {
        raiz->dir = remover_avl(raiz->dir, id, removido);
    } else if (raiz->esq == NULL && raiz->dir == NULL) {
        free(raiz);                             /* caso 1: folha */
        *removido = 1;
        return NULL;
    } else if (raiz->esq == NULL || raiz->dir == NULL) {
        filho = (raiz->esq != NULL) ? raiz->esq : raiz->dir;
        free(raiz);                             /* caso 2: um filho */
        *removido = 1;
        return filho;
    } else {
        suc = menor_no(raiz->dir);              /* caso 3: dois filhos */
        raiz->id = suc->id;
        strcpy(raiz->nome, suc->nome);
        raiz->pp = suc->pp;
        strcpy(raiz->data, suc->data);
        raiz->dir = remover_avl(raiz->dir, suc->id, removido);
    }
    return balancear(raiz);
}

/* Monta uma arvore nova inserindo as jogadas do vetor, na ordem dele.
 * avl = 1 usa a insercao da AVL; avl = 0, a da BST. */
No *montar_arvore(Jogada *v, int n, int avl)
{
    int i, status, repetidas = 0;
    No *raiz = NULL;

    for (i = 0; i < n; i++) {
        status = 0;
        if (avl)
            raiz = inserir_avl(raiz, v[i].id, v[i].nome, v[i].pp, v[i].data, &status);
        else
            raiz = inserir_bst(raiz, v[i].id, v[i].nome, v[i].pp, v[i].data, &status);

        if (status == 0) repetidas++;
        else if (status == -1) break;              /* faltou memoria */
    }
    if (repetidas > 0)
        printf("Aviso: %d IDs repetidos foram ignorados.\n", repetidas);
    return raiz;
}

/* Libera todos os nos em pos-ordem: os filhos antes do pai. */
void liberar_arvore(No *raiz)
{
    if (raiz == NULL) return;
    liberar_arvore(raiz->esq);
    liberar_arvore(raiz->dir);
    free(raiz);
}

/* Compara duas jogadas pelo ID, no formato que o qsort exige:
 * recebe dois enderecos genericos (void *) e devolve -1, 0 ou 1. */
int comparar_jogadas(const void *a, const void *b)
{
    const Jogada *x = (const Jogada *) a;
    const Jogada *y = (const Jogada *) b;

    if (x->id < y->id) return -1;
    if (x->id > y->id) return 1;
    return 0;
}

/* Busca cada jogada do vetor na arvore e imprime uma linha da tabela.
 * O contador nasce aqui, zerado, e vai por ponteiro para a busca. */
void medir_arvore(const char *cenario, const char *tipo, No *raiz, Jogada *v, int n)
{
    int i, comparacoes = 0;

    for (i = 0; i < n; i++)
        buscar(raiz, v[i].id, &comparacoes);

    printf("%-34s | %-6s | %6d | %18d | %15.2f\n", cenario, tipo,
           calcular_altura(raiz), comparacoes, (double) comparacoes / n);
}

/* Experimento comparativo: monta BST e AVL novas em dois cenarios,
 * mede cada uma e libera as quatro arvores. */
void experimento(Jogada *v, int n)
{
    char cen_a[64], cen_b[64];
    Jogada *ord;
    No *bst, *avl;

    if (n == 0) {
        printf("Nao ha jogadas para o experimento.\n");
        return;
    }

    /* Cenario B precisa das jogadas em ordem crescente de ID:
     * copia o vetor e ordena a copia, sem mexer no original. */
    ord = (Jogada *) malloc(n * sizeof(Jogada));
    if (ord == NULL) {
        printf("Erro: memoria insuficiente para o experimento.\n");
        return;
    }
    memcpy(ord, v, n * sizeof(Jogada));
    qsort(ord, n, sizeof(Jogada), comparar_jogadas);

    sprintf(cen_a, "A (ordem original), %d registros", n);
    sprintf(cen_b, "B (ordem crescente), %d registros", n);

    printf("\n%-34s | %-6s | %6s | %18s | %15s\n",
           "Cenario", "Arvore", "Altura", "Comparacoes totais", "Media por busca");

    bst = montar_arvore(ord, n, 0);               /* Cenario B */
    avl = montar_arvore(ord, n, 1);
    medir_arvore(cen_b, "BST", bst, v, n);
    medir_arvore(cen_b, "AVL", avl, v, n);
    liberar_arvore(bst);
    liberar_arvore(avl);

    bst = montar_arvore(v, n, 0);                 /* Cenario A */
    avl = montar_arvore(v, n, 1);
    medir_arvore(cen_a, "BST", bst, v, n);
    medir_arvore(cen_a, "AVL", avl, v, n);
    liberar_arvore(bst);
    liberar_arvore(avl);

    free(ord);
}

/* Mostra a mensagem e le uma linha do teclado, sem o '\n'.
 * Retorna 0 se a entrada acabou (Ctrl+D). */
int ler_linha(const char *msg, char *buf, int tam)
{
    int c;

    printf("%s", msg);
    if (fgets(buf, tam, stdin) == NULL) return 0;
    if (strchr(buf, '\n') == NULL)            /* linha maior que o espaco: */
        while ((c = getchar()) != '\n' && c != EOF)
            ;                                 /* descarta o resto */
    buf[strcspn(buf, "\r\n")] = '\0';
    return 1;
}

/* Le um numero inteiro, repetindo a pergunta ate a resposta ser valida. */
int ler_inteiro(const char *msg, int *x)
{
    char buf[TAM_LINHA], *fim;
    long lido;

    while (ler_linha(msg, buf, TAM_LINHA)) {
        lido = strtol(buf, &fim, 10);
        if (fim != buf && *fim == '\0') {
            *x = (int) lido;
            return 1;
        }
        printf("Valor invalido. Digite um numero inteiro.\n");
    }
    return 0;
}

/* Le um numero real maior que zero, repetindo ate ser valido. */
int ler_real(const char *msg, double *x)
{
    char buf[TAM_LINHA], *fim;

    while (ler_linha(msg, buf, TAM_LINHA)) {
        *x = strtod(buf, &fim);
        if (fim != buf && *fim == '\0' && *x > 0) return 1;
        printf("Valor invalido. Digite um numero maior que zero (ex.: 350.5).\n");
    }
    return 0;
}

/* Opcao 1: insere uma jogada nova nas duas arvores.
 * bst e avl sao os enderecos das variaveis que guardam as raizes,
 * porque a insercao pode trocar a raiz. */
void opcao_inserir(No **bst, No **avl)
{
    int id, status = 0, comp = 0;
    double pp;
    char nome[TAM_NOME], data[TAM_DATA];
    No *antiga;

    if (!ler_inteiro("ID do beatmap: ", &id)) return;
    antiga = buscar(*bst, id, &comp);
    if (antiga != NULL) {
        printf("Ja existe uma jogada nesse mapa (%.3f pp). "
               "Remova-a antes (opcao 3) para inserir a nova.\n", antiga->pp);
        return;
    }
    if (!ler_linha("Nome do mapa: ", nome, TAM_NOME)) return;
    if (!ler_real("pp: ", &pp)) return;
    if (!ler_linha("Data (AAAA-MM-DD HH:MM:SS): ", data, TAM_DATA)) return;

    *bst = inserir_bst(*bst, id, nome, pp, data, &status);
    if (status == 1)
        *avl = inserir_avl(*avl, id, nome, pp, data, &status);
    if (status == 1) printf("Jogada inserida nas duas arvores.\n");
    else printf("Erro: memoria insuficiente.\n");
}

/* Opcao 2: busca pelo ID e mostra as comparacoes de cada arvore. */
void opcao_buscar(No *bst, No *avl)
{
    int id, comp_bst = 0, comp_avl = 0;
    No *no;

    if (!ler_inteiro("ID do beatmap: ", &id)) return;
    no = buscar(bst, id, &comp_bst);
    buscar(avl, id, &comp_avl);

    if (no != NULL) imprimir_jogada(no);
    else printf("Nenhuma jogada com o ID %d.\n", id);
    printf("Comparacoes: BST %d | AVL %d\n", comp_bst, comp_avl);
}

/* Opcao 3: remove pelo ID nas duas arvores. */
void opcao_remover(No **bst, No **avl)
{
    int id, rem_bst = 0, rem_avl = 0;

    if (!ler_inteiro("ID do beatmap a remover: ", &id)) return;
    *bst = remover_bst(*bst, id, &rem_bst);
    *avl = remover_avl(*avl, id, &rem_avl);

    if (rem_bst && rem_avl) printf("Jogada %d removida das duas arvores.\n", id);
    else printf("Nenhuma jogada com o ID %d.\n", id);
}

/* Opcao 4: escolhe a arvore e o percurso. */
void opcao_percursos(No *bst, No *avl)
{
    int arv, tipo;
    No *raiz;

    if (!ler_inteiro("Arvore (1 = BST, 2 = AVL): ", &arv)) return;
    if (arv != 1 && arv != 2) {
        printf("Arvore invalida.\n");
        return;
    }
    if (!ler_inteiro("Percurso (1 = em ordem, 2 = pre-ordem, 3 = pos-ordem): ", &tipo)) return;

    raiz = (arv == 1) ? bst : avl;
    if (tipo == 1) em_ordem(raiz);
    else if (tipo == 2) pre_ordem(raiz);
    else if (tipo == 3) pos_ordem(raiz);
    else printf("Percurso invalido.\n");
}

/* Opcao 6: le um valor de pp e simula a jogada. */
void opcao_simular(No *raiz)
{
    double x;

    if (!ler_real("pp da jogada nova: ", &x)) return;
    simular(raiz, x);
}

void mostrar_menu(void)
{
    printf("\n===== Jogadas do osu! - BST e AVL =====\n");
    printf("1. Inserir jogada\n");
    printf("2. Buscar jogada pelo ID\n");
    printf("3. Remover jogada pelo ID\n");
    printf("4. Percursos (em ordem, pre-ordem, pos-ordem)\n");
    printf("5. Altura das arvores\n");
    printf("6. Simular jogada de x pp\n");
    printf("7. Experimento comparativo\n");
    printf("0. Sair\n");
}

/* Menu interativo. Monta a BST e a AVL com as jogadas do arquivo,
 * executa as opcoes e libera as duas arvores ao sair. */
void menu(Jogada *v, int n)
{
    int op = -1;
    No *bst = montar_arvore(v, n, 0);
    No *avl = montar_arvore(v, n, 1);

    while (op != 0) {
        mostrar_menu();
        if (!ler_inteiro("Opcao: ", &op)) break;    /* a entrada acabou */

        switch (op) {
        case 1: opcao_inserir(&bst, &avl); break;
        case 2: opcao_buscar(bst, avl); break;
        case 3: opcao_remover(&bst, &avl); break;
        case 4: opcao_percursos(bst, avl); break;
        case 5:
            printf("Altura da BST: %d | Altura da AVL: %d\n",
                   calcular_altura(bst), calcular_altura(avl));
            break;
        case 6: opcao_simular(bst); break;
        case 7: experimento(v, n); break;
        case 0: printf("Liberando as arvores e saindo.\n"); break;
        default: printf("Opcao invalida.\n");
        }
    }

    liberar_arvore(bst);
    liberar_arvore(avl);
}

int main(void)
{
    int n = 0;
    Jogada *v = ler_jogadas("jogadas_osu.csv", &n);

    menu(v, n);

    free(v);
    return 0;
}
