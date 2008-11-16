#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include "utils.h"
#include "../src/link_list.h"
#include "../src/header.h"
#include "../src/checksum.h"

int main()
{
  TIMER_LIST = (struct node *)malloc(sizeof(struct node));
  TIMER_LIST->next = (void *)0;

  struct node *node, *node1, *node2, *node3, *node4;
  node = (struct node *)malloc(sizeof(struct node));
  node1 = (struct node *)malloc(sizeof(struct node));
  node2 = (struct node *)malloc(sizeof(struct node));
  node3 = (struct node *)malloc(sizeof(struct node));
  node4 = (struct node *)malloc(sizeof(struct node));
  packet_t packet;
  packet.header.seq = 1;

  //create 5 nodes
  make_node(&packet, node1);
  node->data = 4;
  node2->data = 5;
  node3->data = 7;
  node4->data = 9;

  //calculate the checksum for node num 4 and check it
  assert(fill_header(23, 44, 0, ACK, (packet_t*)&node->sent_packet) == 0);
  u_char buffer[5] = {23,32,34,1,56};
  assert(fill_packet((u_char*) buffer, (packet_t*)&node->sent_packet, 5) == 0);
  u_char dest_addr[] = {75,63,126,132};
  u_char src_addr[] = {13,12,73,10};
  node->sent_packet.header.checksum = htons(add_checksum(13, src_addr, dest_addr,
      1, (u_short *)&node->sent_packet));
  assert(add_checksum(13, src_addr, dest_addr, 1, (u_short *)&node->sent_packet) == 0);

  //append them to the list
  append(node1);
  append(node);
  append(node2);
  append(node3);
  append(node4);

  struct node *p;
  p = TIMER_LIST->next;

  //check the list
  assert(p->data == 1);
  p = p->next;
  assert(p->data == 4);
  p = p->next;
  assert(p->data == 5);
  p = p->next;
  assert(p->data == 7);
  p = p->next;
  assert(p->data == 9);
  assert(p->next == NULL);

  //delete the node with data = 4 (a node in the middle of list)
  struct node *r;
  r = find(4);
  delnode(r);

  //check the list
  p = TIMER_LIST->next;
  assert(p->data == 1);
  p = p->next;
  assert(p->data == 5);
  p = p->next;
  assert(p->data == 7);
  p = p->next;
  assert(p->data == 9);
  assert(p->next == NULL);

  //do the same thing to the node with data = 9 (the last node of list)
  r = find(9);
  delnode(r);

  p = TIMER_LIST->next;
  assert(p->data == 1);
  p = p->next;
  assert(p->data == 5);
  p = p->next;
  assert(p->data == 7);
  assert(p->next == NULL);

  //do the same thing to the node with data = 1 (the first node of list)
  r = find(1);
  delnode(r);

  p = TIMER_LIST->next;
  assert(p->data == 5);
  p = p->next;
  assert(p->data == 7);
  assert(p->next == NULL);

  free(TIMER_LIST);

  return 0;
}
