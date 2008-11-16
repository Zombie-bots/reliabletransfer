#include "cong_control.h"
#include <stdio.h>

struct cong_control cong_window = {SS, 1, 16, 0};

void reac_ack()
{
  cong_window.ack_n ++;
  if (cong_window.mode == SS) {
    cong_window.size ++;
    if (cong_window.size > cong_window.threshold) {
      cong_window.mode = CA;
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
  cong_window.mode = SS;
}

void reac_tripack()
{
  cong_window.size = (size_t)(cong_window.size/2);
  cong_window.threshold = 0;
  cong_window.ack_n = 0;
  cong_window.mode = CA;
}
