#!/usr/bin/env sh
set -eu

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)

echo "[1/3] Root tests"
make -C "$ROOT_DIR" clean >/dev/null
make -C "$ROOT_DIR" test

echo "[2/3] Root sanitizer"
make -C "$ROOT_DIR" sanitize

echo "[3/3] Labs"
for lab in "$ROOT_DIR"/labs/[0-9][0-9]-*
do
    echo "== $(basename "$lab") =="
    make -C "$lab" clean >/dev/null
    make -C "$lab" run
    make -C "$lab" sanitize
done

echo "VALIDATION PASS"
