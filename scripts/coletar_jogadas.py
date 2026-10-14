"""
Coleta as 200 melhores jogadas (Best Performance) de um perfil do osu!
pela API v2 oficial e gera jogadas_osu.csv em ordem cronológica.

Antes de rodar:
  1. Crie uma aplicação OAuth em https://osu.ppy.sh/home/account/edit
     (seção "OAuth" -> "New OAuth Application"; a callback URL pode ficar vazia).
  2. Defina as variáveis de ambiente com o Client ID e o Client Secret:
       Linux/macOS:  export OSU_CLIENT_ID=...   export OSU_CLIENT_SECRET=...
       Windows (PowerShell):  $env:OSU_CLIENT_ID="..."; $env:OSU_CLIENT_SECRET="..."
  3. pip install requests
  4. python3 scripts/coletar_jogadas.py   (a partir da raiz do projeto)
"""

import csv
import getpass
import os
import sys
from collections import Counter

import requests

USER_ID = 13699780
MODE = "osu"
# O CSV fica na raiz do projeto, ao lado do trabalho.c, de onde quer que o script seja rodado.
ARQUIVO_SAIDA = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "jogadas_osu.csv")


def obter_token(client_id, client_secret):
    resposta = requests.post(
        "https://osu.ppy.sh/oauth/token",
        data={
            "client_id": client_id,
            "client_secret": client_secret,
            "grant_type": "client_credentials",
            "scope": "public",
        },
        headers={"Accept": "application/json"},
        timeout=30,
    )
    if resposta.status_code != 200:
        print("Falha ao obter o token. Resposta do servidor:", resposta.text)
        print(f"Client ID lido: [{client_id}]  |  tamanho do secret: {len(client_secret)}")
    resposta.raise_for_status()
    return resposta.json()["access_token"]


def buscar_melhores(token):
    jogadas = []
    # Quatro páginas de 50 (0-49, 50-99, 100-149, 150-199), sem depender
    # do limite máximo por requisição. O perfil mostra até 200 jogadas.
    for offset in (0, 50, 100, 150):
        resposta = requests.get(
            f"https://osu.ppy.sh/api/v2/users/{USER_ID}/scores/best",
            params={"mode": MODE, "limit": 50, "offset": offset},
            headers={"Authorization": f"Bearer {token}", "Accept": "application/json"},
            timeout=30,
        )
        resposta.raise_for_status()
        jogadas.extend(resposta.json())
    return jogadas


def main():
    # strip() remove espaços e aspas que às vezes vêm junto ao colar.
    client_id = (os.environ.get("OSU_CLIENT_ID") or "").strip().strip('"').strip("'")
    client_secret = (os.environ.get("OSU_CLIENT_SECRET") or "").strip().strip('"').strip("'")
    # Se as variáveis de ambiente não existirem, pergunta direto no terminal.
    if not client_id:
        client_id = input("Client ID (número da aplicação OAuth): ").strip()
    if not client_secret:
        client_secret = getpass.getpass("Client Secret (não aparece enquanto digita/cola): ").strip()
    if not client_id or not client_secret:
        sys.exit("Client ID e Client Secret são obrigatórios.")

    token = obter_token(client_id, client_secret)
    scores = buscar_melhores(token)

    linhas = []
    for s in scores:
        beatmap = s["beatmap"]
        beatmapset = s.get("beatmapset") or {}
        nome = f'{beatmapset.get("title", "?")} [{beatmap.get("version", "?")}]'
        nome = nome.replace(";", ",")  # o separador do arquivo é ponto e vírgula
        data = s.get("ended_at") or s.get("created_at")  # o nome do campo varia conforme o formato da API
        linhas.append({
            "nome_do_mapa": nome,
            "id_beatmap": beatmap["id"],
            "pp": f'{s["pp"]:.3f}',
            "data": data.replace("T", " ")[:19],
        })

    # Ordem cronológica, que é a ordem original do Cenário A.
    linhas.sort(key=lambda linha: linha["data"])

    with open(ARQUIVO_SAIDA, "w", newline="", encoding="utf-8") as arquivo:
        escritor = csv.DictWriter(
            arquivo,
            fieldnames=["nome_do_mapa", "id_beatmap", "pp", "data"],
            delimiter=";",
        )
        escritor.writeheader()
        escritor.writerows(linhas)

    print(f"{len(linhas)} jogadas salvas em {ARQUIVO_SAIDA}")
    if len(linhas) != 200:
        print(f"ATENÇÃO: esperava 200 jogadas e vieram {len(linhas)}.")

    repetidos = [i for i, n in Counter(l["id_beatmap"] for l in linhas).items() if n > 1]
    if repetidos:
        print("ATENÇÃO: IDs de beatmap repetidos:", repetidos)
        print("O desempate pelo id_beatmap não basta; será preciso outro critério.")
    else:
        print("Nenhum ID de beatmap repetido: o desempate pelo id_beatmap funciona.")

    pps = Counter(l["pp"] for l in linhas)
    empates = {pp: n for pp, n in pps.items() if n > 1}
    print("Empates de pp:", empates if empates else "nenhum")


if __name__ == "__main__":
    main()
