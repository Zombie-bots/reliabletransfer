/*
 *  This is Rudp server which send data to client, The server use select() function to poll send
 *  and receive socet.
 *  */
#include "server.h"
#include <netinet/in.h>
#include <sys/select.h>

int main(int argc, char **argv)
{
  
  /* Default input value  */
  char ch, *filename = (char*) "sendfile.txt", *dst_ip_str = (char*) "127.0.0.1";
  int port = 3322;
  int ack_port = 3323;
  char *source_ip_str= (char *) "127.0.0.1";
  int reuse_addr=1;
  /* Socket  */
  fd_set sendfd,ackfd;
  struct timeval tv;
  struct sockaddr_in dst_addr;
  struct sockaddr_in ack_addr;
  int sock,ack_sock;
  char receive_buf[BUFFERSIZE];
  char send_buf[BUFFERSIZE];
  FILE *fp;
  int send_byte=0,read_byte=0;
  int recv_size=0;	/* size of bytes recv from clent,
			   recv_size/PACKET_SIZE=number of ack recv*/

  /* Read command line options */
  while ((ch = getopt(argc, argv, "f:p:a:s:")) != -1) {
    switch (ch) {
    case 's':
      source_ip_str=optarg;
      break;
    case 'a':
      dst_ip_str = optarg;
      break;
    case 'f':
      filename = optarg;
      break;
    case 'p':
      port = atoi(optarg);
      break;
    case '?':
      if (optopt == 'f' || optopt == 'p' || optopt == 'a') {
	fprintf(stderr, "Option -%c requires an argument\n", optopt);
      }
      else {
	fprintf(stderr, "Unknown option\n");
      }
      exit(EXIT_FAILURE);
    }
  }

  printf ("Rudp server now connect to %s:%d from %s, Sending file %s\n"\
	  ,dst_ip_str,port,source_ip_str,filename);
	
  /* create socket */

  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    fprintf(stderr, "Error: can not open send socket\n");
    exit(EXIT_FAILURE);
  }
  /* create ack socket */
  if ((ack_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    fprintf(stderr, "Error: can not open send socket\n");
    exit(EXIT_FAILURE);
  }
	
  /* Set socket reuse. Release port after program finished. */
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,
	     sizeof(reuse_addr));
  setsockopt(ack_sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,
	     sizeof(reuse_addr));
	
  /* translate ip address string to ip addres data
   * we need to use this ip to calculate right check sum*/
  memset((char *)&ack_addr, 0, sizeof(ack_addr));
  ack_addr.sin_family = AF_INET;
  ack_addr.sin_port = htons(ack_port);
  if (inet_aton(source_ip_str, &ack_addr.sin_addr) == 0) {
    fprintf(stderr, "Error: invalid ack address\n");
    exit(1);
  }
  memset((char *)&dst_addr, 0, sizeof(dst_addr));
  dst_addr.sin_family = AF_INET;
  dst_addr.sin_port = htons(port);
  if (inet_aton(dst_ip_str, &dst_addr.sin_addr) == 0) {
    fprintf(stderr, "Error: invalid destination address\n");
    exit(1);
  }

  /* We need to bind this udp so we know which port returns ack */
  if (bind(ack_sock, (struct sockaddr *) &ack_addr, sizeof(ack_addr)) == -1) {
    fprintf(stderr, "Error: can not bind ack socket\n");
    exit(EXIT_FAILURE);
  }
  printf("Bind on port %d \n",ack_port);
  /* Open send file */

  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Error: can not open file\n");
    exit(1);
  }
  TIMER_LIST = (struct node *)malloc(sizeof(struct node));
  TIMER_LIST->next = (void *)0;
  TIMEOUT.tv_sec = 0;
  TIMEOUT.tv_usec = 1000000;
  //printf("before  dst %s \n",inet_ntoa(dst_addr.sin_addr));
  /* Fix me: Time out should be dynamic  */
  while(1)
    {
      tv.tv_sec=3;
      tv.tv_usec=0;
      FD_ZERO(&sendfd);
      FD_ZERO(&ackfd);
      FD_SET(sock,&sendfd);
      FD_SET(ack_sock,&ackfd);
      if ((select(FD_SETSIZE,&ackfd,&sendfd,NULL,&tv))<=0) {
	/* Time out code put here */
	printf("time out \n");
      }
      /* Receive ACK */
      if (FD_ISSET(ack_sock,&ackfd)) {
		  
	recv_size=recvfrom(ack_sock,
			   (void *)receive_buf,
			   BUFFERSIZE, 0,
			   NULL,NULL);
	printf("Recv ACK size %d \n",recv_size);
	continue;
      }
	
      /* Send data */
      if (FD_ISSET(sock,&sendfd))
	{
	  if ((read_byte=fread(send_buf,1,BUFFERSIZE,fp))>0)
	    {
	      send_byte+=rudp_send(sock, send_buf,	\
				   read_byte-send_byte, \
				   0,(struct sockaddr *)&dst_addr, \
				   sizeof(dst_addr),&ack_addr);
	      printf("send_byts %d, read_byte %d\n",send_byte,read_byte);
	      send_byte=0;
	      /* Sleep after send, give receiver little more time */
	      usleep(1000);
	    }
	  else
	    {	/* Finish all data */
	      break;
	    }
	}
		
			
			
    }

  printf("Timer list:\n");
  struct node *np;
  np = TIMER_LIST->next;
  while (np != 0) {
    printf("seq: %u\n", np->data);
    np = np->next;
  }
  //printf("%s\n",send_buf);
  printf("Complete sending %s to %s:%d\n", filename, dst_ip_str, port);
  fclose(fp);
  return 0;
}