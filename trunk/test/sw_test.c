/*This file test the sliding window algorithm
 * */
#include <stdio.h>
#include <assert.h>
#include "../src/sw.h"
#include "../src/cong_control.h"

int main(int argc, char *argv[]) {
	int send_seq_num = initial_seq_number;
	int ack_num = initial_seq_number;
	int rec_seq_num = init_rec_seq_number;
	int ret_val = FALSE;
	int i;
	enum ack ack_result;
	enum recPkt rpResult;

	send_window = 4;
	cong_window.size = 8;
	//sender send 4 packet and the window size is 4
	for (i = 0; i < send_window; i++) {
		ret_val = sender_send_packet(send_seq_num);
		assert(ret_val==TRUE);
		send_seq_num++;
		last_packet_sent++;
	}
	/* Sender already sent 4 packet, can not send any more */
	ret_val = sender_send_packet(send_seq_num);
	assert(ret_val==FALSE);

	// Receive one ack
	ack_num = ack_num + 1;
	ack_result = sender_receive_ack(ack_num);
	assert(ack_result==correct_ack);

	ret_val = sender_send_packet(send_seq_num);
	last_packet_sent++;
	assert(ret_val==TRUE);
	// Send one packet, Then should not send anymore
	ret_val = sender_send_packet(send_seq_num);
	assert(ret_val==FALSE);

	//receiver send 4 packet and the window size is 4
	for (i = 0; i < 4; i++) {
		rpResult = receiver_receive_packet(rec_seq_num + i);
		assert(rpResult==acceptPkt);
	}
	// receiver already sent 4 packet, can not send any more
	rpResult = receiver_receive_packet(rec_seq_num);
	assert(rpResult==dropPkt);
	return 0;
}
