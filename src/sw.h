#ifndef SW_HEADER
#define SW_HEADER
#define FALSE 0
#define TRUE 1

 enum ack {correct_ack,in_window_ack,out_window_ack};
 enum recPkt {dropPkt,acceptPkt};



/*
 * this function finds minimum value between two variable
 */
short min(int x,int y);
/*
 * update sliding window variables when sender send packet
 */
int sender_send_packet(short send_seq_number);

/*
 * update sliding window variables when sender receive acknowledge
 */
 enum ack sender_receive_ack( short send_seq_number);


/*
 * update sliding window variables when receiver receive packet
 */
enum recPkt receiver_receive_packet(short recieve_seq_number);

#endif
