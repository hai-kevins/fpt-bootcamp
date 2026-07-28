#!/usr/bin/env bash
set -euo pipefail
root="$(cd "$(dirname "$0")/.." && pwd)"
for lab in 01-static-ring-buffer-queue 02-blocking-queue-host 04-message-pool 05-software-timer-list; do
  echo "== $lab =="
  make -C "$root/labs/$lab" clean test
 done
