#include "model.h"
bool tick_reached(uint32_t n,uint32_t d){return (int32_t)(n-d)>=0;}bool tick_before(uint32_t a,uint32_t b){return (int32_t)(a-b)<0;}
void delayed_init(delayed_t*d){if(d)d->count=0;}
bool delayed_insert(delayed_t*d,task_t*t){if(!d||!t||t->linked||d->count>=16)return false;size_t i=0;while(i<d->count&&!tick_before(t->wake,d->items[i]->wake))++i;for(size_t j=d->count;j>i;--j)d->items[j]=d->items[j-1];d->items[i]=t;t->linked=true;++d->count;return true;}
task_t*delayed_take_due(delayed_t*d,uint32_t now){if(!d||d->count==0||!tick_reached(now,d->items[0]->wake))return 0;task_t*t=d->items[0];for(size_t i=1;i<d->count;++i)d->items[i-1]=d->items[i];--d->count;t->linked=false;return t;}
bool delayed_validate(const delayed_t*d){if(!d||d->count>16)return false;for(size_t i=0;i<d->count;++i){if(!d->items[i]||!d->items[i]->linked)return false;if(i&&tick_before(d->items[i]->wake,d->items[i-1]->wake))return false;}return true;}
