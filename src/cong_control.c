#include "cong_control.h"
#include <stdio.h>

struct cong_control cong_window = {SS, 1, 16, 0};
int ss_n = 0;
int ca_n = 0;

void reac_ack()
{
  /* count number of packets in each phase */
  if (cong_window.mode == SS) {
    ss_n ++;
  } else {
    ca_n ++;
  }

  cong_window.ack_n ++;
  if (cong_window.mode == SS) {
    cong_window.size ++;
    if (cong_window.size > cong_window.threshold) {
      cong_window.mode = CA;
      printf("* switch from Slow Start mode to Congestion Avoidance mode *\n");
    }
  } else if ((cong_window.ack_n-cong_window.threshold)%cong_window.size == 0) {
      cong_window.size ++;
      cong_window.threshold = cong_window.ack_n;
  }
}

void reac_timeout()
{
  cong_window.threshold = (size_t)(cong_window.size/2);
  cong_window.size = 1;
  cong_window.ack_n = 0;
  if (cong_window.mode == CA) {
    printf("* switch from Congestion Avoidance mode to Slow Start mode *\n");
    cong_window.mode = SS;
  }
}

void reac_tripack()
{
  cong_window.size = (size_t)(cong_window.size/2);
  cong_window.threshold = 0;
  cong_window.ack_n = 0;
  if (cong_window.mode == SS) {
    printf("* switch from Slow Start mode to Congestion Avoidance mode *\n");
    cong_window.mode = CA;
  }
}
