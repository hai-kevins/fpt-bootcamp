#include "serializer.h"
#include <stdio.h>
int main(void){ed_frame_t f={0},d;uint8_t b[ED_FRAME_MAX_SIZE];size_t n=0;f.source_node=1U;f.destination_node=2U;f.signal=0x55AAU;f.sequence=9U;f.payload_length=2U;f.payload[0]=7U;f.payload[1]=8U;int ok=ed_frame_encode(&f,b,sizeof(b),&n)&&ed_frame_decode(b,n,&d);(void)printf("bytes=%zu signal=0x%04X\n",n,d.signal);return(ok&&d.sequence==9U&&d.payload[1]==8U)?0:1;}
