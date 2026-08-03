#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
    int val;
    struct ListNode* next;
} ListNode;

// === ONLY CODE WITHIN THE DESIGNATED AREA . DO NOT MODIFY OTHER PARTS ===
ListNode* reverse(ListNode* head) {
    ListNode *prev = NULL, *cur = head;

    while (cur) {
        ListNode *next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }

    return prev;
}

ListNode* removeNodes(ListNode* head) {
    if (head == NULL || head->next == NULL)
        return head;

    head = reverse(head);

    int maxVal = head->val;
    ListNode *cur = head;

    while (cur->next) {
        if (cur->next->val >= maxVal) {
            maxVal = cur->next->val;
            cur = cur->next;
        } else {
            ListNode *tmp = cur->next;
            cur->next = tmp->next;
            free(tmp);
        }
    }

    return reverse(head);
}
// === DO NOT MODIFY OTHER PARTS ===

static ListNode* newNode(int value) {
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    node->val = value;
    node->next = NULL;
    return node;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    ListNode* head = NULL;
    ListNode* tail = NULL;
    for (int i = 0; i < n; i++) {
        int value;
        scanf("%d", &value);
        ListNode* node = newNode(value);
        if (tail) tail->next = node;
        else head = node;
        tail = node;
    }
    head = removeNodes(head);
    printf("\n__JUDGE_RESULT__\n");
    for (ListNode* cur = head; cur; cur = cur->next) {
        if (cur != head) printf(" ");
        printf("%d", cur->val);
    }
    return 0;
}