# Revisão do código linha por linha

Explicação do `trabalho.c` completo em linguagem simples, função por função, na ordem do arquivo. Serve para estudar e para preparar a explicação do vídeo.

Os números de linha valem para o `trabalho.c` de 18/10/2026 (720 linhas, código completo).

| Parte | Linhas | Ênfase no vídeo |
|---|---|---|
| 1. struct `No` | 19–32 | **alta** |
| 2. struct `Jogada` | 34–41 | baixa |
| 3. `comparar_chaves` | 43–50 | média |
| 4. `criar_no` (malloc) | 52–71 | **alta** |
| 5. `inserir_bst` | 73–95 | **alta** |
| 6. leitura do CSV | 97–172 | baixa |
| 7. percursos | 181–206 | média |
| 8. `calcular_altura` | 208–218 | média |
| 9. AVL: altura e fator de balanço | 220–240 | **alta** |
| 10. rotações | 242–262 | **alta** |
| 11. `balancear` | 264–284 | **alta** |
| 12. `inserir_avl` | 286–308 | **alta** |
| 13. `buscar` | 310–324 | média |
| 14. `contar_acima` | 326–335 | média |
| 15. `simular` | 337–352 | média |
| 16. remoção na BST | 354–398 | **alta** |
| 17. `remover_avl` | 400–432 | média |
| 18. `montar_arvore` | 434–454 | baixa |
| 19. `liberar_arvore` (free) | 456–463 | **alta** |
| 20. experimento | 465–534 | média |
| 21. menu e `main` | 536–720 | média |

## 1. A struct `No`: o que é um nó (linhas 19–32)

Pense no nó como **uma ficha** com espaços para preencher. Cada jogada do arquivo vira uma ficha dessas.

```c
typedef struct No {
```
"Vou criar um tipo novo de ficha, chamado `No`."

```c
    int id;
```
Um espaço para um número inteiro: o ID do mapa. **É por ele que a árvore organiza as fichas.**

```c
    char nome[TAM_NOME];
```
Um espaço para texto: o nome do mapa. `char [...]` é uma sequência de letras, e `TAM_NOME` (256) é o tamanho máximo.

```c
    double pp;
```
Um espaço para um número com vírgula: o pp da jogada. Fica só guardado; a árvore não usa o pp para se organizar.

```c
    char data[TAM_DATA];
```
Texto com a data da jogada.

```c
    int altura;
```
Um número que a AVL vai usar mais tarde.

```c
    struct No *esq;
    struct No *dir;
```
Esses dois não guardam dados da jogada. O `*` quer dizer **ponteiro**, ou seja, um endereço. `esq` guarda **onde está** a ficha do filho da esquerda, e `dir`, a do filho da direita. São as "setas" do desenho da árvore. Quando não há filho, a seta aponta para `NULL`, que quer dizer "nada".

Dentro da struct é preciso escrever `struct No *`, porque o nome curto `No` só passa a existir depois da linha `} No;`.

```c
} No;
```
Fim da ficha. A partir daqui, `No` é o nome do tipo.

**Desenho de uma ficha:**
```
┌─────────────────────────────┐
│ id:     2828520             │
│ nome:   Letters to me [...] │
│ pp:     339.707             │
│ data:   2022-11-24 22:04:21 │
│ altura: 1                   │
│ esq: ──► (outra ficha ou NULL)
│ dir: ──► (outra ficha ou NULL)
└─────────────────────────────┘
```

**A regra da árvore:** seguindo a seta `esq`, você só encontra IDs **menores**. Seguindo a `dir`, só IDs **maiores**.

---

## 2. A struct `Jogada`: uma linha do CSV (linhas 34–41)

```c
typedef struct {
    int id;
    char nome[TAM_NOME];
    double pp;
    char data[TAM_DATA];
} Jogada;
```

É uma ficha mais simples que o `No`: os mesmos dados da jogada, mas **sem as setas e sem a altura**, porque ela não fica numa árvore. Ela fica num **vetor**, uma ficha ao lado da outra, na ordem do arquivo.

**Por que existe:** o experimento precisa montar 4 árvores novas (BST e AVL, nos Cenários A e B), e o Cenário B exige as jogadas **ordenadas pelo ID**. Só dá para ordenar num vetor. Então o arquivo é lido uma vez para o vetor, e todas as árvores do programa são montadas a partir dele.

```
jogadas_osu.csv ──► ler_jogadas ──► vetor v (200 jogadas, ordem do arquivo)
                                         │
                                         ├─► montar_arvore(v, n, 0) ──► BST
                                         └─► montar_arvore(v, n, 1) ──► AVL
```

Aqui não há `struct Jogada *` dentro da struct, então o `typedef` pode ser escrito sem nome depois do `struct`.

---

## 3. `comparar_chaves` (linhas 43–50)

**Para que serve:** responder uma pergunta só. O ID que estou procurando é menor, maior ou igual ao ID desta ficha? É essa resposta que decide se a árvore vai para a esquerda ou para a direita.

```c
int comparar_chaves(int id, No *no)
```
- `int` no começo: a função **devolve** um número inteiro como resposta.
- `int id`: o ID que eu tenho na mão, por exemplo 2828520.
- `No *no`: o endereço de uma ficha da árvore, a que vou comparar.

```c
    if (id < no->id) return -1;
```
`no->id` quer dizer "o ID que está dentro da ficha `no`". A **seta `->`** é como se lê um campo de uma ficha pelo endereço dela.
"Se o meu ID é menor que o da ficha, responde **-1**." (vai para a esquerda)

```c
    if (id > no->id) return 1;
```
"Se é maior, responde **1**." (vai para a direita)

```c
    return 0;
```
Se não é menor nem maior, só pode ser igual: responde **0**. (achou)

O `return` encerra a função na hora. Se a primeira linha já responde -1, as outras nem rodam.

**Exemplo:** procurando 2828520 numa ficha com ID 1925409.
- 2828520 < 1925409? Não.
- 2828520 > 1925409? **Sim**, então devolve 1: vai para a direita.

**Por que uma função separada, se são só 3 linhas?** Porque no experimento cada chamada dela conta como **uma comparação**. Com a comparação num lugar só, fica fácil saber exatamente o que está sendo contado.

### Por onde a comparação começa?

O `comparar_chaves` não sabe, e nem precisa saber: ele só compara **uma** ficha, a que recebe. Quem decide por onde começar e qual a próxima ficha é **quem chama**, a `buscar` e a `inserir_bst`. E elas sempre começam pela **raiz**, a ficha do topo.

1. No `menu`, a variável `bst` guarda o endereço da raiz, montada pela `montar_arvore`:
   ```c
   No *bst = montar_arvore(v, n, 0);
   ```
2. Na opção 2, o `menu` passa esse endereço para a `opcao_buscar`, que o passa para a busca:
   ```c
   no = buscar(bst, id, &comp_bst);
   ```
3. Dentro da `buscar`, esse endereço chega com o nome `raiz`, e é ali que a primeira comparação acontece.

A raiz é a **primeira jogada do arquivo**, Shitteru (1925409). Quando ela foi inserida, a árvore estava vazia, então ela ficou no topo, e todas as outras foram descendo a partir dela.

```
buscar começa com raiz = Shitteru (1925409)
  comparar_chaves(2828520, Shitteru)  → 1 (maior)  → raiz = raiz->dir
  comparar_chaves(2828520, Koise)     → 1 (maior)  → raiz = raiz->dir
  comparar_chaves(2828520, Letters)   → 0 (igual)  → achou!
```

O `comparar_chaves` é só o "juiz" de cada passo; quem anda pela árvore é a `buscar`.

---

## 4. `criar_no` (linhas 52–71)

O `criar_no` cria **uma ficha solta**, não a árvore. A ficha nasce sem ligação com nada. Quem a **encaixa** na árvore, ligando as setas, é a `inserir_bst`. O `criar_no` fabrica a peça, e a `inserir_bst` monta.

```c
No *criar_no(int id, const char *nome, double pp, const char *data)
```
Recebe os dados de uma jogada e **devolve o endereço** da ficha nova (`No *`). Os `const` só avisam que a função não vai alterar os textos que recebeu.

```c
    No *novo = (No *) malloc(sizeof(No));
```
A linha mais importante:
- `sizeof(No)`: "qual o tamanho de uma ficha?" São **328 bytes**: os campos somam 320, e o compilador acrescenta 8 bytes de alinhamento, para o `double` e os ponteiros começarem em endereços múltiplos de 8.
- `malloc(...)`: "sistema, me reserva 328 bytes de memória". Ele devolve o endereço desse espaço.
- `No *novo = ...`: guardo esse endereço na variável `novo`.

É a **alocação dinâmica** que o enunciado pede: cada ficha é criada na hora, quando precisa.

### O `(No *)` e o `void *`

O `(No *)` não é um ponteiro vazio: é uma **conversão de tipo** (*cast*). O `malloc` não sabe o que você vai guardar no espaço, então devolve um **`void *`**: um endereço genérico, sem tipo. O `(No *)` diz "trate esse endereço como o endereço de uma ficha `No`". Depois disso, o C aceita `novo->id`, `novo->pp` etc.

| | O que é |
|---|---|
| `void *` | um endereço sem tipo ("algo está aqui") |
| `No *` | um endereço de uma ficha `No` |
| `NULL` | um endereço que não aponta para nada (esse sim é o ponteiro "vazio") |

Em C o cast é **opcional**: `No *novo = malloc(sizeof(No));` também funciona, porque o C converte `void *` sozinho. Ele é escrito para deixar explícito o que acontece, e em C++ é obrigatório.

```c
    if (novo == NULL) {
        printf("Erro: memoria insuficiente ao criar no.\n");
        return NULL;
    }
```
Se a memória acabou, o `malloc` devolve `NULL`. Preencher uma ficha em `NULL` quebraria o programa (segmentation fault), por isso o teste vem antes. O enunciado exige esse teste.

```c
    novo->id = id;
```
Preenche o ID da ficha com o ID recebido.

```c
    strncpy(novo->nome, nome, TAM_NOME - 1);
    novo->nome[TAM_NOME - 1] = '\0';
```
Texto em C não se copia com `=`, por isso o `strncpy` ("copia no máximo 255 letras"). A segunda linha garante que o texto termina com `'\0'`, o marcador de "fim do texto" do C, mesmo se o nome for grande demais.

```c
    novo->pp = pp;
    strncpy(novo->data, data, TAM_DATA - 1);
    novo->data[TAM_DATA - 1] = '\0';
```
O mesmo para o pp (número, copia com `=`) e para a data (texto, com `strncpy`).

```c
    novo->altura = 1;
```
Uma ficha sozinha tem altura 1. A AVL vai usar isso.

```c
    novo->esq = NULL;
    novo->dir = NULL;
```
As duas setas apontam para "nada": a ficha nasce **sem filhos**. Toda ficha nova entra na árvore como folha.

```c
    return novo;
```
Devolve o endereço da ficha pronta para quem chamou, a `inserir_bst`.

```
novo ──► ┌──────────────────────┐
         │ id: 2828520          │
         │ nome: Letters to me  │
         │ pp: 339.707          │
         │ data: 2022-11-24 ... │
         │ altura: 1            │
         │ esq: NULL  dir: NULL │
         └──────────────────────┘
         (solta, ainda fora da árvore)
```

**Ênfase alta no vídeo:** o `malloc`, o teste do `NULL` e as setas em `NULL`.

---

## 5. `inserir_bst` (linhas 73–95)

É **recursiva**: a função chama a si mesma.

**A ideia:** a ficha nova desce a árvore como na busca (menor vai para a esquerda, maior para a direita) até encontrar **um lugar vazio** (`NULL`). É ali que ela entra.

```c
No *inserir_bst(No *raiz, int id, const char *nome, double pp, const char *data, int *status)
```
Recebe a ficha onde a descida está agora (`raiz`), os dados da jogada nova e um `status` por ponteiro, para avisar a quem chamou como terminou. **Devolve um endereço** (`No *`).

```c
    int cmp;
```
Guarda a resposta do `comparar_chaves` (-1, 0 ou 1).

```c
    if (raiz == NULL) {
        No *novo = criar_no(id, nome, pp, data);
        *status = (novo != NULL) ? 1 : -1;
        return novo;
    }
```
**Chegou num lugar vazio: é aqui que a ficha entra.**
- Cria a ficha com o `criar_no`.
- `*status = (novo != NULL) ? 1 : -1;` é um "if numa linha só": se a ficha foi criada, status 1 ("inseriu"); se o `malloc` falhou, -1 ("faltou memória").
- Devolve o endereço da ficha nova.

```c
    cmp = comparar_chaves(id, raiz);
```
Não está vazio: compara o ID novo com o desta ficha.

```c
    if (cmp < 0) {
        raiz->esq = inserir_bst(raiz->esq, id, nome, pp, data, status);
```
**Menor: continua a descida pela esquerda.** A função chama a si mesma, começando da filha da esquerda, e o **resultado** dessa chamada é gravado na seta `raiz->esq`.

```c
    } else if (cmp > 0) {
        raiz->dir = inserir_bst(raiz->dir, id, nome, pp, data, status);
```
Maior: o mesmo, pela direita.

```c
    } else {
        *status = 0;
    }
```
Igual: o ID já existe. Não insere de novo e avisa com status 0.

```c
    return raiz;
```
Devolve a própria ficha, sem mudança.

### O truque: `raiz->esq = inserir_bst(raiz->esq, ...)`

É assim que a ficha nova **fica ligada** à árvore. Exemplo: inserindo Letters (2828520) numa árvore com Shitteru e Koise.

```
Shitteru (1925409)
     \
     Koise (2054967)
          \
          NULL   ← o lugar vazio
```

**Descendo:**
1. Em Shitteru: 2828520 é maior, então chama `inserir_bst(Shitteru->dir, ...)`, que é Koise.
2. Em Koise: maior, então chama `inserir_bst(Koise->dir, ...)`, que é `NULL`.
3. Em `NULL`: cria a ficha Letters e **devolve o endereço dela**.

**Voltando (cada chamada termina e entrega o resultado para a de cima):**
- 3 → 2: o endereço de Letters volta para a linha `Koise->dir = ...`. A seta direita de Koise **passa a apontar para Letters**. A ficha está ligada.
- 2 → 1: Koise devolve a si mesma (`return raiz`), e `Shitteru->dir = Koise`. A seta continua igual.
- 1 → quem chamou: Shitteru devolve a si mesma, e a raiz continua a mesma.

```
Shitteru (1925409)
     \
     Koise (2054967)
          \
          Letters (2828520)   ← ligada
```

**Por que a função devolve a raiz?** Por causa do primeiro caso: com a árvore **vazia**, a primeira ficha inserida vira a raiz, e quem chamou precisa receber esse endereço. Por isso a `montar_arvore` faz `raiz = inserir_bst(raiz, ...)`. Na AVL isso fica ainda mais importante, porque as rotações trocam a raiz.

**Para o vídeo:** explicar a descida (menor à esquerda, maior à direita) e a volta (o endereço sobe e é gravado na seta).

---

## 6. Leitura do CSV: `separar_linha` e `ler_jogadas` (linhas 97–172)

Ênfase baixa no vídeo: mostrar rodando e explicar em uma frase.

### `separar_linha`

Recebe uma linha como `Letters to me [Endless Journey];2828520;339.707;2022-11-24 22:04:21` e separa os quatro campos.

- O `strrchr(linha, ';')` acha o **último** `;` da linha. A função troca esse `;` por `'\0'` (fim de texto) e repete três vezes. Assim os campos são lidos **da direita para a esquerda**: data, pp, ID. O que sobra no começo é o nome.
- Isso protege o nome: se um mapa tiver `;` no nome, ele fica todo no começo e não quebra a leitura.
- O ID é convertido com `strtol` e o pp com `strtod`. O ponteiro `fim` aponta para onde a conversão parou: se parou no começo, não havia número; se não parou no fim do texto, sobrou lixo. Nos dois casos a linha é inválida e a função devolve 0.
- O cabeçalho (`nome_do_mapa;id_beatmap;pp;data`) é recusado por essa mesma regra: `id_beatmap` não é número.

### `ler_jogadas`

```c
    while (fgets(linha, TAM_LINHA, fp) != NULL)    /* 1a passada: conta as linhas */
        linhas++;

    v = (Jogada *) malloc(linhas * sizeof(Jogada));
    if (v == NULL) { ... }

    rewind(fp);                                    /* volta ao inicio do arquivo */
    while (fgets(linha, TAM_LINHA, fp) != NULL) {  /* 2a passada: guarda as jogadas */
        ...
        if (separar_linha(linha, v[*n].nome, &v[*n].id, &v[*n].pp, v[*n].data))
            (*n)++;
```

- Para reservar o vetor, é preciso saber o tamanho. Por isso o arquivo é lido **duas vezes**: a primeira só conta as linhas, o `rewind` volta ao início, e a segunda guarda as jogadas.
- `malloc(linhas * sizeof(Jogada))` reserva espaço para todas de uma vez, e o retorno é testado contra `NULL`.
- `v[*n].id`: no vetor se usa o **ponto** (`.`), porque `v[*n]` é a ficha em si, não um endereço. A seta (`->`) é usada quando se tem o endereço da ficha, como na árvore.
- `linha[strcspn(linha, "\r\n")] = '\0';` corta a quebra de linha do fim, inclusive a do Windows (`\r\n`).
- O vetor é liberado com `free(v)` no fim do `main`.

---

## 7. Os percursos: `em_ordem`, `pre_ordem` e `pos_ordem` (linhas 181–206)

### Em ordem (linhas 181–188)

Imprime todas as fichas, do menor ID para o maior.

```c
    if (raiz == NULL) return;
```
Se não há ficha aqui, não faz nada e volta. É o que faz a recursão parar.

```c
    em_ordem(raiz->esq);
```
**Primeiro** imprime tudo que está à esquerda (os IDs menores).

```c
    imprimir_jogada(raiz);
```
**Depois** imprime esta ficha.

```c
    em_ordem(raiz->dir);
```
**Por último**, tudo que está à direita (os IDs maiores).

Como em cada ficha os menores saem antes e os maiores depois, a lista inteira sai em ordem crescente de ID. Isso também serve para **verificar a árvore**: se alguma ficha estivesse do lado errado, apareceria um ID fora de ordem. E os pp saem fora de ordem, o que confirma que o pp não influencia a posição das fichas.

### Pré-ordem e pós-ordem (linhas 190–206)

```c
void pre_ordem(No *raiz)          void pos_ordem(No *raiz)
{                                 {
    if (raiz == NULL) return;         if (raiz == NULL) return;
    imprimir_jogada(raiz);            pos_ordem(raiz->esq);
    pre_ordem(raiz->esq);             pos_ordem(raiz->dir);
    pre_ordem(raiz->dir);             imprimir_jogada(raiz);
}                                 }
```

Os três percursos visitam as mesmas fichas. Só muda **quando** a ficha é impressa. Cada ficha tem uma lista de 3 tarefas, sempre na mesma ordem, e cada ficha é impressa **uma vez só**:

| Percurso | Tarefas de cada ficha | Quando a ficha se imprime |
|---|---|---|
| pré-ordem | imprimir, esquerda, direita | antes das filhas |
| em ordem | esquerda, imprimir, direita | entre as filhas |
| pós-ordem | esquerda, direita, imprimir | depois das filhas |

Quando uma ficha chega na tarefa "esquerda" ou "direita", ela **pausa** e espera a filha terminar a lista dela inteira. Quando a filha termina, a ficha continua da tarefa seguinte. "Voltar" não é começar uma regra nova: é retomar a lista da ficha de cima de onde ela parou.

**Exemplo com 5 jogadas reais** (usado no teste da Etapa 3):

```
        B          B = Brand New Theater   (3466914)
       / \         C = caramel heaven      (2327059)
      C   Y        A = Atashi              (3221529)
       \   \       Y = Yakusoku            (4397861)
        A   F      F = Fake out            (4834142)
```

- **Pré-ordem:** B, C, A, Y, F.
- **Em ordem:** C, A, B, Y, F (IDs em ordem crescente).
- **Pós-ordem:** A, C, F, Y, B. A raiz é sempre a **última**, porque espera a árvore inteira. Na volta, uma ficha que ainda tem a direita pendente desce por ela antes de se imprimir: B só se imprime depois de Y e F.

A pós-ordem é a ordem do `liberar_arvore`.

---

## 8. `calcular_altura` (linhas 208–218)

```c
int calcular_altura(No *raiz)
{
    int he, hd;

    if (raiz == NULL) return 0;
    he = calcular_altura(raiz->esq);
    hd = calcular_altura(raiz->dir);
    return 1 + (he > hd ? he : hd);
}
```

A **altura** é o número de fichas do caminho mais longo da raiz até uma folha. Árvore vazia tem altura 0, e uma ficha sozinha, 1.

- Pergunta a altura dos dois lados (`he` e `hd`), pega a **maior** e soma 1, que é a própria ficha.
- `(he > hd ? he : hd)` é um "if numa linha só": se `he` é maior, usa `he`; senão, `hd`.
- Ela **percorre a árvore inteira**. É a que o menu (opção 5) e o experimento usam.
- O nome é `calcular_altura`, e não `altura`, para não confundir com o campo `altura` da struct, que a AVL usa.

Na árvore de 5 jogadas acima, a altura é 3 (B → C → A ou B → Y → F). Na BST com as 200 jogadas, 17.

---

## 9. A AVL: altura guardada e fator de balanço (linhas 220–240)

**O problema que a AVL resolve:** inserindo 10, 20 e 30 em ordem numa BST, cada número vai para a direita, e a árvore vira uma **lista degenerada**. Com as 200 jogadas em ordem crescente de ID, seriam 200 níveis. É o Cenário B do experimento.

```c
int altura_no(No *no)
{
    if (no == NULL) return 0;
    return no->altura;
}
```
Só **lê** o campo `altura` guardado na ficha. É instantâneo, diferente da `calcular_altura`, que percorre a árvore toda. A AVL consulta alturas a cada inserção, então precisa dessa versão rápida.

```c
void atualizar_altura(No *no)
{
    int he = altura_no(no->esq);
    int hd = altura_no(no->dir);
    no->altura = 1 + (he > hd ? he : hd);
}
```
Recalcula a altura de **uma** ficha a partir das alturas já guardadas nas filhas. É a mesma conta da `calcular_altura`, mas sem descer a árvore.

```c
int fator_balanco(No *no)
{
    return altura_no(no->esq) - altura_no(no->dir);
}
```
**Fator de balanço = altura da esquerda − altura da direita.** A regra da AVL: o fator de toda ficha tem que ser **-1, 0 ou 1**. Se der **2** (pesado à esquerda) ou **-2** (pesado à direita), a ficha está desequilibrada e é preciso rotacionar.

No exemplo 10 → 20 → 30, a ficha 10 tem esquerda vazia (0) e direita com altura 2: fator **-2**.

---

## 10. As rotações (linhas 242–262)

**Ênfase alta no vídeo:** o roteiro pede para explicar uma rotação.

Rotacionar é **girar** três fichas em torno da ligação entre um pai e um filho, como uma **gangorra**: um lado desce e o outro sobe.

```
Rotação à direita no 4:

      4   ← pai: desce para a direita            2
     /                                          / \
    2     ← pivô: sobe e vira o topo    →      1   4
   / \                                            /
  1   3   ← o 3 (lado de dentro) troca de pai    3
```

```c
No *rotacao_direita(No *raiz)
{
    No *nova = raiz->esq;      /* o filho da esquerda vai subir */
    raiz->esq = nova->dir;     /* o filho direito dele troca de pai */
    nova->dir = raiz;          /* a raiz antiga desce para a direita */
    atualizar_altura(raiz);    /* primeiro o que desce */
    atualizar_altura(nova);    /* depois o que sobe */
    return nova;               /* nova raiz deste pedaco */
}
```

Linha por linha, com `raiz` = 4:
- `nova = raiz->esq`: o **pivô** é o filho da esquerda, o 2. É ele que vai subir.
- `raiz->esq = nova->dir`: o 3 sai de baixo do 2 e vira filho esquerdo do 4. Ele era maior que o 2 e menor que o 4, então esse é o único lugar que respeita a regra da BST.
- `nova->dir = raiz`: o 4 vira filho direito do 2.
- `atualizar_altura(raiz)` e depois `atualizar_altura(nova)`: **primeiro o que desce, depois o que sobe**. A altura do 2 depende da altura do 4, que agora é filho dele. Na ordem inversa, o 2 calcularia com um valor velho.
- `return nova`: devolve a ficha que subiu, e quem chamou grava esse endereço na seta do pai. É o mesmo truque da inserção.

A `rotacao_esquerda` é o **espelho**: o pivô é o filho da direita, que sobe, e o pai desce para a esquerda.

| | Rotação à direita | Rotação à esquerda |
|---|---|---|
| Pivô (quem sobe) | o filho da esquerda | o filho da direita |
| Quem desce | o pai, para a direita | o pai, para a esquerda |
| Quando usar | pesado à esquerda | pesado à direita |

**Por que "rotação" e não "troca":** leia os números da esquerda para a direita antes e depois: 1, 2, 3, 4 nos dois casos. **A ordem não muda**, só muda quem está mais alto. É como girar um colar de contas: a sequência fica igual. Por isso o percurso em ordem dá o mesmo resultado antes e depois de qualquer rotação, e a AVL nunca quebra a regra da BST. A rotação também **nunca copia dados** entre fichas: só muda 3 setas.

**Por que não simplesmente "jogar a ficha lá embaixo":** a ficha que desce leva a subárvore dela junto. Pendurá-la embaixo de outra pode deixar outro ponto desequilibrado, e achar o lugar exigiria descer a árvore, o que é uma inserção. A rotação mexe sempre em 3 setas, em qualquer tamanho de árvore, e sempre tira um nível do lado pesado.

```
O 4 tem fator 2. "Jogando o 4 embaixo do 3":     Com a rotação à direita no 4:
          4                  2                            2
         / \                / \                          / \
        2   5      →       1   3   ← fator -2           1   4
       / \                /     \                      /   / \
      1   3              0       4                    0   3   5
     /                            \
    0                              5
```

---

## 11. `balancear`: quando rotacionar, e qual rotação (linhas 264–284)

```c
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
```

Atualiza a altura da ficha, calcula o fator e, se estiver desequilibrada, decide entre os **4 casos**. O nome do caso descreve o **formato** do desequilíbrio, e não a rotação que se faz:

```
Caso 1: esquerda-esquerda     Caso 2: direita-direita
     30                         10
    /                             \
   20      → rotação à direita     20     → rotação à esquerda
  /          em 30                   \      em 10
 10                                   30

Caso 3: esquerda-direita       Caso 4: direita-esquerda
     30                         10
    /                             \
   10      → DUPLA:                30     → DUPLA:
     \       1º esquerda no 10    /         1º direita no 30
      20     2º direita no 30    20         2º esquerda no 10
```

- **Como diferenciar:** olhe o fator do **filho** do lado pesado. Se ele pende para o mesmo lado, uma rotação simples basta. Se pende para o lado contrário (zigue-zague), é **dupla**: a primeira rotação endireita o zigue-zague e transforma o caso 3 no 1 (ou o 4 no 2), e a segunda resolve.
- **Por que `< 0` e `> 0`, e não `<= 0`:** na remoção pode acontecer de o filho ter fator **0**. Nesse caso a rotação simples resolve, como no teste de remover 5, 7 e 6 da AVL de 1 a 7.
- É o "rotações simples **e duplas**" que o enunciado pede.

---

## 12. `inserir_avl` (linhas 286–308)

É a `inserir_bst` com **uma diferença só**: a última linha.

```c
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
```

Em vez de `return raiz`, faz `return balancear(raiz)`. Como a volta da recursão passa por **todas as fichas do caminho, de baixo para cima**, cada uma corrige a sua altura e se equilibra, se precisar. E o que a `balancear` devolve (a ficha que ficou no topo daquele pedaço, talvez outra depois de uma rotação) é gravado na seta do pai pela linha `raiz->esq = inserir_avl(...)`.

**Exemplo: inserindo 1, 2 e 3.** Ao inserir o 3, na volta da recursão, a ficha 1 fica com esquerda 0 e direita 2: fator -2. O filho da direita (2) pende para o mesmo lado, então é o caso 2: rotação à esquerda no 1. O 2 sobe e vira a raiz.

**Teste da Etapa 4:** inserindo 1 a 7 em ordem crescente (o pior caso da BST), a AVL deu a árvore perfeita, com pré-ordem 4, 2, 1, 3, 6, 5, 7 e altura 3. Uma BST teria altura 7.

**O mesmo conjunto pode formar mais de uma AVL válida.** A regra só exige fatores entre -1 e 1. Mas o algoritmo é determinístico: com a mesma sequência de operações, ele sempre chega na mesma árvore.

---

## 13. `buscar` (linhas 310–324)

Acha uma ficha pelo ID e conta quantas comparações fez.

```c
No *buscar(No *raiz, int id, int *comparacoes)
```
Recebe a raiz, o ID procurado e o **endereço do contador**. Devolve o endereço da ficha encontrada, ou `NULL` se não encontrar.

```c
    while (raiz != NULL) {
```
"Enquanto eu estiver em cima de uma ficha, continuo procurando." Se cair em `NULL`, o ID não existe.

```c
        cmp = comparar_chaves(id, raiz);
        (*comparacoes)++;
```
Compara com a ficha atual e **soma 1 no contador**. O `*` em `(*comparacoes)` quer dizer "a variável que está nesse endereço": o contador mora em outra função, e a `buscar` só tem o endereço dele. É o "passar por ponteiro, sem variável global" que o enunciado pede.

```c
        if (cmp == 0) return raiz;
```
Igual: achou. Devolve o endereço desta ficha.

```c
        if (cmp < 0) raiz = raiz->esq;
        else raiz = raiz->dir;
```
**É aqui que a busca anda.** A variável `raiz` passa a apontar para a filha da esquerda ou da direita, e o `while` repete a comparação nessa ficha nova.

```c
    return NULL;
```
Saiu do `while` porque chegou em `NULL`: a jogada não existe.

**Por que aqui é laço e na inserção é recursão?** A busca só **olha**, não muda nenhuma seta, então basta ir trocando de ficha num laço. A inserção precisa **gravar** o endereço da ficha nova na seta do pai, e a volta da recursão faz isso naturalmente.

**O que a busca devolve:** a ficha inteira, ou seja, ID, pp, data e nome. A pessoa entra com a chave e recebe os dados associados a ela. No uso real, o que importa é o pp antigo daquele mapa, para comparar com um score novo.

**Os testes da Etapa 2** (feitos com uma função de teste que o menu substituiu):
- ID 2828520 (Letters to me): **3 comparações** (Shitteru → Koise → Letters).
- ID 1 (não existe, menor que todos): a busca vai sempre para a esquerda, passando pelos 6 nós do caminho mais à esquerda (1925409 → 1811389 → 847977 → 714190 → 519080 → 438456) até `NULL`: **6 comparações**.

**O par `&` e `*`:** quem chama cria o contador e passa o endereço dele.

```c
    int comp_bst = 0;                          /* em opcao_buscar */
    no = buscar(bst, id, &comp_bst);
```

| | Onde | Significa |
|---|---|---|
| `&comp_bst` | em quem chama | "o endereço do contador" |
| `*comparacoes` | dentro da `buscar` | "o contador que está nesse endereço" |

---

## 14. `contar_acima` (linhas 326–335)

Conta quantas jogadas da árvore têm pp **maior ou igual** a x.

```c
void contar_acima(No *raiz, double x, int *acima)
```
Recebe a ficha atual, o valor x e o **endereço do contador** `acima`, como na busca. Não devolve nada (`void`): a resposta fica no contador.

```c
    if (raiz == NULL) return;
```
Sem ficha aqui: volta. É o que faz a recursão parar.

```c
    if (raiz->pp >= x) (*acima)++;
```
Se o pp desta ficha é maior ou igual a x, soma 1 no contador. Com `>=`, se a jogada nova empatar com uma existente, ela fica logo abaixo dela.

```c
    contar_acima(raiz->esq, x, acima);
    contar_acima(raiz->dir, x, acima);
```
Repete a mesma coisa nas duas filhas. Repare que passa `acima` sem `&`: aqui `acima` **já é** um endereço, então só é repassado adiante.

**Por que visita todas as fichas, e não só um caminho como a busca?** Porque a árvore está organizada pelo **ID**, não pelo pp. Uma jogada de pp alto pode estar em qualquer lugar da árvore, então não há como saber de antemão de que lado ela está. É a comparação do cadastro por CPF: achar alguém pelo CPF é rápido, mas contar quem ganha mais de 5 mil exige olhar todo mundo.

Esse jeito de visitar (primeiro a ficha, depois a esquerda, depois a direita) é a **pré-ordem**. Para contar, a ordem não importa: o total é o mesmo.

---

## 15. `simular` (linhas 337–352)

Mostra em que posição uma jogada de x pp entraria no perfil e quanto ela valeria.

```c
    int acima = 0, n, i;
    double peso = 1.0;
```
O contador `acima` nasce aqui, em zero. O `peso` começa em 1, que é o peso da 1ª posição (100%).

```c
    contar_acima(raiz, x, &acima);
```
Conta as jogadas com pp ≥ x. O `&acima` passa o endereço do contador, igual ao `&comparacoes` da busca.

```c
    n = acima + 1;
```
Se há `acima` jogadas na frente, a nova entra na posição seguinte. Exemplo: 3 jogadas com pp maior, então a nova é a 4ª.

```c
    for (i = 1; i < n; i++)
        peso = peso * 0.95;
```
Calcula 0,95^(n−1): multiplica o peso por 0,95 uma vez para cada posição abaixo da 1ª. Na posição 1 o laço não roda, e o peso fica 1. Na posição 4 ele roda 3 vezes: 0,95 × 0,95 × 0,95 = 0,857375.

O laço substitui o `pow()`, que fica no `math.h` e exigiria `-lm` no comando de compilação. O comando do enunciado não tem `-lm`.

```c
    printf("Jogada de %.3f pp: posicao %d | valor com o peso da posicao: %.3f pp\n",
           x, n, x * peso);
```
Mostra o pp da jogada, a posição e o valor da jogada com o peso da posição, que é x vezes o peso. Não é quanto o total do perfil sobe: o total sobe um pouco menos, porque as jogadas abaixo da nova descem uma posição e perdem 5% do valor cada (REFORMULACAO §2.6). O `%.3f` imprime com 3 casas decimais.

| Situação | Resultado |
|---|---|
| x maior que todas as jogadas | posição 1, vale x inteiro |
| x menor que todas | última posição, depois de todas |
| x igual ao pp de alguma jogada | fica logo abaixo dela |
| árvore vazia | posição 1 |

---

## 16. A remoção na BST: `menor_no` e `remover_bst` (linhas 354–398)

**Ênfase alta no vídeo:** o roteiro pede a remoção com dois filhos.

Primeiro a função desce até a ficha, como na busca. Ao chegar, o que fazer depende de quantos filhos ela tem:

```
Caso 1: folha             Caso 2: um filho           Caso 3: dois filhos
   P                         P                          [X]  ← não sai do lugar
    \                         \                         /  \
    [X]  → some               [X]  → o filho           A    B
                                \    sobe para              /
                                 F   o lugar dela         [S] ← sucessor: o menor
                                                                da direita
```

```c
No *menor_no(No *raiz)
{
    while (raiz->esq != NULL)
        raiz = raiz->esq;
    return raiz;
}
```
O menor ID de uma subárvore é a ficha **mais à esquerda**: desce pela esquerda até não ter mais.

```c
    if (raiz == NULL) return NULL;          /* o ID nao existe */

    cmp = comparar_chaves(id, raiz);
    if (cmp < 0) {
        raiz->esq = remover_bst(raiz->esq, id, removido);
    } else if (cmp > 0) {
        raiz->dir = remover_bst(raiz->dir, id, removido);
```
Desce procurando, igual à inserção. Se chegar em `NULL`, o ID não existe. Na volta, cada pai grava na sua seta o que sobrou no lugar.

```c
    } else if (raiz->esq == NULL && raiz->dir == NULL) {
        /* caso 1: folha */
        free(raiz);
        *removido = 1;
        return NULL;
```
**Caso 1:** libera a ficha e devolve `NULL`. O pai passa a apontar para nada.

```c
    } else if (raiz->esq == NULL || raiz->dir == NULL) {
        /* caso 2: um filho, que sobe para o lugar do no */
        filho = (raiz->esq != NULL) ? raiz->esq : raiz->dir;
        free(raiz);
        *removido = 1;
        return filho;
```
**Caso 2:** guarda o único filho, libera a ficha e devolve o filho. O pai passa a apontar para ele. O filho é guardado **antes** do `free`, porque depois do `free` não se pode mais ler `raiz->esq`.

```c
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
```
**Caso 3:** a ficha não pode simplesmente sair, senão os dois filhos ficariam sem lugar.
- Pega o **sucessor**: o menor ID da subárvore direita. Ele é maior que tudo da esquerda e menor que tudo da direita, então pode ocupar este lugar sem quebrar a regra.
- **Copia todos os dados dele** (ID, nome, pp e data) para esta ficha. Se copiasse só o ID, a ficha ficaria com o nome e o pp de outra jogada.
- Remove o sucessor lá de baixo. Ele sempre cai no caso 1 ou 2, porque o menor de uma subárvore nunca tem filho à esquerda.

**Onde está o `free`:** nos casos 1 e 2, sempre na ficha que realmente sai da árvore. No caso 3, quem sai é a ficha do sucessor, liberada na chamada recursiva.

**Teste da Etapa 3:** removendo a raiz Shitteru (1925409, dois filhos), os dados do sucessor Believe in ourselves (1936713) foram copiados para a ficha da raiz. A busca por 1936713 passou a achá-lo com **1 comparação**, com o nome, o pp e a data certos.

---

## 17. `remover_avl` (linhas 400–432)

É a `remover_bst` com `return balancear(raiz);` no fim, para cada ficha do caminho se equilibrar na volta. Os três casos e os `free` são iguais.

**Por que os casos 1 e 2 não chamam `balancear`?** Porque a ficha some. O que volta é `NULL` ou o filho, cuja subárvore já estava equilibrada. Quem precisa se rebalancear é o **pai**, e ele faz isso na volta da recursão dele.

**Teste da Etapa 4:** na AVL de 1 a 7, removendo 5, 7 e 6, o lado direito do 4 ficou vazio e o esquerdo com altura 2: fator 2. O filho da esquerda (2) tinha fator 0, então uma rotação simples à direita no 4 resolveu.

```
      4                  2
     /                  / \
    2         →        1   4
   / \                    /
  1   3                  3
```
Pré-ordem 2, 1, 4, 3, altura 3.

---

## 18. `montar_arvore` (linhas 434–454)

```c
    for (i = 0; i < n; i++) {
        status = 0;
        if (avl)
            raiz = inserir_avl(raiz, v[i].id, v[i].nome, v[i].pp, v[i].data, &status);
        else
            raiz = inserir_bst(raiz, v[i].id, v[i].nome, v[i].pp, v[i].data, &status);
        ...
```

Percorre o vetor e insere cada jogada numa árvore nova, **na ordem do vetor**. Com `avl = 0` usa a inserção da BST; com `avl = 1`, a da AVL. É usada 6 vezes: as duas árvores do menu e as quatro do experimento. Só imprime algo se houver ID repetido, para não poluir a tabela do experimento.

---

## 19. `liberar_arvore` (linhas 456–463)

Devolve ao sistema, com `free`, a memória de todas as fichas.

```c
    if (raiz == NULL) return;
```
Sem ficha: volta.

```c
    liberar_arvore(raiz->esq);
    liberar_arvore(raiz->dir);
```
**Primeiro** libera tudo que está à esquerda e à direita.

```c
    free(raiz);
```
**Só depois** libera esta ficha.

**Por que nessa ordem (pós-ordem)?** As setas `esq` e `dir` estão **dentro** da ficha. Se ela fosse liberada primeiro, as setas iriam junto, e as filhas ficariam perdidas na memória, sem ninguém apontando para elas: um vazamento.

Foi o que o teste do AddressSanitizer de 14/10 mostrou (está no Diário). Com a chamada comentada, ele acusou 1 vazamento direto, a raiz, que só a variável `bst` apontava, e 199 indiretos, as fichas que só eram alcançáveis a partir da raiz perdida.

**Ênfase alta no vídeo:** mostrar onde está cada `free`.

---

## 20. O experimento comparativo (linhas 465–534)

### `comparar_jogadas`: a regra de ordenação para o `qsort`

```c
int comparar_jogadas(const void *a, const void *b)
{
    const Jogada *x = (const Jogada *) a;
    const Jogada *y = (const Jogada *) b;

    if (x->id < y->id) return -1;
    if (x->id > y->id) return 1;
    return 0;
}
```
O `qsort` é o ordenador da `stdlib.h`. Ele serve para vetores de qualquer tipo, então não sabe o que é uma `Jogada`: ele entrega para essa função **dois `void *`**, endereços genéricos. A função converte para `Jogada *`, como o `(No *)` do `malloc`, e responde quem vem antes, com -1, 0 ou 1.

### `medir_arvore`: uma linha da tabela

```c
    int i, comparacoes = 0;

    for (i = 0; i < n; i++)
        buscar(raiz, v[i].id, &comparacoes);
```
- **O contador nasce aqui, zerado**, uma vez para cada árvore, como o enunciado manda, e vai por ponteiro para a `buscar`. Sem variável global.
- Busca **cada jogada** do arquivo e soma as comparações.
- `(double) comparacoes / n` evita a divisão inteira: sem o `(double)`, 9,44 viraria 9.

### `experimento`

```c
    ord = (Jogada *) malloc(n * sizeof(Jogada));    /* copia do vetor */
    memcpy(ord, v, n * sizeof(Jogada));
    qsort(ord, n, sizeof(Jogada), comparar_jogadas); /* ordena a copia pelo ID */

    bst = montar_arvore(ord, n, 0);                 /* Cenario B */
    avl = montar_arvore(ord, n, 1);
    medir_arvore(cen_b, "BST", bst, v, n);
    medir_arvore(cen_b, "AVL", avl, v, n);
    liberar_arvore(bst);
    liberar_arvore(avl);

    bst = montar_arvore(v, n, 0);                   /* Cenario A */
    ...
    free(ord);
```
- O Cenário B ordena uma **cópia** do vetor, para não mexer na ordem original.
- As 4 árvores são liberadas logo depois de medidas, e a cópia no fim.
- A tabela sai na ordem do enunciado: B primeiro, depois A.

### O resultado

| Cenário | Árvore | Altura | Comparações | Média |
|---|---|---|---|---|
| B (ordem crescente) | BST | 200 | 20100 | 100,50 |
| B (ordem crescente) | AVL | 8 | 1353 | 6,76 |
| A (ordem original) | BST | 17 | 1888 | 9,44 |
| A (ordem original) | AVL | 9 | 1368 | 6,84 |

**Por que os números ficaram assim:**
1. **BST no B:** cada ID novo é maior que os anteriores e vai sempre para a direita: lista degenerada de altura 200. Buscar a k-ésima custa k comparações, e 1 + 2 + … + 200 = 200 × 201 ÷ 2 = 20.100.
2. **AVL no B:** a mesma entrada deu altura **8**, a menor possível para 200 jogadas (8 níveis comportam até 255 fichas; 7 níveis, só 127). Média quase 15 vezes menor.
3. **BST no A:** a ordem por data não é a ordem por ID, então a BST não degenerou. Mas mapas mais novos tendem a ter ID maior, e a ordem também não é aleatória: altura 17, o dobro do ideal, e cerca de 38% mais comparações que a AVL.
4. **AVL nos dois:** 8 e 9 de altura. **A ordem de entrada quase não afeta a AVL.** A BST depende da ordem dos dados, e a AVL garante o desempenho em qualquer ordem.

**Por que a AVL ficou melhor no B (8) que no A (9):** em ordem crescente, ela rotaciona sempre do mesmo jeito e acaba montando uma árvore quase completa, como no teste de 1 a 7. Na ordem original, ela só corrige quando a diferença entre os lados chega a 2, então aceita uma árvore um pouco menos perfeita, com um nível a mais. Ainda dentro da garantia (8 a 10 para 200 jogadas).

---

## 21. O menu e o `main` (linhas 536–720)

Ênfase média no vídeo: aparece funcionando na demonstração.

### Leitura do teclado: `ler_linha`, `ler_inteiro`, `ler_real`

- `ler_linha` mostra a pergunta, lê uma linha com `fgets` e corta o `'\n'`. Se a pessoa digitar mais do que cabe, descarta o resto, para não sobrar texto para a próxima pergunta. Devolve 0 se a entrada acabar (Ctrl+D).
- `ler_inteiro` e `ler_real` repetem a pergunta até a resposta ser válida, com a mesma checagem do `fim` da `separar_linha`. Se o professor digitar `abc`, o programa avisa e pergunta de novo, em vez de travar ou entrar em loop.

### Uma função por opção

O enunciado pede "cada operação é uma função com parâmetros". Cada opção do menu tem a sua:

| Opção | Função | O que faz |
|---|---|---|
| 1 | `opcao_inserir` | lê ID, nome, pp e data e insere nas duas árvores. Se o ID já existe, mostra o pp antigo e recusa |
| 2 | `opcao_buscar` | busca nas duas árvores e mostra as comparações de cada uma |
| 3 | `opcao_remover` | remove nas duas árvores |
| 4 | `opcao_percursos` | escolhe a árvore e o percurso |
| 5 | (no `menu`) | altura das duas árvores |
| 6 | `opcao_simular` | lê o pp e chama a `simular` |
| 7 | `experimento` | a tabela do experimento |
| 0 | (no `menu`) | sai e libera as duas árvores |

### `No **`: ponteiro para ponteiro

```c
void opcao_inserir(No **bst, No **avl)
...
    *bst = inserir_bst(*bst, id, nome, pp, data, &status);
```
A inserção e a remoção podem **trocar a raiz**, então a função precisa mudar a variável `bst` que está no `menu`. É o mesmo truque do contador: o `menu` passa `&bst`, o endereço da variável que guarda a raiz, e a função escreve lá com `*bst = ...`. Como a variável já é um ponteiro, o endereço dela é um "ponteiro para ponteiro".

| | contador | raiz |
|---|---|---|
| a variável | `int comparacoes` | `No *bst` |
| quem chama passa | `&comparacoes` | `&bst` |
| a função recebe | `int *comparacoes` | `No **bst` |
| a função muda com | `(*comparacoes)++` | `*bst = ...` |

A busca, os percursos e a simulação não mudam a raiz, então recebem `No *` comum.

### `menu`

Monta a BST e a AVL com `montar_arvore`, repete o laço "mostra as opções, lê, executa" até a opção 0 e, ao sair, libera as duas árvores. Se a entrada acabar (Ctrl+D), também sai e libera.

### `main`

```c
int main(void)
{
    int n = 0;
    Jogada *v = ler_jogadas("jogadas_osu.csv", &n);

    menu(v, n);

    free(v);
    return 0;
}
```
Só chamadas de função, como o enunciado exige. Ao fim, toda a memória foi devolvida: as duas árvores pelo `menu`, as quatro do experimento pelo `experimento`, e o vetor pelo `free(v)`.

