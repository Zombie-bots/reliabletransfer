#ifndef UDP_OUTPUT_HEADER
#define UDP_OUTPUT_HEADER
#include "header.h"
#include "checksum.h"
#include "timeout.h"
#include "link_list.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

void retran(int socket, const struct sockaddr *to, socklen_t tolen);
int rudp_send(int socket, char *buffer, size_t length, int flags,\
	      const struct sockaddr *to, socklen_t tolen, \
	      struct in_addr *src_addr);
#endif
