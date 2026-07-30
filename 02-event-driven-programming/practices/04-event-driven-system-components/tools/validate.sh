#!/usr/bin/env sh
set -eu

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
cd "$ROOT_DIR"

printf '%s\n' '== Root tests =='
make clean >/dev/null
make test
make run
make sanitize

passed=0
for lab in labs/[0-9][0-9]-*; do
    printf '\n== %s ==\n' "$lab"
    make -C "$lab" clean >/dev/null
    make -C "$lab" run
    make -C "$lab" sanitize
    passed=$((passed + 1))
done

printf '\nValidation PASS: root + %s labs\n' "$passed"
