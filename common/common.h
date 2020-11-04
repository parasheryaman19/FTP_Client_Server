/* File Name : common.h
 *
 * Projet Name : FTP (File Transfer Protocol) Project
 * Subject : Introduction to Programming Tools 
 * Yaman Parasher and Mahmoud Nasser
 * 
 *
 * This file belongs to the common part of the whole project that defines the common header files and associated functions.
 * To make everything easier to understand we have split or organize the whole code of the project in multiple files (for ex. we have common.c and    
common.h in the common folder that defines the common functions and header files associated with the project, ftpserver.c and ftpserver.h to define 
the server part and similarly ftpclient.c and ftpclient.h to define the client side.
*/





#ifndef COMMON_H
#define COMMON_H

#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>		// getaddrinfo()
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>



// constants
#define DEBUG				1
#define MAXSIZE 			512 	// maximum buffer size
#define CLIENT_PORT_ID		30020


/* This struct holds command code and argument */
struct command {
	char arg[255];
	char code[5];
};




// Creating a listening socket on remote host where returns -1 on error or socket fd on success
int socket_create(int port);


// Creating a new socket for incoming client connection request where returns -1 on error or fd of newly created socket
int socket_accept(int sock_listen);


// Connecting to remote host at given port where returns socket fd on success, -1 on error
int socket_connect(int port, char *host);



// Receiving data on sockfd where returns -1 on error or number of bytes received on success
int recv_data(int sockfd, char* buf, int bufsize);


// Sending resposne code on sockfd where Returns -1 on error or 0 on success
int send_response(int sockfd, int rc);



//UTILITY FUNCTIONS

// Trimming whiteshpace and line ending characters from a string
void trimstr(char *str, int n);



// Reading input from the command line
void read_input(char* buffer, int size);


#endif







