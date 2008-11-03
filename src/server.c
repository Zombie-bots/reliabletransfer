#include "server.h"

int main(int argc, char **argv)
{
	char ch, *filename = (char*) "sendfile.txt", *dst_ip_str = (char*) "127.0.0.1";
	int port = 5320;

	while ((ch = getopt(argc, argv, "f:p:a:")) != -1) {
		switch (ch) {
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
			exit(0);
		}
	}

	struct sockaddr_in dst_addr;
	int sock;

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		fprintf(stderr, "Error: can not open socket\n");
		exit(EXIT_FAILURE);
	}

	memset((char *)&dst_addr, 0, sizeof(dst_addr));
	dst_addr.sin_family = AF_INET;
	dst_addr.sin_port = htons(port);
	if (inet_aton(dst_ip_str, &dst_addr.sin_addr) == 0) {
		fprintf(stderr, "Error: invalid destination address\n");
		exit(1);
	}

	char send_buf[BUFFERSIZE];
	FILE *fp;
	int send_byte=0,read_byte=0;

	if ((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "Error: can not open file\n");
		exit(1);
	}

	while ((read_byte=fread(send_buf,1,BUFFERSIZE,fp))>0) {
		do{
			send_byte+=sendto(sock, send_buf, read_byte-send_byte, 0, (struct sockaddr *)&dst_addr, sizeof(dst_addr));
		}	
		while(send_byte!=read_byte);
		//printf("send_byts %d, read_byte %d\n",send_byte,read_byte);
		send_byte=0;
		usleep(10000); 	/* Sleep after send, give receiver little more time */
	}

	//printf("%s\n",send_buf);
	printf("Complete sending %s to %s:%d\n", filename, dst_ip_str, port);
	fclose(fp);
	return 0;
}


