# Wire Protocol

```text
SOF | Version | SrcNode | DstNode | SrcAO | DstAO | Signal | Seq | Length | Payload | CRC16
```

- SOF: `0xA5`
- Version: `1`
- Signal and sequence: little-endian 16-bit
- Payload: up to 16 bytes in this study repository
- CRC: CRC-16/CCITT over header and payload
