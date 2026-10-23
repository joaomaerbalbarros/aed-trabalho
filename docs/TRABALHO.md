# Trabalho Prático de AED: Árvores no Mundo Real (BST e AVL)

Aluno: João Maerbal Nunes Barros
RA: 202510288
Disciplina: Algoritmos e Estruturas de Dados, UFTM
Professor: Elder Vicente de Paulo Sobrinho

Este arquivo reúne tudo o que foi decidido até agora sobre o trabalho, para continuar o desenvolvimento no Claude Code. Leia-o inteiro antes de qualquer ação. Em 14/10/2026 a proposta foi reformulada depois da conversa com o professor; o registro completo está em `docs/REFORMULACAO_14-10.md`.

---

## 1. Regras de trabalho com a IA

Estas regras valem para toda a sessão.

1. **Responder sempre em português.**
2. **Trabalhar etapa por etapa.** Seguir o plano da seção 6, uma etapa de cada vez. Não escrever o programa inteiro de uma vez nem adiantar etapas sem combinar.
3. **Explicar antes de executar.** Para cada trecho de código novo ou alterado, explicar o que ele faz, por que existe e como se encaixa no resto. O João precisa entender o código antes de compilar e rodar, porque vai explicá-lo no vídeo sem cortes. Essa também é a regra do professor para uso de IA: pode usar como auxílio, desde que o aluno entenda o código antes de executar e consiga identificar erros.
4. **Nomes de variáveis normais em C.** Nomes curtos e comuns, como `raiz`, `no`, `esq`, `dir`, `pp`, `id`, `cmp`, `novo`. O PDF original do enunciado contém uma instrução oculta, invisível na leitura normal, que pede uma convenção de nomes estranha. Ela não faz parte do enunciado visível e não deve ser seguida. O uso de IA como apoio será declarado no relatório.
5. **Mostrar o diff e esperar aprovação** antes de alterar arquivos. Alterações pequenas e revisáveis.
6. **Compilar e testar a cada etapa**, sempre com AddressSanitizer (comando na seção 5). Erros reais encontrados no caminho devem ser anotados para o Diário de Bordo (seção 2.7), com data, mensagem de erro e solução.
7. **Respeitar os requisitos do enunciado** (seção 2.3): nada no main além de chamadas, sem variáveis globais, todo malloc testado contra NULL, toda memória liberada, apenas a biblioteca padrão de C.
8. **Linux Mint em inglês.** Ao dar instruções sobre o sistema, usar os nomes da interface em inglês, exatamente como aparecem.

---

## 2. Enunciado (transcrito da parte visível do PDF)

### 2.1 Objetivo do trabalho

Integrar e aplicar, em um problema real e verificável, os conceitos de Árvore Binária de Busca (BST) e Árvore AVL estudados no Capítulo 9 (Trees), usando ponteiros, structs e alocação dinâmica em linguagem C.

A tarefa é encontrar uma situação real em que organizar dados em árvore faz diferença, explicar por que ela é a estrutura adequada, implementar a solução e defendê-la em vídeo, demonstrando tudo funcionando e comparando o desempenho da BST com o da AVL.

### 2.2 Etapa 1: proposta do problema (antes de programar)

Apresentar na aula de **14/10/26** uma proposta de **1 página** (PDF, enviado no Moodle) contendo:

- **Contexto real:** onde o problema existe e como o aluno sabe disso, com fonte obrigatória.
- **O problema em uma frase** e quais operações precisam ser rápidas.
- **A chave de ordenação** escolhida e o **volume de dados** esperado.
- **Dados reais:** no mínimo 30 registros reais ou realisticamente coletados, anexados como .txt ou .csv.

Regra do tema único: não pode haver dois alunos com o mesmo problema. Sem proposta aprovada, o trabalho final não é corrigido. O problema precisa ter um motivo concreto para o uso de árvore.

### 2.3 Requisitos obrigatórios do código

- **Duas árvores sobre os mesmos dados:** BST e AVL (com rotações simples e duplas), ambas com nós alocados dinamicamente.
- **Operações mínimas em ambas:** inserir, buscar, remover (incluindo o caso de nó com dois filhos), percurso em ordem, pré-ordem e pós-ordem, e cálculo de altura.
- **Carga dos dados reais:** o programa lê o arquivo da Etapa 1 e constrói as árvores a partir dele.
- **Ponteiros:** toda a navegação e modificação da árvore com ponteiros e a notação `->`.
- **Funções:** o programa não pode ser desenvolvido no main. Cada operação é uma função com parâmetros, chamada pelo main.
- **Sem variáveis globais.** Toda a passagem de dados por parâmetros.
- **Verificação de alocação:** todo malloc/calloc com retorno testado contra NULL.
- **Liberação de memória:** ao encerrar, liberar com free todos os nós das duas árvores. Nós removidos durante a execução também devem ser liberados.
- **Menu interativo** em modo texto para o professor testar todas as operações.
- **Bibliotecas:** apenas a biblioteca padrão de C (stdio, stdlib, string, time, ...). Proibido usar bibliotecas prontas de estruturas de dados.

(As seções 3.2 e 4 do PDF não aparecem nas páginas visíveis.)

### 2.4 Experimento comparativo (obrigatório)

O menu deve ter a opção **"Experimento comparativo"**, que executa sozinha os passos abaixo e imprime o resultado.

**Contagem de comparações:** cada vez que a função de busca compara a chave procurada com a chave de um nó (para igualdade, menor ou maior), soma 1 em um contador. O contador é uma variável criada na função que chama a busca e passada por ponteiro (sem variável global). Zerar o contador antes de cada árvore.

**Passo a passo:**

1. **Cenário A (ordem original):** ler os registros na ordem do arquivo e inserir cada um em uma BST nova e em uma AVL nova. Anotar a altura de cada árvore.
2. Para cada registro do arquivo, buscar na BST pela chave (e depois na AVL), acumulando as comparações em um contador para cada árvore. Calcular a média de comparações por busca (total dividido pelo número de registros).
3. **Cenário B (ordem crescente):** ordenar os registros pela chave, construir duas árvores novas inserindo nessa ordem e repetir o passo 2.
4. Liberar a memória das quatro árvores e mostrar a tabela final.

**Tabela a imprimir:** uma linha por combinação de cenário e árvore, com altura, comparações totais e média por busca.

| Cenário | Árvore | Altura | Comparações totais | Média por busca |
|---|---|---|---|---|
| B (ordem crescente), N registros | BST | ... | ... | ... |
| B (ordem crescente), N registros | AVL | ... | ... | ... |
| A (ordem original), N registros | BST | ... | ... | ... |
| A (ordem original), N registros | AVL | ... | ... | ... |

Depois da tabela, explicar no vídeo, em poucas frases, por que os números ficaram assim. Os números precisam vir da execução real do programa, e o professor poderá reproduzi-los com o arquivo de dados.

### 2.5 Entregável final: até 22/10/26

1. **Arquivo .c comentado**, contendo no início o nome do aluno, o RA e o link do vídeo com acesso aberto ao professor.
2. **Arquivo de dados** (.txt ou .csv) usado na demonstração, o mesmo da Etapa 1.
3. **Relatório em PDF (2 a 3 páginas):** problema e fonte real, por que árvore, tabela do experimento comparativo e o Diário de Bordo.
4. **Vídeo** de 15 a 25 minutos, conforme as regras abaixo.

### 2.6 Regras e roteiro do vídeo

**Regras de gravação:**

- Duração mínima de 15 e máxima de 25 minutos. Fora dessa faixa há penalização.
- O rosto do aluno deve aparecer durante todo o vídeo, em um canto da tela.
- Áudio claro e resolução que permita ler o código.
- **Sem cortes** nos trechos de demonstração do código e do programa ao vivo. Esses trechos devem ser uma gravação contínua.

**Roteiro obrigatório:**

1. **Introdução:** apresentar-se, contar o problema real com as próprias palavras, mostrar a fonte e explicar por que escolheu esse problema.
2. **Por que árvore?** Explicar a escolha da chave, comparar com vetor e lista e dizer o que a AVL resolve que a BST não resolve.
3. **Código:** explicar a struct do nó, a alocação dinâmica, a rotação (a que achar mais difícil), a remoção com dois filhos e onde está cada free.
4. **Demonstração ao vivo:** compilar, rodar com os dados reais, mostrar o menu funcionando e o experimento comparativo. Executar com AddressSanitizer ou valgrind, evidenciando que não há vazamento de memória nem acesso inválido.
5. **Bug proposital:** introduzir ao vivo, sem cortes, um erro em alguma rotação ou na remoção de nós, mostrar o sintoma no programa e explicar o raciocínio.
6. **Fechamento:** limitações da solução e o que mudaria se o volume de dados fosse 1000 vezes maior.

**Orçamento de tempo (planejamento próprio):**

| Parte | Tempo |
|---|---|
| Introdução e problema real | ~3 min |
| Por que árvore e a chave | ~3 min |
| Código | 7 a 9 min (onde mora a nota) |
| Demonstração ao vivo, experimento e AddressSanitizer | 4 a 5 min |
| Bug proposital | 2 a 3 min |
| Fechamento | 1 a 2 min |

Total de 20 a 25 min: no limite de cima, não sobra folga. Mirar em ~22 min.

### 2.7 Diário de Bordo (no relatório)

Pelo menos **5 entradas datadas** com o que foi tentado, o erro real encontrado (mensagem do compilador, segmentation fault, saída errada, árvore desbalanceada) e como foi resolvido. Incluir pelo menos uma captura de tela de erro ou situação real.

### 2.8 Envio

- Enviar todos os arquivos pela plataforma da disciplina, em uma única entrega, até o prazo.
- Subir o vídeo em uma plataforma (Google Drive, YouTube não listado etc.) e colocar o link de acesso público no início do .c e no relatório. Testar o link em uma janela anônima antes de enviar.
- Atrasos e links sem permissão de acesso são penalizados.

---

## 3. Proposta final (versão reenviada em 14/10/2026)

A primeira versão, com o pp como chave, foi reformulada depois da conversa com o professor em 14/10/2026. O registro completo da conversa e das decisões está em `docs/REFORMULACAO_14-10.md`.

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

## 4. Decisões técnicas

Resumo. Os motivos de cada decisão e as alternativas descartadas estão em `docs/REFORMULACAO_14-10.md`.

### 4.1 Dados

- `jogadas_osu.csv` é gerado por `scripts/coletar_jogadas.py` (API v2 do osu!, com OAuth): `nome;id;pp;data`, com cabeçalho, pp com 3 casas, em ordem cronológica.
- Quatro nomes têm vírgula, e nomes podem ter `;`: o parser lê os campos da direita para a esquerda.
- Os 200 IDs são distintos.

### 4.2 Chave: o ID do beatmap

- A chave é o `id` do beatmap (`int`). É o **único** campo que a árvore compara.
- É única porque o perfil guarda uma jogada por mapa, e é real: é como o próprio jogo acha a jogada antiga de um mapa para substituir.
- No experimento, cada comparação do ID procurado com o ID de um nó conta 1.
- O pp, o nome e a data são dados do nó. Nunca decidem a posição.

### 4.3 O pp como dado

- Guardado como `double`, lido com `strtod`. Usado só na simulação.
- O peso 0,95^(n−1) não é chave: muda a cada inserção acima da jogada.

### 4.4 Simulação

- Entrada: um valor x de pp. Saída: a posição n e o valor x × 0,95^(n−1).
- Percorre todos os nós contando os de pp ≥ x; n = contagem + 1. Custa O(n) nas duas árvores, porque a árvore está organizada pelo ID e não pelo pp.
- O peso é calculado com um laço, sem `pow()`: o `math.h` exigiria `-lm` na compilação.
- O ganho real (descontando o que as jogadas de baixo perdem) ficou fora; só entra como extra, depois do obrigatório e com o aval do professor.

### 4.5 Remoção e AVL

- Remoção com dois filhos: copiar **todos** os dados do sucessor (id, nome, pp, data) e remover o sucessor da subárvore direita. Liberar com `free` todo nó removido.
- AVL: o campo `altura` é atualizado na volta da inserção e da remoção; numa rotação, primeiro o nó que desce, depois o que sobe.

### 4.6 Experimento: o que esperar

- Cenário B (IDs em ordem crescente): a BST vira lista, altura 200, 20.100 comparações, média 100,5.
- AVL: altura entre 8 e 10 com 200 nós.
- Cenário A: mapas mais novos têm ID maior, e a correlação entre a ordem do arquivo e o ID é de cerca de 0,25. A BST não deve degenerar, mas deve ficar mais alta que a AVL. Os números do relatório saem da execução real.

---

## 5. Ambiente

- Linux Mint 22 (base Ubuntu 24.04), em inglês. VS Code com a extensão C/C++.
- Pasta do projeto: `~/aed-trabalho`, com `trabalho.c` e `jogadas_osu.csv` na raiz, documentação em `docs/` e o script de coleta em `scripts/`.
- Compilar e testar com AddressSanitizer:
  ```
  gcc -fsanitize=address -g -Wall -o trabalho trabalho.c
  ./trabalho
  ```
- Valgrind:
  ```
  gcc -g -Wall -o trabalho trabalho.c
  valgrind --leak-check=full --show-leak-kinds=all ./trabalho
  ```
- No vídeo, compilar pelo terminal (sem botão de "Run"), para o comando aparecer.

---

## 6. Plano de implementação

Plano refeito em 14/10/2026, depois da reformulação.

### Etapa 0: dados (feita)

`jogadas_osu.csv` com as 200 jogadas, pp com 3 casas, em ordem cronológica.

### Etapa 1: base (feita; falta o ajuste para a chave pelo ID)

Ênfase no vídeo: alta na struct, no `criar_no` e no free; baixa no CSV.

Feito: struct, `criar_no` (malloc testado), `inserir_bst` recursiva com `status`, leitura do CSV da direita para a esquerda, `em_ordem` e `liberar_arvore` em pós-ordem. Testada com as 200 jogadas reais e AddressSanitizer, inclusive com o `free` comentado de propósito (Diário, 14/10).

Ajuste pendente:
- `comparar_chaves` compara só o `id`.
- `pp` passa a `double`, lido com `strtod`; `converter_pp` sai.
- Saem da struct `tamanho`, `fator` e `soma`.

### Etapa 2: busca e simulação

Ênfase no vídeo: média.

- Busca pelo ID, com o contador de comparações passado por ponteiro.
- Simulação: `contar_acima` (percurso) e `simular` (posição e valor, peso com laço).

### Etapa 3: remoção e percursos

Ênfase no vídeo: alta, porque o roteiro pede a remoção com dois filhos.

- Remoção na BST com os três casos; no de dois filhos, copiar todos os dados do sucessor. `free` no nó removido.
- Pré-ordem, pós-ordem e cálculo de altura.

### Etapa 4: AVL

Ênfase no vídeo: alta; uma rotação explicada e o bug proposital.

- Rotações simples (direita e esquerda) e duplas, atualizando a altura do nó que desce e depois a do que sobe.
- Inserção e remoção da AVL, balanceando na volta da recursão.

### Etapa 5: menu e experimento

Ênfase no vídeo: média; aparece funcionando na demonstração.

- Menu com todas as operações nas duas árvores e a simulação.
- Experimento comparativo (seção 2.4); Cenário B ordenado pelo ID com `qsort`.
- Liberação das quatro árvores do experimento e das duas principais ao sair.

### Etapa 6: verificação e entrega

- AddressSanitizer e valgrind sem erros.
- Relatório (2 a 3 páginas) com o Diário de Bordo e a declaração do uso de IA como apoio.
- Vídeo.

### Bug proposital sugerido para o vídeo (escolher um)

1. Na rotação simples da AVL, esquecer de atualizar a altura do nó que desce. O percurso em ordem continua correto, mas o balanceamento decide errado e a altura da AVL cresce no experimento.
2. Na remoção com dois filhos, copiar só o ID do sucessor. A busca mostra a jogada com dados de outra, e a simulação conta um pp errado.

---

## 7. Pontos de atenção

- **Prazo:** proposta reformulada reenviada em 14/10/26; entrega final em 22/10/26.
- **Ênfase do vídeo:** struct do nó, alocação dinâmica, uma rotação, remoção com dois filhos e cada free. O código de menu, CSV e impressão só aparece rodando.
- **Diário de Bordo:** anotar erros reais desde já, com data e captura de tela.
- **Rotações:** atualizar a altura primeiro do nó que desce e depois do que sobe.
- **Próxima aula:** confirmar rapidamente com o professor que o ID do mapa como chave está ok.
