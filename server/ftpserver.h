/* File Name: ftpserver.h
 *
 * Projet Name : FTP  Project
 * Subject : Introduction to Programming Tools 
 * Yaman Parasher and Mahmoud Nasser
 * 
 * This is the Server side of FTP Server Cient model which runs with custom client. 
 * 
 * Direction to run the server: 
 *    ./ftserve PORT# (For ex. ./ftserve 9000)
 *
 * Valid commands as instructed in the objectives: 
 *    ls (to list the files in the server)
 *    get <filename> (to transfer file from server to client side)
 *    put <filename> (to transfer the file from client to the server)   
 *    bye (to close the connection with a 'Goodbye' message)
 *    
 */






#ifndef FTSERVE_H
#define FTSERVE_H

#include "../common/common.h"


/**
 * Sending file specified in filename over data connection 
 * Sending control message over control connection
 * Handling case of null or invalid filename
 */
void ftserve_retr(int sock_control, int sock_data, char* filename);



// Sending list of files in current directory over data connection where return -1 on error or 0 on success
int ftserve_list(int sock_data, int sock_control);




// Opening data connection to the client where return socket for data connection or -1 on error
int ftserve_start_data_conn(int sock_control);



// Authenticating a user's credentials where return 1 if authenticated or 0 if not
int ftserve_check_user(char*user, char*pass);



// Logging in the connected client
int ftserve_login(int sock_control);


// Waiting for command from client and send response where just returns just the response code

int ftserve_recv_cmd(int sock_control, char*cmd, char*arg);



//Child process handles connection to client
void ftserve_process(int sock_control);


int ftserver_get(int data_sock, int sock_control, char* arg);

#endif
