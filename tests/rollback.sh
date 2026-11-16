#!/usr/bin/env bash
set -eu

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
make -C "$ROOT"

TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
cd "$TMP"

printf '3\n3\n' > customer.txt

cat > commands.txt <<'INPUT'
RQ 0 1
*
RQ 1 1
*
RQ 0 2
INPUT

"$ROOT/banker" 3

cat > expected.txt <<'EXPECTED'
Allocate to customer 0 the resources 1
MAXIMUM | ALLOCATION | NEED
3 | 1 | 2
3 | 0 | 3
AVAILABLE 2
The customer 1 request 1 was denied because result in an unsafe state
MAXIMUM | ALLOCATION | NEED
3 | 1 | 2
3 | 0 | 3
AVAILABLE 2
Allocate to customer 0 the resources 2
EXPECTED

diff -u expected.txt result.txt
echo "PASS: rollback preservou o estado do sistema"
