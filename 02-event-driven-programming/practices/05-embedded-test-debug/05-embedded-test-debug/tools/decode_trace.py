#!/usr/bin/env python3
import argparse
import struct
from pathlib import Path

RECORD = struct.Struct("<IHHBBBB")

def main() -> int:
    parser = argparse.ArgumentParser(
        description="Decode fixed-size event trace records."
    )
    parser.add_argument("file", type=Path)
    args = parser.parse_args()

    data = args.file.read_bytes()
    if len(data) % RECORD.size != 0:
        raise SystemExit(
            f"Invalid size {len(data)}; record size is {RECORD.size}"
        )

    for index in range(0, len(data), RECORD.size):
        values = RECORD.unpack_from(data, index)
        timestamp, signal, argument, kind, source, destination, state = values
        print(
            f"t={timestamp:8d} type={kind:2d} "
            f"src={source:2d} dst={destination:2d} "
            f"sig={signal:4d} arg={argument:5d} state={state:2d}"
        )

    return 0

if __name__ == "__main__":
    raise SystemExit(main())
