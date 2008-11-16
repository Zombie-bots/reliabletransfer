#include "timeout.h"
#include <sys/time.h>

int EST_RTT = 0;
int DEV = 0;
struct timeval TIMEOUT;

/* This function implements Jacobson/Karel algorithm */
void timeout(int sam_rtt)
{
  sam_rtt -= EST_RTT;            // diff
  EST_RTT += (sam_rtt >> 3);     // mul. delta, delta = 1/8
  if (sam_rtt < 0)
    sam_rtt = -sam_rtt;          // abs(diff)
  sam_rtt -= DEV;                // diff - DEV
  DEV += (sam_rtt >> 2);         // mul. sigma, sigma = 1/4
  TIMEOUT.tv_usec = EST_RTT + (DEV << 2);
  TIMEOUT.tv_sec=0;
  while (TIMEOUT.tv_usec > 1000000) {
    TIMEOUT.tv_usec -= 1000000;
    TIMEOUT.tv_sec ++;
  }
}
