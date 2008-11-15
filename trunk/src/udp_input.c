#include "udp_input.h"
#include "client.h"
#include <assert.h>

struct delay_node *DELAY_HEAD;

void init_dh()
{
  DELAY_HEAD = (struct delay_node *)malloc(sizeof(struct delay_node));
  DELAY_HEAD->next = NULL;
}

void insert_dh(u_short seq)
{
  struct delay_node *p, *node;
  p = DELAY_HEAD;

  while (p->next != NULL) {
    p = p->next;
  }

  node = (struct delay_node *)malloc(sizeof(struct delay_node));
  node->seq = seq;
  node->tv = time(NULL)+delay_t;
  node->next = NULL;
  p->next = node;

  /*
  printf("delay array:\n");
  for (p=DELAY_HEAD->next; p!=NULL; p=p->next) {
    printf("seq: %u time: %ld\n",p->seq,p->tv);
  }
  */
}

void traverse_dh()
{
  struct delay_node *p;
  p = DELAY_HEAD;
  packet_t ack_p;

  while (p->next!=NULL) {
    if (p->next->tv<=time(NULL)) {
      printf("send delayed ack for packet %u\n",p->next->seq);
      make_ack(p->next->seq, &ack_p);
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

void make_ack(u_short seq, packet_t *ack_p)
{
  ack_p->header.ack = seq;
  ack_p->header.flag = ACK;
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
    //printf("send normal ack for %u\n",seq);
  }
}

int rudp_recv(int sock, char *receive_buf, struct sockaddr_in *self_addr,\
              struct sockaddr *src_addr, unsigned *src_addr_len, size_t *recv_size)
{
  size_t size;
  char *p=receive_buf;
  char buffer[PACKET_SIZE];
  header_t head;

  do {
    size = recvfrom(sock, (void *)buffer, PACKET_SIZE, 0, src_addr, src_addr_len);
    assert (size==PACKET_SIZE);
    assert (add_checksum(size, (u_char *)&(((struct sockaddr_in *)src_addr)->sin_addr.s_addr),
            (u_char *)&(self_addr->sin_addr.s_addr), size%2, (u_short *)buffer) == 0);
    read_header(&head,(packet_t *)buffer);
    assert (head.offset==PAYLOAD_SIZE || head.flag==FIN);

    //printf("Seq %d Ack %d Offset %d, Flag %d \n",head.seq,head.ack,head.offset,head.flag);
    *recv_size += head.offset;
    if (packet_lost(drop_p) && head.flag!=FIN) {
      p+=head.offset;
      printf("packet %u dropped\n",head.seq);
      continue;
    } else {
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
