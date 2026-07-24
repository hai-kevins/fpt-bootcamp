#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
for lab in \
  01-singly-linked-list \
  02-intrusive-doubly-list \
  03-list-invariants \
  04-tcb-layout \
  05-static-task-creation \
  06-task-stack-initialization \
  07-all-task-registry \
  08-priority-ready-queues \
  09-tcb-multiple-lists
do
  echo "== $lab =="
  make -C "$ROOT/labs/$lab" clean test
 done
