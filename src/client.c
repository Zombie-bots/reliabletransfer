#include "client.h"
#include "udp_input.h"
struct sockaddr_in src_addr, self_addr;
int sock;
int drop_p = 0, delay_p = 0;
int delay_t = 500000;

int main(int argc, char **argv)
{
  char ch, *filename = (char *) "receivefile.txt", *sa = (char *) "127.0.0.1";
  int port = 3322;
 
  while ((ch = getopt(argc, argv, "f:p:a:o:l:t:")) != -1) {
    switch (ch) {
      case 'f':
        filename = optarg;
        break;
      case 'a':
        sa = optarg;
        break;
      case 'o':
        drop_p = atoi(optarg);
        break;
      case 'l':
        delay_p = atoi(optarg);
        break;
      case 't':
        delay_t = atoi(optarg);
        break;
      case 'p':
        port = atoi(optarg);
        break;
      case '?':
        if (optopt=='f' || optopt=='p'|| optopt=='a' || optopt=='l'\
            || optopt=='o' || optopt=='t') {
          fprintf(stderr, "Option -%c requires an argument\n", optopt);
        } else {
          fprintf(stderr, "Unknown option\n");
        }
        exit(EXIT_FAILURE);
    }
  }

  if ((sock = socket(AF_INET,SOCK_DGRAM, 0)) == -1) {
    fprintf(stderr, "Error: can not open socket\n");
    exit(EXIT_FAILURE);
  }

  memset((char *) &self_addr, 0, sizeof(self_addr));
  self_addr.sin_family = AF_INET;
  self_addr.sin_port = htons(port);
  if (inet_aton(sa, &self_addr.sin_addr) == 0) {
    fprintf(stderr, "Error: invalid local host address\n");
    exit(EXIT_FAILURE);
  }
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

  DELAY_HEAD = (struct delay_node *)malloc(sizeof(struct delay_node));
  DELAY_HEAD->next = NULL;

  printf("Got prepared to receive file\n");

  size_t recv_size = 0, write_size = 0;
  int flag;

  do {
    flag = rudp_recv(sock, receive_buf, &self_addr, (struct sockaddr *)&src_addr,\
                     &src_addr_len, &recv_size);
    do {
      write_size = fwrite(receive_buf+write_size, 1, recv_size-write_size, fp);
    } while (write_size != recv_size);
      printf("receive_bytes %d, write_bytes %d\n",recv_size, write_size);
    recv_size = write_size = 0;
  } while (flag != FIN);

  fclose(fp);
  printf("Complete receiving file %s\n",filename);
  return 0;
}
