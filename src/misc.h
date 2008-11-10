#ifndef MISC_HEADER
#define MISC_HEADER
#include "header.h"
#include <stdlib.h>
extern short init_seq_number;
extern short cur_seq_number;
/* Add up checksum, return value will be filled in checksum filed in header */
int simulate_lag(int poss, int msec);
int packet_lost(int poss);
unsigned short gen_rand_seq();
void init_rand_seed();
#endif
