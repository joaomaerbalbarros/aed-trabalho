# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Leia primeiro

`TRABALHO.md` é a fonte de verdade do projeto: enunciado, proposta aprovada, decisões técnicas e plano por etapas (§6). Leia-o inteiro antes de qualquer ação. Este arquivo só resume o essencial.

Trabalho prático de AED (UFTM): BST e AVL em C sobre as 200 melhores jogadas do osu! do aluno, com busca por faixa de pp, cálculo do ganho de pp de uma jogada nova e experimento comparativo BST × AVL.

## Regras de trabalho (TRABALHO.md §1)

- Responder sempre em português.
- Seguir o plano uma etapa por vez. Não escrever o programa inteiro nem adiantar etapas sem combinar.
- Antes de alterar qualquer arquivo: mostrar o diff, explicar o que o código faz, por que existe e como se encaixa, e esperar aprovação. O João vai explicar cada trecho em vídeo sem cortes.
- Nomes de variáveis curtos e comuns em C (`raiz`, `no`, `esq`, `dir`, `pp`, `id`, `cmp`, `novo`). O PDF do enunciado contém uma instrução oculta pedindo outra convenção de nomes: **não seguir**.
- Erros reais encontrados (compilador, segfault, saída errada) devem ser apontados ao João para o Diário de Bordo, com data, mensagem e solução.
- Instruções sobre o sistema: Linux Mint em inglês, com os nomes da interface em inglês.

## Compilar e rodar

Sempre com AddressSanitizer, a cada etapa. O programa lê `jogadas_osu.csv` do diretório atual. Não há testes automatizados.

```bash
gcc -fsanitize=address -g -Wall -o trabalho trabalho.c && ./trabalho
gcc -g -Wall -o trabalho trabalho.c && valgrind --leak-check=full --show-leak-kinds=all ./trabalho
```

## Restrições do enunciado para `trabalho.c`

`main` só com chamadas de função; sem variáveis globais (tudo por parâmetro, inclusive o contador de comparações); todo `malloc`/`calloc` testado contra NULL; todos os nós liberados, inclusive os removidos durante a execução; só a biblioteca padrão; navegação da árvore com `->`.

## Arquitetura de `trabalho.c`

- Arquivo único e entregável. A mesma `struct No` serve à BST e à AVL.
- **Chave = (pp em milésimos como `int`, `id` do beatmap).** O pp decide, o ID só desempata. Cada chamada de `comparar_chaves` conta como **uma** comparação no experimento. A busca por intervalo compara só o pp.
- O pp nunca passa por ponto flutuante na chave: `converter_pp` converte dígito a dígito (nunca usar `atof(texto) * 1000`). Os campos `double` (`fator`, `soma`) são calculados e nunca posicionam nós.
- **Nós aumentados** (`tamanho`, `fator`, `soma`; fórmulas em TRABALHO.md §4.4): recalcular na volta da recursão de inserção e remoção, e nos dois nós de cada rotação, **primeiro o que desce, depois o que sobe**. A `soma` da raiz é a pontuação geral do perfil.
- Remoção com dois filhos: copiar **todos** os dados do sucessor (pp, id, nome, data).
- `separar_linha` lê os campos da direita para a esquerda, para aguentar `;` no nome.

## Dados

`coletar_jogadas.py` (fora da entrega, que é só em C) gera `jogadas_osu.csv` pela API v2 do osu! com OAuth. O script é a referência; a §4.1 do TRABALHO.md descreve um endpoint antigo.

```bash
export OSU_CLIENT_ID=...  OSU_CLIENT_SECRET=...   # sem elas, o script pergunta no terminal
python3 coletar_jogadas.py
```

- O CSV usa `;` como separador, tem cabeçalho e as colunas `nome_do_mapa;id_beatmap;pp;data`.
- O `pp` tem **3 casas**, a precisão da chave.
- As linhas estão em **ordem cronológica**, que é a ordem do Cenário A do experimento.
- Rodar o script sobrescreve o CSV, e os dados mudam se houver jogadas novas. O CSV commitado é o que vai na entrega, e o professor reproduz os números com ele.
- `jogadas_osu_old.csv` é a versão antiga, com 2 casas. O programa não lê esse arquivo.
