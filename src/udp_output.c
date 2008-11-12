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
	packet_t packet;
	struct node * p_node;
	for (;send_size<length;send_size+PAYLOAD_SIZE)
	{
		if (send_size+PAYLOAD_SIZE<length) /* if the last packet */
		{
			//p_node=(struct node *)malloc(sizeof(sturct node));
			fill_header(cur_seq_number+1,0,PAYLOAD_SIZE,ACK,&packet);
			fill_packet(&buffer[send_size],&packet,PAYLOAD_SIZE);
			real_send_size=sendto(socket,&packet,PAYLOAD_SIZE,0,to,tolen);
			assert(real_send_size==PAYLOAD_SIZE);
		}
		else
		{		/* Last packet */
			memset(&packet,0,PACKET_SIZE);
			fill_header(cur_seq_number+1,0,length-send_size,FIN,&packet);
			fill_packet(&buffer[send_size],&packet,length-send_size);
			real_send_size=sendto(socket,&packet,length-send_size,0,to,tolen);
			assert(real_send_size==length-send_size);
		}
		if (real_send_size==-1)
		{
			perror("Fatal Error in rudp_send");
		}
		
		cur_seq_number++;
	}
	return send_size;
       
}
