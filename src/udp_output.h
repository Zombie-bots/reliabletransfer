#ifndef UDP_OUTPUT_HEADER
#define UDP_OUTPUT_HEADER
#include "header.h"
#include "checksum.h"
#include "link_list.h"

int rudp_send(int socket, const void *buffer, size_t length, int flags);
#endif
