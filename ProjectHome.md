# Project Goal #
Today, applications have two choices when it comes to the transport channel: TCP or UDP. While UDP is an unreliable protocol, TCP provides reliability, flow control, and congestion control. TCP has an explicit connection establishment phase, which some application may not find desirable. Your job in this project is to design and implement a file transfer application which has all the good features of TCP without the connection establishment phase. Your application, reliable UDP, should build its transport functionality on top of UDP. The goal of this project is to make you familiar with the implementation of TCP and to solidify your understanding of the fundamental networking issues related to reliable data transfer.

# Project Specification #
You must implement both server and client sides for reliable UDP. Your programs must be written in C/C++ and should work on the CS Linux machines. The steps involved in finishing this project are the following:

Design header: Design a header that both the server and the client will use. This header may be a simplified version of the actual header TCP uses. The simplifying assumptions you should make are:

Packets are a fixed 1024 bytes (make sure to document how you decide to pad the packet if the file to be transferred is not a multiple of 1024 bytes)
No options exist, implying that the header length is fixed.
the only flag to be supported is the ACK flag, which is used to indicate whether the packet contains data or acknowledgment.
The AdvertisedWindow is a fixed parameter, negotiated offline and not part of the header (your server and client programs should accept this as a command line parameter).
There cannot be any urgent data, implying that there is no need to provision for UrgPtr.
Make sure to discuss the header you design in your project write-up.
Implement sliding window algorithm for reliability: The receiver should use duplicate acknowledgments to indicate a lost or out-of-order packet. Cumulative acknowledgments are allowed.

Implement adaptive retransmission: Implement Jacobson/Karels algorithm to estimate round trip time (RTT).

Implement congestion control: TCP has two distinct phases of data transmission: slow start and congestion avoidance (additive increase/multiplicative decrease). Your program should implement both of these. Your program should print to standard out when the server switches from slow start to congestion avoidance occurs. At the end of the transmission, the server should indicate the number and percentage of packets that are transferred in each phase.
Simulate loss and reordering: You client program should simulate various network conditions. Toward that goal, you should enable and disable each of these features as needed during your demonstration:
Variable packet loss rate: You can simulate different rates of packet loss by probabilistically dropping packets. You perform probabilistic dropping by using the rand() function and a drop percentage on the server.
Packet reordering: You can simulate packet reordering simply be sending the packets out of order. When this option is turned on, the server should probabilisticly store a packet in a buffer and send it later out of order, instead of sending it in the correct sequence.
Experiment with these parameters even before the demonstration and document their effect on the number of packets transmitted in slow start and congestion avoidance phases.
# Sending Data #
To test your implementation, you must be able to transfer data. In this project, we will assume a simple file transfer mechanism from a server to a client. The server should be started first and then listen for connections from clients. The port to listen on should be read from a command line argument. When a client connects, it will send a file name to the server, which the server should read and send to the client using reliable UDP. Your server does NOT need to be able to support multiple simultaneous clients, but should be able to support multiple file transfers sequentially (if a client requests a file, the server should send it, and then return to waiting for new clients, not exit).

The client must save the data it recieves from the server in a file. The client does not need to be interactive. It can simply take the file name to request, the address of the server, and the port the server is listening on as command line arguments. It should save the transferred file locally in a file with the same name. Both the client and the server should be coded to allow the transmission of both text and binary data files.
