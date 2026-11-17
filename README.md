# Algoritmo de Banker

Atividade da disciplina de Infraestrutura de Software, que consiste na implementação do algoritmo de Banker em C.

Fiz a implementação utilizando o Ubuntu pelo WSL.

## Como rodar

Primeiramente, coloque os arquivos customer.txt e commands.txt na pasta do projeto.

```bash
make
./banker 10 5 7
```

O resultado da execução estará no arquivo result.txt.

Para remover o executável:

```bash
make clean
```

## Testes

Realizei alguns testes para verificar o funcionamento do algoritmo, o tratamento de erros e possíveis vazamentos de memória.

Os resultados desses testes estão no arquivo evidencias.log.
