/*
 * LeetCode provides this structure:
 *
 * struct ListNode
 * {
 *     int val;
 *     struct ListNode *next;
 * };
 */

struct ListNode *partition(struct ListNode *head, int x)
{
    struct ListNode beforeDummy;
    struct ListNode afterDummy;
    struct ListNode *beforeTail = &beforeDummy;
    struct ListNode *afterTail = &afterDummy;

    beforeDummy.next = 0;
    afterDummy.next = 0;

    while (head != 0)
    {
        struct ListNode *nextNode = head->next;
        head->next = 0;

        if (head->val < x)
        {
            beforeTail->next = head;
            beforeTail = head;
        }
        else
        {
            afterTail->next = head;
            afterTail = head;
        }

        head = nextNode;
    }

    beforeTail->next = afterDummy.next;
    return beforeDummy.next;
}
