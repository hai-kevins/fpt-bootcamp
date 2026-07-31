#!/usr/bin/env bash
set -euo pipefail
root="$(cd "$(dirname "$0")/.." && pwd)"
for lab in \
  01-task-state-machine \
  02-efficient-blocking \
  03-delayed-list-tick-wrap \
  04-delay-until \
  05-semaphore-host \
  07-mutex-ownership \
  08-priority-inheritance \
  09-suspend-resume
do
  echo "== $lab =="
  make -C "$root/labs/$lab" clean test
 done
