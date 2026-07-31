#!/usr/bin/env bash
set -euo pipefail
root="$(cd "$(dirname "$0")/.." && pwd)"
python3 "$root/tools/check_structure.py"
"$root/tools/run_host_tests.sh"
make -C "$root" clean all
for lab in 03-queue-from-isr 06-timer-service-task 07-timestamp-backends 08-context-switch-benchmark 09-event-message-latency 10-target-communication-benchmark-console; do
  make -C "$root/labs/$lab" clean all
 done
