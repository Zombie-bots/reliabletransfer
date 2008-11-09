#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "../src/link_list.h"
#include "../src/header.h"
#include "../src/checksum.h"

int main()
{
  TIMER_LIST = (struct node *)malloc(sizeof(struct node));
  TIMER_LIST->next = NULL;
  struct node node, node1, node2, node3, node4;

  //create 5 nodes
  node1.data = 1;
  node1.timeout_counter = 3;
  node.data = 4;
  node.timeout_counter = 3;
  node2.data = 5;
  node2.timeout_counter = 4;
  node3.data = 7;
  node3.timeout_counter = 3;
  node4.data = 9;
  node4.timeout_counter = 5;

  //calculate the checksum for node num 4 and check it
  assert(fill_header(2, 4, 0, ACK, (packet_t*) &node.sent_packet) == 0);
  u_char buffer[4] = {0,1,1,0};
  assert(fill_packet((u_char*) buffer, (packet_t*) &node.sent_packet, 4) == 0);
  u_char dest_addr[] = {1,2,3,4};
  u_char src_addr[] = {4,3,2,1};
  node.sent_packet.header.checksum = add_checksum(4, src_addr, dest_addr,
      0, (u_short *)&node.sent_packet.payload);
  u_short sum;
  sum = dest_addr[0]+dest_addr[2]+src_addr[0]+src_addr[2]+buffer[0]+buffer[2];
  sum = sum<<8;
  sum += dest_addr[0]+dest_addr[2]+src_addr[0]+src_addr[2]+buffer[0]+buffer[2];
  sum += 17+4;
  sum = ~(u_short)sum;
  assert(node.sent_packet.header.checksum == sum);

  //append them to the list
  append(&node1);
  append(&node);
  append(&node2);
  append(&node3);
  append(&node4);

  struct node *p;
  p = TIMER_LIST->next;

  //check the list
  assert(p->data == 1 && p->timeout_counter == 3);
  p = p->next;
  assert(p->data == 4 && p->timeout_counter == 3);
  p = p->next;
  assert(p->data == 5 && p->timeout_counter == 4);
  p = p->next;
  assert(p->data == 7 && p->timeout_counter == 3);
  p = p->next;
  assert(p->data == 9 && p->timeout_counter == 5);
  assert(p->next == NULL);

  //delete the node with data = 4 (a node in the middle of list)
  delnode(4);

  //check the list
  p = TIMER_LIST->next;
  assert(p->data == 1 && p->timeout_counter == 3);
  p = p->next;
  assert(p->data == 5 && p->timeout_counter == 4);
  p = p->next;
  assert(p->data == 7 && p->timeout_counter == 3);
  p = p->next;
  assert(p->data == 9 && p->timeout_counter == 5);
  assert(p->next == NULL);
  return 0;

  //do the decreasetime three times
  int i = 3;
  while (i > 0) {
    decreasetime();
    i--;
  }

  //check the list
  p = TIMER_LIST->next;
  assert(p->data == 1 && p->timeout_counter == 0);
  p = p->next;
  assert(p->data == 5 && p->timeout_counter == 1);
  p = p->next;
  assert(p->data == 7 && p->timeout_counter == 0);
  p = p->next;
  assert(p->data == 9 && p->timeout_counter == 2);
  assert(p->next == NULL);

  //move the nodes with timeout_counter as 0 to the end of list
  p = TIMER_LIST;
  while (p->next != NULL) {
    if (p->next->timeout_counter == 0) {
      p->next->timeout_counter = 10;
      if (p->next->next != NULL) {
        movetoend(p);
      }
    } else {
      p = p->next;
    }
  }

  //check the list
  p = TIMER_LIST->next;
  assert(p->data == 5 && p->timeout_counter == 1);
  p = p->next;
  assert(p->data == 9 && p->timeout_counter == 2);
  p = p->next;
  assert(p->data == 1 && p->timeout_counter == 10);
  p = p->next;
  assert(p->data == 7 && p->timeout_counter == 10);
  assert(p->next == NULL);

  //add a new node with timeout_counter as 0 to the end of list
  struct node node5;
  node5.data = 11;
  node5.timeout_counter = 0;
  append(&node5);

  //do the movetoend function. do not need move the new node, just set the timeout_counter as 10
  p = TIMER_LIST;
  while (p->next != NULL) {
    if (p->next->timeout_counter == 0) {
      p->next->timeout_counter = 10;
      if (p->next->next != NULL) {
        movetoend(p);
      }
    } else {
      p = p->next;
    }
  }

  //check the list
  p = TIMER_LIST->next;
  assert(p->data == 5 && p->timeout_counter == 1);
  p = p->next;
  assert(p->data == 9 && p->timeout_counter == 2);
  p = p->next;
  assert(p->data == 1 && p->timeout_counter == 10);
  p = p->next;
  assert(p->data == 7 && p->timeout_counter == 10);
  p = p->next;
  assert(p->data == 11 && p->timeout_counter == 10);
  assert(p->next == NULL);

  //delete the node with data = 7 (a node in the middle of list)
  delnode(7);

  //check the list
  p = TIMER_LIST->next;
  assert(p->data == 5 && p->timeout_counter == 1);
  p = p->next;
  assert(p->data == 9 && p->timeout_counter == 2);
  p = p->next;
  assert(p->data == 1 && p->timeout_counter == 10);
  p = p->next;
  assert(p->data == 11 && p->timeout_counter == 10);
  assert(p->next == NULL);

  //do the same thing to the node with data = 11 (the last node of list)
  delnode(11);

  p = TIMER_LIST->next;
  assert(p->data == 5 && p->timeout_counter == 1);
  p = p->next;
  assert(p->data == 9 && p->timeout_counter == 2);
  p = p->next;
  assert(p->data == 1 && p->timeout_counter == 10);
  assert(p->next == NULL);

  //do the same thing to the node with data = 5 (the first node of list)
  delnode(5);

  p = TIMER_LIST->next;
  assert(p->data == 9 && p->timeout_counter == 2);
  p = p->next;
  assert(p->data == 1 && p->timeout_counter == 10);
  assert(p->next == NULL);

  free(TIMER_LIST);

  return 0;
}
