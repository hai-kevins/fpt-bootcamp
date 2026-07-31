#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
python3 "$ROOT/tools/check_structure.py"

make -C "$ROOT" clean all
make -C "$ROOT/labs/01-exception-stack-inspection" clean all
make -C "$ROOT/labs/02-initial-task-stack" clean test run
make -C "$ROOT/labs/03-svc-first-task" clean all
make -C "$ROOT/labs/04-cooperative-pendsv" clean all
make -C "$ROOT/labs/05-priority-scheduler-host" clean test run
make -C "$ROOT/labs/06-fixed-priority-target" clean all
make -C "$ROOT/labs/07-systick-time-slice" clean all
make -C "$ROOT/labs/08-isr-preemption" clean all
make -C "$ROOT/labs/09-round-robin" clean all
make -C "$ROOT/labs/10-context-switch-stress" clean all

echo "All automated checks: PASS"
