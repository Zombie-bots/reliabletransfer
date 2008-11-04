#include "header.h"
/* fill header without checksum */
int fill_header(u_short seq, u_short ack, u_short offset, u_short flag, u_char* buffer)
{
	header_t h;
	h.seq=seq;
	h.ack=ack;
	h.offset=offset;
	h.flag=flag;
	memcpy(buffer,h,sizeof(header_t));
	return 0;
}
