/*This file test the sliding window algorithm
 * */
#include <stdio.h>
#include <../src/sw.c>
#include <assert.h>
#define FALSE 0
#define TRUE 1
extern last_packet_send;
extern last_packet_ack;
extern send_window;

int main(int argc, char *argv[])
{
	last_packet_send=0;
	last_packet_ack=0;
	send_window=4;
	
	int seq_num=1000;
	int ret_val=FALSE;
	int i;
	for (i=0;i<4;i++)
	{
		ret_val=sender_send_packet(seq);
		assert(ret_val==TRUE);
		seq++;
	}
	/* Sender already sent 4 packet, can not send any more */
	ret_val=sender_send_packet(seq);
	assert(ret_val==FALSE);
	/* Receive one ack*/
	sender_receive_ack();
	ret_val=sender_send_packet(seq);
	assert(ret_val==TRUE);
	/* Send one packet, Then should not send anymore */
	ret_val=sender_send_packet(seq);
	assert(ret_val==FALSE);
	return 0
	
	       
	
	
	
}
