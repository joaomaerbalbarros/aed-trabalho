# Roteiro do vídeo e respostas do enunciado

Trabalho de AED: BST e AVL com as jogadas do osu!
Aluno: João Maerbal Nunes Barros, RA 202510288

Este arquivo é o roteiro do vídeo, na ordem que o enunciado pede. Cada parte está dividida em blocos, e cada bloco tem duas partes: **Tela**, com o que aparece na tela naquele momento, e **Fala**, com o que dizer. As falas estão escritas para serem ditas em voz alta; adapte para as suas palavras. No fim estão as perguntas que o professor pode fazer e o que vai no relatório.

As linhas citadas valem para o `trabalho.c` final (720 linhas). As explicações detalhadas de cada função estão em `docs/REVISAO_CODIGO.md`.

**Meta de duração:** uns 20 a 22 minutos, para ficar com folga dentro da faixa de 15 a 25. Cronometre um ensaio.

---

## Antes de gravar

- [ ] **Conferir que o `trabalho.c` está sem nenhum `//` de teste.** Em 18/10 a linha `liberar_arvore(bst);` da função `menu` (linha 706) estava comentada por causa do teste do print. Com ela comentada, o ASan acusa vazamento na demonstração.
- [ ] O seu rosto aparece num canto da tela durante o vídeo inteiro.
- [ ] Aumentar o zoom do VS Code com **Ctrl+=** e a fonte do terminal, para o código ficar legível.
- [ ] No VS Code, **Ctrl+Shift+O** abre a lista de funções do arquivo. Digite o nome e pule direto, sem rolar o código procurando.
- [ ] Rodar `clear` antes de cada execução. O terminal do VS Code às vezes esconde a última linha da saída (está no Diário de Bordo). Melhor ainda: usar o **Terminal** do Linux Mint, separado.
- [ ] Compilar **pelo terminal**, digitando o comando, nunca pelo botão Run, para o comando aparecer no vídeo.
- [ ] Conferir que o `jogadas_osu.csv` está na pasta onde o programa roda.
- [ ] Apagar o `saida.txt` que sobrou na pasta (`rm saida.txt`).
- [ ] Deixar abertos: o `trabalho.c`, o terminal, o perfil do osu! (https://osu.ppy.sh/users/13699780), a wiki do pp (https://osu.ppy.sh/wiki/en/Performance_points) e a página dos desenhos.
- [ ] As partes 4 e 5 são gravadas **sem cortes**.
- [ ] A **página dos desenhos** ("Jogadas osu! em BST e AVL") está organizada pelos mesmos números de bloco deste roteiro. A barra no topo tem um botão para cada bloco. A regra é sempre a mesma: primeiro a página mostra a ideia, depois o VS Code mostra onde ela acontece no código. Aumente o zoom do navegador com **Ctrl+=**.

**Onde cada pergunta do enunciado é respondida:**

| Pergunta do enunciado | Bloco |
|---|---|
| Qual é o problema real e qual é a fonte? | 1.1 a 1.3 |
| Por que escolheu esse problema? | 1.4 |
| Por que essa chave? | 2.1 |
| Por que árvore, e não vetor ou lista? | 2.2 |
| O que a AVL resolve que a BST não resolve? | 2.3 |
| Struct do nó | 3.1 |
| Alocação dinâmica | 3.2 |
| Rotação | 3.5 e 3.6 |
| Remoção com dois filhos | 3.7 |
| Onde está cada free | 3.8 |
| Demonstração com AddressSanitizer | 4.1 a 4.5 |
| Por que os números do experimento ficaram assim | 4.4 |
| Bug proposital | 5.1 a 5.3 |
| Limitações | 6.1 |
| E com 1000 vezes mais dados | 6.2 |

---

## Parte 1: Introdução (2 a 3 min)

### 1.1 O jogo

**Tela**
- 10 a 15 segundos de jogatina do osu!.

**Fala**
> Esse aqui é o osu!, um jogo de ritmo de computador que eu jogo há 7 anos. Cada música tem os seus mapas, que no jogo se chamam beatmaps, e cada vez que eu termino um mapa, aquilo vira uma jogada, com a minha pontuação.

### 1.2 O pp e o perfil

**Tela**
- O seu perfil no navegador (https://osu.ppy.sh/users/13699780), na lista de Melhor Performance.
- Aponte com o mouse o número rosa e o número branco de uma jogada.

**Fala**
> A pontuação mais importante do jogo é o pp, de Performance Points. Ela é calculada pela dificuldade do mapa e por quão bem eu joguei. Aqui é o meu perfil, e é dele que saem os dados do trabalho. O perfil guarda a minha melhor jogada em cada mapa e monta um ranking com elas, da que vale mais pp para a que vale menos. Esse número rosa é o pp da jogada, e esse número branco é quanto ela vale de fato no meu perfil.

### 1.3 O peso e a fonte

**Tela**
- A wiki oficial do pp (https://osu.ppy.sh/wiki/en/Performance_points), na fórmula do peso.

**Fala**
> O pp total do perfil, que define a minha posição no ranking mundial, não é a soma simples dessas jogadas. Cada uma entra com um peso que depende da posição: a primeira vale 100%, a segunda 95%, a terceira 95% de 95%, e assim por diante. Essa é a fórmula da wiki oficial do jogo. Os dados do trabalho são as minhas 200 melhores jogadas, coletadas do meu perfil pela API oficial do jogo em 14/10/2026.

### 1.3b O meme do 727 (opcional, no máximo 30 segundos)

**Tela**, nesta ordem:
1. Alguns segundos da live do Shigetora (Cookiezi) jogando o Blue Zenith.
2. A tela de resultado: xi – Blue Zenith [FOUR DIMENSIONS] +HR, 727 pp, 03.01.2016.
3. A tela de resultado do Aireu em Guess Who Is Back, 12/22/2020. Aponte com o mouse o **727pp (+258pp)** na linha Beatmap Ranking e depois o **15,042pp (+26pp)** na linha Overall Ranking.

**Fala**
> O pp ficou tão importante na comunidade que virou meme. Em 2016, o Cookiezi fez esta jogada no Blue Zenith, e ela valeu 727 pp. Em 2020, o Aireu fez exatamente 727 pp em outro mapa e apontou para a tela, e daí veio o "When You See It".
>
> Mas olha este detalhe: a jogada vale 727 pp, e o perfil dele subiu só 26. Por dois motivos. Primeiro, ele já tinha uma jogada nesse mapa, de 469 pp, e a nova substituiu a antiga, porque o perfil guarda só a melhor jogada de cada mapa. Segundo, ele já tinha jogadas melhores, então essa entrou mais abaixo no ranking, com um peso menor, e as jogadas abaixo dela desceram uma posição. O meu trabalho faz a primeira parte dessa conta: descobre em que posição a jogada entra e quanto ela vale com o peso dessa posição.

Os números da fala saem da própria tela: 727 − 258 = 469 é o valor da jogada antiga naquele mapa, e +26 é quanto o total do perfil subiu. Não cite posição nem peso exatos da jogada do Aireu: eles não aparecem na tela e não foram confirmados. E não diga que o programa calcula o +26: quanto o perfil inteiro sobe é a limitação do bloco 6.1.

### 1.4 O problema e por que eu escolhi

**Tela**
- Volta para o perfil, ou só o seu rosto.

**Fala**
> Então, quando eu faço uma jogada nova, a pergunta que todo jogador faz é: em que posição ela entraria e quanto ela valeria? É isso que o meu trabalho responde. Eu escolhi esse problema porque ele é meu de verdade: eu jogo há 7 anos, os dados são do meu próprio perfil, e essa é uma pergunta que eu mesmo faço quando vou tentar uma jogada.
>
> As jogadas do meu perfil ficam guardadas em duas árvores, uma BST e uma AVL, e em cima delas o programa faz a simulação: eu digito um valor de pp, e ele me diz em que posição essa jogada entraria e quanto ela valeria com o peso dessa posição.

---

## Parte 2: Por que árvore? (3 min)

### 2.1 A chave

**Tela**
- Página, seção **2.1**: a tabela do arquivo e os três cartões (ID serve, pp não serve, peso não serve).

**Fala**
> A chave de cada nó é o ID do beatmap, um número inteiro que o próprio jogo usa para identificar cada mapa. Como o perfil guarda uma única jogada por mapa, o ID identifica a jogada sem ambiguidade, como um CPF. Cada nó guarda também o nome do mapa, o pp e a data, mas a árvore só compara o ID.
>
> Eu não usei o pp como chave porque duas jogadas podem ter o mesmo pp, e eu teria que inventar um critério de desempate. Também não usei o peso, porque ele depende da posição e muda toda vez que entra uma jogada nova acima. O peso é o resultado que a simulação calcula.
>
> E isso reflete o funcionamento do jogo: quando eu melhoro o score de um mapa, o jogo procura a jogada antiga daquele mapa e substitui. Buscar, remover e inserir pelo ID são exatamente as operações da árvore.

Diga aqui, uma vez: "essa página eu montei com apoio de IA para ilustrar o funcionamento".

### 2.2 Árvore contra vetor e lista

**Tela**
- Página, seção **2.2**: a tabela de vetor, lista e árvore.

**Fala**
> No vetor desordenado e na lista, achar a jogada de um mapa exige olhar todas. No vetor ordenado a busca é rápida, mas cada inserção ou remoção empurra os elementos. A árvore é a única que faz as três operações com custo proporcional à altura, e na AVL essa altura fica sempre perto de log n.

### 2.3 O que a AVL resolve

**Tela**
- Página, seção **2.3**: a BST virando lista ao lado da AVL, e o aviso de que a AVL não acelera a simulação.

**Fala**
> A BST depende da ordem em que as jogadas chegam. Se os IDs chegarem em ordem crescente, cada jogada nova vai para a direita da anterior, e a árvore vira uma lista, como aqui: buscar uma jogada passa a percorrer quase todas. A AVL guarda a altura de cada nó e, depois de cada inserção ou remoção, confere se algum nó ficou desequilibrado. Se ficou, ela faz rotações, e a altura volta para perto de log n. Então a AVL garante busca, inserção e remoção rápidas em qualquer ordem de chegada.
>
> Mas tem uma coisa que a AVL não resolve: a simulação. As duas árvores estão ordenadas pelo ID, então as duas precisam olhar todas as jogadas para contar quantas têm pp maior que o da jogada nova.

---

## Parte 3: Código (7 a 8 min)

É a parte que mais pesa na nota. A leitura do CSV, o menu e as funções de impressão não precisam aparecer aqui; elas aparecem funcionando na demonstração.

### 3.1 A struct do nó

**Tela**
- Página, seção **3.1**: o nó desenhado com cada campo.
- Depois, no VS Code: `typedef struct No` (linhas 19–32). Aponte o `id`, depois os dados, a `altura` e os ponteiros `esq` e `dir`.

```c
typedef struct No {
    int id;                 /* CHAVE: o unico campo que a arvore compara */
    char nome[TAM_NOME];
    double pp;              /* usado so na simulacao */
    char data[TAM_DATA];
    int altura;
    struct No *esq;         /* IDs menores */
    struct No *dir;         /* IDs maiores */
} No;
```

**Fala**
> Cada nó é uma jogada. O ID é a chave, e é o único campo que a árvore compara para decidir se vai para a esquerda ou para a direita. O nome, o pp e a data são os dados da jogada, que vêm junto. A altura é usada pela AVL para saber se precisa rotacionar. Os dois ponteiros apontam para os filhos: à esquerda ficam os IDs menores e à direita os maiores.

### 3.2 A alocação dinâmica

**Tela**
- Sem página aqui. Direto no VS Code: `criar_no` (linhas 52–71).
- Aponte o `malloc` e, logo abaixo, o teste contra `NULL`.

**Fala**
> Eu não sei de antemão quantas jogadas vão existir: o arquivo pode crescer e o usuário pode inserir mais pelo menu. Por isso cada nó é criado na hora em que a jogada entra, com malloc, que reserva o tamanho exato de um nó. Logo depois eu testo se o retorno é NULL, porque se a memória acabar o programa não pode seguir usando um ponteiro inválido. Os dois ponteiros começam em NULL, porque todo nó novo entra como folha. Cada nó criado aqui vai ter que ser liberado com free em algum momento.

Se perguntarem: o `(No *)` antes do `malloc` converte o `void *`, o endereço genérico que o `malloc` devolve, para "endereço de um nó". Em C ele é opcional. O nó ocupa 328 bytes: os campos somam 320, e o compilador acrescenta 8 de alinhamento.

### 3.3 Inserir e buscar na BST

**Tela**
- Página, seção **3.3**: as cinco inserções na ordem do arquivo e a busca do mapa 2327059.
- Depois, no VS Code: `inserir_bst` (linhas 73–95) e `buscar` (linhas 310–324), só de passagem. Na inserção, aponte a linha `raiz->esq = inserir_bst(raiz->esq, ...)`. Na busca, aponte o `(*comparacoes)++`.

**Fala**
> A inserção desce a árvore comparando o ID: se é menor, vai para a esquerda; se é maior, vai para a direita, até achar um lugar vazio, onde o nó é criado. Na volta da recursão, o endereço do nó novo é gravado no ponteiro do pai, por esta linha. Se o ID já existe, a jogada é recusada, porque o perfil só tem uma jogada por mapa. A busca faz o mesmo caminho. Cada nó visitado é uma comparação, e é esse contador, passado por ponteiro, que o experimento usa.

### 3.4 A inserção na AVL

**Tela**
- Página, seção **3.4**: as cinco inserções na AVL, com as duas rotações marcadas.
- Depois, no VS Code: `inserir_avl` (linhas 286–308). Aponte a última linha, `return balancear(raiz);`, que é a única diferença para a `inserir_bst`.

**Fala**
> A inserção da AVL faz exatamente a mesma descida. A única diferença é esta última linha: em vez de devolver o nó direto, ela chama o balanceamento. Como a volta da recursão passa por todos os nós do caminho, de baixo para cima, cada um atualiza a própria altura e se equilibra, se precisar. Buscar, percorrer, calcular a altura e simular são as mesmas funções para as duas árvores. Toda a diferença entre BST e AVL está na inserção, na remoção e nas rotações.

### 3.5 A rotação

**Tela**
- Página, seção **3.5**: o antes e depois com A, B e C, e as linhas comentadas.
- Depois, no VS Code: `rotacao_esquerda` (linhas 253–262). Aponte as três ligações de ponteiro e depois as duas atualizações de altura.

```c
No *rotacao_esquerda(No *raiz)
{
    No *nova = raiz->dir;
    raiz->dir = nova->esq;
    nova->esq = raiz;
    atualizar_altura(raiz);
    atualizar_altura(nova);
    return nova;
}
```

No código, o nó A da página é a `raiz`, e o B é a `nova`.

**Fala**
> Vou explicar a rotação à esquerda, que é a que aparece quando os IDs chegam em ordem crescente. O nó A, que no código é a raiz, ficou com a direita mais alta que a esquerda. O filho direito, B, que no código é a nova, sobe para o lugar de A, e A desce e vira o filho esquerdo de B. Se B tinha um filho esquerdo, ele passa a ser o filho direito de A, porque esses IDs são maiores que A e menores que B. Depois eu atualizo as alturas, primeiro de A, que desceu, e depois de B, que subiu, porque a altura de B depende da de A. A função devolve B, que é a nova raiz dessa parte da árvore.
>
> Repare que, lendo da esquerda para a direita, a ordem dos IDs é a mesma antes e depois. Só muda quem está mais alto. Por isso a rotação nunca quebra a regra da árvore de busca.

### 3.6 O balanceamento e a rotação dupla

**Tela**
- Página, seção **3.6**: o zigue-zague e as duas rotações.
- Depois, no VS Code: `balancear` (linhas 264–284). Aponte o `fator_balanco` e os dois `if` que decidem entre rotação simples e dupla.

**Fala**
> O balanceamento calcula o fator de balanço: a altura da esquerda menos a altura da direita. Se passar de 1 para um dos lados, ele rotaciona. Quando o desequilíbrio está em linha reta, uma rotação simples resolve. Quando está em zigue-zague, por exemplo o nó pesa para a direita mas o filho direito pesa para a esquerda, primeiro eu rotaciono o filho para alinhar e depois o nó. Essa é a rotação dupla. Para saber qual é o caso, basta olhar o fator do filho do lado pesado.

### 3.7 A remoção com dois filhos

**Tela**
- Página, seção **3.7**: o antes e depois, com o sucessor marcado.
- Depois, no VS Code: `remover_bst` (linhas 364–398), no trecho do caso de dois filhos (linhas 386–396). Aponte o `menor_no`, a cópia dos quatro campos e a chamada recursiva.

```c
        suc = menor_no(raiz->dir);
        raiz->id = suc->id;
        strcpy(raiz->nome, suc->nome);
        raiz->pp = suc->pp;
        strcpy(raiz->data, suc->data);
        raiz->dir = remover_bst(raiz->dir, suc->id, removido);
```

**Fala**
> Se o nó tem dois filhos, ele não pode simplesmente sair, porque os dois filhos ficariam sem pai. Então eu procuro o sucessor, que é o menor ID da subárvore direita: desço uma vez para a direita e depois tudo para a esquerda. Copio todos os dados do sucessor para o nó, ID, nome, pp e data. Se eu copiasse só o ID, o nó ficaria com o nome e o pp de outra jogada. Depois removo o sucessor da subárvore direita. O sucessor nunca tem filho à esquerda, senão ele não seria o menor, então essa segunda remoção sempre cai no caso fácil, de folha ou de um filho. A remoção da AVL, a remover_avl, é igual, com o balanceamento na volta.

### 3.8 Onde está cada free

**Tela**
- Página, seção **3.8**: a ordem dos frees em pós-ordem.
- Depois, no VS Code: **Ctrl+F** buscando `free(`. O VS Code destaca todas as ocorrências. Passe por elas uma a uma.

| Linha | Onde | O que libera |
|---|---|---|
| 378 | `remover_bst`, caso 1 | a folha removida |
| 384 | `remover_bst`, caso 2 | o nó com um filho removido |
| 415 | `remover_avl`, caso 1 | a folha removida |
| 420 | `remover_avl`, caso 2 | o nó com um filho removido |
| 462 | `liberar_arvore` | cada nó, em pós-ordem |
| 533 | `experimento` | a cópia ordenada do vetor |
| 718 | `main` | o vetor de jogadas lido do arquivo |

**Fala**
> Aqui estão todos os frees do programa. Estes quatro são da remoção, na BST e na AVL: o nó que sai da árvore é liberado na hora. No caso de dois filhos, quem sai é o sucessor, que cai num desses casos. Este é o liberar_arvore: ele percorre a árvore em pós-ordem, primeiro os filhos e depois o pai, porque se eu liberasse o pai antes, perderia os ponteiros para os filhos e eles vazariam. Ele libera as duas árvores do menu quando o programa fecha e as quatro árvores do experimento logo depois de medidas. Este outro libera a cópia do vetor que eu ordeno para o Cenário B, e este último, no main, o vetor com as jogadas do arquivo.

### 3.9 A simulação

**Tela**
- Página, seção **3.9**: a árvore com a ordem de visita e a conta da posição e do peso.
- Depois, no VS Code: `contar_acima` (linhas 326–335) e `simular` (linhas 337–352). Aponte o percurso que visita todos os nós e o laço do peso.

**Fala**
> A simulação percorre a árvore inteira e conta quantas jogadas têm pp maior ou igual ao da jogada nova. A posição é essa contagem mais um, e o peso é 0,95 multiplicado por ele mesmo uma vez para cada posição acima. Eu calculo o peso com esse laço, sem a função pow, porque o pow precisa de uma biblioteca a mais na compilação.

---

## Parte 4: Demonstração ao vivo, sem cortes (4 a 5 min)

### 4.1 Compilar

**Tela**
- Terminal, digitando: `clear && gcc -fsanitize=address -g -Wall -o trabalho trabalho.c && ./trabalho`

**Fala**
> Vou compilar com o AddressSanitizer, que vigia a memória enquanto o programa roda. Se tiver vazamento ou acesso inválido, ele avisa no fim. O programa lê as 200 jogadas do arquivo e monta as duas árvores.

### 4.2 As operações

**Tela**, nesta ordem (sequência já testada):
1. **Opção 5** (altura): BST 17 e AVL 9.
2. **Opção 2** (buscar) com `2828520`: aparece o Letters to me, com as comparações da BST e da AVL lado a lado.
3. **Opção 1** (inserir) com `2828520`: o programa recusa e mostra o pp antigo.
4. **Opção 1** com um ID novo, por exemplo `5000000`, um nome qualquer, `450` de pp e uma data. Depois a **opção 2** com `5000000`, para mostrar que ela entrou.
5. **Opção 3** (remover) com `1925409` (Shitteru): é a **raiz da BST**, então tem dois filhos. Depois a **opção 2** com `1936713` (Believe in ourselves), o sucessor: na BST ele é achado com **1 comparação**, porque os dados dele foram copiados para o nó da raiz.
6. **Opção 3** com `5000000`, e a **opção 2** de novo: não existe mais.
7. **Opção 4** (percursos): a AVL em pré-ordem. A primeira linha é a raiz da AVL. Não precisa rolar as 200 linhas.
8. Uma resposta errada: `abc` no lugar da opção. O programa pede de novo.

**Fala**
> Narre o que está fazendo: qual ID está buscando, quantas comparações deu em cada árvore, qual jogada removeu e por que ela tinha dois filhos.

### 4.3 A simulação

**Tela**
- **Opção 6**, primeiro com `727`, depois com `400`, depois com `350`.

| Valor | Posição | Valor com o peso |
|---|---|---|
| 727 | 1 | 727,000 pp |
| 400 | 6 | 309,512 pp |
| 350 | 66 | 12,477 pp |

**Fala**
> Uma jogada de 727 pp seria a minha melhor, então entraria em primeiro e valeria inteira. Uma de 400 entraria em sexto e valeria uns 309. E olha uma de 350, que parece boa: ela entraria só em 66º, e com o peso dessa posição valeria só 12 pp. É por isso que, depois de um tempo, só jogadas muito boas fazem diferença no perfil.

Esses números são do CSV entregue, sem nenhuma alteração. Na sequência do bloco 4.2, a jogada de teste (5000000) já foi removida, mas o Shitteru (1925409, 351,049 pp) também: por isso, se a simulação vier depois do 4.2, o 727 e o 400 continuam iguais, mas o **350 entra em 65º e vale 13,133 pp**. Ajuste a fala, ou faça a simulação antes das remoções.

### 4.4 O experimento

**Tela**
- **Opção 7** e a tabela impressa pelo programa.
- Depois, se quiser reforçar: página, seção **4.4**, com a explicação dos cenários.

| Cenário | Árvore | Altura | Comparações | Média |
|---|---|---|---|---|
| B (ordem crescente) | BST | 200 | 20100 | 100,50 |
| B (ordem crescente) | AVL | 8 | 1353 | 6,76 |
| A (ordem original) | BST | 17 | 1888 | 9,44 |
| A (ordem original) | AVL | 9 | 1368 | 6,84 |

**Fala**
> No Cenário B, as jogadas entram em ordem crescente de ID. A BST recebe sempre um ID maior que todos os anteriores, então ela vira uma lista com altura 200. Buscar a jogada que está na posição k custa k comparações, então o total é 1 + 2 + … + 200, que dá 20.100, com média de 100,5. A AVL, com a mesma entrada, ficou com altura 8, que é a menor possível para 200 jogadas, e a média caiu para menos de 7: quase 15 vezes menos comparações.
>
> No Cenário A, as jogadas entram na ordem do arquivo, que é por data. Mapas mais novos têm ID maior, então a data e o ID têm uma relação fraca: a ordem não é aleatória, mas também não é crescente. Por isso a BST não degenera, mas fica com altura 17, quase o dobro da AVL, e faz cerca de 38% mais comparações.
>
> E a AVL ficou com 8 e 9 nos dois cenários: a ordem de entrada quase não muda nada para ela. Essa é a diferença principal: a BST depende da ordem em que os dados chegam, e a AVL garante o desempenho em qualquer ordem.

Se perguntarem por que a AVL ficou melhor em ordem crescente (8) que na original (9): em ordem crescente, ela rotaciona sempre do mesmo jeito e acaba montando uma árvore quase completa. Na ordem original, ela só corrige quando a diferença entre os lados chega a 2, então aceita uma árvore um pouco menos perfeita, com um nível a mais. Ainda dentro da garantia, que vai de 8 a 10 para 200 jogadas.

### 4.5 Sair

**Tela**
- **Opção 0** e o terminal depois que o programa fecha.

**Fala**
> Ao sair, o programa libera as duas árvores. O AddressSanitizer não acusou nada, então não tem vazamento nem acesso inválido de memória.

Opcional, se sobrar tempo: o valgrind.
```bash
gcc -g -Wall -o trabalho trabalho.c && valgrind --leak-check=full ./trabalho
```
No fim: `All heap blocks were freed -- no leaks are possible` e `ERROR SUMMARY: 0 errors`.

---

## Parte 5: Bug proposital, sem cortes (2 a 3 min)

Testado em 18/10/2026: o sintoma aparece com os dados reais. Ensaie uma vez antes de gravar mesmo assim.

### 5.1 Introduzir o bug

**Tela**
- `rotacao_esquerda`, comentando a linha 259, `atualizar_altura(raiz);`.

```c
    nova->esq = raiz;
    // atualizar_altura(raiz);
    atualizar_altura(nova);
    return nova;
```

**Fala**
> Agora eu vou introduzir um erro de propósito. Vou tirar a linha que atualiza a altura do nó que desce na rotação à esquerda.

### 5.2 Mostrar o sintoma

**Tela**
- Recompilar e rodar. **Opção 5** (altura) e depois **opção 7** (experimento).
- Apontar a altura da AVL.

| | AVL correta | AVL com o bug |
|---|---|---|
| Altura com as jogadas do arquivo (opção 5 e Cenário A) | 9 | **12** |
| Altura no Cenário B | 8 | **69** |
| Média de comparações no Cenário B | 6,76 | **28,42** |

A BST não muda, porque ela não usa rotação.

**Fala**
> O programa continua funcionando, e o percurso em ordem continua certo, porque os ponteiros estão corretos. Mas o nó que desceu fica com uma altura velha, e o que subiu calcula a própria altura a partir dela. A partir daí, a AVL toma decisões de balanceamento erradas: no Cenário B, a altura dela foi de 8 para 69, e a média de comparações quadruplicou. A árvore ainda é uma árvore de busca válida, mas deixou de ser equilibrada.

### 5.3 Corrigir

**Tela**
- Descomentar a linha, recompilar e rodar a opção 7 de novo.

**Fala**
> Voltando a linha, a altura volta para 8. Esse erro é traiçoeiro justamente porque a árvore continua parecendo certa: só dá para perceber olhando a altura. E mostra por que a ordem importa: primeiro o nó que desce, depois o que sobe.

**Opção 2, se preferir um bug na remoção:** tirar o `free(raiz);` do caso 1 da `remover_bst` (linha 378). Remover a folha `1770368` (ReI) e sair com a opção 0: o ASan acusa o vazamento de **1 bloco de 328 bytes**, criado no `criar_no`. Tirar o `//`, repetir, e o ASan fica em silêncio. Um print desse relatório serve de captura de tela para o Diário de Bordo.

---

## Parte 6: Fechamento (2 min)

### 6.1 Limitações

**Tela**
- O seu rosto, ou a página dos desenhos.

**Fala**
> A primeira limitação é a simulação: como a árvore está organizada pelo ID, ela precisa olhar todas as jogadas para contar quantas têm pp maior. Para ela ser rápida, a árvore teria que estar ordenada pelo pp, mas o pp não identifica uma jogada. Eu preferi a chave que identifica a jogada de verdade.
>
> A segunda é que o programa mostra quanto a jogada nova vale, mas não quanto o perfil inteiro sobe. As jogadas abaixo dela descem uma posição e perdem 5% do valor, e calcular isso exigiria ordenar as jogadas pelo pp.
>
> A terceira é que os dados são uma foto do perfil num dia: as 200 jogadas que o site mostra, sem o bônus por quantidade de mapas jogados. E, para registrar um score melhor num mapa que já está na árvore, é preciso remover a jogada antiga e inserir a nova.

### 6.2 E com 1000 vezes mais dados

**Tela**
- O seu rosto.

**Fala**
> Com 1000 vezes mais dados, seriam 200 mil jogadas. Na AVL quase nada muda: a altura ficaria entre 18 e 25, então cada busca faria só algumas dezenas de comparações. Na BST, depende da ordem: se os IDs chegarem crescentes, cada busca poderia passar por até 200 mil nós. E tem um problema prático: as funções são recursivas, e numa BST degenerada com 200 mil níveis a recursão poderia estourar a pilha do programa.
>
> A simulação passaria a visitar 200 mil nós a cada consulta. Se fossem muitas consultas, eu manteria uma segunda árvore organizada pelo pp, só para elas, ao lado da árvore pelo ID. É o que um banco de dados faz quando cria um índice para outro campo.
>
> A memória também pesaria: cada nó ocupa 328 bytes, a maior parte por causa do vetor de 256 caracteres do nome. Com 200 mil nós, dá uns 66 MB por árvore, e daria para reduzir alocando o nome só com o tamanho necessário.

---

## Perguntas que o professor pode fazer

**"Por que o ID do mapa?"**
O perfil guarda uma jogada por mapa, então o ID identifica a jogada sem ambiguidade. Quando faço um score novo, o jogo procura a jogada antiga daquele mapa pelo ID e substitui. É isso que a árvore faz.

**"E o pp?"**
É só um dado da jogada, como o nome e a data. A árvore nunca compara pp.

**"A simulação percorre tudo, então para que árvore?"**
A árvore resolve rápido o que precisa ser rápido: achar, inserir e remover a jogada de um mapa. Contar quantas jogadas passam de x pp exige olhar todas, como contar no cadastro por CPF quantas pessoas ganham mais de 5 mil.

**"Por que não hash?"**
Hash acha uma chave exata, mas não guarda ordem: não faria os percursos em ordem nem a comparação de altura do experimento. E o enunciado pede BST e AVL.

**"Por que na rotação se atualiza primeiro o nó que desce?"**
Porque o que sobe vira pai do que desceu, e a altura dele depende da altura do filho já corrigida. O bug proposital mostra o que acontece quando essa linha falta.

**"Essa é a única AVL possível com esses números?"**
Não. A regra só exige fatores entre -1 e 1, e existem outras árvores válidas. Mas o algoritmo é determinístico: com a mesma sequência de operações, sempre chega na mesma árvore.

**"Por que a rotação, e não simplesmente pendurar o nó lá embaixo?"**
O nó que desce leva a subárvore dele junto, e pendurá-lo embaixo de outro pode desequilibrar outro ponto. Achar o lugar também exigiria descer a árvore. A rotação mexe sempre em 3 ponteiros e sempre tira um nível do lado pesado.

**"Por que a remoção com dois filhos copia todos os dados?"**
Porque o nó continua no lugar e passa a representar a jogada do sucessor. Copiando só o ID, ele ficaria com o nome e o pp de outra jogada.

**"E se dois pp forem iguais na simulação?"**
A jogada nova fica logo abaixo da existente. A ordem entre as duas não muda a pontuação, porque elas valem o mesmo.

**"Você usou IA?"**
Usei como apoio, como o senhor permitiu, e está declarado no relatório. O código foi feito etapa por etapa, eu compilei e rodei com o AddressSanitizer a cada etapa, e os erros reais estão no Diário de Bordo.

---

## Relatório (2 a 3 páginas)

O enunciado pede quatro coisas no relatório. Dá para reaproveitar as falas acima, passando para texto corrido:

1. **Problema e fonte real:** blocos 1.1 a 1.4, com os links do perfil e da wiki.
2. **Por que árvore:** blocos 2.1 a 2.3, com a tabela de vetor, lista e árvore.
3. **Tabela do experimento comparativo:** a tabela do bloco 4.4, com a explicação.
4. **Diário de Bordo:** `docs/DIARIO.md` já tem as 5 entradas datadas. Falta pelo menos uma **captura de tela**: o print do ASan do bloco 5 (opção 2) ou o da importação no LibreOffice.

Incluir também uma frase declarando o uso de IA como apoio para revisar a proposta e implementar o código, e o link do vídeo.
