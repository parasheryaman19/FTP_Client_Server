/* File Name : ftpclient.h
 *
 * Projet Name : FTP (File Transfer Protocol) Project
 * Subject : Introduction to Programming Tools 
 * Yaman Parasher and Mahmoud Nasser
 * 
 *
 * This is the client side of FTP Server Cient model which runs with custom server. 
 * 
 *
 * Direction to use the client side: 
 *    ./ftpclient SERVER_HOSTNAME PORT# ( For ex. If you are running the server on port lets say 9000 then the command to run the ftpclient would be like ./ftpclient 127.0.0.1 9000 )
 *
 *
 * Valid commands as instructed in the objectives: 
 *    ls ( to list the files in the server)
 *    get <filename> (to transfer file from server to client side)
 *    put <filename> (to transfer the file from client to the server)   
 *    bye (to close the connection with a 'Goodbye' message)
 *    
 */





#ifndef FTCLIENT_H
#define FTCLIENT_H

#include "../common/common.h"


// Receive a response from server and returns -1 on error or return code on success
int read_reply();


// Printing response message
void print_reply(int rc);


// Parsing command in cstruct
int ftclient_read_command(char* buf, int size, struct command *cstruct);


// Do get <filename> command 
int ftclient_get(int data_sock, int sock_control, char* arg);


// Open data connection
int ftclient_open_conn(int sock_con);


// Do list commmand

int ftclient_list(int sock_data, int sock_con);


/*
 * Input: cmd struct with a code and an arg
 * Concats code + arg into a string and sends to the server
 */
int ftclient_send_cmd(struct command *cmd);


// Getting login details from user and send to server for authentication

void ftclient_login();



void ftserve_retr(int sock_control, int sock_data, char* filename);

#endif
