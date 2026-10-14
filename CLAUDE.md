# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Visão geral

Trabalho de AED (Algoritmos e Estruturas de Dados). O repositório tem, por enquanto, só a etapa de coleta de dados: `coletar_jogadas.py` baixa as 200 melhores jogadas (Best Performance) do perfil do osu! `USER_ID = 13699780` (modo `osu`) pela API v2 oficial e grava `jogadas_osu.csv`. O CSV é o conjunto de dados de entrada do trabalho e está versionado; não há testes, build nem lint.

Código, comentários, nomes de variáveis e mensagens estão em português. Mantenha esse padrão.

## Regerar o CSV

```bash
pip install requests
export OSU_CLIENT_ID=...  OSU_CLIENT_SECRET=...   # app OAuth em osu.ppy.sh/home/account/edit
python coletar_jogadas.py
```

Sem as variáveis de ambiente, o script pede Client ID/Secret no terminal (o secret via `getpass`). Rodar sobrescreve `jogadas_osu.csv`, e os dados mudam se o jogador tiver novas jogadas — o CSV commitado é o conjunto de referência.

## Formato de `jogadas_osu.csv`

- Separador `;` (por isso `;` em nomes de mapa é trocado por `,`), UTF-8, com cabeçalho.
- Colunas: `nome_do_mapa` (`Título [dificuldade]`), `id_beatmap` (int), `pp` (string com 3 casas decimais), `data` (`AAAA-MM-DD HH:MM:SS`, UTC da API).
- Linhas em **ordem cronológica** por `data` — essa é a ordem original do "Cenário A" do trabalho.

## Premissas que o restante do trabalho usa

O script valida, ao final, duas propriedades das quais a ordenação depende:
- **Desempate por `id_beatmap`**: chaves com `pp` iguais são desempatadas pelo `id_beatmap`, o que só funciona se não houver IDs repetidos (o script avisa se houver).
- **Empates de `pp`**: o script lista os valores de `pp` repetidos, relevantes para testar estabilidade/desempate dos algoritmos.
- Espera exatamente 200 linhas (4 páginas de 50 via `offset`).
