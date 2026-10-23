# Reformulação da proposta após a conversa com o Prof. Elder (14/10/2026)

Aluno: João Maerbal Nunes Barros
RA: 202510288
Disciplina: Algoritmos e Estruturas de Dados, UFTM

Este documento registra tudo o que aconteceu depois da conversa com o professor na aula de 14/10/2026: o que ele disse, como cada ponto foi analisado, a arquitetura reformulada, o motivo de cada decisão e a verificação contra o enunciado. O conteúdo foi validado pelo João. Ele **substitui** as decisões técnicas e o plano do `TRABALHO.md` onde houver conflito.

Prazo: a proposta reformulada deve ser reenviada no Moodle **até 23:59 de 14/10/2026**.

---

## 1. A conversa com o professor

Na aula, o professor avisou que quem tinha a proposta aprovada podia começar a fazer e quem não tinha devia conversar com ele. O professor ainda não tinha visto o documento da proposta enviado anteriormente, então a conversa serviu para definir com ele o que o trabalho vai ser de fato. O João foi tirar dúvidas, e a conversa aconteceu em duas rodadas.

### 1.1 Primeira rodada

**Escopo.** O professor reduziu o escopo: além das operações obrigatórias do enunciado, fica **apenas a simulação**, "pra não ficar muita coisa".

**Operações obrigatórias.** Busca, inserção e remoção, que são o que está explícito no enunciado, seriam feitas sobre as jogadas do osu!.

**O pp como chave.** O professor apontou que o pp **não é uma informação inequívoca**, porque duas jogadas podem ter o mesmo pp. O João explicou que o CSV não tem pp repetido e que o critério de desempate seria o ID do mapa. O professor rebateu que isso **não reflete uma situação real**, com uma comparação: se duas pessoas tivessem o mesmo CPF, o juiz não olharia a idade para desempatar, porque ele só tem o CPF na mão. Um critério de desempate desse tipo seria esquecido depois e não estaria diferenciado no banco de dados.

**O peso.** O João explicou a simulação: cada jogada tem um peso, e uma top play nova influencia a pontuação geral com base nesse peso. Exemplo dado: fazer uma jogada de 200 pp quando a jogada número 1 vale 201 pp contribui muito mais para o perfil do que uma de 100 pp. O professor tratou o peso como uma **informação inequívoca**, porque depende da posição da jogada.

**O bônus.** O professor lembrou que existe um bônus pela quantidade de mapas jogados. Isso já estava coberto: a proposta desconsidera esse bônus.

**Conclusão da rodada, nas palavras do professor:** o pp não deve ser chave; o peso deve ser chave.

### 1.2 Segunda rodada

O João voltou, explicou e desenhou. O professor contra-argumentou:

- **Os decimais não entram como método.** Usar o pp em milésimos para tornar as chaves distintas não é um modo realista.
- O João explicou que **o ID de cada mapa é único**, mas que queria buscar jogadas acima de um valor, por exemplo acima de 200 pp.
- O professor respondeu que **isso se trata de árvore min-max**, que ele não deu. O conteúdo dado foi **árvore binária**.

### 1.3 O que ficou definido pela conversa

| Ponto | Posição do professor |
|---|---|
| Escopo extra | só a simulação |
| Busca por faixa de pp | fora (não é conteúdo dado) |
| pp como chave | recusado (não é inequívoco) |
| Desempate pelo ID do mapa | recusado como critério de desempate (não é realista) |
| pp em milésimos para distinguir chaves | recusado (não é realista) |
| Peso | informação inequívoca, depende da posição |
| Bônus por mapas jogados | já desconsiderado na proposta |

---

## 2. Análise depois da conversa

### 2.1 O peso como chave

O peso de uma jogada é 0,95^(n−1), em que n é a posição dela no ranking. Ele é de fato único para cada posição. O problema é que **o peso muda**: quando entra uma jogada nova no meio do ranking, todas as que estão abaixo descem uma posição e mudam de peso. Se o peso estivesse gravado em cada nó como chave, uma única inserção obrigaria a reescrever a chave de boa parte da árvore, e numa árvore de busca a chave de um nó não pode mudar depois que ele está lá.

Além disso, uma jogada nova ainda não tem posição antes de entrar. Na hora de inserir, a única informação disponível é o pp dela. Ou seja, o peso é **resultado** do cálculo, e não algo que se possa usar para posicionar o nó.

Conclusão: o peso é o que a simulação **calcula e mostra**, mas não serve como chave gravada.

### 2.2 Alternativas consideradas e descartadas

| Alternativa | Por que foi descartada |
|---|---|
| pp como chave, com desempate pelo ID do mapa | recusada pelo professor (critério de desempate artificial) |
| pp em milésimos para distinguir chaves | recusada pelo professor (não é realista) |
| pp como chave, com repetidos indo sempre para o mesmo lado | o professor recusou o pp como chave duas vezes; insistir colocaria a aprovação em risco |
| pp como chave, com uma lista de jogadas em cada nó | mesmo motivo acima |
| ID do score (identificador da jogada no banco do osu!) como desempate | continua sendo desempate do pp, que foi recusado |
| Peso como chave gravada | muda a cada inserção, ver 2.1 |
| Busca por faixa de pp | fora do conteúdo dado, segundo o professor |

### 2.3 A decisão: a chave é o ID do beatmap

O ID do beatmap é um número inteiro que identifica cada mapa no site do osu!. Ele foi escolhido porque responde a todas as objeções do professor:

- **É único.** O perfil guarda uma única jogada por mapa, então o ID identifica a jogada sem ambiguidade.
- **É real.** Não é um critério inventado nem um desempate. É o identificador que o próprio jogo usa, o "CPF" da jogada no perfil, na comparação do professor.
- **Não tem decimais.** É um inteiro.
- **Reflete o funcionamento do jogo.** Quando o jogador faz um score novo em um mapa, o osu! procura a jogada antiga daquele mapa e a substitui se a nova for melhor. Isso é exatamente buscar pelo ID, remover e inserir, que são as operações do enunciado.

O pp passa a ser **um dado da jogada**, como o nome e a data. A árvore nunca compara pp.

### 2.4 A dúvida de ter ID e pp no mesmo nó

Isso é o padrão de qualquer árvore de busca: o nó tem **uma chave** e **dados associados** a ela. Funciona como o cadastro de um aluno organizado pela matrícula: a árvore compara só a matrícula, e o nome e a nota vêm junto no nó, sem participar da ordem. Não há como isso ser barrado.

### 2.5 A simulação redefinida

**Entrada:** só um número de pp. Sem ID, sem mapa, nada mais.

**Saída:**

- a **posição** em que uma jogada com esse pp ficaria no perfil;
- o **pp pesado**, que é quanto essa jogada vale no perfil: x × 0,95^(n−1).

**Mecanismo:**

1. Percorrer a árvore uma vez, visitando todos os nós.
2. Contar quantas jogadas têm pp maior ou igual a x.
3. A posição é essa contagem mais um.
4. O pp pesado é x multiplicado por 0,95 elevado à posição menos um.

Em caso de pp igual ao de uma jogada existente, a jogada nova fica logo abaixo dela. A ordem entre duas jogadas de mesmo pp não altera nenhum valor do perfil, porque trocando as duas de lugar a soma fica igual.

### 2.6 O ganho real, considerado e deixado de fora

O pp pesado é o que a jogada nova **soma**. O total do perfil sobe um pouco menos que isso, porque as jogadas abaixo da nova descem uma posição e cada uma perde 5% do valor atual. Essa conclusão não está escrita na wiki com essas palavras; ela vem diretamente da fórmula p × 0,95^(n−1).

Exemplo verificado, com três jogadas de 400, 300 e 200 pp:

- Antes: 400 + 300 × 0,95 + 200 × 0,95² = 400 + 285 + 180,5 = **865,5**
- Entra uma de 350, na 2ª posição. O pp pesado dela é 350 × 0,95 = **332,5**
- Depois: 400 + 332,5 + 300 × 0,95² + 200 × 0,95³ = 400 + 332,5 + 270,75 + 171,475 = **1174,725**
- Ganho real: 1174,725 − 865,5 = **309,225**
- Diferença entre 332,5 e 309,225: 23,275, que é exatamente 5% de (285 + 180,5)

**Por que ficou fora:** para calcular o que as jogadas de baixo perdem, é preciso saber o valor atual de cada uma, e isso depende da posição de cada uma, o que exige ordenar as jogadas por pp. Com a árvore organizada pelo ID, isso significaria copiar os pp para um vetor e ordenar com `qsort`. O professor já tinha dito que "árvore não ordena", e ver um vetor sendo ordenado na simulação abriria espaço para a pergunta "então para que a árvore?".

**Decisão:** a proposta reformulada mostra só a posição e o pp pesado. O ganho real pode entrar depois como extra, se sobrar tempo com o obrigatório pronto, e só depois de conversar com o professor.

### 2.7 A simulação é O(n), e por que isso não é problema

A simulação precisa olhar todas as jogadas, porque a árvore está organizada pelo ID e não pelo pp. Ela custa O(n) nas duas árvores, independentemente do formato.

Isso não contradiz o uso de árvore, e a explicação usa a comparação do próprio professor: num cadastro organizado pelo CPF, achar, cadastrar ou remover uma pessoa pelo CPF é rápido. Mas a pergunta "quantas pessoas ganham mais de 5 mil reais?" exige olhar o cadastro inteiro, porque ele não está organizado pelo salário. No trabalho:

- **O ID do mapa é o CPF.** Inserir, buscar e remover pelo ID são rápidos. É isso que o enunciado avalia e o experimento mede.
- **O pp é o salário.** "Quantas jogadas têm mais de x pp?" exige olhar todas.

Além disso, percorrer a árvore é conteúdo obrigatório do enunciado: os três percursos e o cálculo de altura visitam todos os nós, ou seja, são O(n) por natureza.

**A proposta não escreve O(n) nem O(log n)** e **não lista a simulação entre as operações rápidas**. As operações que ela diz que precisam ser rápidas são a busca, a inserção e a remoção pelo ID.

---

## 3. Arquitetura reformulada

### 3.1 O nó

```c
typedef struct No {
    int id;            /* CHAVE: o único campo que a árvore compara */
    char nome[256];    /* dados da jogada: guardados, nunca comparados */
    double pp;         /* usado só na simulação */
    char data[32];
    int altura;        /* usado pela AVL para decidir as rotações */
    struct No *esq;    /* IDs menores */
    struct No *dir;    /* IDs maiores */
} No;
```

Decisão tomada depois: o pp fica como `double`, lido com `strtod`. Como não é mais chave, a precisão deixou de importar para a árvore, e a `converter_pp` sai do código.

### 3.2 O programa

```
jogadas_osu.csv  (200 jogadas, ordem cronológica)
        │
        ▼
  carregar_arquivo ──► BST (chave: ID do mapa)
                   └─► AVL (chave: ID do mapa)

MENU
 1. Inserir jogada ............ nas duas árvores (ID repetido é recusado)
 2. Buscar jogada pelo ID ..... mostra a jogada e as comparações
 3. Remover jogada pelo ID .... nas duas árvores (inclui 2 filhos)
 4. Percursos ................. em ordem, pré-ordem, pós-ordem
 5. Altura .................... BST e AVL
 6. Simular jogada de x pp .... posição e pp pesado
 7. Experimento comparativo ... cenários A e B, tabela
 0. Sair ...................... libera as duas árvores
```

### 3.3 Custo de cada operação

O custo das operações pela chave depende da **altura h** da árvore. Na AVL, h fica sempre perto de log n. Na BST, h depende da ordem em que as jogadas entraram e pode chegar a n.

| Operação | Como funciona | BST | AVL |
|---|---|---|---|
| Carregar o CSV | lê linha por linha e insere nas duas árvores | n inserções de O(h) | O(n log n) |
| Inserir | desce comparando o ID até um lugar vazio | O(log n) equilibrada, O(n) degenerada | O(log n), mais rotações O(1) |
| Buscar | desce comparando o ID, contando comparações | igual à inserção | O(log n) |
| Remover | acha pelo ID e trata os 3 casos | igual à inserção | O(log n), rebalanceando na volta |
| Percursos | visitam todos os nós | O(n) | O(n) |
| Altura | função recursiva | O(n) | O(n), ou O(1) lendo o campo da raiz |
| Simulação | percorre todos os nós contando pp ≥ x | O(n) | O(n) |
| Liberar | pós-ordem, um `free` por nó | O(n) | O(n) |

### 3.4 Os três casos da remoção

```
1. folha        → free no nó, o pai aponta para NULL
2. um filho     → o pai passa a apontar para o filho, free no nó
3. dois filhos  → acha o sucessor (menor ID da subárvore direita),
                  copia TODOS os dados dele (id, nome, pp, data) para o nó,
                  remove o sucessor da subárvore direita (cai no caso 1 ou 2)
```

Na AVL, a altura é atualizada e o balanceamento é verificado na volta da recursão.

### 3.5 O mecanismo da simulação

```c
/* Percorre a árvore inteira e conta as jogadas com pp >= x.
 * O formato da árvore não importa: todo nó é visitado uma vez, então é O(n). */
void contar_acima(No *raiz, double x, int *acima)
{
    if (raiz == NULL) return;
    if (raiz->pp >= x) (*acima)++;   /* pp igual conta como acima */
    contar_acima(raiz->esq, x, acima);
    contar_acima(raiz->dir, x, acima);
}

/* Mostra a posição de uma jogada de x pp e quanto ela valeria no perfil. */
void simular(No *raiz, double x)
{
    int acima = 0, n, i;
    double peso = 1.0;

    contar_acima(raiz, x, &acima);
    n = acima + 1;

    for (i = 1; i < n; i++)          /* peso = 0,95^(n-1), sem pow() */
        peso = peso * 0.95;

    printf("Posição: %d | valor no perfil: %.2f pp\n", n, x * peso);
}
```

**Sem `pow()`:** o `pow` está no `math.h` e exige `-lm` na compilação. O comando do enunciado, `gcc -fsanitize=address -g -Wall -o trabalho trabalho.c`, não tem `-lm`, então com `pow` o programa poderia nem compilar com o comando do professor. O laço resolve sem biblioteca extra.

**Situações da simulação:**

| Situação | Resultado |
|---|---|
| x maior que todas as jogadas | posição 1, vale x inteiro |
| x menor que todas | posição n+1, última |
| x igual ao pp de alguma jogada | fica logo abaixo dela |
| árvore vazia | posição 1 |
| x negativo ou zero | o menu recusa e pede de novo |

**Exemplo com 5 jogadas reais do perfil (pp arredondado):**

```
                 [3466914 | 398 pp]
                 /                \
      [2327059 | 399 pp]      [4397861 | 440 pp]
                \                       \
          [3221529 | 448 pp]      [4834142 | 433 pp]
```

A árvore está ordenada pelo ID; os pp ficam espalhados sem ordem. Para x = 420, o `contar_acima` visita os nós em pré-ordem (raiz, esquerda, direita):

```
visita os 5 nós:   398  399  448  440  433
pp >= 420?          não  não  sim  sim  sim   → acima = 3
posição n = 4
pp pesado = 420 × 0,95³ = 420 × 0,857375 = 360,10 pp
```

### 3.6 O experimento comparativo

```
Cenário A: insere as jogadas na ordem do arquivo (cronológica)
Cenário B: copia as jogadas para um vetor, ordena pelo ID com qsort,
           insere nessa ordem

Para cada cenário e cada árvore:
   zera o contador (criado fora da busca, passado por ponteiro)
   busca cada jogada pelo ID, somando as comparações
   anota a altura, o total e a média
Libera as 4 árvores e imprime a tabela.
```

O vetor ordenado aqui não é problema: o próprio enunciado manda ordenar os registros pela chave no Cenário B.

**O que esperar com 200 jogadas:**

- **BST no Cenário B:** vira uma lista. Altura 200; buscar a k-ésima jogada custa k comparações; total 1 + 2 + … + 200 = **20.100** comparações, média **100,5**.
- **AVL nos dois cenários:** altura entre 8 e 10 (com 200 nós, a altura mínima possível é 8 e a máxima de uma AVL é 10), poucas comparações por busca.
- **BST no Cenário A:** depende de quanto a ordem por data se parece com a ordem por ID. Mapas mais novos têm ID maior, e no CSV a correlação entre a posição no arquivo e o ID é de cerca de 0,25: fraca, mas positiva. A BST não deve degenerar, mas deve ficar mais alta que a AVL. É o experimento que vai mostrar.

---

## 4. Verificação contra o enunciado

| Requisito do enunciado | Como o programa atende | |
|---|---|---|
| Contexto real com fonte | perfil do osu! do João, link do perfil e da wiki | ✓ |
| Problema em uma frase | 1ª frase da seção 2 da proposta | ✓ |
| Operações que precisam ser rápidas | busca, inserção e remoção pelo ID | ✓ |
| Chave de ordenação | ID do beatmap | ✓ |
| Volume de dados | 200 jogadas; estrutura serve para o histórico | ✓ |
| Mínimo de 30 registros reais | 200 jogadas reais no CSV | ✓ |
| Motivo concreto para usar árvore | achar, substituir e remover a jogada de um mapa | ✓ |
| BST e AVL sobre os mesmos dados | as duas carregadas do mesmo CSV | ✓ |
| Rotações simples e duplas | na inserção e na remoção da AVL | ✓ |
| Nós alocados dinamicamente | `malloc` no `criar_no` | ✓ |
| Inserir, buscar e remover nas duas | pelo ID, nas duas árvores | ✓ |
| Remoção com dois filhos | caso 3, com o sucessor | ✓ |
| Percursos em ordem, pré e pós | três funções recursivas | ✓ |
| Cálculo de altura | função recursiva | ✓ |
| Carga dos dados reais | `jogadas_osu.csv` | ✓ |
| Navegação com ponteiros e `->` | em toda a árvore | ✓ |
| Nada desenvolvido no main | o main só chama o menu e libera | ✓ |
| Sem variáveis globais | tudo por parâmetro | ✓ |
| `malloc` testado contra NULL | no `criar_no` | ✓ |
| `free` em todos os nós, inclusive removidos | na remoção e no `liberar_arvore` | ✓ |
| Menu interativo | todas as operações e a simulação | ✓ |
| Só a biblioteca padrão | stdio, stdlib, string; sem `math.h` | ✓ |
| Experimento com contador por ponteiro | contador criado fora da busca | ✓ |
| Cenários A e B, tabela, liberar as 4 | conforme 3.6 | ✓ |

**Verificação contra as objeções do professor:**

| Objeção | Como a reformulação responde | |
|---|---|---|
| pp não é inequívoco | pp deixa de ser chave; vira dado da jogada | ✓ |
| Desempate artificial não é realista | não há desempate: o ID é único por natureza | ✓ |
| Decimais não são método | a chave é inteira; os decimais do pp não afetam a árvore | ✓ |
| Busca por faixa é min-max | busca por faixa removida | ✓ |
| Só a simulação como extra | a simulação é o único extra | ✓ |
| "Árvore não ordena" | a simulação não ordena nada; só conta num percurso | ✓ |
| Bônus por mapas jogados | desconsiderado, como já dizia a proposta | ✓ |

---

## 5. O que muda em relação ao plano anterior (`TRABALHO.md`)

**Sai:**

- chave composta (pp em milésimos, ID do beatmap);
- pp em milésimos como requisito da chave;
- nós aumentados com tamanho, fator e soma ponderada;
- busca por intervalo de pp;
- cálculo do ganho em O(log n) com os campos aumentados;
- bug proposital baseado na atualização dos campos aumentados.

**Entra:**

- chave = ID do beatmap (inteiro);
- pp como dado da jogada (`double`);
- simulação por percurso: posição e pp pesado;
- cálculo do peso com laço, sem `pow()`.

**Novo bug proposital sugerido para o vídeo** (escolher um):

1. **Na rotação simples da AVL, esquecer de atualizar a altura do nó que desce.** A árvore continua parecendo ordenada no percurso em ordem, mas o balanceamento passa a decidir errado, e a altura da AVL no experimento cresce.
2. **Na remoção com dois filhos, copiar só o ID do sucessor**, sem o nome, o pp e a data. A busca pelo ID passa a mostrar a jogada com dados de outra, e a simulação passa a contar um pp errado.

**Arquivos:**

- `jogadas_osu.csv`: não precisa mudar. Já tem o ID e o pp.
- `trabalho.c` (etapa 1): precisa de ajuste. A comparação passa a ser só pelo ID, o pp vira dado comum (`double`), a `converter_pp` sai, e os campos tamanho, fator e soma saem da struct.
- `TRABALHO.md` e `CLAUDE.md`: atualizados com este documento.

---

## 6. Respostas prontas para a defesa

**"Por que o ID do mapa?"**
O perfil guarda uma jogada por mapa, então o ID identifica a jogada sem ambiguidade. Quando eu faço um score novo, o jogo procura a jogada antiga daquele mapa pelo ID e substitui. É isso que a árvore faz.

**"E o pp?"**
É só um dado da jogada, igual ao nome e à data. A árvore nunca compara pp.

**"A simulação percorre tudo, então para que árvore?"**
É como um cadastro por CPF: achar uma pessoa é rápido, mas contar quantas ganham mais que um valor exige olhar todas. A minha árvore acha a jogada pelo ID rápido, e a simulação conta quantas têm mais pp que a nova.

**"E se dois pp forem iguais na simulação?"**
A jogada nova fica logo abaixo da existente. A ordem entre as duas não muda a pontuação, porque elas valem o mesmo.

**"O que a AVL resolve que a BST não resolve?"**
Se as jogadas entrarem em ordem crescente de ID, a BST vira uma lista e cada busca passa por quase todas as jogadas. A AVL faz rotações e continua equilibrada. O experimento mostra isso na tabela.

**Limitação para o fechamento do vídeo:**
Para a simulação ser rápida, a árvore teria que estar organizada pelo pp, mas o pp não identifica uma jogada. Escolhi a chave que identifica a jogada de verdade, e a simulação percorre a árvore.

---

## 7. Proposta reformulada (texto enviado)

**Proposta de trabalho: Algoritmos e Estruturas de Dados**
Nome: João Maerbal Nunes Barros
RA: 202510288
Tema: Gerenciamento das jogadas do meu perfil do osu! com BST e AVL e simulação do valor de uma jogada nova.

**1. Contexto**
O osu! é um jogo de ritmo em que cada jogada em um beatmap recebe uma pontuação chamada pp, ou Performance Points. Sou jogador e uso os dados do meu próprio perfil. Cada jogada contribui para a pontuação geral com p × 0,95^(n−1), em que p é o pp da jogada e n é a sua posição entre as melhores jogadas do jogador. O perfil guarda só a melhor jogada de cada beatmap, então um score melhor em um mapa já jogado substitui o anterior. Desconsidero o pequeno bônus pela quantidade de beatmaps jogados.

**2. O problema**
O jogador quer saber em que posição uma jogada de x pp entraria no seu perfil e quanto ela valeria com o peso dessa posição. Para isso, o perfil precisa estar sempre atualizado, e precisam ser rápidas a busca da jogada de um mapa, a inserção de jogadas novas e a remoção de jogadas superadas. Em um vetor ou lista desordenada, encontrar a jogada de um mapa exige percorrer todas, e em um vetor ordenado cada inserção ou remoção desloca elementos, enquanto na árvore o custo é proporcional à altura. A BST e a AVL são construídas a partir do mesmo arquivo e implementam as mesmas operações. O experimento comparativo insere as jogadas em dois cenários, na ordem cronológica do arquivo e em ordem crescente de ID, caso em que a BST degenera em lista, problema que a AVL corrige com rotações. A simulação percorre as jogadas guardadas na árvore e conta quantas têm pp maior ou igual a x. A jogada nova entra na posição seguinte, n, e o seu valor é x × 0,95^(n−1).

**3. Chave e volume**
A chave é o ID do beatmap, um número inteiro que identifica cada mapa no site do osu!. Como o perfil guarda uma única jogada por mapa, o ID identifica a jogada sem ambiguidade e sem critério de desempate. O pp não serve como chave porque duas jogadas podem ter o mesmo valor, por isso é guardado apenas como dado da jogada. O peso de cada jogada também não serve como chave, porque depende da posição e muda sempre que uma jogada nova entra acima dela. Ele é o resultado que a simulação calcula. O arquivo tem as 200 jogadas da seção Melhor Performance, mas a estrutura serve para todo o histórico do jogador, que cresce com o tempo de jogo.

**4. Dados**
As jogadas estão no arquivo jogadas_osu.csv, em anexo, com as colunas nome do beatmap, ID do beatmap, pp e data, separadas por ponto e vírgula e em ordem cronológica.
Fonte: dados coletados do meu perfil (https://osu.ppy.sh/users/13699780) em 14/10/2026. Fórmula e funcionamento do pp conforme a wiki oficial do osu!: https://osu.ppy.sh/wiki/en/Performance_points.

---

## 8. Pendências

- [x] Reenviar no Moodle até 23:59 de 14/10/2026 o PDF da proposta reformulada e o `jogadas_osu.csv` atual (3 casas), removendo o CSV antigo de 2 casas.
- [x] Confirmar que a data da coleta no texto (14/10/2026) é a data em que o CSV foi gerado.
- [x] Atualizar o `TRABALHO.md` e o `CLAUDE.md` com este documento.
- [ ] Ajustar o `trabalho.c` da etapa 1 para a chave pelo ID.
- [ ] Na próxima aula, confirmar rapidamente com o professor que o ID do mapa como chave está ok.
