# Árvores no Mundo Real: jogadas do osu! com BST e AVL

Trabalho prático de Algoritmos e Estruturas de Dados (UFTM).
Aluno: João Maerbal Nunes Barros.

O programa organiza as 200 melhores jogadas de um perfil do osu! em uma
Árvore Binária de Busca (BST) e em uma Árvore AVL, com a chave sendo o ID do
beatmap. Ele permite inserir, buscar e remover jogadas, fazer os três
percursos, simular quantos pp uma jogada nova acrescentaria ao perfil e
comparar o desempenho das duas árvores.

Vídeo de apresentação: https://www.youtube.com/watch?v=bza74MyH1AQ

## Como compilar e rodar

Requer apenas `gcc` e a biblioteca padrão de C. Rode a partir desta pasta,
porque o programa lê `jogadas_osu.csv` do diretório atual.

```bash
gcc -fsanitize=address -g -Wall -o trabalho trabalho.c
./trabalho
```

O menu oferece:

1. Inserir jogada
2. Buscar jogada pelo ID
3. Remover jogada pelo ID
4. Percursos (em ordem, pré-ordem, pós-ordem)
5. Altura das árvores
6. Simular jogada de x pp
7. Experimento comparativo

## Resultados do experimento

| Cenário | Árvore | Altura | Comparações (total) | Comparações (média) |
|---|---|---|---|---|
| A: ordem do arquivo | BST | 17 | 1888 | 9,44 |
| A: ordem do arquivo | AVL | 9 | 1368 | 6,84 |
| B: IDs em ordem crescente | BST | 200 | 20100 | 100,50 |
| B: IDs em ordem crescente | AVL | 8 | 1353 | 6,76 |

No cenário B a BST degenera em lista, e a AVL continua com altura logarítmica.
A análise completa está no relatório.

## Dados

`jogadas_osu.csv` contém as 200 jogadas da seção Melhor Performance do perfil
https://osu.ppy.sh/users/13699780, coletadas em 14/10/2026 pela API oficial do
osu!. As colunas são nome do beatmap, ID do beatmap, pp (3 casas decimais) e
data, separadas por `;` e em ordem cronológica.

O script `scripts/coletar_jogadas.py` gera o arquivo novamente. Os dados mudam
se o jogador fizer jogadas novas; o CSV deste repositório é o usado nos
resultados do trabalho.

## Estrutura

| Caminho | Conteúdo |
|---|---|
| `trabalho.c` | o programa |
| `jogadas_osu.csv` | os dados |
| `docs/relatorio.pdf` | relatório entregue |
| `docs/enunciado.pdf` | enunciado do trabalho |
| `docs/TRABALHO.md` | proposta, decisões técnicas e plano de etapas |
| `docs/REFORMULACAO_14-10.md` | mudanças combinadas com o professor em 14/10 (chave = ID) |
| `docs/DIARIO.md` | Diário de Bordo: erros encontrados e como foram resolvidos |
| `docs/REVISAO_CODIGO.md` | revisão do código linha por linha |
| `docs/ROTEIRO_VIDEO.md` | roteiro do vídeo |
| `scripts/` | script de coleta dos dados |

## Uso de IA

O desenvolvimento contou com IA (Claude, da Anthropic) como apoio, conforme
permitido pelo professor e declarado no relatório. Os commits feitos com esse
apoio levam a linha `Co-Authored-By: Claude`.
