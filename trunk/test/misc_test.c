#include "../src/misc.h"
#include "utils.h"
#include <time.h>

int main()
{
  int test = 1000;
  int return_true = 0;
  int i;

  for (i=0; i<test; i++) {
    if (simulate_lag(30) == 1) {
      return_true ++;
    }
  }

  assert (return_true >= 280 && return_true <= 320);
  return_true = 0;

  for (i=0; i<test; i++) {
    if (packet_lost(50) == 1) {
      return_true ++;
    }
  }

  assert (return_true >= 480 && return_true <= 520);

  /*
  for(int i=0; i<100;i++) {
    test=gen_rand_seq();
    cout<<test<<endl;
  }*/

  return 0;
}
