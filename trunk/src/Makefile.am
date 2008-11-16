# Copyright (C) 2008 Lei Chen <lc6@indiana.edu>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

SERVER_BIN = server 
CLIENT_BIN = client
SERVER_SRC = 
DEPS = $(wildcard ./src/*.h)
CLIENT_OBJ = client.o udp_input.o checksum.o misc.o header.o sw.o
SERVER_OBJ = server.o udp_output.o timeout.o link_list.o checksum.o misc.o header.o cong_control.o sw.o
CFLAGS = -g -Wall
#CFLAGS += -fprofile-arcs -ftest-coverage  # for coverage measurement
FMT = -linux

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(SERVER_BIN) $(CLIENT_BIN) 

$(CLIENT_BIN): $(CLIENT_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) 

$(SERVER_BIN): $(SERVER_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(CLIENT_OBJ) $(CLIENT_BIN) $(SERVER_OBJ) $(SERVER_BIN) *.gcno *.gcda *.gcov TAGS
