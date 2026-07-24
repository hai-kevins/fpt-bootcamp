#!/usr/bin/env bash
set -euo pipefail
root="$(cd "$(dirname "$0")/.." && pwd)"
python3 "$root/tools/check_structure.py"
"$root/tools/run_host_tests.sh"
make -C "$root" clean all
make -C "$root/labs/06-isr-semaphore-wake" clean all
make -C "$root/labs/10-target-synchronization-playground" clean all
