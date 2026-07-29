# Algorithm Practices

Bộ bài tập luyện cấu trúc dữ liệu và thuật toán bằng ngôn ngữ C, được sắp xếp từ **Week 1** đến **Extra III** theo từng nhóm kiến thức.

## Overview

| Section | Main topics | Problems |
|---|---|---:|
| [Week 1](./week-01) | `Array` · `String` · `Two Pointers` · `Sliding Window` | 10 |
| [Week 2 & 3](./week-02%2603) | `Linked List` · `Queue` · `Stack` · `Sorting` | 20 |
| [Week 4](./week-04) | `Linear Search` · `Binary Search` | 10 |
| [Week 5](./week-05) | `Breadth-First Search` · `Graph` · `Tree` · `Matrix` | 10 |
| [Week 6](./week-06) | `Depth-First Search` · `Graph` · `Tree` · `Backtracking` | 11 |
| [Extra I](./extra-01) | `Stack` · `String` · `Linked List` · `Array` · `Union-Find` · `Intervals` | 30 |
| [Extra II](./extra-02) | `Heap` · `Priority Queue` · `Quickselect` · `Median` · `Monotonic Stack` | 9 |
| [Extra III](./extra-03) | `Graph` · `Dijkstra` · `0-1 BFS` · `Bellman-Ford` · `Shortest Path` | 6 |
| **Total** | **Easy: 45 · Medium: 52 · Hard: 9** | **106** |

## Directory Structure

```text
practices/
├── week-01/
│   └── <problem-id>-<problem-name>/
│       └── <problem-id>-<problem-name>.c
├── week-02&03/
│   └── <problem-id>-<problem-name>/
│       └── <problem-id>-<problem-name>.c
├── week-04/
│   └── <problem-id>-<problem-name>/
│       └── <problem-id>-<problem-name>.c
├── week-05/
│   └── <problem-id>-<problem-name>/
│       └── <problem-id>-<problem-name>.c
├── week-06/
│   └── <problem-id>-<problem-name>/
│       └── <problem-id>-<problem-name>.c
├── extra-01/
│   └── <problem-id>-<problem-name>/
│       └── <problem-id>-<problem-name>.c
├── extra-02/
│   └── <problem-id>-<problem-name>/
│       └── <problem-id>-<problem-name>.c
└── extra-03/
    └── <problem-id>-<problem-name>/
        └── <problem-id>-<problem-name>.c
```

Mỗi bài sử dụng mã LeetCode gồm bốn chữ số, ví dụ `0088-merge-sorted-array`. Tên thư mục và tên file C luôn giống nhau.

# Week 1 — Array, String, Two Pointers, Sliding Window

**Topics:** `Array` · `String` · `Two Pointers` · `Sliding Window`

| No. | Problem | Difficulty |
|---:|---|---|
| 1 | [0088-merge-sorted-array](https://github.com/haikevins/dsa/tree/main/practices/week-01/0088-merge-sorted-array) | Easy |
| 2 | [0020-valid-parentheses](https://github.com/haikevins/dsa/tree/main/practices/week-01/0020-valid-parentheses) | Easy |
| 3 | [1662-check-if-two-string-arrays-are-equivalent](https://github.com/haikevins/dsa/tree/main/practices/week-01/1662-check-if-two-string-arrays-are-equivalent) | Easy |
| 4 | [1431-kids-with-the-greatest-number-of-candies](https://github.com/haikevins/dsa/tree/main/practices/week-01/1431-kids-with-the-greatest-number-of-candies) | Easy |
| 5 | [0657-robot-return-to-origin](https://github.com/haikevins/dsa/tree/main/practices/week-01/0657-robot-return-to-origin) | Easy |
| 6 | [0389-find-the-difference](https://github.com/haikevins/dsa/tree/main/practices/week-01/0389-find-the-difference) | Easy |
| 7 | [0643-maximum-average-subarray-i](https://github.com/haikevins/dsa/tree/main/practices/week-01/0643-maximum-average-subarray-i) | Easy |
| 8 | [0003-longest-substring-without-repeating-characters](https://github.com/haikevins/dsa/tree/main/practices/week-01/0003-longest-substring-without-repeating-characters) | Medium |
| 9 | [0049-group-anagrams](https://github.com/haikevins/dsa/tree/main/practices/week-01/0049-group-anagrams) | Medium |
| 10 | [0383-ransom-note](https://github.com/haikevins/dsa/tree/main/practices/week-01/0383-ransom-note) | Easy |

# Week 2 & 3 — Linked List, Queue, Stack, Sorting

**Topics:** `Linked List` · `Queue` · `Stack` · `Sorting`

| No. | Problem | Difficulty |
|---:|---|---|
| 1 | [0109-convert-sorted-list-to-binary-search-tree](https://github.com/haikevins/dsa/tree/main/practices/week-02%2603/0109-convert-sorted-list-to-binary-search-tree) | Medium |
| 2 | [0225-implement-stack-using-queues](https://github.com/haikevins/dsa/tree/main/practices/week-02%2603/0225-implement-stack-using-queues) | Easy |
| 3 | [0232-implement-queue-using-stacks](https://github.com/haikevins/dsa/tree/main/practices/week-02%2603/0232-implement-queue-using-stacks) | Easy |
| 4 | [1544-make-the-string-great](https://github.com/haikevins/dsa/tree/main/practices/week-02%2603/1544-make-the-string-great) | Easy |
| 5 | [0739-daily-temperatures](https://github.com/haikevins/dsa/tree/main/practices/week-02%2603/0739-daily-temperatures) | Medium |
| 6 | [0150-evaluate-reverse-polish-notation](https://github.com/haikevins/dsa/tree/main/practices/week-02%2603/0150-evaluate-reverse-polish-notation) | Medium |
| 7 | [0735-asteroid-collision](https://github.com/haikevins/dsa/tree/main/practices/week-02%2603/0735-asteroid-collision) | Medium |
| 8 | [1209-remove-all-adjacent-duplicates-in-string-ii](https://github.com/haikevins/dsa/tree/main/practices/week-02%2603/1209-remove-all-adjacent-duplicates-in-string-ii) | Medium |
| 9 | [0622-design-circular-queue](https://github.com/haikevins/dsa/tree/main/practices/week-02%2603/0622-design-circular-queue) | Medium |
| 10 | [0203-remove-linked-list-elements](https://github.com/haikevins/dsa/tree/main/practices/week-02%2603/0203-remove-linked-list-elements) | Easy |
| 11 | [0237-delete-node-in-a-linked-list](https://github.com/haikevins/dsa/tree/main/practices/week-02%2603/0237-delete-node-in-a-linked-list) | Medium |
| 12 | [0019-remove-nth-node-from-end-of-list](https://github.com/haikevins/dsa/tree/main/practices/week-02%2603/0019-remove-nth-node-from-end-of-list) | Medium |
| 13 | [0086-partition-list](https://github.com/haikevins/dsa/tree/main/practices/week-02%2603/0086-partition-list) | Medium |
| 14 | [0561-array-partition](https://github.com/haikevins/dsa/tree/main/practices/week-02%2603/0561-array-partition) | Easy |
| 15 | [0977-squares-of-a-sorted-array](https://github.com/haikevins/dsa/tree/main/practices/week-02%2603/0977-squares-of-a-sorted-array) | Easy |
| 16 | [0268-missing-number](https://github.com/haikevins/dsa/tree/main/practices/week-02%2603/0268-missing-number) | Easy |
| 17 | [0414-third-maximum-number](https://github.com/haikevins/dsa/tree/main/practices/week-02%2603/0414-third-maximum-number) | Easy |
| 18 | [0217-contains-duplicate](https://github.com/haikevins/dsa/tree/main/practices/week-02%2603/0217-contains-duplicate) | Easy |
| 19 | [0075-sort-colors](https://github.com/haikevins/dsa/tree/main/practices/week-02%2603/0075-sort-colors) | Medium |
| 20 | [0179-largest-number](https://github.com/haikevins/dsa/tree/main/practices/week-02%2603/0179-largest-number) | Medium |

# Week 4 — Linear Search and Binary Search

**Topics:** `Linear Search` · `Binary Search`

| No. | Problem | Difficulty |
|---:|---|---|
| 1 | [0069-sqrtx](https://github.com/haikevins/dsa/tree/main/practices/week-04/0069-sqrtx) | Easy |
| 2 | [0704-binary-search](https://github.com/haikevins/dsa/tree/main/practices/week-04/0704-binary-search) | Easy |
| 3 | [0875-koko-eating-bananas](https://github.com/haikevins/dsa/tree/main/practices/week-04/0875-koko-eating-bananas) | Medium |
| 4 | [1388-pizza-with-3n-slices](https://github.com/haikevins/dsa/tree/main/practices/week-04/1388-pizza-with-3n-slices) | Hard |
| 5 | [1237-find-positive-integer-solution-for-a-given-equation](https://github.com/haikevins/dsa/tree/main/practices/week-04/1237-find-positive-integer-solution-for-a-given-equation) | Medium |
| 6 | [1346-check-if-n-and-its-double-exist](https://github.com/haikevins/dsa/tree/main/practices/week-04/1346-check-if-n-and-its-double-exist) | Easy |
| 7 | [0169-majority-element](https://github.com/haikevins/dsa/tree/main/practices/week-04/0169-majority-element) | Easy |
| 8 | [0367-valid-perfect-square](https://github.com/haikevins/dsa/tree/main/practices/week-04/0367-valid-perfect-square) | Easy |
| 9 | [0034-find-first-and-last-position-of-element-in-sorted-array](https://github.com/haikevins/dsa/tree/main/practices/week-04/0034-find-first-and-last-position-of-element-in-sorted-array) | Medium |
| 10 | [0153-find-minimum-in-rotated-sorted-array](https://github.com/haikevins/dsa/tree/main/practices/week-04/0153-find-minimum-in-rotated-sorted-array) | Medium |

# Week 5 — Breadth-First Search

**Topics:** `Breadth-First Search` · `Graph` · `Tree` · `Matrix`

| No. | Problem | Difficulty |
|---:|---|---|
| 1 | [0773-sliding-puzzle](https://github.com/haikevins/dsa/tree/main/practices/week-05/0773-sliding-puzzle) | Hard |
| 2 | [0200-number-of-islands](https://github.com/haikevins/dsa/tree/main/practices/week-05/0200-number-of-islands) | Medium |
| 3 | [0994-rotting-oranges](https://github.com/haikevins/dsa/tree/main/practices/week-05/0994-rotting-oranges) | Medium |
| 4 | [1926-nearest-exit-from-entrance-in-maze](https://github.com/haikevins/dsa/tree/main/practices/week-05/1926-nearest-exit-from-entrance-in-maze) | Medium |
| 5 | [0111-minimum-depth-of-binary-tree](https://github.com/haikevins/dsa/tree/main/practices/week-05/0111-minimum-depth-of-binary-tree) | Easy |
| 6 | [0542-01-matrix](https://github.com/haikevins/dsa/tree/main/practices/week-05/0542-01-matrix) | Medium |
| 7 | [0785-is-graph-bipartite](https://github.com/haikevins/dsa/tree/main/practices/week-05/0785-is-graph-bipartite) | Medium |
| 8 | [0463-island-perimeter](https://github.com/haikevins/dsa/tree/main/practices/week-05/0463-island-perimeter) | Easy |
| 9 | [0690-employee-importance](https://github.com/haikevins/dsa/tree/main/practices/week-05/0690-employee-importance) | Medium |
| 10 | [0559-maximum-depth-of-n-ary-tree](https://github.com/haikevins/dsa/tree/main/practices/week-05/0559-maximum-depth-of-n-ary-tree) | Easy |

# Week 6 — Depth-First Search

**Topics:** `Depth-First Search` · `Graph` · `Tree` · `Backtracking`

| No. | Problem | Difficulty |
|---:|---|---|
| 1 | [0733-flood-fill](https://github.com/haikevins/dsa/tree/main/practices/week-06/0733-flood-fill) | Easy |
| 2 | [0104-maximum-depth-of-binary-tree](https://github.com/haikevins/dsa/tree/main/practices/week-06/0104-maximum-depth-of-binary-tree) | Easy |
| 3 | [0690-employee-importance](https://github.com/haikevins/dsa/tree/main/practices/week-06/0690-employee-importance) | Medium |
| 4 | [0079-word-search](https://github.com/haikevins/dsa/tree/main/practices/week-06/0079-word-search) | Medium |
| 5 | [0547-number-of-provinces](https://github.com/haikevins/dsa/tree/main/practices/week-06/0547-number-of-provinces) | Medium |
| 6 | [0841-keys-and-rooms](https://github.com/haikevins/dsa/tree/main/practices/week-06/0841-keys-and-rooms) | Medium |
| 7 | [0543-diameter-of-binary-tree](https://github.com/haikevins/dsa/tree/main/practices/week-06/0543-diameter-of-binary-tree) | Easy |
| 8 | [0934-shortest-bridge](https://github.com/haikevins/dsa/tree/main/practices/week-06/0934-shortest-bridge) | Medium |
| 9 | [0226-invert-binary-tree](https://github.com/haikevins/dsa/tree/main/practices/week-06/0226-invert-binary-tree) | Easy |
| 10 | [0872-leaf-similar-trees](https://github.com/haikevins/dsa/tree/main/practices/week-06/0872-leaf-similar-trees) | Easy |
| 11 | [1304-find-n-unique-integers-sum-up-to-zero](https://github.com/haikevins/dsa/tree/main/practices/week-06/1304-find-n-unique-integers-sum-up-to-zero) | Easy |

# Extra I — Mixed Data Structures and Algorithms

**Topics:** `Stack` · `String` · `Linked List` · `Array` · `Union-Find` · `Intervals`

| No. | Problem | Difficulty |
|---:|---|---|
| 1 | [0921-minimum-add-to-make-parentheses-valid](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0921-minimum-add-to-make-parentheses-valid) | Medium |
| 2 | [1021-remove-outermost-parentheses](https://github.com/haikevins/dsa/tree/main/practices/extra-01/1021-remove-outermost-parentheses) | Easy |
| 3 | [1370-increasing-decreasing-string](https://github.com/haikevins/dsa/tree/main/practices/extra-01/1370-increasing-decreasing-string) | Easy |
| 4 | [0556-next-greater-element-iii](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0556-next-greater-element-iii) | Medium |
| 5 | [0496-next-greater-element-i](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0496-next-greater-element-i) | Easy |
| 6 | [0853-car-fleet](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0853-car-fleet) | Medium |
| 7 | [2126-destroying-asteroids](https://github.com/haikevins/dsa/tree/main/practices/extra-01/2126-destroying-asteroids) | Medium |
| 8 | [1974-minimum-time-to-type-word-using-special-typewriter](https://github.com/haikevins/dsa/tree/main/practices/extra-01/1974-minimum-time-to-type-word-using-special-typewriter) | Easy |
| 9 | [0796-rotate-string](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0796-rotate-string) | Easy |
| 10 | [1381-design-a-stack-with-increment-operation](https://github.com/haikevins/dsa/tree/main/practices/extra-01/1381-design-a-stack-with-increment-operation) | Medium |
| 11 | [0539-minimum-time-difference](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0539-minimum-time-difference) | Medium |
| 12 | [0547-number-of-provinces](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0547-number-of-provinces) | Medium |
| 13 | [0721-accounts-merge](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0721-accounts-merge) | Medium |
| 14 | [0088-merge-sorted-array](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0088-merge-sorted-array) | Easy |
| 15 | [0004-median-of-two-sorted-arrays](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0004-median-of-two-sorted-arrays) | Hard |
| 16 | [0209-minimum-size-subarray-sum](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0209-minimum-size-subarray-sum) | Medium |
| 17 | [0495-teemo-attacking](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0495-teemo-attacking) | Easy |
| 18 | [0986-interval-list-intersections](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0986-interval-list-intersections) | Medium |
| 19 | [0707-design-linked-list](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0707-design-linked-list) | Medium |
| 20 | [0206-reverse-linked-list](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0206-reverse-linked-list) | Easy |
| 21 | [0876-middle-of-the-linked-list](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0876-middle-of-the-linked-list) | Easy |
| 22 | [0083-remove-duplicates-from-sorted-list](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0083-remove-duplicates-from-sorted-list) | Easy |
| 23 | [0160-intersection-of-two-linked-lists](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0160-intersection-of-two-linked-lists) | Easy |
| 24 | [0024-swap-nodes-in-pairs](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0024-swap-nodes-in-pairs) | Medium |
| 25 | [0142-linked-list-cycle-ii](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0142-linked-list-cycle-ii) | Medium |
| 26 | [0023-merge-k-sorted-lists](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0023-merge-k-sorted-lists) | Hard |
| 27 | [0026-remove-duplicates-from-sorted-array](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0026-remove-duplicates-from-sorted-array) | Easy |
| 28 | [0053-maximum-subarray](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0053-maximum-subarray) | Medium |
| 29 | [0041-first-missing-positive](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0041-first-missing-positive) | Hard |
| 30 | [0153-find-minimum-in-rotated-sorted-array](https://github.com/haikevins/dsa/tree/main/practices/extra-01/0153-find-minimum-in-rotated-sorted-array) | Medium |

# Extra II — Heap, Priority Queue and Median

**Topics:** `Heap` · `Priority Queue` · `Quickselect` · `Median` · `Monotonic Stack`

| No. | Problem | Difficulty |
|---:|---|---|
| 1 | [0215-kth-largest-element-in-an-array](https://github.com/haikevins/dsa/tree/main/practices/extra-02/0215-kth-largest-element-in-an-array) | Medium |
| 2 | [1046-last-stone-weight](https://github.com/haikevins/dsa/tree/main/practices/extra-02/1046-last-stone-weight) | Easy |
| 3 | [0347-top-k-frequent-elements](https://github.com/haikevins/dsa/tree/main/practices/extra-02/0347-top-k-frequent-elements) | Medium |
| 4 | [0378-kth-smallest-element-in-a-sorted-matrix](https://github.com/haikevins/dsa/tree/main/practices/extra-02/0378-kth-smallest-element-in-a-sorted-matrix) | Medium |
| 5 | [0451-sort-characters-by-frequency](https://github.com/haikevins/dsa/tree/main/practices/extra-02/0451-sort-characters-by-frequency) | Medium |
| 6 | [0703-kth-largest-element-in-a-stream](https://github.com/haikevins/dsa/tree/main/practices/extra-02/0703-kth-largest-element-in-a-stream) | Easy |
| 7 | [0295-find-median-from-data-stream](https://github.com/haikevins/dsa/tree/main/practices/extra-02/0295-find-median-from-data-stream) | Hard |
| 8 | [0480-sliding-window-median](https://github.com/haikevins/dsa/tree/main/practices/extra-02/0480-sliding-window-median) | Hard |
| 9 | [0402-remove-k-digits](https://github.com/haikevins/dsa/tree/main/practices/extra-02/0402-remove-k-digits) | Medium |

# Extra III — Graph and Shortest Path

**Topics:** `Graph` · `Dijkstra` · `0-1 BFS` · `Bellman-Ford` · `Shortest Path`

| No. | Problem | Difficulty |
|---:|---|---|
| 1 | [0743-network-delay-time](https://github.com/haikevins/dsa/tree/main/practices/extra-03/0743-network-delay-time) | Medium |
| 2 | [1514-path-with-maximum-probability](https://github.com/haikevins/dsa/tree/main/practices/extra-03/1514-path-with-maximum-probability) | Medium |
| 3 | [1976-number-of-ways-to-arrive-at-destination](https://github.com/haikevins/dsa/tree/main/practices/extra-03/1976-number-of-ways-to-arrive-at-destination) | Medium |
| 4 | [0778-swim-in-rising-water](https://github.com/haikevins/dsa/tree/main/practices/extra-03/0778-swim-in-rising-water) | Hard |
| 5 | [0787-cheapest-flights-within-k-stops](https://github.com/haikevins/dsa/tree/main/practices/extra-03/0787-cheapest-flights-within-k-stops) | Medium |
| 6 | [2290-minimum-obstacle-removal-to-reach-corner](https://github.com/haikevins/dsa/tree/main/practices/extra-03/2290-minimum-obstacle-removal-to-reach-corner) | Hard |

## Build Notes

Các file chỉ chứa lời giải theo chữ ký hàm của LeetCode. Khi kiểm thử cục bộ, hãy thêm `main()` hoặc một test harness riêng rồi biên dịch bằng GCC:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic solution.c -o solution
./solution
```

Để phát hiện lỗi truy cập bộ nhớ và hành vi không xác định:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic \
    -fsanitize=address,undefined -g solution.c -o solution
./solution
```
