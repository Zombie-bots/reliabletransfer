#ifndef LINK_LIST_HEADER
#define LINK_LIST_HEADER
#include "header.h"
#define NULL 0
struct node
{
  int data;//SeqNum
  int timeout_counter;
  packet_t sent_packet;
  struct node *next;
};

extern struct node  *TIMER_LIST;

void decreasetime();
void append(struct node *pnode);
void movetoend(struct node *temp);
void delnode(int num);

#endif
