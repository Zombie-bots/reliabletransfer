#include "sw.h"
#include <assert.h>
//extern short send_congestion_window;
//extern short initial_seq_number;
/*
 * sender sequence variable
 */

/*
 * receiver sequence  variable
 */
//extern short  rcvWindow; //receive window
//extern short last_byte_received; //receive next packet , last byte written
//extern short next_byte_expected;

/*
 * this function finds minimum value between two variable
 */
/*
 * sender sequence variable
 */
unsigned short last_packet_sent = 0; //send next packet
unsigned short last_packet_acked = 0; //last byte ack
unsigned short send_window = 8; //advertised  window, max rcv buffer, last bye written

unsigned short send_congestion_window = 6;

unsigned short initial_seq_number = 0;

/*
 * receiver sequence  variable
 */
unsigned short rcvWindow = 4; //receive window
unsigned short last_byte_received = 1003; //receive next packet , last byte written
unsigned short next_byte_expected = 0;
unsigned short init_rec_seq_number = 0;

short
min(int x, int y)
{
  if (send_window <= send_congestion_window)
    {
      return send_window;
    }
  else
    {
      return send_congestion_window;
    }
}
/*
 * update sliding window variables when sender send packet
 */
int
sender_send_packet(short send_seq_number)
{
  printf("error1111! lastsen: %d, lastAck: %d , seq se:  %d", last_packet_sent,
      last_packet_acked, send_seq_number);

  if (last_packet_sent - last_packet_acked < min(send_window,
      send_congestion_window))
    {
      //last_packet_sent += 1;
      // printf("error1! lastsen: %d, lastAck: %d ", last_packet_sent, last_packet_acked);
      printf("\n error1!");

      return 1;
    }
  else
    {
      printf("\n error2!");
      return 0;
    }
}

/*
 * update sliding window variables when sender receive acknowledge
 */
enum ack
sender_receive_ack(short send_seq_number)
{
  printf("\n eeeeeeerror 0  senSq:  %d, lastAck: %d , last sen:  %d!",
      send_seq_number, last_packet_acked, last_packet_sent);

  if ((send_seq_number == last_packet_acked + 1) && (last_packet_acked
      < last_packet_sent) && (last_packet_sent <= (send_window
      + last_packet_acked)))
    {
      last_packet_acked = last_packet_acked + 1;
      printf("error 3!");

      return correct_ack;
    }
  else if (last_packet_acked < send_seq_number && send_seq_number
      < (send_window + last_packet_acked))
    {
      printf("error2!");

      return in_window_ack;
    }

  else if (send_seq_number < last_packet_acked && send_seq_number
      > (send_window + last_packet_acked))
    {
      printf("error1!");

      return out_window_ack;
    }

}

/*
 * update sliding window variables when receiver receive packet
 */
enum recPkt
receiver_receive_packet(short recieve_seq_number)
{
  //assert(next_byte_expected<=last_byte_received);
  //printf("\n eeeeeeeeeeerror0! expect: %d , lastrec: %d, sqrec %d", next_byte_expected, last_byte_received, recieve_seq_number);
  if (init_rec_seq_number == 0)
    {

      init_rec_seq_number = recieve_seq_number;
    }
  /*
   if (next_byte_expected==last_byte_received){
   last_byte_received=last_byte_received+1;
   //printf("error1!");

   }
   if (recieve_seq_number >  rcvWindow ||recieve_seq_number == rcvWindow || recieve_seq_number < next_byte_expected || recieve_seq_number > last_byte_received ){
   //printf("error2!");

   return dropPkt;
   }
   */

  if (next_byte_expected == recieve_seq_number)
    {
      next_byte_expected = next_byte_expected + 1;
      printf("next packet: %d\n",next_byte_expected);

      return acceptPkt;
    }
  else
    {
      return dropPkt;
    }

  /*
   if(next_byte_expected<recieve_seq_number && recieve_seq_number<last_byte_received){
   printf("error4!");

   return dropPkt;
   }
   */
}

/*
 * generate a random 16 bits sequence number
 */
unsigned short gen_rand_seq()
{
   unsigned short seqNumber;
   seqNumber=rand();
   return seqNumber;
}

void init_rand()
{
  srand(1234);
}

