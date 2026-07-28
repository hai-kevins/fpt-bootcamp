from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
required=['README.md','Makefile','startup/startup.c','linker/memory.ld','include/queue.h','src/queue.c','include/message_pool.h','src/message_pool.c','include/software_timer.h','src/software_timer.c','include/benchmark_clock.h','src/benchmark_clock.c','include/benchmark_stats.h','src/benchmark_stats.c','labs/README.md']
required += [f'labs/{name}/README.md' for name in ['01-static-ring-buffer-queue','02-blocking-queue-host','03-queue-from-isr','04-message-pool','05-software-timer-list','06-timer-service-task','07-timestamp-backends','08-context-switch-benchmark','09-event-message-latency','10-target-communication-benchmark-console']]
missing=[p for p in required if not (ROOT/p).is_file()]
if missing:
 print('STRUCTURE CHECK: FAIL')
 for p in missing: print('missing:',p)
 raise SystemExit(1)
print(f'STRUCTURE CHECK: PASS ({len(required)} required files)')
