#include "misc.h"

/*
 * input possibility range from 0-100 , max sleep time msec,
 *  It randomly sleep for less msec millaseconds. If no sleep return
 *  0, else return sleep time.
 */
int simulate_lag(int poss, int msec)
{
 int tempRand=0;
 int temp=rand();

 tempRand =(int)(100.0*(temp/(RAND_MAX + 1.0)));

 if (tempRand < poss) {
   usleep(msec*1000);
   return 1;
 } else
   return 0;
}

/*
 * input possibility range from 0-100,
 * if rand is below poss, return true, else return false
 */
int packet_lost(int poss)
{
 int temp;

 temp =(int)(100.0*(rand()/(RAND_MAX + 1.0)));

 if (temp < poss) {
   return 1;
 } else
   return 0;
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
