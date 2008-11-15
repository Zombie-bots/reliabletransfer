#ifndef UDP_INPUT_HEADER
#define UPD_INPUT_HEADER

#include "header.h"
#include "checksum.h"
#include "link_list.h"
#include "client.h"
//#include "../test/utils.h"

int rudp_recv(int sock, char *receive_buf, struct sockaddr_in *self_addr,
              struct sockaddr *src_addr, unsigned *src_addr_len, size_t *recv_size);

#endif
