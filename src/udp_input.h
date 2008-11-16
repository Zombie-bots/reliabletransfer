#ifndef UDP_INPUT_HEADER
#define UPD_INPUT_HEADER

#include "checksum.h"
#include "link_list.h"
#include "misc.h"
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct delay_node
{
  u_short seq;
  struct timeval tv;
  struct delay_node *next;
};

extern struct delay_node *DELAY_HEAD;

void insert_dh(u_short seq);
void traverse_dh();
void make_ack(u_short seq, packet_t *ack_p);
void send_ack(u_short seq);
int rudp_recv(int sock, char *receive_buf, struct sockaddr_in *self_addr,\
              struct sockaddr *src_addr, unsigned *src_addr_len, size_t *recv_size);

#endif
