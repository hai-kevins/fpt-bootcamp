#include "slist.h"
#include "test.h"

int main(void)
{
    slist_t l;
    slist_node_t a, b, c;
    slist_init(&l);
    slist_node_init(&a, 1);
    slist_node_init(&b, 2);
    slist_node_init(&c, 3);
    TEST_ASSERT(slist_validate(&l));
    TEST_ASSERT(slist_push_back(&l, &a));
    TEST_ASSERT(slist_push_back(&l, &b));
    TEST_ASSERT(slist_push_front(&l, &c));
    TEST_ASSERT(l.head == &c && l.tail == &b && l.count == 3);
    TEST_ASSERT(!slist_push_back(&l, &a));
    TEST_ASSERT(slist_find(&l, 2) == &b);
    TEST_ASSERT(slist_pop_front(&l) == &c);
    TEST_ASSERT(slist_validate(&l));
    TEST_PASS("singly linked list");
    return 0;
}
