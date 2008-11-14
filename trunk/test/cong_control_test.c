#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "../src/cong_control.h"

int main ()
{
  int i;
  int ss = 0, ca = 0;

  for (i=0; i<25; i++) {
    if (cong_window.mode == SS) {
      ss ++;
    } else {
      ca ++;
    }
    reac_ack();
  }
  assert(ss==20 && ca==5);
  assert(cong_window.size==21);
  reac_timeout();
  assert(cong_window.size==1 && cong_window.threshold==10);
  for (i=0; i<25; i++) {
    if (cong_window.mode == SS) {
      ss ++;
    } else {
      ca ++;
    }
    reac_ack();
  }
  //printf("ss: %d  ca: %d\n",ss,ca);return 0;
  assert(ss==30 && ca==20);
  //printf("size: %d  threshold: %d\n",cong_window.size,cong_window.threshold);return 0;
  assert(cong_window.size==12);
  reac_tripack();
  assert(cong_window.size==6);
  for (i=0; i<10; i++) {
    if (cong_window.mode == SS) {
      ss ++;
    } else {
      ca ++;
    }
    reac_ack();
  }
  assert(ss==30 && ca==30);
  assert(cong_window.size==7);
  reac_tripack();
  assert(cong_window.size==3);
  for (i=0; i<100; i++) {
    if (cong_window.mode == SS) {
      ss ++;
    } else {
      ca ++;
    }
    reac_ack();
  }
  assert(ss==30 && ca==130);
  assert(cong_window.size==14);
  reac_timeout();
  assert(cong_window.size==1 && cong_window.threshold==7);
  for (i=0; i<10; i++) {
    if (cong_window.mode == SS) {
      ss ++;
    } else {
      ca ++;
    }
    reac_ack();
  }
  assert(ss==37 && ca==133);
  assert(cong_window.size==8);
  return 0;
}
