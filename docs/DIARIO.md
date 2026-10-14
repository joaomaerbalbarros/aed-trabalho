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
