/*This file test the sliding window algorithm
 * */
#include <stdio.h>
#include <assert.h>
#include "../src/sw.h"

extern short last_packet_send;
extern short last_packet_ack;
extern short send_window;
extern short initial_seq_number;

int main(int argc, char *argv[])
{

/*	last_packet_send=0;
	last_packet_ack=0;
	send_window=4;*/
	int seq_num=initial_seq_number;
	int ack_num=initial_seq_number;
	int ret_val=FALSE;
	int i;
	enum ack ack_result;
	for (i=0;i<4;i++)
	{
		ret_val=sender_send_packet(seq_num);
		printf("i %d val %d \n", i,ret_val);
		assert(ret_val==TRUE);
		seq_num++;
	}
	/* Sender already sent 4 packet, can not send any more */
	ret_val=sender_send_packet(seq_num);
	printf(" val %d \n", ret_val);

	assert(ret_val==FALSE);
	/* Receive one ack*/
	ack_result=sender_receive_ack(++ack_num);
	printf(" ack result  %d \n", ack_result);

	assert(ack_result==correct_ack);

	ret_val=sender_send_packet(seq_num);
	assert(ret_val==TRUE);
	/* Send one packet, Then should not send anymore */
	ret_val=sender_send_packet(seq_num);
	assert(ret_val==FALSE);
	return 0;





}
