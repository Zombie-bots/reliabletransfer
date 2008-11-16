#ifndef LINK_LIST_HEADER
#define LINK_LIST_HEADER
#include "header.h"
#include <sys/time.h>

struct node
{
  int data;//SeqNum
  struct timeval send_time;
  struct timeval expire_time;
  packet_t sent_packet;
  struct node *next;
};

extern struct node  *TIMER_LIST;

struct node *find(int num);
void make_node (packet_t *packet, struct node *pnode);
void append(struct node *pnode);
void movetoend(struct node *r);
void delnode(struct node *r);

#endif
