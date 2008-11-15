#ifndef CLIENT_HEADER
#define CLIENT_HEADER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <getopt.h>
#include "header.h"

#define PACKET_NUM 4
#define BUFFERSIZE (PAYLOAD_SIZE)*PACKET_NUM

extern struct sockaddr_in src_addr, self_addr;
extern int sock;
extern int drop_p, delay_p;
extern int delay_t;

#endif
