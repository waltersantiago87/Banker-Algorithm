#!/bin/sh
set -eu

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
make -C "$ROOT"

TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
cd "$TMP"

printf '3\n3\n' > customer.txt

expect_failure() {
    if "$ROOT/banker" 3; then
        echo "FAIL: erro nao identificado"
        exit 1
    else
        test "$?" -eq 1
        echo "PASS: erro identificado"
    fi
}

echo "Teste: comando invalido apos alocacao"
printf 'RQ 0 1\nINVALIDO\n' > commands.txt
expect_failure
test ! -e result.txt

echo "Teste: caractere nulo no arquivo"
printf 'RQ 0 1\000\n' > commands.txt
expect_failure

echo "Teste: impossibilidade de criar result.txt"
printf '*\n' > commands.txt
mkdir result.txt
expect_failure
rmdir result.txt

echo "Todos os testes adicionais passaram."
