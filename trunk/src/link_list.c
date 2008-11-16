#include "link_list.h"
#include "timeout.h"
#include <malloc.h>
#include <assert.h>

struct node *TIMER_LIST;

#define timersum(c,a,b) (c).tv_sec = ((a).tv_sec + (b).tv_sec); \
                       (c).tv_usec = ((a).tv_usec + (b).tv_usec); \
                       if ((c).tv_usec > 1000000000){ \
                           (c).tv_usec -= 1000000000;(c).tv_sec++;}

/*THIS FUNCTION MAKES A NODE CONTAINING GIVEN DATA */
void make_node (packet_t *packet, struct node *pnode)
{
  pnode->data = packet->header.seq;
  pnode->sent_packet = *packet;
  gettimeofday(&pnode->send_time, (void *)0);
  timersum(pnode->expire_time, pnode->send_time, TIMEOUT);
}

/*THIS FUNCTION ADDS A NODE AT THE END OF LINKED LIST */
void append(struct node *pnode)
{
  struct node *r;
  r = TIMER_LIST;

  while (r->next != 0) {
    r = r->next;
  }
  r->next = pnode;
  pnode->next = 0;
}

/*THIS FUNCTION REMOVES A NODE FROM THE LIST */
void delnode(int num)
{
  struct node *r, *temp;
  temp = TIMER_LIST;

  while (temp->next != 0 && temp->next->data != num) {
    temp = temp->next;
  }
  assert(temp->next != 0);
  r = temp->next;
  temp->next = r->next;
  free(r);
}

/*THIS FUNCTION MOVES A NODE TO THE LIST END */
void movetoend(struct node *r)
{
  struct node *temp;

  temp = r->next;
  r->next = temp->next;
  append(temp);
}
