# Árvores no Mundo Real: jogadas do osu! com BST e AVL

Trabalho prático de Algoritmos e Estruturas de Dados (UFTM).
Aluno: João Maerbal Nunes Barros.

O programa organiza as 200 melhores jogadas de um perfil do osu! em uma
Árvore Binária de Busca (BST) e em uma Árvore AVL, com a chave sendo o pp
(Performance Points) da jogada. Ele permite buscar jogadas por faixa de pp,
calcular quantos pp uma jogada nova acrescentaria ao perfil e comparar o
desempenho das duas árvores.

## Como compilar e rodar

Requer apenas `gcc` e a biblioteca padrão de C. Rode a partir desta pasta,
porque o programa lê `jogadas_osu.csv` do diretório atual.

```bash
gcc -fsanitize=address -g -Wall -o trabalho trabalho.c
./trabalho
```

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
| `trabalho.c` | o programa (arquivo entregue) |
| `jogadas_osu.csv` | os dados (arquivo entregue) |
| `docs/TRABALHO.md` | enunciado, proposta, decisões técnicas e plano |
| `docs/DIARIO.md` | Diário de Bordo: erros encontrados e como foram resolvidos |
| `scripts/` | script de coleta dos dados (não faz parte da entrega) |

## Andamento

| Etapa | Conteúdo | Situação |
|---|---|---|
| 0 | coleta dos dados | feita |
| 1 | struct, leitura do CSV, inserção na BST, em ordem, liberação | feita; falta testar com o CSV real |
| 2 | campos aumentados, busca, busca por faixa, ganho | pendente |
| 3 | remoção, pré e pós-ordem, altura | pendente |
| 4 | AVL: rotações e balanceamento | pendente |
| 5 | menu e experimento comparativo | pendente |
| 6 | verificação, relatório e vídeo | pendente |

## Uso de IA

O desenvolvimento contou com IA (Claude) como apoio, conforme permitido pelo
professor e declarado no relatório.
