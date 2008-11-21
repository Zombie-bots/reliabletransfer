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
  int ack_port=63443;
  //int ack_port = 3323;
  char *source_ip_str= (char *) "127.0.0.1";
  int reuse_addr=1;
  /* Socket  */
  fd_set sendfd,ackfd;
  struct timeval tv;
  struct sockaddr_in dst_addr;
  struct sockaddr_in ack_addr;
  int sock;
  char receive_buf[PACKET_SIZE];
  char send_buf[BUFFERSIZE];
  FILE *fp;
  int send_byte=0,read_byte=0;
  int recv_size=0;	/* size of bytes recv from clent,
			   recv_size/PACKET_SIZE=number of ack recv*/

  /* Read command line options */
  while ((ch = getopt(argc, argv, "f:p:a:s:w:")) != -1) {
    switch (ch) {
    case 's':
      source_ip_str = optarg;
      break;
    case 'w':
      send_window = atoi(optarg);
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
      if (optopt == 'f' || optopt == 'p' || optopt == 'a' || optopt=='s' || optopt=='w') {
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

  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
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
  ack_addr.sin_port = htons(ack_port);
  if (inet_aton(source_ip_str, &ack_addr.sin_addr) == 0) {
    fprintf(stderr, "Error: invalid ack address\n");
    exit(1);
  }

  memset((char *)&dst_addr, 0, sizeof(dst_addr));
  dst_addr.sin_family = AF_INET;
  dst_addr.sin_port = htons(ack_port);
  dst_addr.sin_addr.s_addr=INADDR_ANY;

  /* We need to bind this udp so we know which port returns ack */

  if (bind(sock, (struct sockaddr *) &dst_addr, sizeof(dst_addr)) == -1) {
    perror("bind socket:");
    fprintf(stderr, "Error: can not bind ack socket\n");
    exit(EXIT_FAILURE);
  }

  if (inet_aton(dst_ip_str, &dst_addr.sin_addr) == 0) {
    fprintf(stderr, "Error: invalid destination address\n");
    exit(1);
  }

  /* After bind port, change port to destination port */
  dst_addr.sin_port = htons(port);

  printf("Bind on port %d to send and receive packet\n",ack_port);
  /* Open send file */

  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Error: can not open file, Use -f file_name to indicate the file you want to send\n");
    exit(1);
  }
  TIMER_LIST = (struct node *)malloc(sizeof(struct node));
  TIMER_LIST->next = (void *)0;
  TIMEOUT.tv_sec = 1;
  TIMEOUT.tv_usec = 0;
  tv.tv_sec=3;
  tv.tv_usec=0;
  //printf("before  dst %s \n",inet_ntoa(dst_addr.sin_addr));
  int dup_check=0;
  FD_ZERO(&sendfd);
  FD_ZERO(&ackfd);
  FD_SET(sock,&sendfd);
  FD_SET(sock,&ackfd);
  //init_rand();
  
  //last_packet_sent=last_packet_acked=gen_rand_seq();
  //initial_seq_number=last_packet_sent;


  while(1) {
    if ((select(FD_SETSIZE,&ackfd,&sendfd,NULL,&tv))<=0) {
      /* Time out code put here */
      printf("time out \n");
      reac_timeout();
      resend_packet(last_packet_acked,sock,(struct sockaddr *)&dst_addr,sizeof(dst_addr));
      FD_ZERO(&sendfd);
      FD_ZERO(&ackfd);
      FD_SET(sock,&sendfd);
      FD_SET(sock,&ackfd);
      continue;
      //retran(sock,(struct sockaddr *)&dst_addr,sizeof(dst_addr));
    }
    /* Receive ACK */
    if (FD_ISSET(sock,&ackfd)) {
      enum ack recv_ack;
      recv_size=recvfrom(sock,\
			 (void *)receive_buf,PACKET_SIZE	\
			 , 0,					\
			 NULL,NULL);
      assert(recv_size%PACKET_SIZE==0);
      int i;
      header_t head;
      char * recv_p;
      recv_p=receive_buf;
      /* loop through every ack packet */
      for (i=0;i<recv_size/PACKET_SIZE;i++) {
        read_header(&head,(packet_t*)recv_p);
        printf("------receive Ack %d last_packet_acked %d\n",head.ack,last_packet_acked);
	/* Sliding windows receive ack */
	recv_ack=sender_receive_ack(head.ack);
	if(recv_ack==correct_ack||recv_ack==in_window_ack)
	  {
	    /* call reac_ack for congestion control */
	    reac_ack();
	    pro_header_ack(head.ack);
	    print_timer();
	       int small_seq=0;
	    if (TIMER_LIST->next!=0)
	      {
		small_seq=TIMER_LIST->next->data;
	      }
	    if(small_seq!=0)
	      {last_packet_acked=small_seq;}

	    tv.tv_sec=TIMEOUT.tv_sec;
	    tv.tv_usec=TIMEOUT.tv_usec;
	    recv_p+=PACKET_SIZE;
	    printf("The smallest packet is %d\n",small_seq);
	  }
	else if (recv_ack==dup_ack)
	  {
	    dup_check++;
	    printf("dup_check = %d \n",dup_check);
	    if (dup_check==3)	/* 3 duplicate ack, fast retransmit */
	      {
		//sleep(1);
		reac_tripack();
		dup_check=0;
		resend_packet(head.ack,sock,(struct sockaddr *)&dst_addr,sizeof(dst_addr));
		print_timer();
	      }
	  }
      }
      continue;
    }

    /* Send data */
    if (FD_ISSET(sock,&sendfd))
      {
	if ((read_byte=fread(send_buf,1,BUFFERSIZE,fp))>0)
	  {
	    send_byte=rudp_send(sock, send_buf,
				read_byte,			\
				0,(struct sockaddr *)&dst_addr,	\
				sizeof(dst_addr),&ack_addr);
	    if(send_byte<0)
	      {
		/* We need to remove send socket from select() This make sure our
		   time out codes have a chance to work. Otherwise, select() always can
		   send. */
		/* sliding window not allow to send. So we send the same buffer next time */
		printf("fseek back one packet\n");
		fseek(fp,-BUFFERSIZE,SEEK_CUR);
		FD_ZERO(&sendfd);
		FD_ZERO(&ackfd);
		FD_SET(sock,&ackfd);

	      }
	    else
	      {
		FD_ZERO(&sendfd);
		FD_ZERO(&ackfd);
		FD_SET(sock,&sendfd);
		FD_SET(sock,&ackfd);
	      }
	    usleep(10000);
	  }
	else
	  {
	    //perror("Read file error:");
	    printf("Send all data, read_byte %d \n",read_byte);
	    break;		// Finish sending file
	  }


      }
  }

  printf("Complete sending %s to %s:%d\n", filename, dst_ip_str, port);
  printf("Slow Start phase: %d packets, %.2f%% of all packets\n",\
      ss_n,100*((float)ss_n/(ss_n+ca_n)));
  printf("Congestion Avoidance phase: %d packets, %.2f%% of all packets\n",\
      ca_n,100*((float)ca_n/(ss_n+ca_n)));
  fclose(fp);
  return 0;
}
