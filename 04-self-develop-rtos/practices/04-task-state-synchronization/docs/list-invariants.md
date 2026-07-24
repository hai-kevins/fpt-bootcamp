# List Invariants

- Empty: head/tail NULL và count 0.
- Non-empty: head/tail khác NULL.
- Head previous NULL, tail next NULL.
- Forward/backward links đối xứng.
- Node owner đúng list.
- Count đúng số node.
- Không cycle ngoài thiết kế.

Validator đầy đủ dùng trong host/debug, không chạy mỗi PendSV.
