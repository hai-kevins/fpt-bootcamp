#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
python3 "$ROOT/tools/check_structure.py"
"$ROOT/tools/run_host_tests.sh"
make -C "$ROOT" clean all
make -C "$ROOT/labs/10-target-task-inspector" clean all
