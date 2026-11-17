#!/bin/sh
# Run from the project directory; never modify original input files.
set -eu
base=$(pwd)
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT HUP INT TERM
make clean
make
cp banker customer.txt commands.txt expected.txt "$tmp/"
cd "$tmp"
passed=0
run() {
    title=$1; expected=$2; shift 2
    printf '\nTeste: %s\n' "$title"
    if "$@"; then actual=0; else actual=$?; fi
    printf 'retorno: %s (esperado: %s)\n' "$actual" "$expected"
    test "$actual" -eq "$expected"
    passed=$((passed+1))
}
check() {
    printf '\nTeste: %s\n' "$1"; shift
    "$@"
    printf 'OK\n'
    passed=$((passed+1))
}
run 'exemplo do professor' 0 ./banker 10 5 7
check 'comparacao com result.pdf' diff -u expected.txt result.txt
cat result.txt
run 'sem argumentos' 1 ./banker
run 'numero incorreto de recursos' 1 ./banker 10 5
run 'argumento negativo' 1 ./banker 10 -5 7
run 'argumento nao numerico' 1 ./banker 10 abc 7
run 'argumento fora do limite de int' 1 ./banker 99999999999999 5 7
mv customer.txt customer.bak
run 'customer.txt ausente' 1 ./banker 10 5 7
mv customer.bak customer.txt
mv commands.txt commands.bak
run 'commands.txt ausente' 1 ./banker 10 5 7
printf 'RQ 0 0 2\n' > commands.txt
run 'comando com quantidade incorreta' 1 ./banker 10 5 7
printf 'RQ 9 0 0 0\n' > commands.txt
run 'cliente inexistente' 1 ./banker 10 5 7
printf 'RQ 0 0 -1 0\n' > commands.txt
run 'pedido negativo' 1 ./banker 10 5 7
printf 'XX 0 0 0 0\n' > commands.txt
run 'comando desconhecido' 1 ./banker 10 5 7
printf 'RQ 0 0 0 0 extra\n' > commands.txt
run 'comando com token extra' 1 ./banker 10 5 7
: > commands.txt
run 'commands.txt vazio' 1 ./banker 10 5 7
printf '7,5,3\n3,2\n' > customer.txt
mv commands.bak commands.txt
run 'linha de cliente incompleta' 1 ./banker 10 5 7
printf '7,5,3,\n' > customer.txt
run 'cliente com virgula extra' 1 ./banker 10 5 7
: > customer.txt
run 'customer.txt vazio' 1 ./banker 10 5 7
printf '3\n3\n' > customer.txt
printf 'RQ 0 1\nRQ 1 1\n*\nRL 0 2\nRL 0 1\n*\n' > commands.txt
run 'um recurso, dois clientes, rollback e liberacao invalida' 0 ./banker 3
cat > dynamic.expected <<'EOT'
Allocate to customer 0 the resources 1
The customer 1 request 1 was denied because result in an unsafe state
MAXIMUM | ALLOCATION | NEED
3 | 1 | 2
3 | 0 | 3
AVAILABLE 2
The customer 0 release 2 was denied because exceed its current allocation
Release from customer 0 the resources 1
MAXIMUM | ALLOCATION | NEED
3 | 0 | 3
3 | 0 | 3
AVAILABLE 3
EOT
check 'rollback preserva estado e liberacao invalida nao altera matrizes' diff -u dynamic.expected result.txt
cat result.txt
printf '0,1,0,2\n' > customer.txt
printf 'RQ 0 0 1 0 2\n*\nRL 0 0 1 0 2\n*\n' > commands.txt
run 'um cliente, quatro recursos, capacidade zero' 0 ./banker 0 1 0 2
cat > four.expected <<'EOT'
Allocate to customer 0 the resources 0 1 0 2
MAXIMUM | ALLOCATION | NEED
0 1 0 2 | 0 1 0 2 | 0 0 0 0
AVAILABLE 0 0 0 0
Release from customer 0 the resources 0 1 0 2
MAXIMUM | ALLOCATION | NEED
0 1 0 2 | 0 0 0 0 | 0 1 0 2
AVAILABLE 0 1 0 2
EOT
check 'matrizes com quatro recursos' diff -u four.expected result.txt
printf '\nTotal de verificacoes aprovadas: %s\n' "$passed"
