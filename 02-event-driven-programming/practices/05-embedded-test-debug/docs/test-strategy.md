# Test Strategy

## Test pyramid

1. Host unit test cho function/module.
2. Integration test cho Queue + Timer + State Machine.
3. Target test cho peripheral và interrupt.
4. HIL test cho use case sản phẩm.
5. Stress và soak test cho tải và thời gian dài.

## Regression rule

Mỗi bug nên có test tái hiện:

```text
Reproduce -> Write failing test -> Fix -> PASS -> Keep in suite
```

## Exit criteria

- Không warning.
- Root test PASS.
- Sanitizer PASS.
- Pool và queue cleanup invariant đúng.
- Error path, timeout và exhaustion path được test.
