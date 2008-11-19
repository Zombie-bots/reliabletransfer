#include "udp_input.h"
#include "client.h"
#include "sw.h"
#include <assert.h>
//#define ACK_PORT 3323
#define timersum(c,a,b) (c).tv_sec = ((a).tv_sec + (b).tv_sec); \
                       (c).tv_usec = ((a).tv_usec + (b).tv_usec); \
                       if ((c).tv_usec > 1000000000){ \
                           (c).tv_usec -= 1000000000;(c).tv_sec++;}

#define timersubtract(c,a,b) (c).tv_sec = ((a).tv_sec - (b).tv_sec); \
                       (c).tv_usec = ((a).tv_usec - (b).tv_usec); \
                       if ((c).tv_usec < 0){ \
                           (c).tv_usec += 1000000000;(c).tv_sec--;}

struct delay_node *DELAY_HEAD;

void insert_dh(u_short seq)
{
  struct delay_node *p, *node;
  p = DELAY_HEAD;

  while (p->next != NULL) {
    p = p->next;
  }

  node = (struct delay_node *)malloc(sizeof(struct delay_node));
  node->seq = seq;
  gettimeofday(&node->tv, NULL);
  struct timeval delay_tv;
  delay_tv.tv_usec = delay_t;
  delay_tv.tv_sec = 0;
  while (delay_tv.tv_usec >= 1000000) {
    delay_tv.tv_usec -= 1000000;
    delay_tv.tv_sec ++;
  }
  timersum(node->tv, node->tv, delay_tv);
  node->next = NULL;
  p->next = node;

  /*
  printf("delay array:\n");
  for (p=DELAY_HEAD->next; p!=NULL; p=p->next) {
    printf("seq: %u time: %ld\n",p->seq,p->tv);
  }
  */
}

void make_ack(u_short seq, packet_t *ack_p)
{
  ack_p->header.ack = seq+1;
  ack_p->header.flag = ACK;
}

void traverse_dh()
{
  struct timeval current, diff;
  struct delay_node *p;
  p = DELAY_HEAD;
  packet_t ack_p;

  while (p->next!=NULL) {
    gettimeofday(&current, NULL);
    timersubtract(diff,p->next->tv,current);
    if (diff.tv_sec < 0) {
      make_ack(p->next->seq, &ack_p);
      printf("send ack sequence number %u\n",ack_p.header.ack);
      sendto(sock,(void *)&ack_p,PACKET_SIZE,0,
          (struct sockaddr *)(&src_addr),sizeof(src_addr));
      struct delay_node *q;
      q = p->next;
      p->next = q->next;
      free(q);
    } else {
      p = p->next;
    }
  }
}
void send_ack(u_short seq)
{
  packet_t ack_p;

  if (simulate_lag(delay_p)) {
    printf("packet %u delayed\n",seq);
    insert_dh(seq);
  } else {
    make_ack(seq, &ack_p);
    if (delay_p != 0) {
      traverse_dh();
    }
    sendto(sock,(void *)&ack_p,PACKET_SIZE,0,
        (struct sockaddr *)(&src_addr),sizeof(src_addr));
    printf("send ack seq %u \n",seq+1);
  }
}

int rudp_recv(int sock, char *receive_buf, struct sockaddr_in *self_addr,\
              struct sockaddr *src_addr, unsigned *src_addr_len, size_t *recv_size)
{
  size_t size;
  char *p=receive_buf;
  char buffer[PACKET_SIZE];
  header_t head;
  enum recPkt sw_return;

  do {
    size = recvfrom(sock, (void *)buffer, PACKET_SIZE, 0, src_addr, src_addr_len);
    assert (size==PACKET_SIZE);
    assert (add_checksum(size, (u_char *)&(((struct sockaddr_in *)src_addr)->sin_addr.s_addr),
            (u_char *)&(self_addr->sin_addr.s_addr), size%2, (u_short *)buffer) == 0);
    read_header(&head,(packet_t *)buffer);
    assert (head.offset==PAYLOAD_SIZE || head.flag==FIN);
    printf("recevie packet %d\n",head.seq);
    sw_return = receiver_receive_packet(head.seq);
    if (sw_return == dropPkt) {
      send_ack(next_byte_expected-1);
      printf("seq wrong. drop packet %d. send ack for %d\n",head.seq,next_byte_expected);
      continue;
    }

    //printf("Seq %d Ack %d Offset %d, Flag %d \n",head.seq,head.ack,head.offset,head.flag);
    if (packet_lost(drop_p) && head.flag!=FIN) {
      next_byte_expected--;
      //p+=head.offset;
      printf("packet %u dropped\n",head.seq);
      continue;
    } else {
      *recv_size += head.offset;
      read_packet((u_char *)p,(packet_t*)buffer,(u_short)head.offset);
      p+=head.offset;
    }
    send_ack(head.seq);
    if (head.flag==FIN) {
      if (delay_p != 0) {
        while (DELAY_HEAD->next!=NULL) {
          traverse_dh();
        }
      }
      return (int)head.flag;
    }
  } while (*recv_size < BUFFERSIZE);

  return (int)head.flag;
}
