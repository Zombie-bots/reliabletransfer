#ifndef UDP_OUTPUT_HEADER
#define UDP_OUTPUT_HEADER
#include "header.h"
#include "checksum.h"
#include "timeout.h"
#include "link_list.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

void pro_header_ack(int seq);
void retran(int socket, const struct sockaddr *to, socklen_t tolen);
int rudp_send(int socket, char *buffer, size_t length, int flags,\
	      const struct sockaddr *to, socklen_t tolen, \
	      struct sockaddr_in *src_addr);
void resend_packet(int seq_number, int socket, const struct sockaddr *to, socklen_t tolen);
void print_timer();

#endif
