#include "udp_input.h"
#include "client.h"
#include <assert.h>

struct delay_array da[PACKET_NUM];

void init_da()
{
  int i;
  for (i=0; i<PACKET_NUM; i++) {
    da[i].tv = 0;
  }
}

void insert_da(u_short seq)
{
  int i;
  for (i=0; i<PACKET_NUM; i++) {
    if (da[i].tv == 0) {
      da[i].seq = seq;
      da[i].tv = time(NULL)+delay_t;
      break;
    }
  }
  printf("delay array:\n");
  for (i=0; i<PACKET_NUM; i++) {
    if (da[i].tv != 0) {
      printf("seq: %u time: %ld\n",da[i].seq,da[i].tv);
    }
  }
}

void traverse_da()
{
  int i;
  packet_t ack_p;

  for (i=0; i<PACKET_NUM; i++) {
    if (da[i].tv!=0 && da[i].tv<=time(NULL)) {
      printf("send delayed ack for packet %u\n",da[i].seq);
      make_ack(da[i].seq, &ack_p);
      sendto(sock,(void *)&ack_p,PACKET_SIZE,0,
          (struct sockaddr *)(&src_addr),sizeof(src_addr));
      da[i].tv = 0;
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
    insert_da(seq);
  } else {
    make_ack(seq, &ack_p);
    if (delay_p != 0) {
      traverse_da();
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
    //printf("received and saved. now send ack\n");
    send_ack(head.seq);
    if (head.flag==FIN) {
      if (delay_p != 0) {
        int i;
        do {
          traverse_da();
          for (i=0; da[i].tv==0 && i<PACKET_NUM; i++);
        } while (i!=PACKET_NUM);
      }
      return (int)head.flag;
    }
  } while (*recv_size < BUFFERSIZE);

  return (int)head.flag;
}
