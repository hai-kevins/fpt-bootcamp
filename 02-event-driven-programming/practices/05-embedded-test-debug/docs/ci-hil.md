# CI và Hardware-in-the-Loop

## CI host

```text
Format/static checks
      |
      v
Build with -Werror
      |
      v
Unit tests
      |
      v
ASan/UBSan
      |
      v
Artifact and report
```

## HIL

```text
Test PC
  |-- Reset DUT
  |-- Send UART command
  |-- Drive GPIO/CAN input
  |-- Read response
  |-- Check timeout
  v
Device Under Test
```

HIL case cần có precondition, stimulus, expected response, timeout, cleanup và report.
