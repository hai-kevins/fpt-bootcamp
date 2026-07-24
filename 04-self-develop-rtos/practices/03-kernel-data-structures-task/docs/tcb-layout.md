# TCB Layout

`saved_sp` phải ở offset 0 vì Cortex-M3 assembly truy cập trực tiếp. TCB còn chứa stack bounds, entry, argument, name, state, base/effective priority, list nodes và diagnostics. Mọi thay đổi layout phải có static assertion và được kiểm tra với disassembly.
