#include "client.h"
int main(int argc, char **argv)
{
	char ch, *filename = (char*) "receivefile.txt", *src_ip_str = (char*) "127.0.0.1";
	int port = 5320;

	while ((ch = getopt(argc, argv, "f:p:a:")) != -1) {
		switch (ch) {
		case 'a':
			src_ip_str = optarg;
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
			exit(0);
		}
	}

	struct sockaddr_in src_addr, self_addr;
	int sock;

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		fprintf(stderr, "Error: can not open socket\n");
		exit(EXIT_FAILURE);
	}

	memset((char *)&self_addr, 0, sizeof(self_addr));
	self_addr.sin_family = AF_INET;
	self_addr.sin_port = htons(port);
	self_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (struct sockaddr *) &self_addr, sizeof(self_addr)) == -1) {
		fprintf(stderr, "Error: can not bind socket\n");
		exit(1);
	}

	memset((char *)&src_addr, 0, sizeof(src_addr));
	src_addr.sin_family = AF_INET;
	src_addr.sin_port = htons(port);
	if (inet_aton(src_ip_str, &src_addr.sin_addr) == 0) {
		fprintf(stderr, "Error: invalid source address\n");
		exit(1);
	}

	char receive_buf[BUFFERSIZE];
	FILE *fp;
	int src_addr_len = sizeof(src_addr);

	if ((fp = fopen(filename, "w")) == NULL) {
		fprintf(stderr, "Error: can not open file\n");
		exit(1);
	}

	printf("Got prepared to receive file\n");
	
	size_t recv_size=0, write_size=0;
	while ((recv_size=recvfrom(sock, receive_buf, BUFFERSIZE, 0, (struct sockaddr *) &src_addr, &src_addr_len)) > 0) {
		//printf("receiving %s from %s:%d\n", filename, src_ip_str, htons(src_addr.sin_port));
		//printf("recv_size %d\n",recv_size);
		do{
			write_size=fwrite(receive_buf,1,recv_size-write_size,fp);
		}
		while(write_size!=recv_size);
		//printf("write_size %d\n",write_size);
		write_size=0;
		if (recv_size!=BUFFERSIZE)
		{
			fclose(fp);
			break;
		}
		
	}
	return 0;
}

