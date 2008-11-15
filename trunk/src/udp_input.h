#ifndef UDP_INPUT_HEADER
#define UPD_INPUT_HEADER

#include "checksum.h"
#include "link_list.h"
#include "misc.h"
//#include "../test/utils.h"
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct delay_array
{
  u_short seq;
  time_t tv;
};
void init_da();
void insert_da(u_short seq);
void traverse_da();
void make_ack(u_short seq, packet_t *ack_p);
void send_ack(u_short seq);
int rudp_recv(int sock, char *receive_buf, struct sockaddr_in *self_addr,\
              struct sockaddr *src_addr, unsigned *src_addr_len, size_t *recv_size);

#endif
