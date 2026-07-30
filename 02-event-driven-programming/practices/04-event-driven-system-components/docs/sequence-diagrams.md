# Sequence Diagrams

## Local publish-subscribe

```text
Sensor -> PubSub: SENSOR_DATA
PubSub -> Event Pool: allocate copy
PubSub -> Display AO: enqueue
PubSub -> Diagnostic AO: enqueue
Scheduler -> Display AO: dispatch
Scheduler -> Diagnostic AO: dispatch
Scheduler -> Event Pool: release
```

## Remote event

```text
Local AO -> Router -> Serializer -> Data-Link -> UART/IPC -> Parser -> Remote Router -> Remote AO
```
