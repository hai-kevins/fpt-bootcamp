#!/usr/bin/env python3
import argparse
from pathlib import Path

CASES = [
    ("state", "state=IDLE"),
    ("event start", "OK"),
    ("state", "state=RUNNING"),
    ("stats", "queue="),
]

def main() -> int:
    parser = argparse.ArgumentParser(
        description="Validate a captured UART shell transcript."
    )
    parser.add_argument(
        "transcript",
        type=Path,
        help="Text file containing DUT responses."
    )
    args = parser.parse_args()

    text = args.transcript.read_text(encoding="utf-8")
    passed = 0

    for command, expected in CASES:
        ok = expected in text
        print(
            f"[{'PASS' if ok else 'FAIL'}] "
            f"{command!r} expects {expected!r}"
        )
        passed += int(ok)

    print(f"Summary: {passed}/{len(CASES)} PASS")
    return 0 if passed == len(CASES) else 1

if __name__ == "__main__":
    raise SystemExit(main())
