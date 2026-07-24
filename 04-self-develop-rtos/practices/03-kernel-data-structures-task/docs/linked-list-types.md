# Linked-list Types

## Singly list
Một `next` pointer, phù hợp free list và LIFO. Remove known middle node cần previous.

## Doubly list
`previous` và `next`, remove known node O(1), phù hợp ready/wait/delayed lists.

## Circular list
Tail nối lại head, thuận tiện round-robin nhưng cần loop condition chặt.

## Sentinel list
Một node đặc biệt giảm special case cho head/tail. Không dùng container-of lên sentinel.
