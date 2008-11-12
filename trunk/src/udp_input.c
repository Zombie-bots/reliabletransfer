#include "udp_input.h"

int rudp_recv(int sock, char *receive_buf, struct sockaddr_in *self_addr,
              struct sockaddr *src_addr, unsigned *src_addr_len, size_t *recv_size) {
  size_t size;
  int flag;
  char buffer[PACKET_SIZE];

  do {
    size = recvfrom(sock, (void *)buffer, PACKET_SIZE, 0, src_addr, src_addr_len);
    //assert (add_checksum(size, (u_char *)&(((struct sockaddr_in *)src_addr)->sin_addr.s_addr),
      //      (u_char *)&(self_addr->sin_addr.s_addr), size%2, (u_short *)buffer) == 0);
    strncat(receive_buf, buffer+HEADER_SIZE, size-HEADER_SIZE-1);
    printf("receive packet size %d\n",size);
    *recv_size += size-HEADER_SIZE;
    printf("receive buffer size %d\n",*recv_size);
    receive_buf[*recv_size-1] = buffer[size-HEADER_SIZE-1];
    flag = ((packet_t *)buffer)->header.flag;
    printf("flag %u\n",flag);
  } while (*recv_size < BUFFERSIZE && flag != FIN);
  return flag;
}
