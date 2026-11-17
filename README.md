# Banker Algorithm

Implementação do algoritmo de Banker em C, com o objetivo
de evitar deadlocks durante a alocação de recursos.

## Funcionalidades

- Quantidade dinâmica de clientes e recursos.
- Solicitação de recursos (RQ).
- Liberação de recursos (RL).
- Exibição das matrizes (*).
- Verificação de estados seguros e inseguros.
- Restauração do estado após uma alocação insegura.
- Tratamento de erros.
- Gerenciamento dinâmico de memória.

## Ambiente

- Ubuntu 22.04 (WSL 2)
- Linguagem C (C11)
- GCC
- Make
- Valgrind

## Como executar

Clone o repositório:

```bash
git clone https://github.com/waltersantiago87/Banker-Algorithm.git
cd Banker-Algorithm
```

Coloque os arquivos customer.txt e commands.txt
no diretório do projeto antes da execução.

Compile:

```bash
make
```

Execute:

```bash
./banker 10 5 7
```

Visualize o resultado:

```bash
cat result.txt
```

Remova o executável:

```bash
make clean
```

## Arquitetura

| Arquivo | Responsabilidade |
|---|---|
| main.c | Fluxo principal |
| banker.h | Estrutura de dados |
| arguments.c | Validação dos argumentos |
| customers.c | Leitura dos clientes |
| matrices.c | Alocação e liberação de memória |
| safety.c | Algoritmo de segurança |
| request.c | Solicitação de recursos |
| release.c | Liberação de recursos |
| state.c | Exibição das matrizes |
| parser.c | Interpretação dos comandos |
| commands.c | Processamento dos comandos |
| output.c | Gerenciamento do arquivo de saída |
| Makefile | Compilação e limpeza |

## Testes

Para executar os testes automatizados:

```bash
bash testar.sh
```

O script requer customer.txt, commands.txt e expected.txt,
sendo este último a saída de referência do professor.

Teste de rollback:

```bash
bash tests/rollback.sh
```

Testes adicionais de erros:

```bash
bash tests/errors.sh
```

Teste de memória:

```bash
valgrind --leak-check=full --error-exitcode=99 ./banker 10 5 7
```

## Resultados

- 22 verificações automatizadas aprovadas.
- Teste de rollback aprovado.
- Testes adicionais de erros aprovados.
- Saída idêntica à referência do professor.
- 0 erros detectados pelo Valgrind.
- 0 bytes de memória restantes após a execução.

Os resultados estão registrados no arquivo evidencias.log.

## Informações acadêmicas

- Instituição: CESAR School
- Curso: Ciência da Computação
- Disciplina: Infraestrutura de Software
- Atividade: Algoritmo de Banker
