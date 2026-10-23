# Diário de Bordo

Registro dos erros reais encontrados durante o desenvolvimento, para o
relatório. O enunciado pede pelo menos 5 entradas datadas e pelo menos uma
captura de tela.

Modelo de entrada:

## DD/MM/AAAA: título curto

- **O que estava tentando:**
- **Erro encontrado:** (mensagem do compilador, segmentation fault, saída errada...)
- **Como foi resolvido:**
- **Captura de tela:** `img/nome-do-arquivo.png` (se houver)

---

## 14/10/2026: pp do CSV com menos precisão que a chave

- **O que estava tentando:** gerar o `jogadas_osu.csv` com as 200 jogadas reais para carregar nas árvores.
- **Erro encontrado:** o script de coleta gravava o pp com 2 casas decimais (`f'{pp:.2f}'`), mas a chave da proposta é o pp em milésimos, que precisa de 3 casas. Com 2 casas, duas jogadas diferentes apareciam com o mesmo pp, `311.18`: DEEP BLUE SONG (ID 2777125) e Best of my Love (ID 4938553). O `converter_pp` completaria as duas como `311180`, e a ordem entre elas seria decidida só pelo ID.
- **Como foi resolvido:** mudei a formatação no script para 3 casas (`:.3f`) e gerei o CSV de novo. Com a precisão real, os valores são `311.176` e `311.179`, e não há mais pp repetido. As jogadas, IDs e datas continuaram as mesmas; só a coluna do pp mudou.
- **Captura de tela:** —

## 14/10/2026: teste de vazamento com o AddressSanitizer

- **O que estava tentando:** confirmar que o AddressSanitizer detecta memória não liberada. Para isso, comentei de propósito a chamada `liberar_arvore(bst);` no fim do `main` e rodei o programa com as 200 jogadas reais.
- **Erro encontrado:** o relatório do LeakSanitizer tinha milhares de linhas, uma pilha de chamadas para cada nó perdido. Ele se misturava com a listagem das jogadas e o começo sumia do terminal. Para ler, separei as saídas: `./trabalho 2>&1 > /dev/null | head -8` descarta a listagem (saída padrão) e mostra só o início do relatório (saída de erro), e `./trabalho 2>&1 | tail -3` mostra o resumo final. O relatório indicou `Direct leak of 336 byte(s) in 1 object(s)`, alocado pelo `malloc` dentro de `criar_no` (linha 51), chamado por `inserir_bst` (linha 79), e terminou com `SUMMARY: AddressSanitizer: 67200 byte(s) leaked in 200 allocation(s)`. As contas batem: cada nó ocupa `sizeof(No)` = 336 bytes, e 200 nós × 336 = 67.200 bytes.
- **Como foi resolvido:** descomentei `liberar_arvore(bst);`, recompilei e rodei de novo. O AddressSanitizer não acusou mais nada.
- **Captura de tela:** —

## 14/10/2026: CSV parecia quebrado no LibreOffice

- **O que estava tentando:** conferir o `jogadas_osu.csv` abrindo-o no LibreOffice Calc.
- **Erro encontrado:** a linha do mapa `Koise yo Minna, Hai! (TV Size) [Insane]` apareceu partida em duas colunas, empurrando o ID, o pp e a data para a direita. Os pp também apareciam com 2 casas (339.71 em vez de 339.707), o que parecia indicar que o arquivo tinha perdido precisão.
- **Como foi resolvido:** conferi o arquivo cru no terminal: as 200 linhas têm exatamente 4 campos separados por `;`, e o pp está com 3 casas. O problema era a importação do LibreOffice, que usava a **vírgula** também como separador; quatro nomes de mapas têm vírgula. Na janela Text Import, em Separator Options, deixei marcado só Semicolon. As 2 casas eram só a formatação da coluna. Cuidado que fica: não salvar o CSV pelo LibreOffice, porque ele pode regravar o arquivo com o pp arredondado.
- **Captura de tela:** —

## 14/10/2026: última linha da saída sumia no terminal do VS Code

- **O que estava tentando:** testar a busca pelo ID com dois casos: um ID que existe (2828520) e um que não existe (1).
- **Erro encontrado:** a saída terminava em `ID 1 nao encontrado.` sem a linha `Comparacoes: 6`, embora o `printf` dessa linha estivesse fora do `if/else` e devesse rodar sempre.
- **Como foi resolvido:** mandei a saída para um arquivo, sem passar pela tela: `./trabalho > saida.txt`, `cat saida.txt` e `wc -l saida.txt`. O arquivo tinha as 5 linhas esperadas, inclusive `Comparacoes: 6`. O programa estava certo; o terminal do VS Code tinha deixado de mostrar a linha ao redesenhar a tela. Rodar `clear` antes de executar resolveu. Para o vídeo, a lição é limpar o terminal antes de rodar.
- **Captura de tela:** —

## 18/10/2026: função usada antes de ser declarada

- **O que estava tentando:** adicionar a função `testar_percursos`, que monta uma árvore pequena com 5 jogadas reais, imprime a pré-ordem, a em ordem e a pós-ordem e depois libera a árvore com `liberar_arvore`.
- **Erro encontrado:** o compilador avisou `warning: implicit declaration of function 'liberar_arvore' [-Wimplicit-function-declaration]` e, logo depois, `conflicting types for 'liberar_arvore'`. A `testar_percursos` tinha sido colocada no arquivo **antes** da `liberar_arvore`. O C lê o arquivo de cima para baixo, então, no ponto da chamada, ele ainda não conhecia a função e tentou adivinhar como ela era.
- **Como foi resolvido:** movi a `testar_percursos` para depois da `liberar_arvore`, logo antes do `main`. O programa voltou a compilar sem avisos. Regra que fica: em C, toda função precisa aparecer (ou ser declarada) antes do ponto em que é usada.
- **Captura de tela:** —
