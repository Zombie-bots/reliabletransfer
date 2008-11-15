#include "client.h"
#include "udp_input.h"
int main(int argc, char **argv)
{
  char ch, *filename = (char*) "receivefile.txt";
  int port = 5320;

  while ((ch = getopt(argc, argv, "f:p:a:")) != -1) {
    switch (ch) {
      case 'f':
        filename = optarg;
        break;
      case 'p':
        port = atoi(optarg);
        break;
      case '?':
        if (optopt == 'f' || optopt == 'p') {
          fprintf(stderr, "Option -%c requires an argument\n", optopt);
        } else {
          fprintf(stderr, "Unknown option\n");
        }
        exit(0);
    }
  }

  struct sockaddr_in src_addr, self_addr;
  int sock;

  if ((sock = socket(AF_INET,SOCK_DGRAM, 0)) == -1) {
    fprintf(stderr, "Error: can not open socket\n");
    exit(EXIT_FAILURE);
  }

  memset((char *) &self_addr, 0, sizeof(self_addr));
  self_addr.sin_family = AF_INET;
  self_addr.sin_port = htons(port);
  self_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(sock, (struct sockaddr *) &self_addr, sizeof(self_addr)) == -1) {
    fprintf(stderr, "Error: can not bind socket\n");
    exit(EXIT_FAILURE);
  }

  char receive_buf[BUFFERSIZE];
  FILE *fp;
  unsigned src_addr_len = sizeof(src_addr);

  if ((fp = fopen(filename, "w")) == NULL) {
    fprintf(stderr, "Error: can not open file\n");
    exit(EXIT_FAILURE);
  }

  printf("Got prepared to receive file\n");

  size_t recv_size = 0, write_size = 0;
  int flag;
  do {
    flag = rudp_recv(sock, receive_buf, &self_addr, (struct sockaddr *)&src_addr,
                     &src_addr_len, &recv_size);
    do {
      write_size = fwrite(receive_buf+write_size, 1, recv_size-write_size, fp);
    } while (write_size != recv_size);
      printf("write_size %d\n",write_size);
    recv_size = write_size = 0;
    
  } while (flag != FIN);

fclose(fp);
printf("Finish Recv file\n");
//return 0;
}
