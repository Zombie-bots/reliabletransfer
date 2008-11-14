#ifndef CONG_CONTROL_HEADER
#define CONG_CONTROL_HEADER
#include "header.h"

enum mode {SS, CA};

struct cong_control {
  int mode;
  size_t size;
  size_t threshold;
  int ack_n;
};

extern struct cong_control cong_window;

void reac_ack();
void reac_timeout();
void reac_tripack();

#endif
