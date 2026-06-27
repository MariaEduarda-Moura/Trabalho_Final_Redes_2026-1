# Simulador de Roteador IP (Trabalho Final - Redes)

**Status do Projeto**: Concluído.

Este projeto é um simulador em C que reproduz o funcionamento interno de roteadores da internet. Ele integra os planos de controle e dados, indo desde o cálculo de topologia até o despacho de quadros na camada de enlace.

## Principais Funcionalidades Implementadas

- **Construção de Topologia**: Criação de roteadores e links.
- **Roteamento (Plano de Controle)**: Cálculo automático das tabelas de roteamento usando o algoritmo de Dijkstra.
- **Tabela ARP**: Resolução de IPs em endereços MAC físicos.
- **Motor de Forwarding (Plano de Dados)**: Recebe pacotes, checa TTL, compara IPs usando *Longest Prefix Match (LPM)* e despacha.

## Como Compilar e Rodar

O projeto está centralizado em um único menu executável.

1. Compile o projeto utilizando o `Makefile` disponibilizado:
   - No Windows: `mingw32-make`
   - No Linux: `make`
   
> [!WARNING]
> **Atenção usuários de Windows**: O comando para limpar a build (`mingw32-make clean`) utiliza o utilitário nativo `del`. Se você tentar executá-lo de dentro de um terminal Linux/Bash (como o Git Bash), ele retornará um erro dizendo que o comando não foi encontrado. Recomendamos utilizar o **PowerShell** ou **Prompt de Comando (CMD)** para rodar o Makefile.

2. Execute o simulador:
   - No Windows: `.\Roteador.exe`
   - No Linux: `./Roteador`

No Menu Principal, você pode acessar a configuração da topologia (Opção 1) e o laboratório de pacotes (Opção 2).

### 🚀 Guia Recomendado de Teste

Para poupar tempo e não precisar cadastrar roteadores um por um manualmente, siga o fluxo abaixo:

1. No Menu Principal, escolha a **Opção 1** (Topologia de rede e roteadores).
2. Dentro do menu de Topologia, escolha a **Opção 10** (Carregar Topologia de Arquivo).
3. Digite o nome do arquivo pronto: `topologia_desenho.txt` e aperte Enter.
4. (Opcional) Adicione conexões/links entre os roteadores usando a **Opção 4**.
5. Execute o algoritmo de Dijkstra na **Opção 5** para calcular as tabelas de roteamento automaticamente.
6. Volte ao Menu Principal (**Opção 0**) e acesse a **Opção 2** para criar seus pacotes, ver as tabelas ARP funcionando e testar o encaminhamento!
