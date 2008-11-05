# Copyright (C) 2008 Lei Chen <lc6@indiana.edu>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
CC=gcc
HEADER_OBJ=../src/header.o header_test.o
LINK_LIST_OBJ=../src/link_list.o link_list_test.o
TIMEOUT_OBJ=../src/timeout.o timeout_test.o 
MISC_OBJ=../src/misc.o misc_test.o
CFLAGS = -g -Wall
#DEPS= ../src/header.h 
#CFLAGS += -fprofile-arcs -ftest-coverage  # for coverage measurement

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: header_test link_list_test timeout_test misc_test

header_test: $(HEADER_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) 

link_list_test: $(LINK_LIST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

timeout_test: $(TIMEOUT_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

misc_test: $(MISC_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o
	rm -f header_test link_list_test timeout_test misc_test  
