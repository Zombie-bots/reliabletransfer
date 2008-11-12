#ifndef UDP_OUTPUT_HEADER
#define UDP_OUTPUT_HEADER
#include "header.h"
#include "checksum.h"
#include "link_list.h"
#include <sys/socket.h>

int rudp_send(int socket, char *buffer, size_t length, int flags,\
	      const struct sockaddr *to, socklen_t tolen);
#endif
