#ifndef CHECKSUM_HEADER
#define CHECKSUM_HEADER
/* Add up checksum, return value will be filled in checksum filed in header */
u_short add_checksum(u_short len_udp, u_char src_addr[], u_char dest_addr[],
		     bool padding, const u_short *temp);
#endif
