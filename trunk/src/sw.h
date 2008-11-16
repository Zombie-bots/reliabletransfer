#ifndef SW_HEADER
#define SW_HEADER
#define FALSE 0
#define TRUE 1

enum ack {correct_ack,in_window_ack,out_window_ack};
enum recPkt {dropPkt,acceptPkt};

extern unsigned short last_packet_sent; //send next packet
extern unsigned short last_packet_acked; //last byte ack
extern unsigned short send_window; //advertised  window, max rcv buffer, last bye written


extern unsigned short send_congestion_window;
extern unsigned short initial_seq_number;

/*
 * receiver sequence  variable
  */
extern unsigned short rcvWindow; //receive window
extern unsigned short last_byte_received; //receive next packet , last byte written
extern unsigned short next_byte_expected;
extern unsigned short init_rec_seq_number;



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


/*
 * generate a random 16 bits sequence number
 */

unsigned short gen_rand_seq();
/*
  generate seed for seq number
  */
void init_rand();
#endif
