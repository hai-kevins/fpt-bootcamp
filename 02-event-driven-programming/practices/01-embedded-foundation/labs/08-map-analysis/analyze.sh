#!/usr/bin/env sh
set -eu

BUILD_DIR="${1:-build}"
TARGET="${2:-lab08_map_analysis}"
ELF="${BUILD_DIR}/${TARGET}.elf"
MAP="${BUILD_DIR}/${TARGET}.map"

if [ ! -f "${ELF}" ]; then
    echo "ELF not found: ${ELF}" >&2
    echo "Run make first." >&2
    exit 1
fi

echo "== Size =="
arm-none-eabi-size "${ELF}"

echo
echo "== Sections =="
arm-none-eabi-objdump -h "${ELF}"

echo
echo "== Largest symbols =="
arm-none-eabi-nm -S --size-sort "${ELF}" | tail -n 20

echo
echo "== Important linker symbols =="
arm-none-eabi-nm -n "${ELF}" |
    grep -E '(_estack|_sidata|_sdata|_edata|_sbss|_ebss|_etext)' || true

echo
echo "Map file: ${MAP}"
