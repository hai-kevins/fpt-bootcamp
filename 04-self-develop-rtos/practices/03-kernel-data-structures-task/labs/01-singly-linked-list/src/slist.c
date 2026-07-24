#include "slist.h"
void slist_init(slist_t *l){if(l){l->head=0;l->tail=0;l->count=0;}}
void slist_node_init(slist_node_t*n,int v){if(n){n->next=0;n->value=v;n->linked=false;}}
bool slist_push_front(slist_t*l,slist_node_t*n){if(!l||!n||n->linked)return false;n->next=l->head;n->linked=true;l->head=n;if(!l->tail)l->tail=n;l->count++;return true;}
bool slist_push_back(slist_t*l,slist_node_t*n){if(!l||!n||n->linked)return false;n->next=0;n->linked=true;if(l->tail)l->tail->next=n;else l->head=n;l->tail=n;l->count++;return true;}
slist_node_t*slist_pop_front(slist_t*l){if(!l||!l->head)return 0;slist_node_t*n=l->head;l->head=n->next;if(!l->head)l->tail=0;n->next=0;n->linked=false;l->count--;return n;}
slist_node_t*slist_find(const slist_t*l,int v){if(!l)return 0;for(slist_node_t*n=l->head;n;n=n->next)if(n->value==v)return n;return 0;}
bool slist_validate(const slist_t*l){if(!l)return false;if(l->count==0)return !l->head&&!l->tail;if(!l->head||!l->tail||l->tail->next)return false;const slist_node_t*s=l->head,*f=l->head;while(f&&f->next){s=s->next;f=f->next->next;if(s==f)return false;}size_t c=0;const slist_node_t*n=l->head,*last=0;while(n){if(!n->linked)return false;last=n;n=n->next;if(++c>l->count)return false;}return c==l->count&&last==l->tail;}
