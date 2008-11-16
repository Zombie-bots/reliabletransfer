#ifndef TIMEOUT_HEADER
#define TIMEOUT_HEADER

extern int EST_RTT;
extern int DEV;
struct timeval TIMEOUT;

void timeout(int sam_rtt);

#endif
