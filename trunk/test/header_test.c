#include "../src/header.h"
#include <stdio.h>
#include <string.h>
#include "utils.h"
/* Test that fill_header generate right header
 * Test header and packet are right size header=8 packet=1024
 * Test fill_packet generate right packet
 * Test read_header return right header data structure
 * Test read_packet reads correct amount of payload*/
int main(int argc, char *argv[])
{
	init_test(argc,argv);
	assert(sizeof(header_t)==8);
	assert(sizeof(packet_t)==1024);
	packet_t test_packet;
	header_t test_header;
	/* Test write and read header */
	/* fill_header fills buf with header */
	fill_header(100,200,333,ACK,&test_packet);
	/* We have empty test_header, read data in buf and assign it to test_heaer */
	read_header(&test_header,&test_packet);
	assert(test_header.seq==100);
	assert(test_header.ack==200);
	assert(test_header.offset==333);
	assert(test_header.flag==ACK);
	/* Test write and read payload */
	/* Initial send buffer */
	u_char sender_buf[2048],recv_buf[2048];
	memset(recv_buf,0,2048);
	u_char * psend=sender_buf;
	u_char * precv=recv_buf;
	int i;
	for (i=0;i<1016;i++)
	{
		sender_buf[i]=5;
	}
	for (i=1016;i<2032;i++)
	{
		sender_buf[i]=6;
	}
	for (i=2032;i<2048;i++)
	{
		sender_buf[i]=7;
	}
	/* Send 1016 bytes payload packet */
	fill_packet(psend,&test_packet,PAYLOAD_SIZE);
	/* Suppose we send test_packet...... */
	/* OK, Now we receive new data, cast it into test_packet */
	read_packet(precv,&test_packet,PAYLOAD_SIZE);
	for (i=0;i<1016;i++)
	{
		assert(recv_buf[i]==5);
	}
	psend+=PAYLOAD_SIZE;
	precv+=PAYLOAD_SIZE;
	fill_packet(psend,&test_packet,PAYLOAD_SIZE);
	read_packet(precv,&test_packet,PAYLOAD_SIZE);
	for (i=1016;i<2032;i++)
	{
		assert(recv_buf[i]==6);
	}
	psend+=PAYLOAD_SIZE;
	precv+=PAYLOAD_SIZE;
	fill_packet(psend,&test_packet,16);
	read_packet(precv,&test_packet,16);
	for (i=2032;i<2048;i++)
	{
		assert(recv_buf[i]==7);
	}

	return 0;
}
