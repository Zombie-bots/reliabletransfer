#include "link_list.h"
#include "checksum.h"
#include "misc.h"
#include "header.h"
#include "udp_output.h"
#include "server.h"
#include <assert.h>
#include <sys/socket.h>
#include <sys/types.h>

/* Send buffer to remote machine
 * precondition: initial sequence by init_rand() and gen_read_seq()
 * buffer should aligned by 1016 bytes. which n*PAYLOAD size.
 * 
 * */
int rudp_send(int socket, char *buffer, size_t length, int flags,\
	      const struct sockaddr *to, socklen_t tolen)
{
	int send_size=0;
	int real_send_size=0;
	int left_size=length;
	packet_t packet;
	struct node * p_node;
	printf("length %d\n",length);
	for (;send_size<length;send_size+=PAYLOAD_SIZE)
	{
		
		if (left_size>=PAYLOAD_SIZE) /* test if the last packet */
		{
			//p_node=(struct node *)malloc(sizeof(sturct node));
			printf("Normal Packet %d \n",left_size);
			fill_header(cur_seq_number+1,0,PAYLOAD_SIZE,ACK,&packet);
			fill_packet(&buffer[send_size],&packet,PAYLOAD_SIZE);
			real_send_size=sendto(socket,&packet,PACKET_SIZE,0,to,tolen);
			assert(real_send_size==PACKET_SIZE);
			left_size-=PAYLOAD_SIZE;
		}
		else
		{		/*IS  Last packet */
			printf("Last Packet %d \n",left_size);
			memset(&packet,0,PACKET_SIZE);
			fill_header(cur_seq_number+1,0,length-send_size,FIN,&packet);
			fill_packet(&buffer[send_size],&packet,length-send_size);
			real_send_size=sendto(socket,&packet,PACKET_SIZE,0,to,tolen);
			assert(real_send_size==PACKET_SIZE);
			return length;
		}
		if (real_send_size==-1)
		{
			perror("Fatal Error in rudp_send");
		}
	       
		cur_seq_number++;
	}
	return send_size;
       
}
