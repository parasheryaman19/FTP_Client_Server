/* File Name : ftpclient.c
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






// Global declarations
#include "ftpclient.h"
	
int sock_control; 


// Receiving a response from server and returns -1 on error or return code on success
int read_reply(){
	int retcode = 0;
	if (recv(sock_control, &retcode, sizeof retcode, 0) < 0) {
		perror("client: error reading message from server\n");
		return -1;
	}	
	return ntohl(retcode);
}



// Printing the response messages
void print_reply(int rc) 
{
	switch (rc) {
		case 220:
			printf("220 Welcome, server ready.\n");
			break;
		case 221:
			printf("221 Goodbye!\n");
			break;
		case 226:
			printf("226 Closing data connection. Requested file action successful.\n");
			break;
		case 550:
			printf("550 Requested action not taken. File unavailable.\n");
			break;
	}
	
}


//Parsing command in cstruct 
int ftclient_read_command(char* buf, int size, struct command *cstruct)
{
	memset(cstruct->code, 0, sizeof(cstruct->code));
	memset(cstruct->arg, 0, sizeof(cstruct->arg));
	
	printf("ftclient> ");	// Prompt for input		
	fflush(stdout); 	

	// Waiting for user to enter a command
	read_input(buf, size);
		
	char *arg = NULL;
	arg = strtok (buf," ");
	arg = strtok (NULL, " ");

	if (arg != NULL){
		// Storing the argument if there is one
		strncpy(cstruct->arg, arg, strlen(arg));
	}

	// buf = command
	if (strcmp(buf, "ls") == 0) {
		strcpy(cstruct->code, "LIST");		
	}
	else if (strcmp(buf, "get") == 0) {
		strcpy(cstruct->code, "RETR");		
	}
	else if (strcmp(buf, "bye") == 0) {
		strcpy(cstruct->code, "QUIT");		
	}
	else if (strcmp(buf, "put") == 0) {
		strcpy(cstruct->code, "SEND");		
	}
	else { //invalid
		return -1;
	}

	// Storing the code in beginning of the buffer
	memset(buf, 0, 400);
	strcpy(buf, cstruct->code);

	// If there's an arg, then appending it to the buffer
	if (arg != NULL) {
		strcat(buf, " ");
		strncat(buf, cstruct->arg, strlen(cstruct->arg));
	}
	
	return 0;
}



// Do get <filename> command 
int ftclient_get(int data_sock, int sock_control, char* arg)
{
    char data[MAXSIZE];
    int size;
    FILE* fd = fopen(arg, "w");
    
    while ((size = recv(data_sock, data, MAXSIZE, 0)) > 0) {
        fwrite(data, 1, size, fd);
    }

    if (size < 0) {
        perror("error\n");
    }

    fclose(fd);
    return 0;
}


//Opening data connection
int ftclient_open_conn(int sock_con)
{
	int sock_listen = socket_create(CLIENT_PORT_ID);

	// Sending an ACK on control connection
	int ack = 1;
	if ((send(sock_con, (char*) &ack, sizeof(ack), 0)) < 0) {
		printf("client: ack write error :%d\n", errno);
		exit(1);
	}		

	int sock_conn = socket_accept(sock_listen);
	close(sock_listen);
	return sock_conn;
}




//Do list commmand
int ftclient_list(int sock_data, int sock_con)
{
	size_t num_recvd;			// Number of bytes received with recv()
	char buf[MAXSIZE];			// Hold a filename received from server
	int tmp = 0;

	// Waiting for server starting message
	if (recv(sock_con, &tmp, sizeof tmp, 0) < 0) {
		perror("client: error reading message from server\n");
		return -1;
	}
	
	memset(buf, 0, sizeof(buf));
	while ((num_recvd = recv(sock_data, buf, MAXSIZE, 0)) > 0) {
        	printf("%s", buf);
		memset(buf, 0, sizeof(buf));
	}
	
	if (num_recvd < 0) {
	        perror("error");
	}

	// Waiting for server done message
	if (recv(sock_con, &tmp, sizeof tmp, 0) < 0) {
		perror("client: error reading message from server\n");
		return -1;
	}
	return 0;
}



/**
 * Input: cmd struct with a code and an arg
 * Concats code + arg into a string and sends to server
 */
int ftclient_send_cmd(struct command *cmd)
{
	char buffer[MAXSIZE];
	int rc;

	sprintf(buffer, "%s %s", cmd->code, cmd->arg);
	
	// Sending command string to the server
	rc = send(sock_control, buffer, (int)strlen(buffer), 0);	
	if (rc < 0) {
		perror("Error sending command to server");
		return -1;
	}
	
	return 0;
}



// Get login details from the user and sending it to server for authentication
void ftclient_login()
{
	struct command cmd;
	char user[256];
	memset(user, 0, 256);

	// Getting username from user
	printf("Name: ");	
	fflush(stdout); 		
	read_input(user, 256);

	// Sending USER command to the server
	strcpy(cmd.code, "USER");
	strcpy(cmd.arg, user);
	ftclient_send_cmd(&cmd);
	
	// Waiting for go-ahead to send the password
	int wait;
	recv(sock_control, &wait, sizeof wait, 0);

	// Getting password from the user
	fflush(stdout);	
	char *pass = getpass("Password: ");	

	// Send PASS command to the server
	strcpy(cmd.code, "PASS");
	strcpy(cmd.arg, pass);
	ftclient_send_cmd(&cmd);
	
	// Waiting for the response
	int retcode = read_reply();
	switch (retcode) {
		case 430:
			printf("Invalid username/password.\n");
			exit(0);
		case 230:
			printf("Successful login.\n");
			break;
		default:
			perror("error reading message from server");
			exit(1);		
			break;
	}
}


void ftserve_retr(int sock_control, int sock_data, char* filename)
{	
	FILE* fd = NULL;
	char data[MAXSIZE];
	size_t num_read;							
		
	fd = fopen(filename, "r");
	
	if (!fd) {	
		// Sending error code (550 Requested action not taken)
		send_response(sock_control, 550);
		
	} else {	
		// Sending okay (150 File status okay)
		send_response(sock_control, 150);
	
		do {
			num_read = fread(data, 1, MAXSIZE, fd);

			if (num_read < 0) {
				printf("error in fread()\n");
			}

			// Sending block
			if (send(sock_data, data, num_read, 0) < 0)
				perror("error sending file\n");

		   } while (num_read > 0);													
			
		// Sending message: 226: closing connection, file transfer successful
		send_response(sock_control, 226);

		fclose(fd);
	}
}


// Main function
int main(int argc, char* argv[]) 
{		
	int data_sock, retcode, s;
	char buffer[MAXSIZE];
	struct command cmd;	
	struct addrinfo hints, *res, *rp;

	if (argc != 3) {
		printf("usage: ./ftclient hostname port\n");
		exit(0);
	}

	char *host = argv[1];
	char *port = argv[2];

	// Getting matching addresses
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	s = getaddrinfo(host, port, &hints, &res);
	if (s != 0) {
		printf("getaddrinfo() error %s", gai_strerror(s));
		exit(1);
	}
	
	// Finding an address to connect 
	for (rp = res; rp != NULL; rp = rp->ai_next) {
		sock_control = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

		if (sock_control < 0)
			continue;

		if(connect(sock_control, res->ai_addr, res->ai_addrlen)==0) {
			break;
		} else {
			perror("connecting stream socket");
			exit(1);
		}
		close(sock_control);
	}
	freeaddrinfo(rp);


	// Getting connected with welcome message
	printf("Connected to %s.\n", host);
	print_reply(read_reply()); 
	

	//Get the username and password and send it to server for authentication
	ftclient_login();

	while (1) { // loop until the user types quit

		// Get a command from the user
		if ( ftclient_read_command(buffer, sizeof buffer, &cmd) < 0) {
			printf("Invalid command\n");
			continue;	// loop back for another command
		}
		// Sending command to the server
		if (send(sock_control, buffer, (int)strlen(buffer), 0) < 0 ) {
			close(sock_control);
			exit(1);
		}

		retcode = read_reply();		
		if (retcode == 221) {
			//If command was quit, just exit
			print_reply(221);		
			break;
		}
		
		if (retcode == 502) {
			// If invalid command, show error message
			printf("%d Invalid command.\n", retcode);
		} else {			
			// Command is valid (RC = 200), process command
		
			// Open data connection
			if ((data_sock = ftclient_open_conn(sock_control)) < 0) {
				perror("Error opening socket for data connection");
				exit(1);
			}			
			
			// Executing command
			if (strcmp(cmd.code, "LIST") == 0) {
				ftclient_list(data_sock, sock_control);
			}
			else if (strcmp(cmd.code, "RETR") == 0) {
				// Waiting for reply (Checking wether the is file valid)
				if (read_reply() == 550) {
					print_reply(550);		
					close(data_sock);
					continue; 
				}
				ftclient_get(data_sock, sock_control, cmd.arg);
				print_reply(read_reply()); 
			}
			else if(strcmp(cmd.code, "SEND") == 0){
				// Opening data connection with the client
				ftserve_retr(sock_control, data_sock, cmd.arg);
				close(data_sock);

			}
			close(data_sock);
		}

	} // looping back to get more user input

	// Closing the socket (control connection)
	close(sock_control);
    return 0;  
}
