# Trabalho_Final_Redes_2026-1

**Status do Projeto**: Em desenvolvimento (Módulos separados aguardando integração final).

## Como testar os módulos isolados

### 1. Motor de Roteamento e Pacotes (Passo 4)
O arquivo `pacote.c` contém a simulação isolada do Motor de Forwarding, testando as regras de TTL e o algoritmo de Longest Prefix Match (LPM).
Para compilar e testar:
```bash
gcc pacote.c -o pacotes.exe
./pacotes.exe
```
> Utilize o menu interativo para cadastrar rotas provisórias, criar pacotes e testar o comportamento de decisão de encaminhamento do roteador.

### 2. Algoritmo de Dijkstra e Tabelas de Roteamento
Para testar a criação de topologias, links de rede e cálculo das tabelas de roteamento:
```bash
gcc table.c table_structs.c -o roteamento.exe
./roteamento.exe
```

### 3. Tabela ARP
Para testar a funcionalidade de mapeamento e inserção ARP isolada:
```bash
gcc main.c arp.c -o arp.exe
./arp.exe
```

---
**Pendências**: 
- Fazer o "Merge" de todos esses arquivos isolados.
- Falta integrar definitivamente as tabelas de `table.c` com o motor desenvolvido em `pacote.c` num arquivo/menu unificado.
