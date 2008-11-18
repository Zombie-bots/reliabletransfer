/*
 *  This is Rudp server which send data to client, The server use select() function to poll send
 *  and receive socet.
 *  */
#include "server.h"
#include <netinet/in.h>
#include <sys/select.h>
#include <assert.h>
#include "sw.h"
int main(int argc, char **argv)
{

  /* Default input value  */
  char ch, *filename = (char*) "sendfile.txt", *dst_ip_str = (char*) "127.0.0.1";
  int port = 3322;
  int ack_port=3323;
  //int ack_port = 3323;
  char *source_ip_str= (char *) "127.0.0.1";
  int reuse_addr=1;
  /* Socket  */
  fd_set sendfd,ackfd;
  struct timeval tv;
  struct sockaddr_in dst_addr;
  struct sockaddr_in ack_addr;
  int sock;
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
  /*
  if ((ack_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    fprintf(stderr, "Error: can not open send socket\n");
    exit(EXIT_FAILURE);
  }
  */
  /* Set socket reuse. Release port after program finished. */
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,
	     sizeof(reuse_addr));
  /*
  setsockopt(ack_sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,
	     sizeof(reuse_addr));
  */
  /* translate ip address string to ip addres data
   * we need to use this ip to calculate right check sum*/
  memset((char *)&ack_addr, 0, sizeof(ack_addr));
  ack_addr.sin_family = AF_INET;
  ack_addr.sin_port = htons(port);
  if (inet_aton(source_ip_str, &ack_addr.sin_addr) == 0) {
    fprintf(stderr, "Error: invalid ack address\n");
    exit(1);
  }
 
  memset((char *)&dst_addr, 0, sizeof(dst_addr));  
  dst_addr.sin_family = AF_INET;
  dst_addr.sin_port = htons(ack_port);
  if (inet_aton(dst_ip_str, &dst_addr.sin_addr) == 0) {
    fprintf(stderr, "Error: invalid destination address\n");
    exit(1);
  }

  /* We need to bind this udp so we know which port returns ack */

  if (bind(sock, (struct sockaddr *) &dst_addr, sizeof(dst_addr)) == -1) {
    fprintf(stderr, "Error: can not bind ack socket\n");
    exit(EXIT_FAILURE);
  }
  
  /* After bind port, change port to destination port */
  dst_addr.sin_port = htons(port);
  
  printf("Bind on port %d to send and receive packet\n",ack_port);
  /* Open send file */

  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Error: can not open file\n");
    exit(1);
  }
  TIMER_LIST = (struct node *)malloc(sizeof(struct node));
  TIMER_LIST->next = (void *)0;
  TIMEOUT.tv_sec = 1;
  TIMEOUT.tv_usec = 0;
  tv.tv_sec=3;
  tv.tv_usec=0;
  //printf("before  dst %s \n",inet_ntoa(dst_addr.sin_addr));
  int not_finish=1;
  while(1) {
    
    FD_ZERO(&sendfd);
    FD_ZERO(&ackfd);
    FD_SET(sock,&sendfd);
    FD_SET(sock,&ackfd);
    if ((select(FD_SETSIZE,&ackfd,&sendfd,NULL,&tv))<=0) {
      /* Time out code put here */
      printf("time out \n");
      continue;
      //retran(sock,(struct sockaddr *)&dst_addr,sizeof(dst_addr));
    }
    /* Receive ACK */
    if (FD_ISSET(sock,&ackfd)) {
      recv_size=recvfrom(sock,\
          (void *)receive_buf,\
          BUFFERSIZE, 0,\
          NULL,NULL);
      assert(recv_size%PACKET_SIZE==0);
      int i;
      header_t head;
      char * recv_p;
      recv_p=receive_buf;
      /* loop through every ack packet */
      for (i=0;i<recv_size/PACKET_SIZE;i++) {
        read_header(&head,(packet_t*)recv_p);
        printf("Ack %d Offset %d, Flag %d \n",head.ack,head.offset,head.flag);
	/* Sliding windows receive ack */
	sender_receive_ack(head.ack);
        pro_header_ack(head.ack);
        tv.tv_sec=TIMEOUT.tv_sec;
	tv.tv_usec=TIMEOUT.tv_usec;
	recv_p+=PACKET_SIZE;
      }
      continue;
    }
    
    /* Send data */
    if (FD_ISSET(sock,&sendfd))
      {
	/*
	if ((read_byte=fread(send_buf,1,BUFFERSIZE,fp))>0) {
	  send_byte+=rudp_send(sock, send_buf,\
			       read_byte-send_byte, \
			       0,(struct sockaddr *)&dst_addr, \
			       sizeof(dst_addr),&ack_addr);
	  printf("send_byts %d, read_byte %d\n",send_byte,read_byte);
	  send_byte=0;
	  // Sleep after send, give receiver little more time
	  usleep(1000);
	} else   // Finish all data 
	  break;
	*/
       
	if ((read_byte=fread(send_buf,1,BUFFERSIZE,fp))>0)
	  {
	    send_byte=rudp_send(sock, send_buf,	
				read_byte,			\
				0,(struct sockaddr *)&dst_addr,	\
				sizeof(dst_addr),&ack_addr);    
	    //printf("send_byts %d, read_byte %d\n",send_byte,read_byte);
	    // not_finish=(send_byte>0)?1:0;
	    // Sleep after send, give receiver little more time 
	    //printf("read_byte %d\n ",read_byte);
	    usleep(1000);
	  }
	else
	  {
	    perror("Read file error:");
	    printf("Send all data, read_byte %d \n",read_byte);
	    break;		// Finish sending file 
	  }
	
	
      }
  }
  
  printf("Complete sending %s to %s:%d\n", filename, dst_ip_str, port);
  fclose(fp);
  return 0;
}
