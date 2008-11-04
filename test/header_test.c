#include "../src/header.h"
#include "stdio.h"
#include <assert>
int main()
{
	int j=0;
	printf("Size of header %d",sizeof(header_t));
	printf("Size of packet %d",sizeof(packet_t));
	printf("fill header %d \n",fill_header(j));
	return 0;
}
