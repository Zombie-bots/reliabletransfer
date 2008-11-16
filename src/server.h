#ifndef SERVER_HEADER
#define SERVER_HEADER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <getopt.h>
#include "timeout.h"
#include "cong_control.h"
#include "udp_output.h"
#define BUFFERSIZE (PAYLOAD_SIZE)*4

#endif
