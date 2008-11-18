#include "link_list.h"
#include "checksum.h"
#include "sw.h"		/* cur_seq_number,init_seq_number */
#include "header.h"
#include "udp_output.h"
#include "server.h"
#include <assert.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
int already_send=0;
#define timersum(c,a,b) (c).tv_sec = ((a).tv_sec + (b).tv_sec); \
                       (c).tv_usec = ((a).tv_usec + (b).tv_usec); \
                       if ((c).tv_usec > 1000000000){ \
                           (c).tv_usec -= 1000000000;(c).tv_sec++;}

#define timersubtract(c,a,b) (c).tv_sec = ((a).tv_sec - (b).tv_sec); \
                       (c).tv_usec = ((a).tv_usec - (b).tv_usec); \
                       if ((c).tv_usec < 0){ \
                           (c).tv_usec += 1000000000;(c).tv_sec--;}

/* THIS FUNCTION REACTS ON RECEIVING AN ACK */
void pro_header_ack(int seq)
{
  struct timeval current, diff;
  struct node *r;
  unsigned samplertt;

  /* calculate timeout value */
  gettimeofday(&current, (void *)0);
  r = find(seq-1);
  timersubtract(diff,current,r->next->send_time);
  //printf("Sample RTT: %u microseconds\n",(samplertt = 1000000*diff.tv_sec+diff.tv_usec));
  timeout(samplertt);
  //printf("New timeout value: %ld microseconds\n",1000000*TIMEOUT.tv_sec+TIMEOUT.tv_usec);

  /* call reac_ack for congestion control */
  reac_ack();
  //printf("New congestion window size: %d\n",cong_window.size);

  /* delete the node from timer list */
  delnode(r);
  /*
  printf("Timer list:\n");
  struct node *np;
  np = TIMER_LIST->next;
  while (np != 0) {
    printf("seq: %u\n", np->data);
    np = np->next;
  }
  */
}

/* THIS FUNCTION DEALS WITH THE RETRANSMISSION ISSUE */
void retran(int socket, const struct sockaddr *to, socklen_t tolen)
{
  struct timeval current, diff;
  int real_send_size = 0;
  struct node *r;
  r = TIMER_LIST;

  while (r->next != 0) {
    gettimeofday(&current, (void *)0);
    timersubtract(diff,r->next->expire_time,current);
    if (diff.tv_sec < 0) {
      real_send_size = sendto(socket, &r->next->sent_packet, PACKET_SIZE, 0, to, tolen);
      if (real_send_size == -1) {
        perror("Fatal Error in rudp_send");
      }
      gettimeofday(&r->next->send_time, (void *)0);
      timersum(r->expire_time, r->send_time, TIMEOUT);
      movetoend(r);
      //usleep(100000);
    } else {
      r = r->next;
    }
  }
}

/* Send buffer to remote machine
 * precondition: initial sequence by init_rand() and gen_read_seq()
 * buffer should aligned by 1016 bytes. which n*PAYLOAD size.
 *
 * */

int rudp_send(int socket, char *buffer, size_t length, int flags,
    const struct sockaddr *to, socklen_t tolen, struct sockaddr_in *src_addr)
{
  
  int send_size = 0;
  int real_send_size = 0;
  int left_size = length;
  packet_t packet;
  u_short checksum = 0;
  struct sockaddr_in *dest;
  dest = (struct sockaddr_in *) to;
  u_char * s, *d;

  d = (u_char*) &dest->sin_addr.s_addr;
  s = (u_char*) &src_addr->sin_addr.s_addr;
  //printf("send packet %d length %d already_send %d\n",last_packet_sent,length,already_send);
  for (send_size=already_send*(PAYLOAD_SIZE); send_size < length; send_size += (PAYLOAD_SIZE)) {  
    /* Test if we can send packet,  */
    if(sender_send_packet(last_packet_sent))
      {
	/* already_sent is number of packet we have sent in every call
	   if some packet can not send because of sender_send_packet
	   next time, we will start from that packet.*/
	last_packet_sent++;
	already_send++;
	printf ("already_send %d last %d\n",already_send,last_packet_sent);
	
      }
    else 
      {
      printf("rudp_send reject sending  packet. because sliding window return false\n");
      return -1;
      }
    if(already_send==length/PAYLOAD_SIZE)already_send=0;
    //printf("send packet %d left_size %d \n",last_packet_sent,left_size);
    if (left_size >= PAYLOAD_SIZE) {
      /* Not last packet */
      fill_header(last_packet_sent, 0, PAYLOAD_SIZE,ACK, &packet);
      fill_packet((u_char*) &buffer[send_size], &packet, PAYLOAD_SIZE);
      /* calculate checksum */
      checksum = add_checksum(PACKET_SIZE,s, d, 0, (u_short*) &packet);
      packet.header.checksum = htons(checksum);
      assert(add_checksum(PACKET_SIZE, s, d, 0, (u_short *)&packet) == 0);
      /* send out packet */
      real_send_size = sendto(socket, &packet, PACKET_SIZE,0, to, tolen);
      assert(real_send_size==PACKET_SIZE);
      left_size -= PAYLOAD_SIZE;

      struct node *pnode;
      pnode = (struct node *)malloc(sizeof(struct node));
      make_node(&packet, pnode);
      append(pnode);
      /*
      printf("Timer list:\n");
      struct node *np;
      np = TIMER_LIST->next;
      while (np != 0) {
        printf("seq: %u\n", np->data);
        np = np->next;
      }
      */
    } else {
      /*IS  Last packet */
      printf("Last Packet %d \n", left_size);
      memset(&packet, 0, PACKET_SIZE);
      /* fill last packet with FIN flag */
      fill_header(last_packet_sent, 0, length - send_size, FIN,&packet);
      fill_packet((u_char*) &buffer[send_size], &packet, length - send_size);
      /* calculate cheksum */
      checksum = add_checksum(PACKET_SIZE,s, d, 0, (u_short*) &packet);
      packet.header.checksum = htons(checksum);
      assert(add_checksum(PACKET_SIZE, s, d, 0, (u_short *)&packet) == 0);
      real_send_size = sendto(socket, &packet, PACKET_SIZE,0, to, tolen);
      assert(real_send_size==PACKET_SIZE);

      struct node *pnode;
      pnode = (struct node *)malloc(sizeof(struct node));
      make_node(&packet, pnode);
      append(pnode);

      return length;
    }
    if (real_send_size == -1) {
      perror("Fatal Error in rudp_send");
    }
    /* Don't send too fast
       fast send may cause packet lost*/
    //usleep(100000);
    //last_packet_sent++;
  }
  return send_size;
}
