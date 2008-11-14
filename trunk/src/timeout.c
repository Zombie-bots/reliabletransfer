#include "timeout.h"

int EST_RTT=0;
int DEV=0;

/* This function implements Jacobson algorithm */
int timeout(int sam_rtt)
{
  sam_rtt -= EST_RTT;            //diff
  EST_RTT += (sam_rtt >> 3);     //mul. delta
  if (sam_rtt < 0)
    sam_rtt = -sam_rtt;          // abs(diff)
  sam_rtt -= DEV;                // diff - DEV
  DEV += (sam_rtt >> 2);         // mul. detal
  return (EST_RTT + (DEV << 2));
}

/*
main ()
{
  int sam_rtt;

  printf("Input a sample RTT: ");
  scanf("%d",&sam_rtt);
  while(sam_rtt != 0) {
    printf("Timeout result: %d\n", timeout(sam_rtt));
    printf("Input a sample RTT: ");
    scanf("%d",&sam_rtt);
  }
}
*/
