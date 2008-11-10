#include "link_list.h"
#include "checksum.h"
#include "seq.h"
#include "udp_output.h"

int rudp_send(int socket, const void *buffer, size_t length, int flags)
{
	int send_size=0;
	struct node * p_node;
	for (;send_size<length;send_size+PAYLOAD_SIZE)
	{
		p_node=(struct node *)malloc(sizeof(sturct node));
		fill_header(
	}
	
	
}
