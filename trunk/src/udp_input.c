#include "udp_input.h"
#include <assert.h>
int rudp_recv(int sock, char *receive_buf, struct sockaddr_in *self_addr,
              struct sockaddr *src_addr, unsigned *src_addr_len, size_t *recv_size) {
  size_t size;
  char * p=receive_buf;
  char buffer[PACKET_SIZE];
  header_t head;
  do {
    size = recvfrom(sock, (void *)buffer, PACKET_SIZE, 0, src_addr, src_addr_len);
    assert (add_checksum(size, (u_char *)&(((struct sockaddr_in *)src_addr)->sin_addr.s_addr),\
			 (u_char *)&(self_addr->sin_addr.s_addr), size%2, (u_short *)buffer) == 0);
    read_header(&head,(packet_t *)buffer);
    printf("Seq %d Ack %d Offset %d, Flag %d \n",head.seq,head.ack,head.offset,head.flag);
    read_packet(p,(packet_t*)buffer,head.offset);
    p+=head.offset;
    *recv_size += head.offset;
    //printf("receive buffer size %d\n",*recv_size);
    if (head.flag==FIN)
    {
	    return head.flag;
    }
    
  } while (*recv_size < BUFFERSIZE);
  return head.flag;
}
