#include "../src/timeout.h"
#include <stdio.h>
#include "utils.h"

int main(int argc, char *argv[])
{
  init_test(argc,argv);
  fprintfd(stderr,"debug level normal\n");
  fprintft(stderr,"debug level detail\n");
  timeout(32);
  printf("timeout: %ld:%ld\n", (long)TIMEOUT.tv_sec, (long)TIMEOUT.tv_usec);
  return 0;
}
