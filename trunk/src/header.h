#ifndef HEADER_HEADER
#define HEADER_HEADER
#include <sys/types.h>
#define ACK 1
#define FIN 2

typedef struct _header_ {
	u_short seq;
	u_short ack;
	u_short checksum;
	u_short offset:10;
	u_short flag:6;
} header_t;

typedef struct _packet_ {
	header_t header;
	u_char data[1016];
} packet_t;

int fill_header(int a);
#endif
